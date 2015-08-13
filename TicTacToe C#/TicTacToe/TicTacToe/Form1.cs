using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TicTacToe
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            // gui elemente initialisieren
            InitializeComponent();

            // den zufallsgenerator fuer den ersten computer zug erzeugen
            zufallsgenerator = new Random();

            // die ki aufbauen
            ki = new KI();

            // aus der textdatei laden
            // die zugliste und ihren index aufbauen und initialisieren. wird fuer rueckgaenfig und wiederherstellen benoetigt.
            zugliste = new List<Button>();
            zugliste_position = 0;

            // button liste aufbauen
            buttons = new List<Button>();
            buttons.Add(button1);
            buttons.Add(button2);
            buttons.Add(button3);
            buttons.Add(button4);
            buttons.Add(button5);
            buttons.Add(button6);
            buttons.Add(button7);
            buttons.Add(button8);
            buttons.Add(button9);

            // font der buttons anpassen
            setze_font();

            // click handler der spiel buttons anschliessen
            event_setzen();

            // reset durchfuehren
            reset();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }


        private void leere_text()
        {
            // alle elemente leeren. ein leeres element ist als ein leerzeichen definiert
            for (int idx = 0; idx < buttons.Count(); idx++) buttons.ElementAt(idx).Text = " ";
        }


        private void setze_font()
        {
            // alle buttons mit dem neuen font versorgen
            for (int idx = 0; idx < buttons.Count(); idx++) buttons.ElementAt(idx).Font = new System.Drawing.Font("Microsoft Sans Serif", 40F);
        }


        private void event_setzen()
        {
            // den click handler aller spiel buttons anschliessen
            for (int idx = 0; idx < buttons.Count(); idx++) buttons.ElementAt(idx).Click += new System.EventHandler(ClickHandler);
        }


        private void ClickHandler(object sender, System.EventArgs e)
        {
            Button tempButton = (Button)sender;

            // hier auch pruefen ob gewonnen und weiterspielen moeglich ist
            if (tempButton.Text == " " && weiterspielen_moeglich() && !gewonnen() && naechstes_zeichen == "O")
            {
                if (zugliste_position < zugliste.Count()) zugliste.RemoveRange(zugliste_position, zugliste.Count() - zugliste_position);

                // einen zug hinzufuegen und den index fuer die zugliste erhoehen
                zugliste.Add(tempButton);
                zugliste_position++;

                // den zug fertig verarbeiten
                verarbeite_zug(tempButton);

                // aktualiseren ob vor / zurueck verfuegbar ist
                aktualisiere_verfuegbarkeit_vor_zurueck();

                // hier den zug der ki ausfuehren
                fuehre_ki_zug_aus(ki.gebe_naechsten_zug(zustand_spielfeld()));
            }
        }


        private string get_naechstes_zeichen()
        {
            string erg = naechstes_zeichen;

            if (naechstes_zeichen == "X") naechstes_zeichen = "O";
            else naechstes_zeichen = "X";

            return erg;
        }


        private void reset()
        {
            // alle felder entsperren
            entsperre();

            // spieler 2 soll beginnen
            naechstes_zeichen = "X";
            
            // text aller buttons zurueck setzen
            leere_text();

            // zugliste leeren und index resetten
            zugliste.Clear();
            zugliste_position = 0;

            // aktualiseren ob vor / zurueck verfuegbar ist
            aktualisiere_verfuegbarkeit_vor_zurueck();

            // wer dran ist aktualisieren
            if (naechstes_zeichen == "X") statuslabel.Text = "Spieler 1 ist am Zug!";
            else if (naechstes_zeichen == "O") statuslabel.Text = "Spieler 2 ist am Zug!";

            // der erste ki zug ist ein zufaelliger zug, sonst waehlt die ki immer das feld in der mitte, was auf dauer langweilig ist.
            fuehre_ki_zug_aus(zufallsgenerator.Next(0, 9));
        }


        private bool weiterspielen_moeglich()
        {
            bool erg = false;

            // so lange noch ein feld leer ist kann man weiter spielen
            for (int idx = 0; idx < buttons.Count() && erg == false; idx++) if (buttons.ElementAt(idx).Text == " ") erg = true;

            return erg;
        }


        private bool gewonnen(string spielerstring)
        {
            bool erg = false;

            if (buttons.ElementAt(0).Text == spielerstring && buttons.ElementAt(1).Text == spielerstring && buttons.ElementAt(2).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(0).BackColor = Color.Green;
                buttons.ElementAt(1).BackColor = Color.Green;
                buttons.ElementAt(2).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(3).Text == spielerstring && buttons.ElementAt(4).Text == spielerstring && buttons.ElementAt(5).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(3).BackColor = Color.Green;
                buttons.ElementAt(4).BackColor = Color.Green;
                buttons.ElementAt(5).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(6).Text == spielerstring && buttons.ElementAt(7).Text == spielerstring && buttons.ElementAt(8).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(6).BackColor = Color.Green;
                buttons.ElementAt(7).BackColor = Color.Green;
                buttons.ElementAt(8).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(0).Text == spielerstring && buttons.ElementAt(3).Text == spielerstring && buttons.ElementAt(6).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(0).BackColor = Color.Green;
                buttons.ElementAt(3).BackColor = Color.Green;
                buttons.ElementAt(6).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(1).Text == spielerstring && buttons.ElementAt(4).Text == spielerstring && buttons.ElementAt(7).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(1).BackColor = Color.Green;
                buttons.ElementAt(4).BackColor = Color.Green;
                buttons.ElementAt(7).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(2).Text == spielerstring && buttons.ElementAt(5).Text == spielerstring && buttons.ElementAt(8).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(2).BackColor = Color.Green;
                buttons.ElementAt(5).BackColor = Color.Green;
                buttons.ElementAt(8).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(4).Text == spielerstring && buttons.ElementAt(0).Text == spielerstring && buttons.ElementAt(8).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(4).BackColor = Color.Green;
                buttons.ElementAt(0).BackColor = Color.Green;
                buttons.ElementAt(8).BackColor = Color.Green;
            }

            else if (buttons.ElementAt(4).Text == spielerstring && buttons.ElementAt(2).Text == spielerstring && buttons.ElementAt(6).Text == spielerstring)
            {
                erg = true;

                // die felder, die den sieg verursacht haben gruen faerben
                buttons.ElementAt(4).BackColor = Color.Green;
                buttons.ElementAt(2).BackColor = Color.Green;
                buttons.ElementAt(6).BackColor = Color.Green;
            }

            else erg = false;

            return erg;
        }


        private bool gewonnen()
        {
            // wenn ein spieler gewonnen hat ist das spiel gewonnen
            return (gewonnen("X") || gewonnen("O"));
        }


        private void entsperre()
        {
            steuere_zugang(true);
        }


        private void sperre()
        {
            steuere_zugang(false);
        }


        private void steuere_zugang(bool zustand)
        {
            for (int idx = 0; idx < buttons.Count(); idx++)
            {
                buttons.ElementAt(idx).Enabled = zustand;

                if (zustand || (buttons.ElementAt(idx).BackColor != Color.Green && buttons.ElementAt(idx).BackColor != Color.Blue && buttons.ElementAt(idx).BackColor != Color.Red)) buttons.ElementAt(idx).BackColor = Color.White;

                buttons.ElementAt(idx).ForeColor = Color.Black;
            }
        }

        
        private void button10_Click(object sender, EventArgs e)
        {
            reset();
        }


        private void schliessenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void action_rueckgaengig_Click(object sender, EventArgs e)
        {
            // wenn noch nicht am anfang der zugliste angekommen
            if (zugliste_position > 1)
            {
                // wenn der momentane zustand des spiel anzeigt das jemand gewonnen hat muessen alle leeren felder entsperrt werden und die einfaerbungen der siegfelder rueckgaengig gemacht werden
                if (gewonnen())
                {
                    entsperre_unfertige();
                    faerbe_siegfelder_zurueck();
                }

                // position aktualiseren
                zugliste_position--;

                // feld zuruecksetzen um den zug rueckgaengig zu machen
                zugliste.ElementAt(zugliste_position).Text = " ";
                zugliste.ElementAt(zugliste_position).Enabled = true;
                zugliste.ElementAt(zugliste_position).BackColor = Color.White;

                // dafuer sorgen das beim naechsten zug das richtige zeichen verwendet wird
                get_naechstes_zeichen();

                // verfuegbarkeit der vor und zurueck buttons aktualisieren
                aktualisiere_verfuegbarkeit_vor_zurueck();

                // position aktualiseren
                zugliste_position--;

                // feld zuruecksetzen um den zug rueckgaengig zu machen
                zugliste.ElementAt(zugliste_position).Text = " ";
                zugliste.ElementAt(zugliste_position).Enabled = true;
                zugliste.ElementAt(zugliste_position).BackColor = Color.White;

                // dafuer sorgen das beim naechsten zug das richtige zeichen verwendet wird
                get_naechstes_zeichen();

                // verfuegbarkeit der vor und zurueck buttons aktualisieren
                aktualisiere_verfuegbarkeit_vor_zurueck();

                // die info wer am zug ist aktualisieren
                if (naechstes_zeichen == "X") statuslabel.Text = "Spieler 1 ist am Zug!";
                else if (naechstes_zeichen == "O") statuslabel.Text = "Spieler 2 ist am Zug!";
            }
        }

        private void action_wiederholen_Click(object sender, EventArgs e)
        {
            // wenn noch nicht am ende der zugliste angekommen
            if ((zugliste_position + 1) < zugliste.Count())
            {
            // den dem index gemaessen naechsten zug aus der zugliste verarbeiten
            verarbeite_zug(zugliste.ElementAt(zugliste_position));

            // index erhoehen
            zugliste_position++;

            // den dem index gemaessen naechsten zug aus der zugliste verarbeiten
            verarbeite_zug(zugliste.ElementAt(zugliste_position));

            // index erhoehen
            zugliste_position++;

            // verfuegbarkeit der vor und zurueck buttons aktualisieren
            aktualisiere_verfuegbarkeit_vor_zurueck();
            }
        }


        // diese methode wird verwendet wenn ein spieler einen neuen zug ausfuehrt und wenn ein zug aus der zugliste mittels "wiederholen" wieder hergestellt wird
        private void verarbeite_zug(Button tempButton)
        {
            // dem button das naechste zeichen setzen
            tempButton.Text = get_naechstes_zeichen();

            // den button sperren
            tempButton.Enabled = false;

            // farbe entsprechen dem zeichen (und somit dem spieler) setzen
            if (tempButton.Text == "X") tempButton.BackColor = Color.Blue;
            else if (tempButton.Text == "O") tempButton.BackColor = Color.Red;

            // die schrifft soll schwarz sein.
            tempButton.ForeColor = Color.Black;

            // testen ob gewonnen
            if (gewonnen("X"))
            {
                // alle felder sperren
                sperre();

                // darueber informieren das spieler 1 gewonnen hat
                statuslabel.Text = "Spieler 1 hat gewonnen!";
                MessageBox.Show("Spieler 1 hat gewonnen!");
            }

            else if (gewonnen("O"))
            {
                // alle felder sperren
                sperre();

                // darueber informieren das spieler 1 gewonnen hat
                statuslabel.Text = "Spieler 2 hat gewonnen!";
                MessageBox.Show("Spieler 2 hat gewonnen!");
            }

            // testen ob weiter spielen moeglich
            if (!gewonnen() && !weiterspielen_moeglich())
            {
                // darueber informieren das keiner gewonnen hat
                statuslabel.Text = "Patt!";
                MessageBox.Show("Patt!");
            }

            // status meldung
            else if (!gewonnen() && weiterspielen_moeglich())
            {
                // darueber informieren wer am zug ist
                if (naechstes_zeichen == "X") statuslabel.Text = "Spieler 1 ist am Zug!";
                else if (naechstes_zeichen == "O") statuslabel.Text = "Spieler 2 ist am Zug!";
            }
        }


        // unfertige (leere) felder sperren
        private void entsperre_unfertige()
        {
            for (int idx = 0; idx < buttons.Count(); idx++)
            {
                if (buttons.ElementAt(idx).Text == " ") buttons.ElementAt(idx).Enabled = true;
                buttons.ElementAt(idx).ForeColor = Color.Black;
            }
        }


        void aktualisiere_verfuegbarkeit_vor_zurueck()
        {
            if (zugliste_position > 1 && zugliste.Count() > 0) action_rueckgaengig.Enabled = true;
            else action_rueckgaengig.Enabled = false;

            if ((zugliste_position + 1) < zugliste.Count() && zugliste.Count() > 0) action_wiederholen.Enabled = true;
            else action_wiederholen.Enabled = false;
        }


        private void faerbe_siegfelder_zurueck()
        {
            for (int idx = 0; idx < buttons.Count(); idx++)
            {
                if (buttons.ElementAt(idx).BackColor == Color.Green && buttons.ElementAt(idx).Text == "X") buttons.ElementAt(idx).BackColor = Color.Blue;
                else if (buttons.ElementAt(idx).BackColor == Color.Green && buttons.ElementAt(idx).Text == "O") buttons.ElementAt(idx).BackColor = Color.Red;
            }
        }


        public string zustand_spielfeld()
        {
            string erg = "";

            for (int idx = 0; idx < buttons.Count(); idx++) erg += buttons.ElementAt(idx).Text;

            return erg;
        }



        private void fuehre_ki_zug_aus(int feld)
        {
            Button tempButton = null;
            
            if (feld >= 0 && feld < buttons.Count()) tempButton = buttons.ElementAt(feld);

            // hier auch pruefen ob gewonnen und weiterspielen moeglich ist
            if (tempButton != null && tempButton.Text == " " && weiterspielen_moeglich() && !gewonnen())
            {
                if (zugliste_position < zugliste.Count()) zugliste.RemoveRange(zugliste_position, zugliste.Count() - zugliste_position);

                // einen zug hinzufuegen und den index fuer die zugliste erhoehen
                zugliste.Add(tempButton);
                zugliste_position++;

                // den zug fertig verarbeiten
                verarbeite_zug(tempButton);

                // aktualiseren ob vor / zurueck verfuegbar ist
                aktualisiere_verfuegbarkeit_vor_zurueck();
            }
        }

        private List<Button> buttons;
        private string naechstes_zeichen;
        List<Button> zugliste;
        int zugliste_position;
        KI ki;
        Random zufallsgenerator;
    }
}
