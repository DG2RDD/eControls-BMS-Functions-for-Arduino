/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_CDETPO - Fonction de temporisation de commande à l'action et à l'arrêt                                                03/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_CDETPO(boolean CDE, boolean DFT, float CPT[], int RES[], int CFG)                                                     03/06/2015 *
 *                CDE : Demande calculée ou donnée par l'utilisateur (peut être le résultat d'une fonction appelée plus haut)                *
 *                DFT : Défaut de l'équipement commandé. True signale un équipement en état normal. Un false signale un défaut               *
 *                CPT : [00] Compteur de temporisation                                                                                       *
 *                      [01] Tempo entre le front montant de la demande CDE avant l'action sur la commande                                   *
 *                      [02] Tempo entre le front descendant de la demande CDE avant la désactivation de la commande                         *
 *                      [03] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [04] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Commande de l'équipement - forcément un tableau de taille au moins 1                                            *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES maintenu à false                                                                                            *
 *                      0  : RES figée dans son dernier état connu                                                                           *
 *                      1  : RES soumise à CDE (à TMA et TAR près) et à DFT                                                                  *
 *                      2  : RES soumise seulement à DFT (comme si CDE était true en permanence)                                             *
 *                      3  : RES  maintenu à true indépendamment de DFT et CDE                                                               *
 *                      >3 : RES maintenu à false                                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Arduino Leonardo, Arduino Uno                                                                                      *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 27/05/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * 27/05/2015 : DD, SE2R : Cette fonction nécessite l'utilisation de S_SCANTS pour accéder à la valeur du cycle automate                     *
 * 03/06/2015 : DD, SE2R : Intégration du calcul de temps de cycle, passage de la commande en boolean et simplification du code              *
 * 03/06/2015 : DD, SE2R : Passage en mode void et ajout des commentaires d'appartenance à C_CDETPO                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
boolean FCO_XXXX;                        // C_CDETPO - Demande de marche de l'équipement
boolean ED2_DGL_XXXX;                    // C_CDETPO - Etat de la voie de défaut
boolean FCC_XXXX[1];                     // C_CDETPO - Etat de la commande à transmettre à la sortie
float VTT_XXXX[5];                       // C_CDETPO - Table de mesure des temporisations
int VCO_XXXX;                            // C_CDETPO - Commande de forçage, position auto
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_CDETPO - Affectation de la voie 02 en tant qu'entrée de défaut
  pinMode(3, INPUT);                     // C_CDETPO - Affectation de la voie 03 en tant qu'entrée de commande
  pinMode(7, OUTPUT);                    // C_CDETPO - Affectation de la voie 07 en tant que sortie
  VTT_XXXX[1] = 2;                       // C_CDETPO - Paramétrage des temporisations à l'enclenchement
  VTT_XXXX[2] = 5;                       // C_CDETPO - Paramétrage des temporisations au déclenchement
  VCO_XXXX = 1;                          // C_CDETPO - Passage en automatique
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { ED2_DGL_XXXX = true; } else { ED2_DGL_XXXX = false; } // Entrée de défaut NF (shunt = normal)
  if (digitalRead(3) == LOW) { FCO_XXXX = true; } else { FCO_XXXX = false; }         // Entrée de demande de marche NO (shunt = demande)
  BMS.C_CDETPO(FCO_XXXX, ED2_DGL_XXXX, VTT_XXXX, FCC_XXXX, VCO_XXXX);
  if (FCC_XXXX[0]) { digitalWrite(7, HIGH); } else { digitalWrite(7, LOW); }
  Serial.print("Demande : ");
  Serial.print("\t");
  Serial.print(FCO_XXXX);
  Serial.print("\t");
  Serial.print("Defaut : ");
  Serial.print("\t");
  Serial.print(ED2_DGL_XXXX);
  Serial.print("\t");
  Serial.print("Compteur T : ");
  Serial.print("\t");
  Serial.print(VTT_XXXX[0]);
  Serial.print("\t");
  Serial.print("Commande : ");
  Serial.print("\t");
  Serial.println(FCC_XXXX[0]);
}
