/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_CDELEM - Fonction de commande de booléen avec prise en compte de forçage et de défaut - Arduino                       12/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_CDELEM(boolean CDE, boolean DFT, boolean RES[], int CFG)                                                              12/06/2015 *
 *                CDE : Commande calculée ou donnée par l'utilisateur (peut être le résultat d'une fonction appelée plus haut)               *
 *                DFT : Défaut de l'équipement commandé. True signale un équipement en état normal. Un false signale un défaut               *
 *                RES : [00] Commande de l'équipement - forcément un tableau de taille au moins 1                                            *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES maintenu à false                                                                                            *
 *                      0  : RES figée dans son dernier état connu                                                                           *
 *                      1  : RES soumise à CDE et à DEF                                                                                      *
 *                      2  : RES soumise seulement à DEF (comme si CDE était true en permanence)                                             *
 *                      3  : RES  maintenu à true                                                                                            *
 *                      >3 : RES maintenu à false                                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Arduino Leonardo, Arduino Uno, Industruino                                                                         *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 03/06/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * 12/06/2015 : DD, SE2R : Ajout des commentaires d'appartenance à C_CDELEM                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution de la fonction -----------------------------------------------------------------------*
boolean FCO_XXXX;                        // C_CDELEM - Commande de marche depuis l'extérieur
boolean ED2_DGL_XXXX;                    // C_CDELEM - Etat de l'information de défaut
boolean FCC_XXXX[1];                     // C_CDELEM - Etat de la commande à transmettre à la sortie
int VCO_XXXX;                            // C_CDELEM - Commande de forçage, ici en position auto
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  pinMode(0, INPUT);                     // C_CDELEM - Affectation de la voie 00 en tant qu'entrée
  pinMode(1, INPUT);                     // C_CDELEM - Affectation de la voie 01 en tant qu'entrée de commande
  pinMode(7, OUTPUT);                    // C_CDELEM - Affectation de la voie 07 en tant que sortie
  VCO_XXXX = 1;                          // C_CDELEM - Passage du forçage en position auto
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { ED2_DGL_XXXX = true; } else { ED2_DGL_XXXX = false; }  // Entrée de défaut NF (shunt = normal)
  if (digitalRead(3) == LOW) { FCO_XXXX = true; } else { FCO_XXXX = false; }          // Entrée de demande de marche NO (shunt = demande)
  BMS.C_CDELEM(FCO_XXXX, ED2_DGL_XXXX, FCC_XXXX, VCO_XXXX);
  if (FCC_XXXX[0]) { digitalWrite(7, HIGH); } else { digitalWrite(7, LOW); }
  Serial.print("Demande : ");
  Serial.print("\t");
  Serial.print(FCO_XXXX);
  Serial.print("\t");
  Serial.print("Defaut : ");
  Serial.print("\t");
  Serial.print(ED2_DGL_XXXX);
  Serial.print("\t");
  Serial.print("Commande : ");
  Serial.print("\t");
  Serial.println(FCC_XXXX[0]);
}
