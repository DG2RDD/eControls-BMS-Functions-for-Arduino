/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_PIDITR - Fonction de régulation par correction PID itérative                                                          24/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_PIDITR(float MES, float CSG, float VPC[], float VKP[], int CFG)                                                       24/06/2015 *
 *                MES : Mesure de la grandeur de régulation                                                                                  *
 *                CSG : Consigne de régulation                                                                                               *
 *                VPC : [00] : Signal de commande de régulation                                                                              *
 *                      [01] : Mémoire de l'écart du cycle précédent                                                                         *
 *                      [02] : Mémoire de la variation d'écart du cycle précédent                                                            *
 *                      [03] : Valeur de repli de la commande (CFG = 2)                                                                      *
 *                VKR : [00] : Durée du cycle du régulateur (s)                                                                              *
 *                      [01] : Coefficient de transfert (Unités / %)                                                                         *
 *                      [02] : Bande proportionnelle (%)                                                                                     *
 *                      [03] : Temps intégral (s)                                                                                            *
 *                      [04] : Temps dérivé (s)                                                                                              *
 *                      [05] : Ecart (Unités)                                                                                                *
 *                      [06] : Durée du cycle automate (s)                                                                                   *
 *                      [07] : Compteur de durée depuis la dernière exécution de la fonction (s)                                             *
 *                      [08] : Mémoire des millis() de la dernière exécution de la fonction                                                  *
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
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 24/06/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
float E00_TDR_XXXX;                      // R_PIDITR - Mesure de régulation
float VCR_XXXX;                          // R_PIDITR - Consigne de régulation
float VPC_XXXX[4];                       // R_PIDITR - Table des commandes
float VKR_XXXX[9];                       // R_PIDITR - Table des coefficients de régulation
int VCO_XXXX;                            // R_PIDITR - Commande de forçage, ici en position auto
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  VPC_XXXX[3] = 50;                      // R_PIDITR - Valeur de repli en forçage pour VCO = 2
  VKR_XXXX[0] = 1;                       // R_PIDITR - Coefficient : Durée du cycle du régulateur (s)
  VKR_XXXX[1] = 90;                      // R_PIDITR - Coefficient : Plage d'action du régulateur (Unités de mesure)
  VKR_XXXX[2] = 150;                     // R_PIDITR - Coefficient : Bande proportionnelle (%)
  VKR_XXXX[3] = 300;                     // R_PIDITR - Coefficient : Temps intégral (s)
  VKR_XXXX[4] = 0;                       // R_PIDITR - Coefficient : Temps dérivé (s)
  VKR_XXXX[5] = 0;                       // R_PIDITR - Coefficient : Limiteur d'écart (Unités de mesure)
  VCO_XXXX = 1;                          // R_PIDITR - Commande de forçage
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  E00_TDR_XXXX = analogRead(0) / 10.23;                                              // Lecture 0-100% de l'entrée analogique 0
  BMS.R_PIDITR(E00_TDR_XXXX, VCR_XXXX, VPC_XXXX, VKR_XXXX, VCO_XXXX);
  Serial.println(VPC_XXXX[0]);
}
