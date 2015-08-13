#include "TicTacToeLoeser.h"
#include "TicTacToeSpiel.h"
#include <QDebug>

using namespace std;

TicTacToeLoeser::TicTacToeLoeser(QObject *parent) : QObject(parent)
{
}


TicTacToeLoeser::~TicTacToeLoeser()
{
}


void TicTacToeLoeser::start()
{
TicTacToeSpiel *tmp_spiel = new TicTacToeSpiel(this);

rekursion(tmp_spiel);
}


void TicTacToeLoeser::rekursion(TicTacToeSpiel *spiel)
{
if (spiel->spiel_zuende())
{
if (spiel->spieler1_gewonnen()) emit spiel_zuende_spieler_01_gewonnen(spiel->spielverlauf());
else if (spiel->spieler2_gewonnen()) emit spiel_zuende_spieler_02_gewonnen(spiel->spielverlauf());
else spiel_zuende_patt(spiel->spielverlauf());

spiel->deleteLater();
}

else
{
QList<unsigned char> moeglichkeiten(spiel->uebrige_felder());

for (int idx = 0; idx < moeglichkeiten.size(); idx++)
{
TicTacToeSpiel *tmp_spiel = new TicTacToeSpiel(spiel, this);

tmp_spiel->fuehre_zug_aus(moeglichkeiten.at(idx));

rekursion(tmp_spiel);
}
}
}
