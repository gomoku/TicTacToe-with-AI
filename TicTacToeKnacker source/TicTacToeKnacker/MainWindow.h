#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "0.02"

#include <QMainWindow>
#include "ui_MainWindow.h"
#include <QFile>

class QSettings;
class TicTacToeLoeser;
class QThread;
class QFileDialog;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT

public:
MainWindow(QWidget *parent = 0);
~MainWindow();

private slots:
void about();
void einstellungen_laden();
void einstellungen_speichern();
void nehme_spielverlauf_an_spieler_01_gewonnen(QString);
void nehme_spielverlauf_an_spieler_02_gewonnen(QString);
void nehme_spielverlauf_an_patt(QString);
void nehme_dateinamen_an(const QString&);
void los();

signals:
void starte();

private:
QSettings *einstellungen;

TicTacToeLoeser *loeser;
QThread *loeser_thread;
QFile datei_spieler_01_gewonnen, datei_spieler_02_gewonnen, datei_patt;
QFileDialog *datei_dialog;
};

#endif
