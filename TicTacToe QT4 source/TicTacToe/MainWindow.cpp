#include "MainWindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QThread>
#include <QTimer>
#include <QMessageBox>
#include "KI.h"
#include "Random.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), einstellungen(0), naechstes_zeichen("X"), zugliste_position(0)
{
// die benutzeroberflaeche aufbauen
setupUi(this);

// osx spezifische einstellungen vornehmen
#ifdef Q_WS_MAC

setUnifiedTitleAndToolBarOnMac(true);

#endif

// unter symbian os die statusleiste verbergen
#ifdef Q_OS_SYMBIAN

statusBar()->hide();

#endif

// unter macos soll menu_datei samt inhalt nicht zu sehen sein wenn nur action_schliessen enthalten ist
#ifdef Q_OS_MAC

action_schliessen->setVisible(false);
if (menu_datei->actions().size() == 1 && menu_datei->actions().first() == action_schliessen) menu_datei->menuAction()->setVisible(false);

#endif

// das status label erzeugen und in der status leiste verankern
statuslabel = new QLabel(this);
statusBar()->addPermanentWidget(statuslabel);

// den zufallsgenerator initialisieren
Random::initialisiere();

// die ki erstellen ...
ki = new KI();

// ... einen thread fuer ki erzeugen ...
ki_thread = new QThread(this);

// ... ki seinem thread zuteilen ...
ki->moveToThread(ki_thread);

// ... und zuguterletzt den thread starten
ki_thread->start();

// die ki helfer dateien laden
connect(this, SIGNAL(ladeauftrag_ki_helferdaten()), ki, SLOT(lade_ki_helfer()));
emit ladeauftrag_ki_helferdaten();

// die button liste aufbauen
buttons.append(button01);
buttons.append(button02);
buttons.append(button03);
buttons.append(button04);
buttons.append(button05);
buttons.append(button06);
buttons.append(button07);
buttons.append(button08);
buttons.append(button09);

// text der spiel buttons leeren
leere_text();

// einstellungen erstellen
einstellungen = new QSettings("konarski-wuppertal", qApp->applicationName(), this);

// den timer fuer die erste groessenanpassung erstellen. muss nur einmal ausgefuehrt werden
timer_erste_groessenanpassung = new QTimer(this);
timer_erste_groessenanpassung->setInterval(1000);
timer_erste_groessenanpassung->setSingleShot(true);

info_dialog = new QMessageBox(this);
info_dialog->setWindowModality(Qt::WindowModal);
info_dialog->setWindowTitle(tr("TicTacToe"));
info_dialog->setText(tr("Should not see me!"));
info_dialog->addButton(QMessageBox::Ok);

// signal - slot verbindungen
// action_schliessen ermoeglichen
connect(action_schliessen, SIGNAL(triggered(bool)), this, SLOT(close()));

// action_about ermoeglichen
connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));

// action_about_qt ermoeglichen
connect(action_about_qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

// die spiel buttons anschliessen
connect(button01, SIGNAL(clicked()), this, SLOT(annahme_button_01()));
connect(button02, SIGNAL(clicked()), this, SLOT(annahme_button_02()));
connect(button03, SIGNAL(clicked()), this, SLOT(annahme_button_03()));
connect(button04, SIGNAL(clicked()), this, SLOT(annahme_button_04()));
connect(button05, SIGNAL(clicked()), this, SLOT(annahme_button_05()));
connect(button06, SIGNAL(clicked()), this, SLOT(annahme_button_06()));
connect(button07, SIGNAL(clicked()), this, SLOT(annahme_button_07()));
connect(button08, SIGNAL(clicked()), this, SLOT(annahme_button_08()));
connect(button09, SIGNAL(clicked()), this, SLOT(annahme_button_09()));

// den reset button anschliessen
connect(reset_button, SIGNAL(clicked()), this, SLOT(reset()));

// vorwaerts und rueckwaerts ermoeglichen
connect(action_rueckgaengig, SIGNAL(triggered(bool)), this, SLOT(action_rueckgaengig_klick()));
connect(action_wiederholen, SIGNAL(triggered(bool)), this, SLOT(action_wiederholen_klick()));

// anfordern des ki zugs ermoeglichen
connect(this, SIGNAL(anforderung_ki_zug(QString)), ki, SLOT(anforderung_ki_zug(QString)));

// das annehmen des zugs der ki ermoeglichen
connect(ki, SIGNAL(errechneter_zug(int)), this, SLOT(fuehre_ki_zug_aus(int)));

// sperren und entsperren in bezug auf berechnung ermoeglichen
connect(ki, SIGNAL(berechnung_start()), this, SLOT(sperrung_berechnung_start()));
connect(ki, SIGNAL(berechnung_ende()), this, SLOT(entsperrung_berechnung_ende()));

// die erste groessenanpassung per timer ermoeglichen
connect(timer_erste_groessenanpassung, SIGNAL(timeout()), this, SLOT(loese_groessenanpassung_aus()));

// die einstellungen laden
einstellungen_laden();

// direkt einen reset zwecks start eines neuen spiels ausfuehren
reset();
}


void MainWindow::about()
{
QMessageBox::about(this, tr("About TicTacToe"), tr("TicTacToe version %1 \n\nAuthor:\tAndreas Konarski\nLicence:\tgpl v3 or later\n\nContact:\n\nprogrammieren@konarski-wuppertal.de\nwww.konarski-wuppertal.de").arg(VERSION));
}


MainWindow::~MainWindow()
{
// die einstellungen speichern
einstellungen_speichern();

// beendung des ki threads erfragen
ki_thread->quit();

// beendung des ki threads abwarten
ki_thread->wait();

// das ki objekt muss manuell geloescht werden
delete ki;
}


void MainWindow::einstellungen_laden()
{
// die fenstergeometrie laden
restoreGeometry(einstellungen->value("MainWindow/geometrie").toByteArray());
}


void MainWindow::einstellungen_speichern()
{
// die fenstergeometrie speichern
einstellungen->setValue("MainWindow/geometrie", saveGeometry());
}


void MainWindow::leere_text()
{
// alle elemente leeren. ein leeres element ist als ein leerzeichen definiert
for (register int idx = 0; idx < buttons.size(); idx++) buttons[idx]->setText(" ");
}


void  MainWindow::klickVerarbeitung(QPushButton *button)
{
// hier auch pruefen ob gewonnen und weiterspielen moeglich ist
if (button->text() == " " && weiterspielen_moeglich() && !gewonnen() && naechstes_zeichen == "O")
{
if (zugliste_position < zugliste.size())
{
while (zugliste.size() >  zugliste_position) zugliste.removeLast();
}

// einen zug hinzufuegen und den index fuer die zugliste erhoehen
zugliste.append(button);
zugliste_position++;

// den zug fertig verarbeiten
verarbeite_zug(button);

// aktualiseren ob vor / zurueck verfuegbar ist
aktualisiere_verfuegbarkeit_vor_zurueck();

// hier den zug der ki anfordern
emit anforderung_ki_zug(zustand_spielfeld());
}
}


QString MainWindow::get_naechstes_zeichen()
{
QString erg = naechstes_zeichen;

if (naechstes_zeichen == "X") naechstes_zeichen = "O";
else naechstes_zeichen = "X";

return erg;
}


void MainWindow::reset()
{
// alle felder entsperren
entsperre();

// spieler 2 soll beginnen
naechstes_zeichen = "X";
            
// text aller buttons zurueck setzen
leere_text();

// zugliste leeren und index resetten
zugliste.clear();
zugliste_position = 0;

// aktualiseren ob vor / zurueck verfuegbar ist
aktualisiere_verfuegbarkeit_vor_zurueck();

// wer dran ist aktualisieren
if (naechstes_zeichen == "X") statuslabel->setText(tr("The ki is thinking ..."));
else if (naechstes_zeichen == "O") statuslabel->setText("Make your move.");

// der erste ki zug ist ein zufaelliger zug
fuehre_ki_zug_aus(Random::random(0, 8));
}


bool MainWindow::weiterspielen_moeglich()
{
bool erg = false;

// es kann nur weiter gespielt werden wenn noch ein feld leer ist
for (register int idx = 0; idx < buttons.size() && erg == false; idx++) if (buttons.at(idx)->text() == " ") erg = true;

return erg;
}


bool MainWindow::gewonnen(const QString& spielerstring)
{
bool erg = false;

if (buttons.at(0)->text() == spielerstring && buttons.at(1)->text() == spielerstring && buttons.at(2)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(0)->setPalette(palette);
buttons.at(1)->setPalette(palette);
buttons.at(2)->setPalette(palette);
}

else if (buttons.at(3)->text() == spielerstring && buttons.at(4)->text() == spielerstring && buttons.at(5)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(3)->setPalette(palette);
buttons.at(4)->setPalette(palette);
buttons.at(5)->setPalette(palette);
}

else if (buttons.at(6)->text() == spielerstring && buttons.at(7)->text() == spielerstring && buttons.at(8)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(6)->setPalette(palette);
buttons.at(7)->setPalette(palette);
buttons.at(8)->setPalette(palette);
}

else if (buttons.at(0)->text() == spielerstring && buttons.at(3)->text() == spielerstring && buttons.at(6)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(0)->setPalette(palette);
buttons.at(3)->setPalette(palette);
buttons.at(6)->setPalette(palette);
}

else if (buttons.at(1)->text() == spielerstring && buttons.at(4)->text() == spielerstring && buttons.at(7)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(1)->setPalette(palette);
buttons.at(4)->setPalette(palette);
buttons.at(7)->setPalette(palette);
}

else if (buttons.at(2)->text() == spielerstring && buttons.at(5)->text() == spielerstring && buttons.at(8)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(2)->setPalette(palette);
buttons.at(5)->setPalette(palette);
buttons.at(8)->setPalette(palette);
}

else if (buttons.at(4)->text() == spielerstring && buttons.at(0)->text() == spielerstring && buttons.at(8)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(4)->setPalette(palette);
buttons.at(0)->setPalette(palette);
buttons.at(8)->setPalette(palette);
}

else if (buttons.at(4)->text() == spielerstring && buttons.at(2)->text() == spielerstring && buttons.at(6)->text() == spielerstring)
{
erg = true;

// die felder, die den sieg verursacht haben gruen faerben
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::green);

buttons.at(4)->setPalette(palette);
buttons.at(2)->setPalette(palette);
buttons.at(6)->setPalette(palette);
}

else erg = false;

return erg;
}


bool MainWindow::gewonnen()
{
return (gewonnen("X") || gewonnen("O"));
}


void MainWindow::entsperre()
{
steuere_zugang(true);
}


void MainWindow::sperre()
{
steuere_zugang(false);
}


void MainWindow::steuere_zugang(bool zustand)
{
for (register int idx = 0; idx < buttons.size(); idx++)
{
buttons[idx]->setEnabled(zustand);

if (zustand || (buttons.at(idx)->palette().color(QPalette::Button) != Qt::green && buttons.at(idx)->palette().color(QPalette::Button) != Qt::blue && buttons.at(idx)->palette().color(QPalette::Button) != Qt::red))
{
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::white);

buttons[idx]->setPalette(palette);
}
}
}


void MainWindow::action_rueckgaengig_klick()
{
// wenn noch nicht am anfang der zugliste angekommen
if (zugliste_position > 1)
{
// wenn der momentane zustand des spiel anzeigt das jemand gewonnen hat muessen alle leeren felder entsperrt werden und die einfaerbungen der siegfelder rueckgaengig gemacht werden
if (gewonnen())
{
entsperre_unfertige();
faerbe_siegfelder_zurueck();
}

// position aktualiseren
zugliste_position--;

QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::white);

// feld zuruecksetzen um den zug rueckgaengig zu machen
zugliste[zugliste_position]->setText(" ");
zugliste[zugliste_position]->setEnabled(true);
zugliste[zugliste_position]->setPalette(palette);

// dafuer sorgen das beim naechsten zug das richtige zeichen verwendet wird
get_naechstes_zeichen();

// verfuegbarkeit der vor und zurueck buttons aktualisieren
aktualisiere_verfuegbarkeit_vor_zurueck();

// position aktualiseren
zugliste_position--;

// feld zuruecksetzen um den zug rueckgaengig zu machen
zugliste[zugliste_position]->setText(" ");
zugliste[zugliste_position]->setEnabled(true);
zugliste[zugliste_position]->setPalette(palette);

// dafuer sorgen das beim naechsten zug das richtige zeichen verwendet wird
get_naechstes_zeichen();

// verfuegbarkeit der vor und zurueck buttons aktualisieren
aktualisiere_verfuegbarkeit_vor_zurueck();

// die info wer am zug ist aktualisieren
if (naechstes_zeichen == "X") statuslabel->setText(tr("The ki is thinking ..."));
else if (naechstes_zeichen == "O") statuslabel->setText("Make your move.");
}
}


void MainWindow::action_wiederholen_klick()
{
// wenn noch nicht am ende der zugliste angekommen
if ((zugliste_position + 1) < zugliste.size())
{
// den dem index gemaessen naechsten zug aus der zugliste verarbeiten
verarbeite_zug(zugliste.at(zugliste_position));

// index erhoehen
zugliste_position++;

// den dem index gemaessen naechsten zug aus der zugliste verarbeiten
verarbeite_zug(zugliste.at(zugliste_position));

// index erhoehen
zugliste_position++;

// verfuegbarkeit der vor und zurueck buttons aktualisieren
aktualisiere_verfuegbarkeit_vor_zurueck();
}
}


// diese methode wird verwendet wenn ein spieler einen neuen zug ausfuehrt und wenn ein zug aus der zugliste mittels "wiederholen" wieder hergestellt wird
void MainWindow::verarbeite_zug(QPushButton* tempButton)
{
// dem button das naechste zeichen setzen
tempButton->setText(get_naechstes_zeichen());

// den button sperren
tempButton->setEnabled(false);

// farbe entsprechen dem zeichen (und somit dem spieler) setzen
if (tempButton->text() == "X")
{
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::blue);

tempButton->setPalette(palette);
}

else if (tempButton->text() == "O")
{
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::red);

tempButton->setPalette(palette);
}

// testen ob gewonnen
if (gewonnen("X"))
{
sperre();

// darueber informieren das spieler 1 gewonnen hat
statuslabel->setText(tr("The KI wins!"));

// den info dialog anpassen und anzeigen
info_dialog->setText(tr("The KI wins!"));
info_dialog->show();
}

else if (gewonnen("O"))
{
sperre();

// darueber informieren das spieler 1 gewonnen hat
statuslabel->setText(tr("You have won!"));

// den info dialog anpassen und anzeigen
info_dialog->setText(tr("You have won!"));
info_dialog->show();
}

// testen ob weiter spielen moeglich
if (!gewonnen() && !weiterspielen_moeglich())
{
// darueber informieren das keiner gewonnen hat
statuslabel->setText(tr("No winner!"));

// den info dialog anpassen und anzeigen
info_dialog->setText(tr("No winner!"));
info_dialog->show();
}

// status meldung
else if (!gewonnen() && weiterspielen_moeglich())
{
// darueber informieren wer am zug ist
if (naechstes_zeichen == "X") statuslabel->setText(tr("The ki is thinking ..."));
else if (naechstes_zeichen == "O") statuslabel->setText(tr("Make your move."));
}
}


// unfertige (leere) felder sperren
void MainWindow::entsperre_unfertige()
{
for (register int idx = 0; idx < buttons.size(); idx++)
{
QPalette palette(buttons.at(idx)->palette());

palette.setColor(QPalette::ButtonText, Qt::black);

if (buttons.at(idx)->text() == " ") buttons[idx]->setEnabled(true);

buttons[idx]->setPalette(palette);
}
}


void MainWindow::aktualisiere_verfuegbarkeit_vor_zurueck()
{
if (zugliste_position > 1 && zugliste.size() > 0) action_rueckgaengig->setEnabled(true);
else action_rueckgaengig->setEnabled(false);

if ((zugliste_position + 1) < zugliste.size() && zugliste.size() > 0) action_wiederholen->setEnabled(true);
else action_wiederholen->setEnabled(false);
}


void MainWindow::faerbe_siegfelder_zurueck()
{
for (register int idx = 0; idx < buttons.size(); idx++)
{
if (buttons.at(idx)->palette().color(QPalette::Button) == Qt::green && buttons.at(idx)->text() == "X")
{
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::blue);

buttons[idx]->setPalette(palette);
}

else if (buttons.at(idx)->palette().color(QPalette::Button) == Qt::green && buttons.at(idx)->text() == "O")
{
QPalette palette;

palette.setColor(QPalette::ButtonText, Qt::black);
palette.setColor(QPalette::Button, Qt::red);

buttons[idx]->setPalette(palette);
}
}
}


QString MainWindow::zustand_spielfeld()
{
QString erg = "";

for (register int idx = 0; idx < buttons.size(); idx++) erg += buttons.at(idx)->text();

return erg;
}



void MainWindow::fuehre_ki_zug_aus(int feld)
{
QPushButton *tempButton = 0;

if (feld >= 0 && feld < buttons.size() && naechstes_zeichen == "X") tempButton = buttons[feld];

// hier auch pruefen ob gewonnen und weiterspielen moeglich ist
if (tempButton != 0 && tempButton->text() == " " && weiterspielen_moeglich() && !gewonnen())
{
if (zugliste_position < zugliste.size())
{
while (zugliste.size() >  zugliste_position) zugliste.removeLast();
}

// einen zug hinzufuegen und den index fuer die zugliste erhoehen
zugliste.append(tempButton);
zugliste_position++;

// den zug fertig verarbeiten
verarbeite_zug(tempButton);

// aktualiseren ob vor / zurueck verfuegbar ist
aktualisiere_verfuegbarkeit_vor_zurueck();
}
}


void MainWindow::annahme_button_01()
{
klickVerarbeitung(button01);
}


void MainWindow::annahme_button_02()
{
klickVerarbeitung(button02);
}


void MainWindow::annahme_button_03()
{
klickVerarbeitung(button03);
}


void MainWindow::annahme_button_04()
{
klickVerarbeitung(button04);
}


void MainWindow::annahme_button_05()
{
klickVerarbeitung(button05);
}


void MainWindow::annahme_button_06()
{
klickVerarbeitung(button06);
}


void MainWindow::annahme_button_07()
{
klickVerarbeitung(button07);
}


void MainWindow::annahme_button_08()
{
klickVerarbeitung(button08);
}


void MainWindow::annahme_button_09()
{
klickVerarbeitung(button09);
}


void MainWindow::setze_font_spielbuttons(double wert)
{
for (register int idx = 0; idx < buttons.size(); idx++)
{
QFont font = buttons[idx]->font();

font.setPointSize(wert);

buttons[idx]->setFont(font);
}
}


void MainWindow::resizeEvent(QResizeEvent* ereignis)
{
QMainWindow::resizeEvent(ereignis);

loese_groessenanpassung_aus();
}


void MainWindow::sperrung_berechnung_start()
{
for (int idx = 0; idx < buttons.size(); idx++)
{
buttons[idx]->setEnabled(false);
}

reset_button->setEnabled(false);
action_rueckgaengig->setEnabled(false);
action_wiederholen->setEnabled(false);
}


void MainWindow::entsperrung_berechnung_ende()
{
if (!gewonnen() && weiterspielen_moeglich())
for (int idx = 0; idx < buttons.size(); idx++)
{
if (buttons.at(idx)->text() == " ") buttons[idx]->setEnabled(true);
}

reset_button->setEnabled(true);
aktualisiere_verfuegbarkeit_vor_zurueck();
}


void MainWindow::loese_groessenanpassung_aus()
{
#ifdef  Q_WS_MAC

setze_font_spielbuttons(button01->height() * 0.65);

#else

setze_font_spielbuttons(button01->height() * 0.40);

#endif
}


void MainWindow::showEvent(QShowEvent* ereignis)
{
QMainWindow::showEvent(ereignis);

timer_erste_groessenanpassung->start();
}
