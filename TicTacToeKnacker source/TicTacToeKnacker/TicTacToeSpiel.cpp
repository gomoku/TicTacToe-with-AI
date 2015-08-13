#include "TicTacToeSpiel.h"
#include <QDebug>

using namespace std;

TicTacToeSpiel::TicTacToeSpiel(QObject *parent) : QObject(parent), spieler1_dran(true)
{
// die moeglichkeiten aufbauen
for (int idx = 0; idx < 9; idx++) moeglichkeiten.append(idx);
}


TicTacToeSpiel::~TicTacToeSpiel()
{
}


bool TicTacToeSpiel::spiel_zuende() const
{
return (spieler1_gewonnen() || spieler2_gewonnen() || !leeres_feld_ueber());
}


bool TicTacToeSpiel::spieler1_gewonnen() const
{
bool erg = false;

if (gehoert_spieler1.contains(0) && gehoert_spieler1.contains(1) && gehoert_spieler1.contains(2)) erg = true;
else if (gehoert_spieler1.contains(3) && gehoert_spieler1.contains(4) && gehoert_spieler1.contains(5)) erg = true;
else if (gehoert_spieler1.contains(6) && gehoert_spieler1.contains(7) && gehoert_spieler1.contains(8)) erg = true;

else if (gehoert_spieler1.contains(0) && gehoert_spieler1.contains(3) && gehoert_spieler1.contains(6)) erg = true;
else if (gehoert_spieler1.contains(1) && gehoert_spieler1.contains(4) && gehoert_spieler1.contains(7)) erg = true;
else if (gehoert_spieler1.contains(2) && gehoert_spieler1.contains(5) && gehoert_spieler1.contains(8)) erg = true;

else if (gehoert_spieler1.contains(0) && gehoert_spieler1.contains(4) && gehoert_spieler1.contains(8)) erg = true;
else if (gehoert_spieler1.contains(2) && gehoert_spieler1.contains(4) && gehoert_spieler1.contains(6)) erg = true;

return erg;
}


bool TicTacToeSpiel::spieler2_gewonnen() const
{
bool erg = false;

if (gehoert_spieler2.contains(0) && gehoert_spieler2.contains(1) && gehoert_spieler2.contains(2)) erg = true;
else if (gehoert_spieler2.contains(3) && gehoert_spieler2.contains(4) && gehoert_spieler2.contains(5)) erg = true;
else if (gehoert_spieler2.contains(6) && gehoert_spieler2.contains(7) && gehoert_spieler2.contains(8)) erg = true;

else if (gehoert_spieler2.contains(0) && gehoert_spieler2.contains(3) && gehoert_spieler2.contains(6)) erg = true;
else if (gehoert_spieler2.contains(1) && gehoert_spieler2.contains(4) && gehoert_spieler2.contains(7)) erg = true;
else if (gehoert_spieler2.contains(2) && gehoert_spieler2.contains(5) && gehoert_spieler2.contains(8)) erg = true;

else if (gehoert_spieler2.contains(0) && gehoert_spieler2.contains(4) && gehoert_spieler2.contains(8)) erg = true;
else if (gehoert_spieler2.contains(2) && gehoert_spieler2.contains(4) && gehoert_spieler2.contains(6)) erg = true;

return erg;
}


QString TicTacToeSpiel::spielverlauf() const
{
return Spielverlauf;
}


bool TicTacToeSpiel::leeres_feld_ueber() const
{
return !moeglichkeiten.isEmpty();
}


bool TicTacToeSpiel::spieler1_ist_dran() const
{
return spieler1_dran;
}


bool TicTacToeSpiel::spieler2_ist_dran() const
{
return !spieler1_dran;
}


void TicTacToeSpiel::fuehre_zug_aus(int feld)
{
if (!spiel_zuende() && moeglichkeiten.contains(feld))
{
if (spieler1_ist_dran())
{
gehoert_spieler1.append(moeglichkeiten.takeAt(moeglichkeiten.lastIndexOf(feld)));
}

else if (spieler2_ist_dran())
{
gehoert_spieler2.append(moeglichkeiten.takeAt(moeglichkeiten.lastIndexOf(feld)));
}

spieler1_dran = !spieler1_dran;
Spielverlauf += erzeuge_spielfeld();
}
}


QString TicTacToeSpiel::erzeuge_spielfeld() const
{
QString erg;

for (int idx = 0; idx < 9; idx++)
{
if (moeglichkeiten.contains(idx)) erg += " ";
else if (gehoert_spieler1.contains(idx)) erg += "X";
else if (gehoert_spieler2.contains(idx)) erg += "O";
else qDebug() << tr("Unknown field owner in \"const QString TicTacToeSpiel::erzeuge_spielfeld()\"!");
}

erg += "\n";

return erg;
}


TicTacToeSpiel::TicTacToeSpiel(TicTacToeSpiel* anderes, QObject *parent) : QObject(parent), moeglichkeiten(anderes->uebrige_felder()), gehoert_spieler1(anderes->felder_von_spieler1()), gehoert_spieler2(anderes->felder_von_spieler2()), Spielverlauf(anderes->spielverlauf()), spieler1_dran(anderes->spieler1_ist_dran())
{

}


const QList<unsigned char>& TicTacToeSpiel::felder_von_spieler1() const
{
return gehoert_spieler1;
}


const QList<unsigned char>& TicTacToeSpiel::felder_von_spieler2() const
{
return gehoert_spieler2;
}


const QList<unsigned char>& TicTacToeSpiel::uebrige_felder() const
{
return moeglichkeiten;
}



