#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "0.09"

#include <QMainWindow>
#include "ui_MainWindow.h"
#include <QList>

class QSettings;
class KI;
class QLabel;
class QThread;
class QTimer;
class QMessageBox;

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
void leere_text();
void klickVerarbeitung(QPushButton*);
void reset();
void entsperre_unfertige();
void entsperre();
void sperre();
void steuere_zugang(bool);
void action_rueckgaengig_klick();
void action_wiederholen_klick();
void verarbeite_zug(QPushButton*);
void aktualisiere_verfuegbarkeit_vor_zurueck();
void faerbe_siegfelder_zurueck();
void fuehre_ki_zug_aus(int);
void annahme_button_01();
void annahme_button_02();
void annahme_button_03();
void annahme_button_04();
void annahme_button_05();
void annahme_button_06();
void annahme_button_07();
void annahme_button_08();
void annahme_button_09();
void setze_font_spielbuttons(double);
void sperrung_berechnung_start();
void entsperrung_berechnung_ende();
void loese_groessenanpassung_aus();

signals:
void anforderung_ki_zug(QString);
void ladeauftrag_ki_helferdaten();

private:
QSettings *einstellungen;
QLabel *statuslabel;
QList<QPushButton*> buttons, zugliste;
QString naechstes_zeichen;
int zugliste_position;
KI *ki;
QThread *ki_thread;
QTimer *timer_erste_groessenanpassung;
QMessageBox *info_dialog;

QString get_naechstes_zeichen();
bool weiterspielen_moeglich();
bool gewonnen(const QString&);
bool gewonnen();
QString zustand_spielfeld();
virtual void resizeEvent(QResizeEvent*);
virtual void showEvent(QShowEvent*);
};

#endif
