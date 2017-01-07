/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_DISCOR - Fonction de détection temporisée de discordance entre deux valeurs ToR ou analogiques                        29/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG)                                          29/06/2015 *
 *                IN1 : Première valeur à comparer                                                                                           *
 *                IN2 : Deuxième valeur à comparer                                                                                           *
 *                DIF : Différence maximum tolérée entre IN1 et IN2                                                                          *
 *                TPO : [0] Compteur de temporisation                                                                                        *
 *                      [1] Tempo de confirmation de la discordance                                                                          *
 *                      [2] Temps de cycle automate calculé par la fonction                                                                  *
 *                      [3] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                      *
 *                RES : [0] Discordance de l'équipement - forcément un tableau de taille au moins 1                                          *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES maintenu à false donc en discordance                                                                        *
 *                      0  : RES figée dans son dernier état connu                                                                           *
 *                      1  : RES est true sauf si IN1 et IN2 diffèrent de plus de DIF pendant plus de CPT[1] sans discontinuer               *
 *                      2  : RES maintenu à true en permanence                                                                               *
 *                      3  : RES maintenu à true en permanence                                                                               *
 *                      >3 : RES maintenu à false                                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Arduino Leonardo, Arduino Uno                                                                                      *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 29/06/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/

 // Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
boolean FCO_XXXX;                        // C_DISCOR - Demande de marche de l'équipement
boolean ED2_MGL_XXXX;                    // C_DISCOR - Etat de la voie de suivi de marche
boolean FCO_REAR;                        // C_DISCOR - Demande de réarmement
boolean FDM_XXXX[1];                     // C_DISCOR - Etat de la commande à transmettre à la sortie
float VTT_XXXX[4];                       // C_DISCOR - Table de mesure du temps de discordance

void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_DISCOR - Affectation de la voie 00 en tant qu'entrée de retour de marche
  pinMode(3, INPUT);                     // C_DISCOR - Affectation de la voie 01 en tant qu'entrée de demande de marche
  pinMode(4, INPUT);                     // C_DISCOR - Affectation de la voie 04 en tant que BP de réarmement
  VTT_XXXX[1] = 10;                      // C_DISCOR - Paramétrage de la temporisations de confirmation de discordance
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}

void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { ED2_MGL_XXXX = true; } else { ED2_MGL_XXXX = false; } // Entrée de retour de marche NO (shunt = marche)
  if (digitalRead(3) == LOW) { FCO_XXXX = true; } else { FCO_XXXX = false; }         // Entrée de demande de marche NO (shunt = demande)
  if (digitalRead(4) == LOW) { FCO_REAR = true; } else { FCO_REAR = false; }         // Entrée de réarmement NO (shunt = demande)
  if (FCO_REAR) { FDM_XXXX[0] = true; }
  BMS.C_DISCOR(FCO_XXXX, ED2_MGL_XXXX, 0.5, VTT_XXXX, FDM_XXXX, 1 - (2 * (FDM_XXXX[0] == false)));
  Serial.print("Commande : ");
  Serial.print("\t");
  Serial.print(FCO_XXXX);
  Serial.print("\t");
  Serial.print("Retour : ");
  Serial.print("\t");
  Serial.print(ED2_MGL_XXXX);
  Serial.print("\t");
  Serial.print("Rearmement : ");
  Serial.print("\t");
  Serial.print(FCO_REAR);
  Serial.print("\t");
  Serial.print("Compteur T : ");
  Serial.print("\t");
  Serial.print(VTT_XXXX[0]);
  Serial.print("\t");
  Serial.print("Discordance : ");
  Serial.print("\t");
  Serial.println(FDM_XXXX[0]);
}