#ifndef KI_H
#define KI_H

#include <QObject>
#include <QList>

class KI: public QObject
{
Q_OBJECT

public:
KI(QObject *parent = 0);
virtual ~KI();

public slots:
void anforderung_ki_zug(QString);
void lade_ki_helfer();

signals:
void errechneter_zug(int);
void berechnung_start();
void berechnung_ende();

private slots:

private:
QList<QString> HelferSpieler02KI, HelferSpieler01KI, HelferPattKI;

int gebe_naechsten_zug(const QString& zustand_spielfeld);
int zaehle_vorkommen(const QString& was, const QList<QString>& wo);
QList<QString> potentielle_zuege(const QString& zustand);
int erzeuge_zug_aus_differenz(const QString& vorher, const QString& nachher);
QString haeufigster_sieg(const QList<QString>& kandidaten, const QList<QString>& wo_meins, const QList<QString>& wo_gegner);
bool gewonnen(const QString& spielstring);
int finde_sieg_moeglichkeit(const QList<QString>& moeglichkeiten);
};

#endif
