/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANATOR - Fonction de transformation d'un pourcentage en commande PWM                                                  25/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANATOR(float VPC, int VTC[], int VNO, int CFG)                                                                        25/06/2015 *
 *                VPC : Signal de commande de régulation à convertir                                                                         *
 *                VTC : [00] : Durée totale de la pleine course de l'actionneur (s)                                                          *
 *                      [01] : Calcul du temps de cycle automate (s)                                                                         *
 *                      [02] : Compteur de durée depuis la dernière exécution de la fonction (s)                                             *
 *                      [03] : Mémoire des millis() de la dernière exécution de la fonction                                                  *
 *                      [04] : Durée de l'action sur les sorties pour le cycle d'exécution en cours                                          *
 *                VNC : Numéro de la voie de la sortie de commande d'ouverture  de l'actionneur PWM                                          *
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
 * 25/06/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
float VPC_XXXX;                          // R_ANATOR - Table des commandes
float VTC_XXXX[5];                       // R_ANATOR - Table des temporisations
int VNC_VANN;                            // R_ANATOR - Numéro de voie de la sortie de commande de 'actionneur PWM
int VCO_XXXX;                            // R_ANATOR - Commande de forçage, ici en position auto
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  VTC_XXXX[0] = 60;                      // R_ANATOR - Durée de la course complète de la vanne
  VCO_XXXX = 1;                          // R_ANATOR - Commande de forçage
  pinMode(7, OUTPUT);                    // Système  - Affectation de la voie 07 en tant que sortie de commande 1
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  VPC_XXXX = analogRead(0) / 10.23;                                              // Lecture 0-100% de l'entrée analogique 0
  BMS.R_ANATOR(VPC_XXXX, VTC_XXXX, 7, VCO_XXXX);
  Serial.print(VTC_XXXX[2]);
  Serial.print("\t");
  Serial.print(VTC_XXXX[4]);
  Serial.print("\t");
  Serial.println(VPC_XXXX);
}
