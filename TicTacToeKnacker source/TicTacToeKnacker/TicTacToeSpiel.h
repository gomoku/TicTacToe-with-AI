#ifndef TICTACTOESPIEL_H
#define TICTACTOESPIEL_H

#include <QObject>
#include <QList>

class TicTacToeSpiel : public QObject
{
Q_OBJECT

public:
TicTacToeSpiel(QObject *parent = 0);
TicTacToeSpiel(TicTacToeSpiel* anderes, QObject *parent = 0);
virtual ~TicTacToeSpiel();

bool spiel_zuende() const;
bool leeres_feld_ueber() const;
bool spieler1_gewonnen() const;
bool spieler2_gewonnen() const;
QString spielverlauf() const;
bool spieler1_ist_dran() const;
bool spieler2_ist_dran() const;
void fuehre_zug_aus(int);
const QList<unsigned char>& felder_von_spieler1() const;
const QList<unsigned char>& felder_von_spieler2() const;
const QList<unsigned char>& uebrige_felder() const;

public slots:

signals:


private slots:

private:
QList<unsigned char> moeglichkeiten, gehoert_spieler1, gehoert_spieler2;
QString Spielverlauf;
bool spieler1_dran;

QString erzeuge_spielfeld() const;
};

#endif
