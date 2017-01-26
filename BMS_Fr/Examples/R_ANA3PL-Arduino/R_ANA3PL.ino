/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PL - Fonction de transformation d'un pourcentage en commande 3-Points sur deux points binaires                    30/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PL(float VPC[], float VTC[], boolean FCC[], int CFG)                                                              30/06/2015 *
 *                VPC : [00] : Signal de commande de régulation à convertir                                                                  *
 *                      [01] : Signal de commande retranscrit sur l'actionneur 3-points en fin du cycle courant                              *
 *                VTC : [00] : Durée du cycle de l'action 3-Points (s)                                                                       *
 *                      [01] : Durée totale de la pleine course de l'actionneur (s)                                                          *
 *                      [02] : Calcul du temps de cycle automate (s)                                                                         *
 *                      [03] : Compteur de durée depuis la dernière exécution de la fonction (s)                                             *
 *                      [04] : Mémoire des millis() de la dernière exécution de la fonction                                                  *
 *                      [05] : Durée de l'action sur les sorties pour le cycle d'exécution en cours                                          *
 *                FCC : [00] : Commande bolléenne d'ouverture  de l'actionneur 3-points                                                      *
 *                      [01] : Commande bolléenne de fermeture de l'actionneur 3-points                                                      *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VPC[0] maintenu à 0                                                                                             *
 *                      0  : VPC[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VPC[0] est fixé par le calcul de régulation PID                                                                 *
 *                      2  : VPC[0] maintenue à VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue à 100                                                                                          *
 *                      >3 : VPC[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] : Mémoire dans l'exécution de l'écart du cycle précédent                                                        *
 *                      [01] : Mémoire dans l'exécution de la variation d'écart du cycle précédent                                           *
 *                      [02] : Mémoire dans l'exécution de la variation d'écart du cycle précédent                                           *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Arduino Leonardo, Arduino Uno, Industruino                                                                         *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 30/06/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
float VPC_XXXX[2];                       // R_ANA3PL - Table des commandes en sortie du régulateur
float VTC_XXXX[6];                       // R_ANA3PL - Table des temporisations de commande de la vanne 3 points
boolean FCC_XXXX[2];                     // R_ANA3PL - Table des commandes de sorties de la vanne 3 points
int VCO_XXXX;                            // R_ANA3PL - Commande de forçage, ici en position auto
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  VTC_XXXX[0] = 30;                      // R_ANA3PL - Durée du cycle de rafraîchissement de l'action sur la vanne
  VTC_XXXX[1] = 180;                     // R_ANA3PL - Durée de la course complète de la vanne
  VCO_XXXX = 1;                          // R_ANA3PL - Commande de forçage
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  VPC_XXXX[0] = analogRead(0) / 10.23;                                              // Lecture 0-100% de l'entrée analogique 0
  BMS.R_ANA3PL(VPC_XXXX, VTC_XXXX, FCC_XXXX, VCO_XXXX);
  Serial.print(VTC_XXXX[3]);
  Serial.print("\t");
  Serial.print(VTC_XXXX[5]);
  Serial.print("\t");
  Serial.print(VPC_XXXX[0]);
  Serial.print("\t");
  Serial.println(VPC_XXXX[1]);
}
