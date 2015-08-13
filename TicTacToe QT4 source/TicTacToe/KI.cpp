#include "KI.h"
#include <QFile>
#include <QDebug>

using namespace std;

KI::KI(QObject *parent) : QObject(parent)
{
}


KI::~KI()
{
}


int KI::gebe_naechsten_zug(const QString& zustand_spielfeld)
{
// das ergebnis mit einem unmoeglichen wert vorinitialisieren
int erg = -1;

// alle moeglichen zustaende nach dem naechsten zug erfassen
QList<QString> moeglichkeiten = potentielle_zuege(zustand_spielfeld);
            
// testen ob eine dieser moeglichkeiten unmittelbar zum sieg fuehrt. dies ist noetig da die ki ansonsten zu defensive spiel und spiele unentschieden abschliesst, obwohl ein sieg in greifbarer naehe lag.
int position_sieg = finde_sieg_moeglichkeit(moeglichkeiten);

// wenn keine der moeglichkeiten zum unmittelbaren sieg fuehrte die ki bemuehen
if (position_sieg == -1)
{
// den naechsten zustand des spielfelds bestimmen der in den meissten faellen zum sieg fuehrt ...
QString ziel_zustand = haeufigster_sieg(moeglichkeiten, HelferSpieler01KI, HelferSpieler02KI);

// ... wenn nichts gefunden wurde stattdessen auf patt spielen
if (ziel_zustand.size() == 0)
{
ziel_zustand = haeufigster_sieg(moeglichkeiten, HelferPattKI, HelferSpieler02KI);
}

// aus dem ursprungs zustand und dem ziel zustand den noetigen zug bestimmen
erg = erzeuge_zug_aus_differenz(zustand_spielfeld, ziel_zustand);
}

else erg = erzeuge_zug_aus_differenz(zustand_spielfeld, moeglichkeiten.at(position_sieg));

return erg;
}


int KI::zaehle_vorkommen(const QString& was, const QList<QString>& wo)
{
return wo.count(was);
}


QList<QString> KI::potentielle_zuege(const QString& zustand)
{
QList<QString> erg;

for (int idx = 0; idx < zustand.size(); idx++)
{
if (zustand.at(idx) == ' ')
{
QString tmp(zustand);

tmp[idx] = 'X';

erg.append(tmp);
}
}

return erg;
}


int KI::erzeuge_zug_aus_differenz(const QString& vorher, const QString& nachher)
{
int erg = -1;

for (int idx = 0; erg == -1 && idx < vorher.size() && idx < nachher.size(); idx++)
{
if (vorher.at(idx) != nachher.at(idx)) erg = idx;
}

return erg;
}


QString KI::haeufigster_sieg(const QList<QString>& kandidaten, const QList<QString>& wo_meins, const QList<QString>& wo_gegner)
{
QString erg = "";
int treffer = 0;

for (int idx_kandidaten = 0; idx_kandidaten < kandidaten.size(); idx_kandidaten++)
{
int treffer_meins = zaehle_vorkommen(kandidaten.at(idx_kandidaten), wo_meins);
int treffer_gegner = zaehle_vorkommen(kandidaten.at(idx_kandidaten), wo_gegner);

if (treffer_meins > treffer_gegner && treffer_meins > treffer)
{
treffer = treffer_meins;
erg = kandidaten.at(idx_kandidaten);
}
}

return erg;
}


bool KI::gewonnen(const QString& spielstring)
{
bool erg = false;

if (spielstring.at(0) == 'X' && spielstring.at(1) == 'X' && spielstring.at(2) == 'X') erg = true;

else if (spielstring.at(3) == 'X' && spielstring.at(4) == 'X' && spielstring.at(5) == 'X') erg = true;

else if (spielstring.at(6) == 'X' && spielstring.at(7) == 'X' && spielstring.at(8) == 'X') erg = true;

else if (spielstring.at(0) == 'X' && spielstring.at(3) == 'X' && spielstring.at(6) == 'X') erg = true;

else if (spielstring.at(1) == 'X' && spielstring.at(4) == 'X' && spielstring.at(7) == 'X') erg = true;

else if (spielstring.at(2) == 'X' && spielstring.at(5) == 'X' && spielstring.at(8) == 'X') erg = true;

else if (spielstring.at(4) == 'X' && spielstring.at(0) == 'X' && spielstring.at(8) == 'X') erg = true;

else if (spielstring.at(4) == 'X' && spielstring.at(2) == 'X' && spielstring.at(6) == 'X') erg = true;

else erg = false;

return erg;
}


int KI::finde_sieg_moeglichkeit(const QList<QString>& moeglichkeiten)
{
int erg = -1;

for (int idx = 0; idx < moeglichkeiten.size() && erg == -1; idx++)
{
if (gewonnen(moeglichkeiten.at(idx))) erg = idx;
}

return erg;
}


void KI::anforderung_ki_zug(QString zustand)
{
emit berechnung_start();

emit errechneter_zug(gebe_naechsten_zug(zustand));

emit berechnung_ende();
}


void KI::lade_ki_helfer()
{
QString dateiname = ":/ki/HelferPattKI.txt";
QFile leser(dateiname);

// datei nur oeffnen wenn vorhanden
if (!QFile::exists(dateiname)) qDebug() << tr("Missing \"%1\"!").arg(dateiname);

if (leser.open(QIODevice::ReadOnly))
{

// solange lesen bis die gesamte datei fertig eingelesen ist
while (!leser.atEnd())
{
QString zeile = QString::fromAscii(leser.readLine());

HelferPattKI.append(zeile.remove('\n'));
}

// den leser schliessen und auf den naechsten ladevorgang vorbereiten
leser.close();
}

else qDebug() << tr("Unable to open \"%1\"!").arg(dateiname);

dateiname = ":/ki/HelferSpieler01KI.txt";
leser.setFileName(dateiname);

// datei nur oeffnen wenn vorhanden
if (!QFile::exists(dateiname)) qDebug() << tr("Missing \"%1\"!").arg(dateiname);

// solange lesen bis die gesamte datei fertig eingelesen ist
if (leser.open(QIODevice::ReadOnly))
{

// solange lesen bis die gesamte datei fertig eingelesen ist
while (!leser.atEnd())
{
QString zeile = QString::fromAscii(leser.readLine());

HelferSpieler01KI.append(zeile.remove('\n'));
}

// den leser schliessen und auf den naechsten ladevorgang vorbereiten
leser.close();
}

else qDebug() << tr("Unable to open \"%1\"!").arg(dateiname);

dateiname = ":/ki/HelferSpieler02KI.txt";
leser.setFileName(dateiname);

// datei nur oeffnen wenn vorhanden
if (!QFile::exists(dateiname)) qDebug() << tr("Missing \"%1\"!").arg(dateiname);

if (leser.open(QIODevice::ReadOnly))
{

// solange lesen bis die gesamte datei fertig eingelesen ist
while (!leser.atEnd())
{
QString zeile = QString::fromAscii(leser.readLine());

HelferSpieler02KI.append(zeile.remove('\n'));
}

// den leser schliessen und auf den naechsten ladevorgang vorbereiten
leser.close();
}

else qDebug() << tr("Unable to open \"%1\"!").arg(dateiname);
}
