/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PT - Fonction de transformation d'un pourcentage en commande 3-Points                                             25/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG)                                                           25/06/2015 *
 *                VPC : [00] : Signal de commande de régulation à convertir                                                                  *
 *                      [01] : Signal de commande retranscrit sur l'actionneur 3-points en fin du cycle courant                              *
 *                VTC : [00] : Durée du cycle de l'action 3-Points (s)                                                                       *
 *                      [01] : Durée totale de la pleine course de l'actionneur (s)                                                          *
 *                      [02] : Calcul du temps de cycle automate (s)                                                                         *
 *                      [03] : Compteur de durée depuis la dernière exécution de la fonction (s)                                             *
 *                      [04] : Mémoire des millis() de la dernière exécution de la fonction                                                  *
 *                      [05] : Durée de l'action sur les sorties pour le cycle d'exécution en cours                                          *
 *                VNO : Numéro de la voie de la sortie de commande d'ouverture  de l'actionneur 3-points                                     *
 *                VNF : Numéro de la voie de la sortie de commande de fermeture de l'actionneur 3-points                                     *
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
float VPC_XXXX[2];                       // R_ANA3PT - Table des commandes
float VTC_XXXX[6];                       // R_ANA3PT - Table des coefficients de régulation
int VNO_OUVR;                            // R_ANA3PT - Numéro de voie de la sortie de commande d'ouverture
int VNF_FERM;                            // R_ANA3PT - Numéro de voie de la sortie de commande de fermeture
int VCO_XXXX;                            // R_ANA3PT - Commande de forçage, ici en position auto
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  VTC_XXXX[0] = 30;                      // R_ANA3PT - Durée du cycle de rafraîchissement de l'action sur la vanne
  VTC_XXXX[1] = 180;                     // R_ANA3PT - Durée de la course complète de la vanne
  VCO_XXXX = 1;                          // R_ANA3PT - Commande de forçage
  pinMode(7, OUTPUT);                    // Système  - Affectation de la voie 07 en tant que sortie de commande d'ouverture
  pinMode(8, OUTPUT);                    // Système  - Affectation de la voie 08 en tant que sortie de commande de fermeture
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  VPC_XXXX[0] = analogRead(0) / 10.23;                                              // Lecture 0-100% de l'entrée analogique 0
  BMS.R_ANA3PT(VPC_XXXX, VTC_XXXX, 7, 8, VCO_XXXX);
  Serial.print(VTC_XXXX[3]);
  Serial.print("\t");
  Serial.print(VTC_XXXX[5]);
  Serial.print("\t");
  Serial.print(VPC_XXXX[0]);
  Serial.print("\t");
  Serial.println(VPC_XXXX[1]);
}
