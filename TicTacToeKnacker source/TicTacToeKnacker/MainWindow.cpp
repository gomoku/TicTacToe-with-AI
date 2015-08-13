#include "MainWindow.h"
#include "TicTacToeLoeser.h"
#include <QSettings>
#include <QMessageBox>
#include <QThread>
#include <QFile>
#include <QFileDialog>

// debug - entfernen ?
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), einstellungen(0), datei_spieler_01_gewonnen(), datei_spieler_02_gewonnen(), datei_patt()
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

// einstellungen erstellen
einstellungen = new QSettings("konarski-wuppertal", qApp->applicationName(), this);

// den loeser erstellen ...
loeser = new TicTacToeLoeser();

// einen thread fuer den loeser erstellen und dem thread uebergeben
loeser_thread = new QThread(this);
loeser->moveToThread(loeser_thread);

loeser_thread->start();

datei_dialog = new QFileDialog(this);
datei_dialog->setWindowModality(Qt::WindowModal);
datei_dialog->setAcceptMode(QFileDialog::AcceptSave);
datei_dialog->setDirectory(QDir::home());
datei_dialog->setDefaultSuffix("txt");

// signal - slot verbindungen
// action_schliessen ermoeglichen
connect(action_schliessen, SIGNAL(triggered(bool)), this, SLOT(close()));

// action_about ermoeglichen
connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));

// action_about_qt ermoeglichen
connect(action_about_qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

// das annehmen von ergebnissen ermoeglichen
connect(loeser, SIGNAL(spiel_zuende_spieler_01_gewonnen(QString)), this, SLOT(nehme_spielverlauf_an_spieler_01_gewonnen(QString)));
connect(loeser, SIGNAL(spiel_zuende_spieler_02_gewonnen(QString)), this, SLOT(nehme_spielverlauf_an_spieler_02_gewonnen(QString)));
connect(loeser, SIGNAL(spiel_zuende_patt(QString)), this, SLOT(nehme_spielverlauf_an_patt(QString)));

// das starten ermoeglichen
connect(los_button, SIGNAL(clicked()), this, SLOT(los()));
connect(this, SIGNAL(starte()), loeser, SLOT(start()));

// das oeffnen eines datei dialogs ermoeglichen
connect(speichern_button, SIGNAL(clicked()), datei_dialog, SLOT(show()));

// das annehmen von dateinamen aus dem datei dialog ermoeglichen
connect(datei_dialog, SIGNAL(fileSelected(const QString&)), this, SLOT(nehme_dateinamen_an(const QString&)));

einstellungen_laden();
}


void MainWindow::about()
{
QMessageBox::about(this, tr("About TicTacToeKnacker."), tr("TicTacToeKnacker version %1 \n\nAuthor:\tAndreas Konarski\nLicence:\tgpl v3 or later\n\nContact:\n\nprogrammieren@konarski-wuppertal.de\nwww.konarski-wuppertal.de").arg(VERSION));
}


MainWindow::~MainWindow()
{
einstellungen_speichern();

loeser_thread->quit();
loeser_thread->wait();

delete loeser;

datei_spieler_01_gewonnen.close();
datei_spieler_02_gewonnen.close();
datei_patt.close();
}


void MainWindow::einstellungen_laden()
{

}


void MainWindow::einstellungen_speichern()
{

}


void MainWindow::nehme_spielverlauf_an_spieler_01_gewonnen(QString txt)
{
datei_spieler_01_gewonnen.write(txt.toAscii());

qDebug() << txt;
}


void MainWindow::nehme_spielverlauf_an_spieler_02_gewonnen(QString txt)
{
datei_spieler_02_gewonnen.write(txt.toAscii());

qDebug() << txt;
}


void MainWindow::nehme_spielverlauf_an_patt(QString txt)
{
datei_patt.write(txt.toAscii());

qDebug() << txt;
}


void MainWindow::nehme_dateinamen_an(const QString& dateiname)
{
QString tmp(dateiname);
tmp.remove(".txt");

dateiname_spieler1_sieg->setText(tmp + "_Player_1_won.txt");
dateiname_spieler2_sieg->setText(tmp + "_Player_2_won.txt");
dateiname_patt->setText(tmp + "_patt.txt");
}


void MainWindow::los()
{
datei_spieler_01_gewonnen.setFileName(dateiname_spieler1_sieg->text());
datei_spieler_02_gewonnen.setFileName(dateiname_spieler2_sieg->text());
datei_patt.setFileName(dateiname_patt->text());

if (datei_spieler_01_gewonnen.open(QIODevice::WriteOnly) && datei_spieler_02_gewonnen.open(QIODevice::WriteOnly) && datei_patt.open(QIODevice::WriteOnly))
{
emit starte();
}

else qDebug() << tr("Cant open files in \"void MainWindow::los()\"!");
}
