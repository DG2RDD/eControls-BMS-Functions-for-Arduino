/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : 07/01/2017 - Fonctions Arduino pour systèmes de GTB - Fichier de code source                                        Version 1.02   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Fonction de cette bibliothèque                                                                                                            *
 *        C_CDELEM - Fonction de commande de booléen avec prise en compte de forçage et de défaut                (Compatible IFS) 12/06/2015 *
 *        C_CDETPO - Fonction de temporisation de commande à l'action et à l'arrêt                               (Compatible IFS) 03/06/2015 *
 *        C_COMMUT - Fonction de calcul de variable pour une Interface de Forçage Standardisée (IFS)                              12/06/2015 *
 *        C_DISCOR - Fonction de détection temporisée de discordance entre deux valeurs ToR ou analogiques       (Compatible IFS) 29/06/2015 *
 *        C_TFONCT - Fonction de calcul de temps de fonctionnement                                               (Compatible IFS) 12/06/2015 *
 *        R_ANA3PL - Fonction de transformation d'un pourcentage en commande 3-Points sur deux points binaires   (Compatible IFS) 30/06/2015 *
 *        R_ANA3PT - Fonction de transformation d'un pourcentage en commande 3-Points                            (Compatible IFS) 25/06/2015 *
 *        R_ANATOL - Fonction de transformation d'un pourcentage en commande PWM vers un point binaire           (Compatible IFS) 08/06/2016 *
 *        R_ANATOR - Fonction de transformation d'un pourcentage en commande PWM                                 (Compatible IFS) 08/06/2016 *
 *        R_PIDITR - Fonction de régulation par correction PID itérative                                         (Compatible IFS) 13/10/2016 *
 *        S_SCANTS - Fonction de calcul du temps de cycle programme en secondes                                                   07/08/2015 *
 *        S_TIMERS - Fonction de définition de temps de cycle à intervalles réguliers                                             31/05/2016 *
 *        T_APPRLN - Fonction d'approximation linéaire a segmnts multiples                                       (Compatible IFS) 23/06/2015 *
 *        T_CLINEB - Fonction de conversion linéaire bornée de valeur numérique                                  (Compatible IFS) 18/05/2015 *
 *        T_COMULT - Fonction de multiplexage d'un tableau de 16 booléens dans un entier non signé               (Compatible IFS) 22/10/2016 *
 *        T_DEMULT - Fonction de démultiplexage d'un entier non signé dans un tableau de booléens                (Compatible IFS) 07/01/2017 *
 *        T_DMULTV - Fonction de démultiplexage 5DI / 1 AI sur une entrée en tension                             (Compatible IFS) 07/09/2016 *
 *        T_HYSTPO - Fonction d'hystérésis (différentiel) avec tempo de confirmation d'activation et désactivation  (IFS partiel) 19/05/2015 *
 *        T_LIMVAR - Fonction de limitation de la vitesse de variation d'une valeur float                        (Compatible IFS) 26/05/2015 *
 *        T_MOYDYN - Fonction de moyenne dynamique (filtre du premier ordre)                                     (Compatible IFS) 07/09/2016 *
 *        T_MULSEC - Fonction de commmande d'équipements multiples avec secours mutuel et gestion de priorité    (Compatible IFS) 12/07/2015 *
 *        T_SEQCSR - Fonction de commmande de charges multiples en séquence à partir d'un pourcentage de commande(Compatible IFS) 29/06/2015 *
 *        T_SEQDEM - Fonction de démarrage en séquence d'équipements sur seuils haut et bas temporisés           (Compatible IFS) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 * You may not use this file except in compliance with the License. You may obtain a copy of the License at :                                *
 *                                        http://www.apache.org/licenses/LICENSE-2.0                                                         *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,     *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions  *
 * and limitations under the License.                                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

#include "Arduino.h"
#include "BMS.h"

BMS::BMS() {}

void BMS::C_CDELEM(boolean CDE, boolean DFT, boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_CDELEM - Fonction de commande de booléen avec prise en compte de forçage et de défaut                (Compatible IFS) 12/06/2015 *
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
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/

switch (CFG) {
    case 0:
		break;
    case 1:
		if (CDE && DFT) {RES[0] = false;} else {RES[0] = true;}
		break;
    case 2:
		if (DFT) {RES[0] = true;} else {RES[0] = false;}
		break;
    case 3:
		RES[0] = true;
		break;
    default:
		RES[0] = false;
		break;
	}
return;}

void BMS::C_CDETPO(boolean CDE, boolean DFT, float CPT[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_CDETPO - Fonction de temporisation de commande à l'action et à l'arrêt                               (Compatible IFS) 03/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_CDETPO(boolean CDE, boolean DFT, float CPT[], boolean RES[], int CFG)                                                 03/06/2015 *
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
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 03/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/

CPT[3] =  max(0, (millis() - CPT[4])) / 1000;
CPT[4] =  millis();
switch (CFG) {
	case 0:
		CPT[0] = 0;
		break;
    case 1:
		if ((CDE && RES[0]) || (!CDE && !RES[0]) || !DFT) { CPT[0] = 0; } else { CPT[0] = CPT[0] + CPT[3]; }
		if ((CDE && !RES[0]) && (CPT[0] > CPT[1]) && DFT) { RES[0] = true; }
		if (((!CDE && RES[0]) && (CPT[0] > CPT[2])) || !DFT) { RES[0] = false; }
		break;
    case 2:
		if (DFT) { RES[0] = true; } else { RES[0] = false; }
		CPT[0]= 0;
		break;
    case 3:
		RES[0] = true;
		CPT[0] = 0;
		break;
    default:
		RES[0] = false;
		CPT[0] = 0;
		break;
	}
return;}

void BMS::C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[]) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_COMMUT - Fonction de calcul de variable de forçage                                                                    12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[])                                                 12/06/2015 *
 *                FX1 : Demande de marche en auto (RES = 1) si true, débrayé (RES = 0) si false                                              *
 *                FX2 : Demande de forçage intermédiaire (RES = 2), prioritaire sur FX1                                                      *
 *                FX3 : Demande de forçage en marche renforcé (RES = 3), prioritaire sur FX1 et FX2                                          *
 *                FX0 : Demande de forçage à l'arrêt (RES = -1), prioritaire sur tout autre forçage                                          *
 *                RES : [00] Commande de l'équipement - forcément un tableau de taille au moins 1                                            *
 *       Interne  NUM : [00] calcul de multiplexage des FXi suivant les puissances de 2                                                      *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ----------------------------------------------------------------------------------------------------------------------------------------- */

int NUM = FX1 + (2 * FX2) + (4 * FX3) + (8 * FX0);
if (NUM < 1) { RES[0] = 0; }
if ((NUM < 2) && (NUM >= 1)) { RES[0] = 1; }
if ((NUM < 4) && (NUM >= 2)) { RES[0] = 2; }
if ((NUM < 8) && (NUM >= 4)) { RES[0] = 3; }
if (NUM >= 8) { RES[0] = -1; }
return;}

void BMS::C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_DISCOR - Fonction de détection temporisée de discordance entre deux valeurs ToR ou analogiques       (Compatible IFS) 29/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG)                                          29/06/2015 *
 *                IN1 : Première valeur à comparer                                                                                           *
 *                IN2 : Deuxième valeur à comparer                                                                                           *
 *                DIF : Différence maximum tolérée entre IN1 et IN2                                                                          *
 *                TPO : [00] Compteur de temporisation                                                                                       *
 *                      [01] Tempo de confirmation de la discordance                                                                         *
 *                      [02] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [03] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Discordance de l'équipement - forcément un tableau de taille au moins 1                                         *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES maintenu à false donc en discordance                                                                        *
 *                      0  : RES figée dans son dernier état connu                                                                           *
 *                      1  : RES est true sauf si IN1 et IN2 diffèrent de plus de DIF pendant plus de CPT[1] sans discontinuer               *
 *                      2  : RES maintenu à true en permanence                                                                               *
 *                      3  : RES maintenu à true en permanence                                                                               *
 *                      >3 : RES maintenu à false                                                                                            *
 *       Interne  NUM : [00] Différence courante entre IN1 et IN2                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 29/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[0];
CPT[2] =  max(0, (millis() - CPT[3])) / 1000;
CPT[3] =  millis();
switch (CFG) {
	case 0:
		CPT[0] = 0;
		break;
	case 1:
		NUM[0] = IN1 - IN2;
		if (abs(NUM[0]) <= DIF)  { CPT[0] = 0; } else { CPT[0] = CPT[0] + CPT[2]; }
		if (CPT[0] > CPT[1]) { RES[0] = false; } else { RES[0] = true; }
		break;
	case 2:
		RES[0] = true;
		CPT[0]= 0;
		break;
	case 3:
		RES[0] = true;
		CPT[0] = 0;
		break;
	default:
		RES[0] = false;
		CPT[0] = 0;
		break;
	}
return;}

void BMS::C_TFONCT(boolean MAR, float RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_TFONCT - Fonction de calcul de temps de fonctionnement                                               (Compatible IFS) 12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_TFONCT(boolean MAR, float RES[], int CFG)                                                                             12/06/2015 *
 *                MAR : Booléen d'état de marche de l'équipement                                                                             *
 *                RES : [00] Compteur d'heures de marche de l'équipement                                                                     *
 *                      [01] Mémoire de millis() du cycle précédent                                                                          *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES[0] annulé                                                                                                   *
 *                      0  : RES[0] figée à sa valeur courante                                                                               *
 *                      1  : RES[0] totalise le temps pendant lequel MAR est True en heures                                                  *
 *                      >1 : RES[0] annulé                                                                                                   *
 *       Interne  SCN : [00] Durée du temps de cycle UC en secondes                                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float SCN;
SCN =  max(0, (millis() - RES[1])) / 1000;
RES[1] =  millis();
switch (CFG) {
	case 0:
		break;
	case 1:
		if (MAR) {RES[0] = RES[0] + (SCN/3600);} else  { RES[0] = RES[0]; }
		break;
	default:
		RES[0] = 0;
		RES[1] =  millis();
		break;
	}
return;}

void BMS::R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PL - Fonction de transformation d'un pourcentage en commande 3-Points sur deux points binaires   (Compatible IFS) 30/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG)                                                              30/06/2015 *
 *                VPC : [00] Signal de commande de régulation en pourcentage à convertir                                                     *
 *                      [01] Signal de commande retranscrit sur l'actionneur 3-points en fin du cycle courant                                *
 *                VTC : [00] Durée du cycle de l'action 3-Points (s)                                                                         *
 *                      [01] Durée totale de la pleine course de l'actionneur (s)                                                            *
 *                      [02] Calcul du temps de cycle automate (s)                                                                           *
 *                      [03] Compteur de durée depuis la dernière exécution de la fonction (s)                                               *
 *                      [04] Mémoire des millis() de la dernière exécution de la fonction                                                    *
 *                      [05] Durée de l'action sur les sorties pour le cycle d'exécution en cours                                            *
 *                RES : [00] Commande booléenne d'ouverture  de l'actionneur 3-points                                                        *
 *                      [01] Commande booléenne de fermeture de l'actionneur 3-points                                                        *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VPC[0] maintenu à 0                                                                                             *
 *                      0  : VPC[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VPC[0] est fixé par le calcul de régulation PID                                                                 *
 *                      2  : VPC[0] maintenue à VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue à 100                                                                                          *
 *                      >3 : VPC[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] Mémoire dans l'exécution de l'écart du cycle précédent                                                          *
 *                      [01] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *                      [02] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 30/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[3];
VTC[2] =  max(0, (millis() - VTC[4])) / 1000;
VTC[3] =  VTC[3] + VTC[2];
VTC[4] =  millis();
if (VTC[3] >= abs(VTC[5])) {
	RES[0] = false;
	RES[1] = false;
}
if (VTC[3] >= VTC[0]) {
	VTC[3] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			NUM[0] = VPC[0] - VPC[1];
			NUM[1] = NUM[0] * VTC[1] / 100;
			NUM[2] = abs(NUM[1]);
			if ((NUM[2] >= VTC[0]) || (VPC[0] < 0.1) || (VPC[0] > 99.9)) {
				if ((NUM[1] < NUM[2]) || (VPC[0] < 0.1)) {
					VTC[5] = 0 - VTC[0]; 
					VPC[1] = max(VPC[1] - (VTC[0] * 100 / VTC[1]), 0);
				}
				if ((NUM[1] > NUM[2]) || (VPC[0] > 99.9)) {
					VTC[5] = VTC[0];
					VPC[1] = min(VPC[1] + (VTC[0] * 100 / VTC[1]), 100);
				}
			} else {
				VTC[5] = 0 + NUM[1]; 
				VPC[1] = VPC[0];
			}
			if (VTC[5] > 0) { RES[0] = true; RES[1] = false; }
			if (VTC[5] < 0) { RES[0] = false; RES[1] = true; }
			return;
		case 2 :
			RES[0] = true;
			RES[1] = false;
			VPC[1] = 100;
			return;
		case 3 :
			RES[0] = true;
			RES[1] = false;
			VPC[1] = 100;
			return;
		default:
			RES[0] = false;
			RES[1] = true;
			VPC[1] = 0;
			return;
		}
	} else {
		return;
	}
return;}

void BMS::R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PT - Fonction de transformation d'un pourcentage en commande 3-Points                            (Compatible IFS) 25/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG)                                                           25/06/2015 *
 *                VPC : [00] Signal de commande de régulation en pourcentage à convertir                                                     *
 *                      [01] Signal de commande retranscrit sur l'actionneur 3-points en fin du cycle courant                                *
 *                VTC : [00] Durée du cycle de l'action 3-Points (s)                                                                         *
 *                      [01] Durée totale de la pleine course de l'actionneur (s)                                                            *
 *                      [02] Calcul du temps de cycle automate (s)                                                                           *
 *                      [03] Compteur de durée depuis la dernière exécution de la fonction (s)                                               *
 *                      [04] Mémoire des millis() de la dernière exécution de la fonction                                                    *
 *                      [05] Durée de l'action sur les sorties pour le cycle d'exécution en cours                                            *
 *                VNO : Numéro de la voie de la sortie de commande d'ouverture  de l'actionneur 3-points                                     *
 *                VNF : Numéro de la voie de la sortie de commande de fermeture de l'actionneur 3-points                                     *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VPC[0] maintenu à 0                                                                                             *
 *                      0  : VPC[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VPC[0] est fixé par le calcul de régulation PID                                                                 *
 *                      2  : VPC[0] maintenue à VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue à 100                                                                                          *
 *                      >3 : VPC[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] Mémoire dans l'exécution de l'écart du cycle précédent                                                          *
 *                      [01] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *                      [02] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 25/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[3];
VTC[2] =  max(0, (millis() - VTC[4])) / 1000;
VTC[3] =  VTC[3] + VTC[2];
VTC[4] =  millis();
if (VTC[3] >= abs(VTC[5])) {
	digitalWrite(VNO, LOW);
	digitalWrite(VNF, LOW);
}
if (VTC[3] >= VTC[0]) {
	VTC[3] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			NUM[0] = VPC[0] - VPC[1];
			NUM[1] = NUM[0] * VTC[1] / 100;
			NUM[2] = abs(NUM[1]);
			if ((NUM[2] >= VTC[0]) || (VPC[0] < 0.1) || (VPC[0] > 99.9)) {
				if ((NUM[1] < NUM[2]) || (VPC[0] < 0.1)) {
					VTC[5] = 0 - VTC[0]; 
					VPC[1] = max(VPC[1] - (VTC[0] * 100 / VTC[1]), 0);
				}
				if ((NUM[1] > NUM[2]) || (VPC[0] > 99.9)) {
					VTC[5] = VTC[0];
					VPC[1] = min(VPC[1] + (VTC[0] * 100 / VTC[1]), 100);
				}
			} else {
				VTC[5] = 0 + NUM[1]; 
				VPC[1] = VPC[0];
			}
			if (VTC[5] > 0) { digitalWrite(VNO, HIGH); digitalWrite(VNF, LOW); }
			if (VTC[5] < 0) { digitalWrite(VNO, LOW); digitalWrite(VNF, HIGH); }
			return;
		case 2 :
			digitalWrite(VNO, HIGH);
			digitalWrite(VNF, LOW);
			VPC[1] = 100;
			return;
		case 3 :
			digitalWrite(VNO, HIGH);
			digitalWrite(VNF, LOW);
			VPC[1] = 100;
		default:
			digitalWrite(VNO, LOW);
			digitalWrite(VNF, HIGH);
			VPC[1] = 0;
			return;
		}
	} else {
		return;
	}
return;}

void BMS::R_ANATOL(float VPC[], float VTC[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANATOL - Fonction de transformation d'un pourcentage en commande PWM                                 (Compatible IFS) 08/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANATOL(float VPC[], int VTC[], boolean RES[], int CFG)                                                                08/06/2016 *
 *                VPC : [00] Signal de commande de régulation en pourcentage à convertir                                                     *
 *                VTC : [00] Durée totale de la pleine course de l'actionneur (s)                                                            *
 *                      [01] Calcul du temps de cycle automate (s)                                                                           *
 *                      [02] Compteur de durée depuis la dernière exécution de la fonction (s)                                               *
 *                      [03] Mémoire des millis() de la dernière exécution de la fonction                                                    *
 *                      [04] Durée de l'action sur la sortie pour le cycle d'exécution en cours                                              *
 *                RES : [00] Commande booléenne de l'actionneur PWM                                                                          *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VPC[0] maintenu à 0                                                                                             *
 *                      0  : VPC[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VPC[0] est fixé par le calcul de régulation PID                                                                 *
 *                      2  : VPC[0] maintenue à VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue à 100                                                                                          *
 *                      >3 : VPC[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] Mémoire dans l'exécution de l'écart du cycle précédent                                                          *
 *                      [01] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *                      [02] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 08/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[3];
VTC[1] =  max(0, (millis() - VTC[3])) / 1000;
VTC[2] =  VTC[2] + VTC[1];
VTC[3] =  millis();
if (VTC[2] >= VTC[4]) { RES[0] = false; }
if (VTC[2] >= VTC[0]) {
	VTC[2] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			VTC[4] = VPC[0] * VTC[0] / 100;
			if (VTC[4] > 0) { RES[0] = true; }
			return;
		case 2 :
			RES[0] = true;
			return;
		case 3 :
			RES[0] = true;
			return;
		default:
			RES[0] = false;
			return;
		}
	} else {
		return;
	}
return;}

void BMS::R_ANATOR(float VPC[], float VTC[], int VNC, int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANATOR - Fonction de transformation d'un pourcentage en commande PWM                                 (Compatible IFS) 08/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANATOR(float VPC[], int VTC[], int VNO, int CFG)                                                                      08/06/2016 *
 *                VPC : [00] Signal de commande de régulation en pourcentage à convertir                                                     *
 *                VTC : [00] Durée totale de la pleine course de l'actionneur (s)                                                            *
 *                      [01] Calcul du temps de cycle automate (s)                                                                           *
 *                      [02] Compteur de durée depuis la dernière exécution de la fonction (s)                                               *
 *                      [03] Mémoire des millis() de la dernière exécution de la fonction                                                    *
 *                      [04] Durée de l'action sur la sortie pour le cycle d'exécution en cours                                              *
 *                VNC : Numéro de la voie de la sortie de commande d'ouverture  de l'actionneur PWM                                          *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VPC[0] maintenu à 0                                                                                             *
 *                      0  : VPC[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VPC[0] est fixé par le calcul de régulation PID                                                                 *
 *                      2  : VPC[0] maintenue à VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue à 100                                                                                          *
 *                      >3 : VPC[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] Mémoire dans l'exécution de l'écart du cycle précédent                                                          *
 *                      [01] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *                      [02] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 08/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[3];
VTC[1] =  max(0, (millis() - VTC[3])) / 1000;
VTC[2] =  VTC[2] + VTC[1];
VTC[3] =  millis();
if (VTC[2] >= VTC[4]) { digitalWrite(VNC, LOW); }
if (VTC[2] >= VTC[0]) {
	VTC[2] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			VTC[4] = VPC[0] * VTC[0] / 100;
			if (VTC[4] > 0) { digitalWrite(VNC, HIGH); }
			return;
		case 2 :
			digitalWrite(VNC, HIGH);
			return;
		case 3 :
			digitalWrite(VNC, HIGH);
			return;
		default:
			digitalWrite(VNC, LOW);
			return;
		}
	} else {
		return;
	}
return;}

void BMS::R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_PIDITR - Fonction de régulation par correction PID itérative                                         (Compatible IFS) 13/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG)                                                       13/10/2016 *
 *                MES : Mesure de la grandeur de régulation                                                                                  *
 *                CSG : Consigne de régulation                                                                                               *
 *                VPC : [00] Signal de commande de régulation                                                                                *
 *                      [01] Mémoire de l'écart du cycle précédent                                                                           *
 *                      [02] Mémoire de la variation d'écart du cycle précédent                                                              *
 *                      [03] Valeur de repli de la commande (CFG = 2)                                                                        *
 *                VKR : [00] Durée du cycle du régulateur (s)                                                                                *
 *                      [01] Coefficient de transfert (Unités)                                                                               *
 *                      [02] Bande proportionnelle (%)                                                                                       *
 *                      [03] Temps intégral (s)                                                                                              *
 *                      [04] Temps dérivé (s)                                                                                                *
 *                      [05] Ecart (Unités)                                                                                                  *
 *                      [06] Durée calculée du cycle automate (s)                                                                            *
 *                      [07] Compteur de durée depuis la dernière exécution de la fonction (s)                                               *
 *                      [08] Mémoire des millis() de la dernière exécution de la fonction                                                    *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VPC[0] maintenu à 0                                                                                             *
 *                      0  : VPC[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VPC[0] est fixé par le calcul de régulation PID                                                                 *
 *                      2  : VPC[0] maintenue à VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue à 100                                                                                          *
 *                      >3 : VPC[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] Mémoire dans l'exécution de l'écart du cycle précédent                                                          *
 *                      [01] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *                      [02] Mémoire dans l'exécution de la variation d'écart du cycle précédent                                             *
 *                      [03] Calcul de la correction à appliquer à VPC[0] pour le cycle en cours                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 27/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[4];
VKR[6] =  max(0, (millis() - VKR[8])) / 1000;
VKR[7] =  VKR[7] + VKR[6];
VKR[8] =  millis();
if (VKR[7] >= VKR[0]) {
	VKR[7] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			NUM[0] = VPC[1];
			NUM[1] = VPC[2];
			NUM[2] = max(VKR[0], VKR[6]);
			if (VKR[1] == 0) { VKR[1] = 1000; }
			if (VKR[2] == 0) { VKR[2] = 1000; }
			if (VKR[3] == 0) { VKR[3] = 10000; }
			if (VKR[5] == 0) { VPC[1] = CSG - MES; } else { VPC[1] = min(VKR[5], max((0-VKR[5]), (CSG - MES))); }
			VPC[2] = VPC[1] - NUM[0];
			NUM[3] = 100 * VPC[2] / (VKR[1] * (VKR[2] / 100) * NUM[2]);
			NUM[3] = NUM[3] + (100 * VPC[1] / (VKR[1] * VKR[3]));
			NUM[3] = NUM[3] + (100 * (VPC[2] - NUM[1]) * VKR[4] / (VKR[1] * NUM[2] * NUM[2]));
			VPC[0] = min(100, max(0, VPC[0] + (NUM[3] * NUM[2])));
			return;
		case 2 :
			VPC[0] = VPC[3];
			return;
		case 3 :
			VPC[0] = 100;
			return;
		default:
			VPC[0] = 0;
			return;
		}
	}
return; }

void BMS::S_SCANTS(float CPT[2]) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : S_SCANTS - Fonction de calcul du temps de cycle programme en secondes                                                   07/08/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        S_SCANTS(float CPT[])                                                                                                   07/08/2015 *
 *                CPT : [00]   : Temps de cycle automate calculé par la fonction                                                             *
 *                      [01]   : Mémorisation de la variable système Millis pour le calcul du temps de cycle                                 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/08/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[0] =  max(0, (millis() - CPT[1])) / 1000;
CPT[1] =  millis();
return;}

void BMS::S_TIMERS(float CPT[9], boolean FCO[7]) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : S_TIMERS - Fonction de définition de temps de cycle à intervalles réguliers                                             31/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        S_TIMERS(float CPT[6], boolean FCO[4])                                                                                  31/05/2016 *
 *                CPT : [00]   : Compteur de secondes, reset toutes les secondes                                                             *
 *                      [01]   : Compteur de secondes, reset toutes les 10 secondes                                                          *
 *                      [02]   : Compteur de secondes, reset toutes les 30 secondes                                                          *
 *                      [03]   : Compteur de secondes, reset toutes les minutes                                                              *
 *                      [04]   : Compteur de secondes, reset toutes les  5 minutes                                                           *
 *                      [05]   : Compteur de secondes, reset toutes les 30 minutes                                                           *
 *                      [06]   : Compteur de secondes, reset toutes les heures                                                               *
 *                      [07]   : Temps de cycle automate calculé par la fonction                                                             *
 *                      [08]   : Mémorisation de la variable système Millis pour le calcul du temps de cycle                                 *
 *                FCO : [00]   : Indicateur de cycle : s'active 1 cycle automate toutes les secondes                                         *
 *                      [01]   : Indicateur de cycle : s'active 1 cycle automate toutes les 10 secondes                                      *
 *                      [02]   : Indicateur de cycle : s'active 1 cycle automate toutes les 30 secondes                                      *
 *                      [03]   : Indicateur de cycle : s'active 1 cycle automate toutes les minutes                                          *
 *                      [04]   : Indicateur de cycle : s'active 1 cycle automate toutes les  5 minutes                                       *
 *                      [05]   : Indicateur de cycle : s'active 1 cycle automate toutes les 30 minutes                                       *
 *                      [06]   : Indicateur de cycle : s'active 1 cycle automate toutes les heures                                           *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 31/05/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[7] =  max(0, (millis() - CPT[8])) / 1000;
CPT[8] = millis();
if (CPT[0] < 1) { FCO[0] = false; CPT[0] = CPT[0] + CPT[7];} else { FCO[0] = true; CPT[0] =  0;} 
if (CPT[1] < 10) { FCO[1] = false; CPT[1] = CPT[1] + CPT[7];} else { FCO[1] = true; CPT[1] =  0;} 
if (CPT[2] < 60) { FCO[2] = false; CPT[2] = CPT[2] + CPT[7];} else { FCO[2] = true; CPT[2] =  0;} 
if (CPT[3] < 300) { FCO[3] = false; CPT[3] = CPT[3] + CPT[7];} else { FCO[3] = true; CPT[3] =  0;} 
if (CPT[4] < 600) { FCO[4] = false; CPT[4] = CPT[4] + CPT[7];} else { FCO[4] = true; CPT[4] =  0;} 
if (CPT[5] < 1800) { FCO[5] = false; CPT[5] = CPT[5] + CPT[7];} else { FCO[5] = true; CPT[5] =  0;} 
if (CPT[6] < 3600) { FCO[6] = false; CPT[6] = CPT[6] + CPT[7];} else { FCO[6] = true; CPT[6] =  0;} 
return;}

void BMS::T_APPRLN(float MES, int VTE[], float VTS[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_APPRLN - Fonction d'approximation linéaire a segmnts multiples                                       (Compatible IFS) 23/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_APPRLN(float MES, int VTE[], float VTS[], int CFG)                                                                    23/06/2015 *
 *                MES : Valeur à convertir                                                                                                   *
 *                      [01] Nombre total des équipements commandés par la fonction                                                          *
 *                VTE : [00] Nombre de bornes de segments d'entrées et de sorties                                                            *
 *                      [01] Borne inférieure du premier segment d'entrée                                                                    *
 *                      [02] Borne supérieure du premier segment d'entrée et borne inférieure du 2e segment d'entrée                         *
 *                      ...    Ce tableau est composé d'integer forcément strictement croissants                                             *
 *                      [NN] Borne supérieure du dernier segment d'entrée. NN = VTE[0]                                                       *
 *                VTS : [00] 	Résultat de la conversion                                                                                       *
 *                      [01] Borne inférieure du premier segment de sortie                                                                   *
 *                      [02] Borne supérieure du premier segment d'entrée et borne inférieure du 2e segment de sortie                        *
 *                      ...  Ce tableau est composé de float. Il n'est pas nécessairement croissant mais éviter les égalités à la suite      *
 *                      [NN] Borne supérieure du dernier segment de sortie. NN = VTE[0]                                                      *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : VTS[0] maintenu à 0                                                                                             *
 *                      0  : VTS[0] figé dans sa dernière valeur connue                                                                      *
 *                      1  : VTS[0] suit la courbe définie par VTE[] et VTS[]                                                                *
 *                      2  : VTS[0] maintenue à VTS[1]                                                                                       *
 *                      3  : VTS[0] maintenue à VTS[VTE[0]]                                                                                  *
 *                      >3 : VTS[0] maintenu à 0                                                                                             *
 *       Interne  NUM : [00] Index pour les diverses itérations                                                                              *
 *                      [01] Index de la table d'entrée : borne supérieure du segment de la table VTE où se trouve MES                       *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 23/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[2];
if (VTE[0] < 2) { return; }
switch (CFG) {
	case 0:
		break;
    case 1 :
		NUM[0] = 2;
		NUM[1] = 0;
		if (MES < VTE[1]) { 
			VTS[0] = VTS[1];
			break;
		}
		if (MES > VTE[VTE[0]]) { 
			VTS[0] = VTS[VTE[0]];
			break;
		}
		for(NUM[0] = 2;  NUM[0] <= VTE[0] ; NUM[0]++) {
			if (MES > VTE[NUM[0] - 1]) { NUM[1] = NUM[0]; }
		}
		if (VTE[NUM[1]] == VTE[NUM[1] - 1]) { 
			VTS[0] =  VTS[NUM[1] - 1];
		} else {
			VTS[0] = VTS[NUM[1] - 1] + (((MES - VTE[NUM[1] - 1]) * (VTS[NUM[1]] - VTS[NUM[1] - 1])) / (VTE[NUM[1]] - VTE[NUM[1] - 1])); 
		}
		break;
	case 2 :
		VTS[0] = VTS[1];
		break;
	case 3 :
		VTS[0] = VTS[VTE[0]];
		
	default:
		VTS[0] = 0;
		break;
	}
return;}

void BMS::T_CLINEB(float CDE[], float RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_CLINEB - Fonction de conversion linéaire bornée de valeur numérique                                  (Compatible IFS) 18/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_CLINEB(float CDE[], float RES[], int CFG)                                                                             18/05/2015 *
 *                CDE : [00] Commande analogique d'entrée, qui doit subir la conversion                                                      *
 *                      [01] Borne 1 sur la commande d'entrée, correspond à une valeur convertie de BS1. Peut être > ou < BE2                *
 *                      [02] Borne 2 sur la commande d'entrée, correspond à une valeur convertie de BS2. Peut être > ou < BE1                *
 *                RES : [00] Résultat de la conversion : si CDE hors [BE1-BE2], BS1 ou BS2. Sinon conversion linéaire BE1-BE2 / BS1-BS2      *
 *                      [01] Borne 1 de conversion, correspond à une valeur d'entrée de BE1. Peut être > ou < BS2                            *
 *                      [02] Borne 2 de conversion, correspond à une valeur d'entrée de BE2. Peut être > ou < BS1                            *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES maintenue à 0                                                                                               *
 *                      0  : RES figée dans son dernier état connu                                                                           *
 *                      1  : RES est le résultat de la conversion linéaire                                                                   *
 *                      2  : RES maintenue à BS1                                                                                             *
 *                      3  : RES maintenue à BS2                                                                                             *
 *                      >3 : RES maintenue à 0                                                                                               *
 *       Interne  NUM : Calcul intermédiaire de la valeur convertie                                                                          *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 18/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM;
switch (CFG) {
	case 0:
		break;
	case 1:
		if (CDE[1] == CDE[2]) { RES[0] = RES[1]; }
		NUM = ((RES[2] - RES[1]) / (CDE[2] - CDE[1]));
		if (CDE[2] > CDE[1]) {
			if (CDE[0] < CDE[1]) { NUM = RES[1]; }
			if (CDE[0] > CDE[2]) { RES[0] = RES[2]; }
			if ((CDE[0] >= CDE[1]) && (CDE[0] <= CDE[2])) { RES[0] = ((CDE[0] - CDE[1]) * NUM) + RES[1]; }
		}
		if (CDE[2] < CDE[1]) {
			if (CDE[0] < CDE[2]) { RES[0] = RES[2]; }
			if (CDE[0] > CDE[1]) { RES[0] = RES[1]; }
			if ((CDE[0] <= CDE[1]) && (CDE[0] >= CDE[2])) { RES[0] = ((CDE[0] - CDE[1]) * NUM) + RES[1]; }
		}
		break;
    case 2:
		RES[0] = RES[1];
		break;
    case 3:
		RES[0] = RES[2];
		break;
    default:
		RES[0] = 0;
		break;
	}
return;}

void BMS::T_COMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_COMULT - Fonction de multiplexage d'un tableau de 16 booléens dans un entier non signé               (Compatible IFS) 22/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_COMULT(unsigned int MPX[], int RNG, Boolean FLG[], int CFG)                                                           22/10/2016 *
 *                MPX : [00] : Entier, Variable porteuse des booléens multiplexés, forcément un tableau de taille au moins 1                 *
 *                RNG : Entier, rang de la variable porteuse dans le tableau des variables porteuses                                         *
 *                FLG : [00] Booléen de rang 0 dans le tableau de taille 16                                                                  *
 *                      [..] Suite du tableau de booléens à multiplexer                                                                      *
 *                      [15] Dernier booléen du tableau à multiplexer                                                                        *
 *                CFG : Commande de forçage                                                                                                  *
 *                      <0  : la sortie MPX[RNG] reste nulle quelle que soit la valeur des FLG[...]                                          *
 *                      0   : la sortie MPX[RNG] reste figée à sa valeur courante                                                            *
 *                      1   : la sortie MPX[RNG] suit les valeurs de FLG[...] encodés suivant les puissances de 2                            *
 *                      2   : la sortie MPX[RNG] prend pour valeur 65535                                                                     *
 *                      3   : la sortie MPX[RNG] prend pour valeur 65535                                                                     *
 *                      >3  : la sortie MPX[RNG] reste nulle quelle que soit la valeur des FLG[...]                                          *
 *       Interne  NUM : [00] Calcul intermédiaire d'addition des puissances de 2                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 06/04/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 22/10/2016 : DD, SE2R : Si mémorisation EEPROM de MPX[RNG], toujours retirer 1 à MPX[RNG] juste après le T_COMULT (pb de bit 0 EEPROM)    *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[1];
switch (CFG) {
	case 0:
		break;
	case 1:
  		NUM[0] = float(FLG[0]);
		for (int i = 1; i < 16; i++) { NUM[0] = NUM[0] + (FLG[i] * pow(2,i)); }
		MPX[RNG] = int(NUM[0]) + 1; 
		break;
	case 2:
		MPX[RNG] = 65535;
		break;
	case 3:
		MPX[RNG] = 65535;
		break;
	default:
		MPX[RNG] = 0;
		break;
	}
return;}

void BMS::T_DEMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DEMULT - Fonction de démultiplexage d'un entier non signé dans un tableau de booléens                (Compatible IFS) 22/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DEMULT(int MPX[], int RNG, Boolean FLG[], int CFG)                                                                    22/10/2016 *
 *                MPX : [00] : Entier, Variable porteuse des booléens multiplexés, forcément un tableau de taille au moins 1                 *
 *                RNG : Entier, rang de la variable porteuse dans le tableau des variables porteuses                                         *
 *                FLG : [00] : Booléen de rang 0 dans le tableau                                                                             *
 *                      [..] : Suite du tableau de booléens à multiplexer                                                                    *
 *                      [15] : Dernier booléen du tableau à multiplexer                                                                      *
 *                CFG : Commande de forçage                                                                                                  *
 *                      <0  : les booléens FLG[xx] restent false quelle que soit la valeur de MPX[RNG]                                       *
 *                      0   : les booléens FLG[xx] restent figés à leur valeur courante                                                      *
 *                      1   : les booléens FLG[xx] suivent les valeurs de MPX[RNG] décodé suivant les puissances de 2                        *
 *                      2   : les booléens FLG[xx] restent true quelle que soit la valeur de MPX[RNG]                                        *
 *                      3   : les booléens FLG[xx] restent true quelle que soit la valeur de MPX[RNG]                                        *
 *                      >3  : les booléens FLG[xx] restent false quelle que soit la valeur de MPX[RNG]                                       *
 *       Interne  NUM : [00] Calcul intermédiaire de décodage des puissances de 2                                                            *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 06/04/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 22/10/2016 : DD, SE2R : Testée et validée sur banc 32U8, 1286, Arduino Leonardo. Si mémorisation EEPROM, toujours redémarrer après charge *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[1];
switch (CFG) {
	case 0:
		break;
	case 1:
		NUM[0] = float(MPX[RNG]);
		for (int i = 15; i >= 0; i--) {
			if (NUM[0] >= pow(2,i)) { FLG[i] = true; } else { FLG[i] = false;}
			NUM[0] = NUM[0] - (FLG[i] * pow(2,i));
        }
		break;
	case 2:
		for (int i = 15; i >= 0; i--) { FLG[i] = true; }
		break;
	case 3:
		for (int i = 15; i >= 0; i--) { FLG[i] = true; }
		break;
	default:
		for (int i = 15; i >= 0; i--) { FLG[i] = false; }
		break;
	}
return;}
  
void BMS::T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - Fonction de démultiplexage 5DI / 1 AI sur une entrée en tension                             (Compatible IFS) 07/01/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG)                                          07/01/2017 *
 *                MES : float, lecture en tension 0-10V vers un pourcentage (V10_p) de l'entrée analogique                                   *
 *                OHM : [00] Valeur de la résistance de charge de base                                                                       *
 *                      [01] Valeur de la résistance de charge de l'entrée digitale 1 du multiplexeur                                        *
 *                      [02] Valeur de la résistance de charge de l'entrée digitale 2 du multiplexeur                                        *
 *                      [03] Valeur de la résistance de charge de l'entrée digitale 3 du multiplexeur                                        *
 *                      [04] Valeur de la résistance de charge de l'entrée digitale 4 du multiplexeur                                        *
 *                      [05] Valeur de la résistance de charge de l'entrée digitale 5 du multiplexeur                                        *
 *                TNS : [00] Calcul de la tension en aval de la résistance de charge RES[0] selon la configuration des RES[1] à [5]          *
 *                      [01] Tension d'alimentation de référence : 24000 mV                                                                  *
 *                      [02] Différentiel de tension de compensation des imprécisions de résistances : 150 à 200 mV                          *
 *                      [03] Tension en aval de la résistance de charge RES[0] mesurée à vide pour une alimentation de référence 24Vdc       *
 *                      [04] Tension en aval de la résistance de charge RES[0] mesurée à vide en conditions réelles                          *
 *                      [05] Tension d'alimentation réelle recalculée à partir de TNS[3] et TNS[4]                                           *
 *                      [06] Somme des résistances de charge en ligne à chaque instant de [00] à [05] au format float                        *
 *                VNC : [00] Nombre de cycles de confirmation antirebond d'une transition d'une des entrées RES                              *
 *                      [01] Compteur de nombre de cycles de différence continue entre RES[1] et RES[6]                                      *
 *                      [02] Compteur de nombre de cycles de différence continue entre RES[2] et RES[7]                                      *
 *                      [03] Compteur de nombre de cycles de différence continue entre RES[3] et RES[8]                                      *
 *                      [04] Compteur de nombre de cycles de différence continue entre RES[4] et RES[9]                                      *
 *                      [05] Compteur de nombre de cycles de différence continue entre RES[5] et RES[10]                                     *
 *                RES : [00] Commande de mémorisation de la tension à vide du multiplexeur à la calibration                                  *
 *                      [01] Etat calculé de l'entrée digitale 1 du multiplexeur après traitement antirebond                                 *
 *                      ...                                                                                                                  *
 *                      [05] Etat calculé de l'entrée digitale 5 du multiplexeur après traitement antirebond                                 *
 *                      [06] Etat calculé de l'entrée digitale 1 du multiplexeur avant traitement antirebond                                 *
 *                      ...                                                                                                                  *
 *                      [10] Etat calculé de l'entrée digitale 5 du multiplexeur avant traitement antirebond                                 *
 *                CFG : Commande de forçage                                                                                                  *
 *                      <0  : les booléens RES[1] à [5] restent false quelle que soit la valeur de MPX[RNG]                                  *
 *                      0   : les booléens RES[1] à [5] restent figés à leur valeur courante                                                 *
 *                      1   : les booléens RES[1] à [5] suivent les valeurs de MES décodé suivant les valeurs de tensions                    *
 *                      2   : les booléens RES[1] à [5] restent true quelle que soit la valeur de MPX[RNG]                                   *
 *                      3   : les booléens RES[1] à [5] restent true quelle que soit la valeur de MPX[RNG]                                   *
 *                      >3  : les booléens RES[1] à [5] restent false quelle que soit la valeur de MPX[RNG]                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/09/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
TNS[1] = 24000;
TNS[6] = float(OHM[5] + OHM[4] + OHM[3] + OHM[2]  + OHM[1] + OHM[0]);
TNS[3] = TNS[1] * (1 - (OHM[0] / TNS[6]));
if (RES[0] == true) {TNS[4] = MES * 100; RES[0] = false; }
TNS[5] = TNS[1] * TNS[4] / TNS[3];
switch (CFG) {
	case 0:
		break;
    case 1:
		TNS[6] = float(OHM[4] + OHM[3] + OHM[2]  + OHM[1] + OHM[0]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[6] = true; } else { RES[6] = false ; }
		TNS[6] = TNS[6] + (float(OHM[5]) * (1 - float(RES[6] == true))) - float(OHM[4]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[7] = true;  } else { RES[7] = false ; }
		TNS[6] = TNS[6] + (float(OHM[4]) * (1 - float(RES[7] == true))) - float(OHM[3]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[8] = true;  } else { RES[8] = false ; }
		TNS[6] = TNS[6] + (float(OHM[3]) * (1 - float(RES[8] == true))) - float(OHM[2]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[9] = true;  } else { RES[9] = false ; }
		TNS[6] = TNS[6] + (float(OHM[2]) * (1 - float(RES[9] == true))) - float(OHM[1]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[10] = true;  } else { RES[10] = false ; }
		for (int I = 1; I < 6; I++) {
			if (RES[I] < RES[I + 5]) { VNC[I] = min(VNC[I] + 1, VNC[0]); }
			if (RES[I] > RES[I + 5]) { VNC[I] = max(VNC[I] - 1, 0); }
			if ((RES[I] == RES[I + 5]) && (VNC[I] != 0) && (VNC[I] != VNC[0])) {VNC[I] = VNC[0] * float(RES[I]); }
			if (VNC[I] == 0) { RES[I] = false; }
			if (VNC[I] == VNC[0]) { RES[I] = true; }
		}
		break;
    case 2:
		for (int i = 1; i < 6; i++) { RES[i] = true; }
		break;
    case 3:
		for (int i = 1; i < 6; i++) { RES[i] = true; }
		break;
    default:
		for (int i = 1; i < 6; i++) { RES[i] = false; }
		break;
	}
return;}

void BMS::T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_HYSTPO - Fonction d'hystérésis (différentiel) avec tempo de confirmation d'activation et désactivation  (IFS partiel) 19/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG)                                                              19/05/2015 *
 *                MES : [00] Valeur analogique de base de la commande                                                                        *
 *                      [01] Seuil bas  de commande. Doit impérativement être inférieur à SHT                                                *
 *                      [02] Seuil haut de commande. Doit impérativement être supérieur à SBS                                                *
 *                CPT : [00] Compteur de secondes de dépassement des seuils bas et haut (RàZ entre les seuils)                               *
 *                      [01] Tempo de confirmation de dépassement du seuil bas  avant action sur la commande                                 *
 *                      [02] Tempo de confirmation de dépassement du seuil haut avant action sur la commande                                 *
 *                      [03] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [04] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Commande de l'équipement - forcément un tableau de taille au moins 1                                            *
 *                CFG : Commande de forçage et de sens d'action des seuils                                                                   *
 *                      <-1: sortie RES désactivée quelle que soit la valeur de MES[0]                                                       *
 *                      -1 : sortie RES désactivée si MES[0] > MES[2] confirmé par CPT[2], activée si MES[0] < MES[1] confirmé par CPT[1]    *
 *                      0  : sortie RES figée à sa valeur courante                                                                           *
 *                      1  : sortie RES activée si MES[0] > MES[2] confirmé par CPT[2], désactivée si MES[0] < MES[1] confirmé par CPT[1]    *
 *                      2  : sortie RES activée quelle que soit la valeur de MES[0]                                                          *
 *                      3  : sortie RES activée quelle que soit la valeur de MES[0]                                                          *
 *                      >3 : sortie RES désactivée quelle que soit la valeur de MES[0]                                                       *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 19/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 26/05/2015 : SE2R, DD : MES[1] doit être inférieur à MES[2], sinon la fonction commute sans tempo ni différentiel au niveau d'un seuil    *
 *                         MES[0] doit passer entre MES[1] et MES[2] pour que le compteur de temporisation MEM[0] s'annule.                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[3] =  max(0, (millis() - CPT[4])) / 1000;
CPT[4] =  millis();
switch (CFG) {
	case 0:
		CPT[0]= 0;
		break;
	case 1:
		if ((MES[0]>= MES[1]) && (MES[0]<= MES[2])) { CPT[0]= 0; } else { CPT[0]= CPT[0] + CPT[3]; }
		if ((MES[0] > MES[2]) && (CPT[0]>= CPT[2])) { RES[0] = true; }
		if ((MES[0] < MES[1]) && (CPT[0]>= CPT[1])) { RES[0] = false; }
		break;
	case -1:
		if ((MES[0]>= MES[1]) && (MES[0]<= MES[2])) { CPT[0]= 0; } else { CPT[0]= CPT[0] + CPT[3]; }
		if ((MES[0] > MES[2]) && (CPT[0]>= CPT[2])) { RES[0] = false; }
		if ((MES[0] < MES[1]) && (CPT[0]>= CPT[1])) { RES[0] = true; }
		break;
	case 2:
		RES[0] = true;
		CPT[0]= 0;
		break;
	case 3:
		RES[0] = true;
		CPT[0]= 0;
		break;
	default:
		RES[0] = false;
		CPT[0]= 0;
		break;
	}
return;}

void BMS::T_LIMVAR(float MES[], float CPT[], int RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_LIMVAR - Fonction de limitation de la vitesse de variation d'une valeur float                        (Compatible IFS) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_LIMVAR(float MES[], float CPT[], int RES[], int CFG)                                                                  26/05/2015 *
 *                MES : [00] Valeur analogique de base de la commande                                                                        *
 *                      [01] Vitesse maximum de diminution de VPC_XXXX                                                                       *
 *                      [02] Vitesse maximum d'augmentation de VPC_XXXX                                                                      *
 *                      [03] Mémoire de RES en début d'exécution de la fonction                                                              *
 *                CPT : [00] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [01] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Commande de l'équipement - forcément un tableau de taille au moins 1                                            *
 *                CFG : Commande de forçage et de sens d'action des seuils                                                                   *
 *                      <0 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 *                      0  : sortie RES[0] figée à sa valeur courante                                                                        *
 *                      1  : sortie RES[0] suit MES[0] en respectant les limites d'augmentation et de diminution                             *
 *                      2  : sortie RES[0] suit MES[0] sans amortissement                                                                    *
 *                      3  : sortie RES[0] suit MES[0] sans amortissement                                                                    *
 *                      >3 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[0] =  max(0, (millis() - CPT[1])) / 1000;
CPT[1] =  millis();
switch (CFG) {
	case 0:
		break;
		case 1:
	//      MES[3] = MES[0];
			if (RES[0] < MES[3]) {
				if (MES[1] == 0) {
					RES[0] = MES[0];
				} else {
					if ((MES[0] - RES[0]) < (MES[2] * CPT[0])) {
						RES[0] = MES[0];
					} else {
						RES[0] = RES[0] + (MES[2] * CPT[0]);
					}
				}
			} else {
				if (MES[2] == 0) { 
					RES[0] = MES[0];
				} else {
					if ((RES[0] - MES[0]) < (MES[1] * CPT[0])) {
						RES[0] = MES[0];
					} else {
						RES[0] = RES[0] - (MES[1] * CPT[0]);
					}
				}
			}
			break;
		case 2:
			RES[0] = MES[0];
			break;
		case 3:
			RES[0] = MES[0];
			break;
		default:
			RES[0] = 0;
			break;
	}
return;}

void BMS::T_MOYDYN(float MES, float RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_MOYDYN - Fonction de moyenne dynamique (filtre du premier ordre)                                     (Compatible IFS) 07/09/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_MOYDYN(float MES, float RES[], int CFG)                                                                               07/09/2015 *
 *                MES : Valeur analogique de base de la commande                                                                             *
 *                RES : [00] Commande de l'équipement - forcément un tableau de taille au moins 2                                            *
 *                      [01] Nombre de cycles sur lequel la moyenne est effectuée                                                            *
 *                CFG : Commande de forçage                                                                                                  *
 *                      <0 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 *                      0  : sortie RES[0] figée à sa valeur courante                                                                        *
 *                      1  : sortie RES[0] suit MES[0] en respectant les limites d'augmentation et de diminution                             *
 *                      2  : sortie RES[0] suit MES[0] sans amortissement                                                                    *
 *                      3  : sortie RES[0] suit MES[0] sans amortissement                                                                    *
 *                      >3 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 *       Interne  NUM : [00] Nombre d'itérations avec détection de mauvais paramétrage de RES[1]                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *                         Si RES[1] n'est pas renseigné, il prend par défaut la valeur 10                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[1];
switch (CFG) {
    case 0:
		break;
    case 1:
		if (RES[1] < 1) { NUM[0] = 10; } else { NUM[0] = RES[1]; } 
		RES[0] = (MES + (RES[0] * ( NUM[0] - 1))) / NUM[0];
		break;
    case 2:
		RES[0] = MES;
		break;
    case 3:
		RES[0] = MES;
		break;
    default:
		RES[0] = 0;
		break;
    }
return; }

void BMS::T_MULSEC(int VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_MULSEC - Fonction de commmande d'équipements multiples avec secours mutuel et gestion de priorité    (Compatible IFS) 12/07/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_MULSEC(float VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG)                                     12/07/2015 *
 *                VNC : [00] Nombre d'équipements demandés en marche                                                                         *
 *                      [01] Nombre total des équipements commandés par la fonction                                                          *
 *                VTP : [00] Inutilisé, maintenu à 0                                                                                         *
 *                      [01] Ordre de priorité de l'équipement 1 lors de la prochaine transition (défaut ou nb demandé VNC[0])               *
 *                      ...  L'ordre de priorité le plus bas est prioritaire à la marche, le plus haut prioritaire à l'arrêt                 *
 *                      [NN] Ordre de priorité du dernier équipement lors de la prochaine transition (défaut ou nb demandé VNC[0])           *
 *                VTD : [00] Inutilisé, maintenu à false                                                                                     *
 *                      [01] Défaut de l'équipement 1. True signale un équipement en état normal. False signale un défaut.                   *
 *                      ...  Ce tableau est composé de booléens                                                                              *
 *                      [NN] Défaut du dernier équipement. True signale un équipement en état normal. False signale un défaut.               *
 *                VMC : [00] Mémoire de NUM[1] : rang de la dernière sortie commandée                                                        *
 *                      [01] Mémoire de VMC[4] : Nombre de sorties à commandées lors du cycle précédent                                      *
 *                      [02] Mémoire de NUM[2] : Nombre de commandes actives dans le cycle courant                                           *
 *                      [03] Mémoire de NUM[3] : Nombre d'équipements en état normal                                                         *
 *                      [04] Mémoire du nombre d'équipements à commander dans le cycle                                                       *
 *                FTC : [00] Information de synthèse de demande de marche : un au moins des équipements est commandé                         *
 *                      [01] Commande de l'équipement 1. True signale un équipement demandé en marche.                                       *
 *                      ...  Ce tableau est composé de booléens                                                                              *
 *                      [NN] Commande du dernier équipement. True signale un équipement demandé en marche.                                   *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : Les FTC[x] sont maintenus à false                                                                               *
 *                      0  : Tous les FTC[x] figés dans leur dernier état connu                                                              *
 *                      1  : Les FTC[x] sont commandés selon VNC[0], des priorités VTP[x], et leurs défauts respectifs VTD[x]                *
 *                      2  : Les FTC[x] sont maintenus à true                                                                                *
 *                      3  : Les FTC[x] sont maintenus à true                                                                                *
 *                      >3 : Les FTC[x] sont maintenus à false                                                                               *
 *       Interne  NUM : [00] Index pour les itérations                                                                                       *
 *                      [01] Rang de l'équipement prioritaire à la marche ou l'arrêt dans les tableaux                                       *
 *                      [02] Somme du contenu du tableau FTC des commandes (nombre de commandes actives)                                     *
 *                      [03] Somme du contenu du tableau VTD des défauts (nombre d'équipements en état normal)                               *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 30/06/2015 : DD, SE2R : Ajout du forçage différencié en 2 et 3 dans le cas de deux équipements commandés                                  *
 * 12/07/2015 : DD, SE2R : Correction d'un bug de fonctionnement à 2 équipements                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[4];
int VTC[int(VNC[1])];
VTP[0] = 0;
VTD[0] = true;
if (VNC[1] < 1) { return; }
switch (CFG) {
    case 0:
		return;
    case 1 :
		if (VNC[0] < VMC[1]) { VMC[4] = VMC[4] - 1; }
		if (VNC[0] > VMC[1]) { VMC[4] = VMC[4] + 1; }
		NUM[0] = 1;
		NUM[2] = 0;
		NUM[3] = 0;
		for(NUM[0] = 1;  NUM[0] <= VNC[1] ; NUM[0]++) {
			if (VTD[NUM[0]] < 0.5) { FTC[NUM[0]] = false; }
			NUM[2] = NUM[2] + FTC[NUM[0]] ;
			NUM[3] = NUM[3] + VTD[NUM[0]] ;
		}
		if (NUM[2] > VMC[4]) { VMC[1] = VMC[1] + 1; }
		if (NUM[2] < VMC[4]) { VMC[1] = VMC[1] - 1; }
		if ((NUM[3] < VMC[3]) && (NUM[2] < VMC[2]))  { VMC[1] = VMC[1] - 1; } 
		if (VMC[4] > VMC[1]) {
			NUM[0] = 1;
			NUM[1] = 1; 
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
				VTC[NUM[0]] = VTP[NUM[0]] + (1000 * (FTC[NUM[0]] > 0.5)) + (10000 * (VTD[NUM[0]] < 0.5));
				if (VTC[NUM[0]] < VTC[NUM[1]]) { NUM[1] = NUM[0]; }
			}
		if (VTD[NUM[1]] > 0.5) { FTC[NUM[1]] = true; } else { FTC[NUM[1]] = false; }
		}
		if (VMC[4] < VMC[1]) {
			NUM[0] = 1;
			NUM[1] = 1; 
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
				VTC[NUM[0]] = VTP[NUM[0]] + (1000 * (VTD[NUM[0]] < 0.5)) + (10000 * (FTC[NUM[0]] > 0.5));
				if (VTC[NUM[0]] > VTC[NUM[1]]) { NUM[1] = NUM[0]; }
			}
			FTC[NUM[1]] = false;
		}
		VMC[0] = NUM[1];
		VMC[1] = VMC[4];
		VMC[2] = NUM[2];
		VMC[3] = NUM[3];
		return;
	case 2 :
		NUM[0] = 1;
		if (VNC[1] = 2) {
			FTC[1] = true;
			FTC[2] = false;
		} else {
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) { FTC[NUM[0]] = true; }
		}
		VMC[0] = NUM[1];
		return;
	case 3 :
		NUM[0] = 1;
		if (VNC[1] = 2) {
			FTC[1] = false;
			FTC[2] = true;
		} else {
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) { FTC[NUM[0]] = true; }
		}
		VMC[0] = NUM[1];
		return;
	default:
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) { FTC[NUM[0]] = false; }
		VMC[0] = NUM[1];
		return;
	}
return; }

void BMS::T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_SEQCSR - Fonction de commmande de charges multiples en séquence à partir d'un % de commande          (Compatible IFS) 29/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG)                                                                29/06/2015 *
 *                VPE : [00] Commande de charge 0-100%                                                                                       *
 *                      [01] Sur la plage relative 0-100% de chaque charge, % d'arrêt de la charge                                           *
 *                      [02] Sur la plage relative 0-100% de chaque charge, % de marche de la charge                                         *
 *                VNC : [00] Nombre d'équipements demandés en marche                                                                         *
 *                      [01] Nombre total des équipements commandés par la fonction                                                          *
 *                FTC : [00] Inutilisé, maintenu à false                                                                                     *
 *                      [01] Commande de l'équipement 1. True signale un équipement demandé en marche.                                       *
 *                      ...  Ce tableau est composé de booléens                                                                              *
 *                      [NN] Commande du dernier équipement. True signale un équipement demandé en marche.                                   *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RE1 et RE2 maintenus à LOW                                                                                      *
 *                      0  : RE1 et RE2 figées dans son dernier état connu                                                                   *
 *                      1  : RE1 et RE2 commandées selon les états de FCO, de la priorité FP2, et de leurs défauts respectifs                *
 *                      2  : RE1 maintenue à HIGH, RE2 maintenue à LOW                                                                       *
 *                      3  : RE1 maintenue à LOW, RE2 maintenue à HIGH                                                                       *
 *                      >3 : RE1 et RE2 maintenus à LOW                                                                                      *
 *       Interne  NUM : [00] Index pour les itérations                                                                                       *
 *                      [01] Rang de l'équipement ptioritaire à la marche ou l'arrêt dans les tableaux                                       *
 *                      [02] Somme du contenu du tableau FTC des commandes (nombre de commandes actives)                                     *
 *                      [03] Somme du contenu du tableau VTD des défauts (nombre d'équipements en état normal)                               *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 29/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[4];
if (VNC[1] < 1) { return; }
switch (CFG) {
    case 0:
		return;
    case 1 :
		NUM[0] = 1;
		NUM[1] = 1;
		NUM[2] = 0;
		for(NUM[0] = 1;  NUM[0] <= VNC[1] ; NUM[0]++) {
			if (((VPE[0] / VNC[1]) - (NUM[0] - 1)) < (VPE[1] /VNC[1])) { FTC[NUM[0]] = false; }
			if (((VPE[0] / VNC[1]) - (NUM[0] - 1)) > (VPE[2] /VNC[1])) { FTC[NUM[0]] = true; }
			NUM[2] = NUM[2] + FTC[NUM[0]];
		}
		VNC[0] = NUM[2];
		if (NUM[2] > 0) { FTC[0] = true; } else { FTC[0] = false; }
		return;
	case 2 :
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
			FTC[NUM[0]] = true;
		}
		VNC[0] = VNC[1];
		return;
	case 3 :
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
			FTC[NUM[0]] = true;
		}
		VNC[0] = VNC[1];
		return;
	default:
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
			FTC[NUM[0]] = false;
		}
		VNC[0] = 0;
		return;
	}
return; }

void BMS::T_SEQDEM(float VPE[], float CPT[], int RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_SEQDEM - Fonction de démarrage en séquence d'équipements sur seuils haut et bas temporisés           (Compatible IFS) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_SEQDEM(float VPE[], float CPT[], int RES[], int CFG)                                                                  26/05/2015 *
 *                VPE : [00] Valeur analogique de base de la commande                                                                        *
 *                      [01] Seuil bas  de commande. Doit impérativement être inférieur à SHT                                                *
 *                      [02] Seuil haut de commande. Doit impérativement être supérieur à SBS                                                *
 *                CPT : [00] Compteur de secondes de dépassement des seuils bas et haut (RàZ entre les seuils)                               *
 *                      [01] Tempo de confirmation de dépassement du seuil bas  avant action sur la commande                                 *
 *                      [02] Tempo de confirmation de dépassement du seuil haut avant action sur la commande                                 *
 *                      [03] Tempo de stabilisation après chaque changement du nombre d'équipements demandés                                 *
 *                      [04] Temps de cycle automate calculé par la fonction                                                                 *
 *                      [05] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Nombre d'équipements demandés                                                                                   *
 *                      [01] Nombre minimum d'équipements demandés                                                                           *
 *                      [02] Nombre minimum d'équipements demandés                                                                           *
 *                CFG : Commande de forçage et de sens d'action des seuils                                                                   *
 *                      <-1: sortie RES[0] désactivée quelle que soit la valeur de VPE[0]                                                    *
 *                      -1 : sortie RES[0] diminue si VPE[0] > VPE[2] confirmé par CPT[2], augmente si VPE[0] < VPE[1] confirmé par CPT[1]   *
 *                      0  : sortie RES[0] figée à sa valeur courante                                                                        *
 *                      1  : sortie RES[0] augmente si VPE[0] > VPE[2] confirmé par CPT[2], diminue si VPE[0] < VPE[1] confirmé par CPT[1]   *
 *                      2  : sortie RES[0] activée quelle que soit la valeur de VPE[0]                                                       *
 *                      3  : sortie RES[0] activée quelle que soit la valeur de VPE[0]                                                       *
 *                      >3 : sortie RES[0] désactivée quelle que soit la valeur de VPE[0]                                                    *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/05/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *                         VPE[1] doit être inférieur à VPE[2], sinon la fonction commute sans tempo ni différentiel au niveau d'un seuil    *
 *                         VPE[0] doit passer entre VPE[1] et VPE[2] pour que le compteur de temporisation MEM[0] s'annule.                  *
 *                         La temporisation de stabilisation ne s'annule pas par un passage entre VPE[1] et VPE[2].                          *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[4] =  max(0, (millis() - CPT[5])) / 1000;
CPT[5] =  millis();
switch (CFG) {
    case 0:
		CPT[0]= 0;
		break;
    case 1:
		if ((VPE[0]>= VPE[1]) && (VPE[0]<= VPE[2])) { CPT[0]= min(0, CPT[0]); } else { CPT[0]= CPT[0] + CPT[4]; }
		if ((VPE[0] > VPE[2]) && (CPT[0]>= CPT[2])) { 
			RES[0] = max(RES[1], min(RES[2], (RES[0] + 1)));
			CPT[0] = 0 - CPT[3];
        }
		if ((VPE[0] < VPE[1]) && (CPT[0]>= CPT[1])) {
			RES[0] = max(RES[1], min(RES[2], (RES[0] - 1))) ;
			CPT[0] = 0 - CPT[3];
        }
      break;
    case -1:
		if ((VPE[0]>= VPE[1]) && (VPE[0]<= VPE[2])) { CPT[0]= 0; } else { CPT[0]= CPT[0] + CPT[4]; }
		if ((VPE[0] > VPE[2]) && (CPT[0]>= CPT[2])) {
			RES[0] = max(RES[1], min(RES[2], (RES[0] - 1)));
			CPT[0] = 0 - CPT[3];
        }
		if ((VPE[0] < VPE[1]) && (CPT[0]>= CPT[1])) {
			RES[0] = max(RES[1], min(RES[2], (RES[0] + 1)));
			CPT[0] = 0 - CPT[3];
        }
      break;
    case 2:
		RES[0] = RES[1];
		CPT[0]= 0;
		break;
    case 3:
		RES[0] = RES[2];
		CPT[0]= 0;
		break;
    default:
		RES[0] = 0;
		CPT[0]= 0;
		break;
    }
return; }