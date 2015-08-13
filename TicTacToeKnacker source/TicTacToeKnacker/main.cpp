/*

Name: TicTacToeKnacker
Autor: Andreas Konarski
Begonnen: 20.12.2011.
Erstellt: 20.12.2011.
Version: 0.02
Lizenz: GPL v3 or later
Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist.

Kontakt: programmieren@konarski-wuppertal.de
home: www.konarski-wuppertal.de

Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.

*/

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
QApplication anwendung(argc, argv);

// die anwendung uebersetzen
QTranslator uebersetzer, uebersetzer2;

QString dateiname_eigene_uebersetzung;
dateiname_eigene_uebersetzung = QString(":/translations/Uebersetzung_%1").arg(QLocale::system().name());

// die uebersetzungs dateien in die uebersetzer laden
uebersetzer.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
uebersetzer2.load(dateiname_eigene_uebersetzung);

// die uebersetzer installieren
anwendung.installTranslator(&uebersetzer);
anwendung.installTranslator(&uebersetzer2);

// den anwendungsnamen setzen
anwendung.setApplicationName("TicTacToeKnacker");

// das hauptfenster erzeugen ...
MainWindow hauptfenster;

// ... und anzeigen
#ifdef Q_OS_SYMBIAN

hauptfenster.showMaximized();

#else

hauptfenster.show();

#endif

return anwendung.exec();
}
