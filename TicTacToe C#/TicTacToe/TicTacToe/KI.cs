using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace TicTacToe
{
    class KI
    {
        public KI()
        {
            HelferSpieler02KI = new List<string>();
            HelferSpieler01KI = new List<string>();
            HelferPattKI = new List<string>();

            string dateiname = "..\\..\\HelferSpieler02KI.txt", zeile;
            StreamReader leser = null;

            // datei nur oeffnen wenn vorhanden
            if (File.Exists(dateiname)) leser = new StreamReader(dateiname);
            else MessageBox.Show("Die Datei \"" + dateiname + "\" konnte nicht geöffnet werden!");

            // solange lesen bis die gesamte datei fertig eingelesen ist
            while ((zeile = leser.ReadLine()) != null) HelferSpieler02KI.Add(zeile);

            // den leser schliessen und auf den naechsten ladevorgang vorbereiten
            leser.Close();
            leser = null;

            dateiname = "..\\..\\HelferSpieler01KI.txt";
            zeile = "";
            
            // datei nur oeffnen wenn vorhanden
            if (File.Exists(dateiname)) leser = new StreamReader(dateiname);
            else MessageBox.Show("Die Datei \"" + dateiname + "\" konnte nicht geöffnet werden!");

            // solange lesen bis die gesamte datei fertig eingelesen ist
            while ((zeile = leser.ReadLine()) != null) HelferSpieler01KI.Add(zeile);

            // den leser schliessen und auf den naechsten ladevorgang vorbereiten
            leser.Close();
            leser = null;

            dateiname = "..\\..\\HelferPattKI.txt";
            zeile = "";

            // datei nur oeffnen wenn vorhanden
            if (File.Exists(dateiname)) leser = new StreamReader(dateiname);
            else MessageBox.Show("Die Datei \"" + dateiname + "\" konnte nicht geöffnet werden!");

            // solange lesen bis die gesamte datei fertig eingelesen ist
            while ((zeile = leser.ReadLine()) != null) HelferPattKI.Add(zeile);

            // den leser schliessen
            leser.Close();
        }


        public int gebe_naechsten_zug(string zustand_spielfeld)
        {
            // das ergebnis mit einem unmoeglichen wert vorinitialisieren
            int erg = -1;

            // alle moeglichen zustaende nach dem naechsten zug erfassen
            List<string> moeglichkeiten = potentielle_zuege(zustand_spielfeld);
            
            // testen ob eine dieser moeglichkeiten unmittelbar zum sieg fuehrt. dies ist noetig da die ki ansonsten zu defensive spiel und spiele unentschieden abschliesst, obwohl ein sieg in greifbarer naehe lag.
            int position_sieg = finde_sieg_moeglichkeit(moeglichkeiten);

            // wenn keine der moeglichkeiten zum unmittelbaren sieg fuehrte die ki bemuehen
            if (position_sieg == -1)
            {
                // den naechsten zustand des spielfelds bestimmen der in den meissten faellen zum sieg fuehrt ...
                string ziel_zustand = haeufigster_sieg(moeglichkeiten, HelferSpieler01KI, HelferSpieler02KI);

                // ... wenn nichts gefunden wurde stattdessen auf patt spielen
                if (ziel_zustand.Length == 0)
                {
                    ziel_zustand = haeufigster_sieg(moeglichkeiten, HelferPattKI, HelferSpieler02KI);
                }

                // aus dem ursprungs zustand und dem ziel zustand den noetigen zug bestimmen
                erg = erzeuge_zug_aus_differenz(zustand_spielfeld, ziel_zustand);
            }

            else erg = erzeuge_zug_aus_differenz(zustand_spielfeld, moeglichkeiten.ElementAt(position_sieg));

            return erg;
        }


        private int zaehle_vorkommen(string was, List<string> wo)
        {
            int erg = 0;

            for (int idx = 0; idx < wo.Count(); idx++)
            {
                if (wo.ElementAt(idx) == was) erg++;
            }

            return erg;
        }


        List<string> potentielle_zuege(string zustand)
        {
            List<string> erg = new List<string>();

            for (int idx = 0; idx < zustand.Length; idx++)
            {
                if (zustand.ElementAt(idx) == ' ')
                {
                    char[] tmp_char = zustand.ToArray();

                    tmp_char[idx] = 'X';

                    erg.Add(new string(tmp_char));
                }
            }

            return erg;
        }


        int erzeuge_zug_aus_differenz(string vorher, string nachher)
        {
            int erg = -1;

            for (int idx = 0; erg == -1 && idx < vorher.Length && idx < nachher.Length; idx++)
            {
                if (vorher.ElementAt(idx) != nachher.ElementAt(idx)) erg = idx;
            }

            return erg;
        }


        string haeufigster_sieg(List<string> kandidaten, List<string> wo_meins, List<string> wo_gegner)
        {
            string erg = "";
            int treffer = 0;

            for (int idx_kandidaten = 0; idx_kandidaten < kandidaten.Count(); idx_kandidaten++)
            {
                int treffer_meins = zaehle_vorkommen(kandidaten.ElementAt(idx_kandidaten), wo_meins);
                int treffer_gegner = zaehle_vorkommen(kandidaten.ElementAt(idx_kandidaten), wo_gegner);

                if (treffer_meins > treffer_gegner && treffer_meins > treffer)
                {
                    treffer = treffer_meins;
                    erg = kandidaten.ElementAt(idx_kandidaten);
                }
            }

            return erg;
        }


        private bool gewonnen(string spielstring)
        {
            bool erg = false;

            if (spielstring.ElementAt(0) == 'X' && spielstring.ElementAt(1) == 'X' && spielstring.ElementAt(2) == 'X') erg = true;

            else if (spielstring.ElementAt(3) == 'X' && spielstring.ElementAt(4) == 'X' && spielstring.ElementAt(5) == 'X') erg = true;

            else if (spielstring.ElementAt(6) == 'X' && spielstring.ElementAt(7) == 'X' && spielstring.ElementAt(8) == 'X') erg = true;

            else if (spielstring.ElementAt(0) == 'X' && spielstring.ElementAt(3) == 'X' && spielstring.ElementAt(6) == 'X') erg = true;

            else if (spielstring.ElementAt(1) == 'X' && spielstring.ElementAt(4) == 'X' && spielstring.ElementAt(7) == 'X') erg = true;

            else if (spielstring.ElementAt(2) == 'X' && spielstring.ElementAt(5) == 'X' && spielstring.ElementAt(8) == 'X') erg = true;

            else if (spielstring.ElementAt(4) == 'X' && spielstring.ElementAt(0) == 'X' && spielstring.ElementAt(8) == 'X') erg = true;

            else if (spielstring.ElementAt(4) == 'X' && spielstring.ElementAt(2) == 'X' && spielstring.ElementAt(6) == 'X') erg = true;

            else erg = false;

            return erg;
        }


        int finde_sieg_moeglichkeit(List<string> moeglichkeiten)
        {
            int erg = -1;

            for (int idx = 0; idx < moeglichkeiten.Count() && erg == -1; idx++)
            {
                if (gewonnen(moeglichkeiten.ElementAt(idx))) erg = idx;
            }

            return erg;
        }

        List<string> HelferSpieler02KI, HelferSpieler01KI, HelferPattKI;
    }
}
