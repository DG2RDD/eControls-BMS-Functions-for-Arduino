/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_COMMUT - Fonction de calcul de variable de forçage                                                                    12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[])                                                 12/06/2015 *
 *                FX1 : Demande de marche en auto (RES = 1) si true, débrayé (RES = 0) si false                                              *
 *                FX2 : Demande de forçage intermédiaire (RES = 2), prioritaire sur FX1                                                      *
 *                FX3 : Demande de forçage en marche renforcé (RES = 3), prioritaire sur FX1 et FX2                                          *
 *                FX0 : Demande de forçage à l'arrêt (RES = -1), prioritaire sur tout autre forçage                                          *
 *                RES : [0] Commande de l'équipement - forcément un tableau de taille au moins 1                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Arduino Leonardo, Arduino Uno, Industruino                                                                         *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 14/05/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *                                                                                    *
 * 03/06/2015 : DD, SE2R : Passage de RES en tableau et simplification du code                                                               *
 * 12/06/2015 : DD, SE2R : Ajout des commentaires d'appartenance à C_COMMUT                                                                  *
 * ----------------------------------------------------------------------------------------------------------------------------------------- */
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution de la fonction -----------------------------------------------------------------------*
boolean FCX_XXXX[4];                     // C_COMMUT - Commandes de forçage extérnes
int VCO_XXXX[0];                         // C_COMMUT - Forçage final
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_COMMUT - Affectation de la voie 00 en tant qu'entrée "Auto"
  pinMode(3, INPUT);                     // C_COMMUT - Affectation de la voie 01 en tant qu'entrée "F2"
  pinMode(4, INPUT);                     // C_COMMUT - Affectation de la voie 02 en tant qu'entrée "F3"
  pinMode(5, INPUT);                     // C_COMMUT - Affectation de la voie 03 en tant qu'entrée "Arrêt"
  Serial.begin(9600);                    // Port COM - setup serial
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { FCX_XXXX[0] = true; } else { FCX_XXXX[0] = false; }
  if (digitalRead(3) == LOW) { FCX_XXXX[1] = true; } else { FCX_XXXX[1] = false; }
  if (digitalRead(4) == LOW) { FCX_XXXX[2] = true; } else { FCX_XXXX[2] = false; }
  if (digitalRead(5) == LOW) { FCX_XXXX[3] = true; } else { FCX_XXXX[3] = false; }
  BMS.C_COMMUT(FCX_XXXX[0], FCX_XXXX[1], FCX_XXXX[2], FCX_XXXX[3], VCO_XXXX);
  Serial.print("Auto : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[0]);
  Serial.print("\t");
  Serial.print("Manu 1 : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[1]);
  Serial.print("\t");
  Serial.print("Manu 2 : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[2]);
  Serial.print("\t");
  Serial.print("Arret : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[3]);
  Serial.print("\t");
  Serial.print("Forçage : ");
  Serial.print("\t");
  Serial.println(VCO_XXXX[0]);
}