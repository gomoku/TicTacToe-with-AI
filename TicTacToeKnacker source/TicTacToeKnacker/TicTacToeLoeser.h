#ifndef TICTACTOELOESER_H
#define TICTACTOELOESER_H

#include <QObject>

class TicTacToeSpiel;

class TicTacToeLoeser : public QObject
{
Q_OBJECT

public:
TicTacToeLoeser(QObject *parent = 0);
~TicTacToeLoeser();

public slots:
void start();

signals:
void spiel_zuende_spieler_01_gewonnen(QString);
void spiel_zuende_spieler_02_gewonnen(QString);
void spiel_zuende_patt(QString);

private slots:
void rekursion(TicTacToeSpiel *spiel);

private:
};

#endif
