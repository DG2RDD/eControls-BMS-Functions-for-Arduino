/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_TFONCT - Fonction de calcul de temps de fonctionnement                                                                12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_TFONCT(boolean MAR, float RES[], int CFG)                                                                             12/06/2015 *
 *                MAR : Booléen d'état de marche de l'équipement                                                                             *
 *                RES : [0]  : Compteur d'heures de marche de l'équipement                                                                   *
 *                      [1]  : Mémoire de millis() du cycle précédent                                                                        *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES[0] annulé                                                                                                   *
 *                      0  : RES[0] figée à sa valeur courante                                                                               *
 *                      1  : RES[0] totalise le temps pendant lequel MAR est True en heures                                                  *
 *                      >1 : RES[0] annulé                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Arduino Leonardo, Arduino Uno, Industruino                                                                         *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 27/05/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * 27/05/2015 : DD, SE2R : Cette fonction nécessite l'utilisation de S_SCANTS pour accéder à la valeur du cycle automate                     *
 * 12/06/2015 : DD, SE2R : Passage de RES en tableau, simplification du code, Ajout des commentaires d'appartenance à C_TFONCT               *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
boolean ED2_MGL_XXXX;                    // C_TFONCT - Etat de l'entrée de retour de marche
int VCO_XXXX;                            // C_TFONCT - Commande de forçage, position auto
float VTO_XXXX[2];                       // C_TFONCT - Table de calcul du temps de fonctionnement de l'équipement
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_TFONCT - Affectation de la voie 02 en tant qu'entrée
  VCO_XXXX = 1;                          // C_TFONCT - Préparamétrage en auto du commutateur de forçage
  Serial.begin(9600);                    // Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == HIGH) { ED2_MGL_XXXX = true; } else { ED2_MGL_XXXX = false; } // Entrée de marche NO
  BMS.C_TFONCT(ED2_MGL_XXXX, VTO_XXXX, VCO_XXXX);
  Serial.print("Etat de marche : ");
  Serial.print("\t");
  Serial.print(ED2_MGL_XXXX);
  Serial.print("\t");
  Serial.print("Compteur T.Marche : ");
  Serial.print("\t");
  Serial.println(VTO_XXXX[0]);
}
