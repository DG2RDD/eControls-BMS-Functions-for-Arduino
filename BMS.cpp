/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : 24/04/2017 - Fonctions Arduino pour syst�mes de GTB - Fichier de code source                                        Version 1.03   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Fonction de cette biblioth�que                                                                                                            *
 *        C_CDELEM - Fonction de commande de bool�en avec prise en compte de for�age et de d�faut                (Compatible IFS) 12/06/2015 *
 *        C_CDETPO - Fonction de temporisation de commande � l'action et � l'arr�t                               (Compatible IFS) 03/06/2015 *
 *        C_COMMUT - Fonction de calcul de variable pour une Interface de For�age Standardis�e (IFS)                              12/06/2015 *
 *        C_DISCOR - Fonction de d�tection temporis�e de discordance entre deux valeurs ToR ou analogiques       (Compatible IFS) 29/06/2015 *
 *        C_TFONCT - Fonction de calcul de temps de fonctionnement                                               (Compatible IFS) 12/06/2015 *
 *        R_ANA3PL - Fonction de transformation d'un pourcentage en commande 3-Points sur deux points binaires   (Compatible IFS) 24/04/2017 *
 *        R_ANA3PT - Fonction de transformation d'un pourcentage en commande 3-Points                            (Compatible IFS) 24/04/2017 *
 *        R_ANATOL - Fonction de transformation d'un pourcentage en commande PWM vers un point binaire           (Compatible IFS) 08/06/2016 *
 *        R_ANATOR - Fonction de transformation d'un pourcentage en commande PWM                                 (Compatible IFS) 08/06/2016 *
 *        R_PIDITR - Fonction de r�gulation par correction PID it�rative                                         (Compatible IFS) 13/10/2016 *
 *        S_SCANTS - Fonction de calcul du temps de cycle programme en secondes                                                   07/08/2015 *
 *        S_TIMERS - Fonction de d�finition de temps de cycle � intervalles r�guliers                                             31/05/2016 *
 *        T_APPRLN - Fonction d'approximation lin�aire a segmnts multiples                                       (Compatible IFS) 31/03/2017 *
 *        T_CLINEB - Fonction de conversion lin�aire born�e de valeur num�rique                                  (Compatible IFS) 18/05/2015 *
 *        T_COMULT - Fonction de multiplexage d'un tableau de 16 bool�ens dans un entier non sign�               (Compatible IFS) 22/10/2016 *
 *        T_DEMULT - Fonction de d�multiplexage d'un entier non sign� dans un tableau de bool�ens                (Compatible IFS) 07/01/2017 *
 *        T_DMULTV - Fonction de d�multiplexage 5DI / 1 AI sur une entr�e en tension                             (Compatible IFS) 07/09/2016 *
 *        T_HYSTPO - Fonction d'hyst�r�sis (diff�rentiel) avec tempo de confirmation d'activation et d�sactivation  (IFS partiel) 19/05/2015 *
 *        T_LIMVAR - Fonction de limitation de la vitesse de variation d'une valeur float                        (Compatible IFS) 26/05/2015 *
 *        T_MOYDYN - Fonction de moyenne dynamique (filtre du premier ordre)                                     (Compatible IFS) 07/09/2016 *
 *        T_MULSEC - Fonction de commmande d'�quipements multiples avec secours mutuel et gestion de priorit�    (Compatible IFS) 12/07/2015 *
 *        T_SEQCSR - Fonction de commmande de charges multiples en s�quence � partir d'un pourcentage de commande(Compatible IFS) 29/06/2015 *
 *        T_SEQDEM - Fonction de d�marrage en s�quence d'�quipements sur seuils haut et bas temporis�s           (Compatible IFS) 26/05/2015 *
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
 * SE2R : C_CDELEM - Fonction de commande de bool�en avec prise en compte de for�age et de d�faut                (Compatible IFS) 12/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_CDELEM(boolean CDE, boolean DFT, boolean RES[], int CFG)                                                                         *
 *                CDE : Commande calcul�e ou donn�e par l'utilisateur (peut �tre le r�sultat d'une fonction appel�e plus haut)               *
 *                DFT : D�faut de l'�quipement command�. True signale un �quipement en �tat normal. Un false signale un d�faut               *
 *                RES : [00] Commande de l'�quipement - forc�ment un tableau de taille au moins 1                                            *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : RES maintenu � false                                                                                            *
 *                      0  : RES fig�e dans son dernier �tat connu                                                                           *
 *                      1  : RES soumise � CDE et � DEF                                                                                      *
 *                      2  : RES soumise seulement � DEF (comme si CDE �tait true en permanence)                                             *
 *                      3  : RES  maintenu � true                                                                                            *
 *                      >3 : RES maintenu � false                                                                                            *
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
 * SE2R : C_CDETPO - Fonction de temporisation de commande � l'action et � l'arr�t                               (Compatible IFS) 03/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_CDETPO(boolean CDE, boolean DFT, float CPT[], boolean RES[], int CFG)                                                            *
 *                CDE : Demande calcul�e ou donn�e par l'utilisateur (peut �tre le r�sultat d'une fonction appel�e plus haut)                *
 *                DFT : D�faut de l'�quipement command�. True signale un �quipement en �tat normal. Un false signale un d�faut               *
 *                CPT : [00] Compteur de temporisation                                                                                       *
 *                      [01] Tempo entre le front montant de la demande CDE avant l'action sur la commande                                   *
 *                      [02] Tempo entre le front descendant de la demande CDE avant la d�sactivation de la commande                         *
 *                      [03] Temps de cycle automate calcul� par la fonction                                                                 *
 *                      [04] M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Commande de l'�quipement - forc�ment un tableau de taille au moins 1                                            *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : RES maintenu � false                                                                                            *
 *                      0  : RES fig�e dans son dernier �tat connu                                                                           *
 *                      1  : RES soumise � CDE (� TMA et TAR pr�s) et � DFT                                                                  *
 *                      2  : RES soumise seulement � DFT (comme si CDE �tait true en permanence)                                             *
 *                      3  : RES  maintenu � true ind�pendamment de DFT et CDE                                                               *
 *                      >3 : RES maintenu � false                                                                                            *
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
 * SE2R : C_COMMUT - Fonction de calcul de variable de for�age                                                                    12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[])                                                            *
 *                FX1 : Demande de marche en auto (RES = 1) si true, d�bray� (RES = 0) si false                                              *
 *                FX2 : Demande de for�age interm�diaire (RES = 2), prioritaire sur FX1                                                      *
 *                FX3 : Demande de for�age en marche renforc� (RES = 3), prioritaire sur FX1 et FX2                                          *
 *                FX0 : Demande de for�age � l'arr�t (RES = -1), prioritaire sur tout autre for�age                                          *
 *                RES : [00] Commande de l'�quipement - forc�ment un tableau de taille au moins 1                                            *
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
 * SE2R : C_DISCOR - Fonction de d�tection temporis�e de discordance entre deux valeurs ToR ou analogiques       (Compatible IFS) 29/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG)                                                     *
 *                IN1 : Premi�re valeur � comparer                                                                                           *
 *                IN2 : Deuxi�me valeur � comparer                                                                                           *
 *                DIF : Diff�rence maximum tol�r�e entre IN1 et IN2                                                                          *
 *                TPO : [00] Compteur de temporisation                                                                                       *
 *                      [01] Tempo de confirmation de la discordance                                                                         *
 *                      [02] Temps de cycle automate calcul� par la fonction                                                                 *
 *                      [03] M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Discordance de l'�quipement - forc�ment un tableau de taille au moins 1                                         *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : RES maintenu � false donc en discordance                                                                        *
 *                      0  : RES fig�e dans son dernier �tat connu                                                                           *
 *                      1  : RES est true sauf si IN1 et IN2 diff�rent de plus de DIF pendant plus de CPT[1] sans discontinuer               *
 *                      2  : RES maintenu � true en permanence                                                                               *
 *                      3  : RES maintenu � true en permanence                                                                               *
 *                      >3 : RES maintenu � false                                                                                            *
 *       Interne  NUM : [00] Diff�rence courante entre IN1 et IN2                                                                            *
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
 *        C_TFONCT(boolean MAR, float RES[], int CFG)                                                                                        *
 *                MAR : Bool�en d'�tat de marche de l'�quipement                                                                             *
 *                RES : [00] Compteur d'heures de marche de l'�quipement                                                                     *
 *                      [01] M�moire de millis() du cycle pr�c�dent                                                                          *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : RES[0] annul�                                                                                                   *
 *                      0  : RES[0] fig�e � sa valeur courante                                                                               *
 *                      1  : RES[0] totalise le temps pendant lequel MAR est True en heures                                                  *
 *                      >1 : RES[0] annul�                                                                                                   *
 *       Interne  SCN : [00] Dur�e du temps de cycle UC en secondes                                                                          *
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
 * SE2R : R_ANA3PL - Fonction de transformation d'un pourcentage en commande 3-Points sur deux points binaires   (Compatible IFS) 24/04/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG)                                                                         *
 *                VPC : [00] Signal de commande de r�gulation en pourcentage � convertir                                                     *
 *                      [01] Signal de commande retranscrit sur l'actionneur 3-points en fin du cycle courant                                *
 *                VTC : [00] Dur�e du cycle de l'action 3-Points (s)                                                                         *
 *                      [01] Dur�e totale de la pleine course de l'actionneur (s)                                                            *
 *                      [02] Calcul du temps de cycle automate (s)                                                                           *
 *                      [03] Compteur de dur�e depuis la derni�re ex�cution de la fonction (s)                                               *
 *                      [04] M�moire des millis() de la derni�re ex�cution de la fonction                                                    *
 *                      [05] Dur�e de l'action sur les sorties pour le cycle d'ex�cution en cours                                            *
 *                RES : [00] Commande bool�enne d'ouverture  de l'actionneur 3-points                                                        *
 *                      [01] Commande bool�enne de fermeture de l'actionneur 3-points                                                        *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : VPC[0] maintenu � 0                                                                                             *
 *                      0  : VPC[0] fig� dans sa derni�re valeur connue                                                                      *
 *                      1  : VPC[0] est fix� par le calcul de r�gulation PID                                                                 *
 *                      2  : VPC[0] maintenue � VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue � 100                                                                                          *
 *                      >3 : VPC[0] maintenu � 0                                                                                             *
 *       Interne  NUM : [00] M�moire dans l'ex�cution de l'�cart du cycle pr�c�dent                                                          *
 *                      [01] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *                      [02] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 30/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[3];
VTC[2] =  max(0, (millis() - VTC[4])) / 1000;
VTC[3] =  VTC[3] + VTC[2];
VTC[4] =  millis();
switch (CFG) {
	case 0:
		return;
	case 1 :
		if (VTC[3] >= VTC[0]) {
			VTC[3] = 0;
			NUM[0] = VPC[0] - VPC[1];
			NUM[1] = NUM[0] * VTC[1] / 100;
			NUM[2] = abs(NUM[1]);
			if ((NUM[2] >= VTC[0]) || (VPC[0] < 0.1) || (VPC[0] > 99.9)) {
				if ((NUM[1] < 0) || (VPC[0] < 0.1)) {
					VTC[5] = 0 - VTC[0]; 
					VPC[1] = max(VPC[1] - (VTC[0] * 100 / VTC[1]), 0);
				}
				if ((NUM[1] > 0) || (VPC[0] > 99.9)) {
					VTC[5] = VTC[0];
					VPC[1] = min(VPC[1] + (VTC[0] * 100 / VTC[1]), 100);
				}
			} else {
				VTC[5] = NUM[1];
				if (abs(VTC[5]) >= VTC[2]) { VPC[1] = VPC[0]; }
			}
		}
		if (VTC[5] == VTC[0])  { RES[0] = true; RES[1] = false; }
		if ((VTC[5] < VTC[0]) && (VTC[5] > VTC[2]))  { VTC[5] = VTC[5] - VTC[2]; RES[0] = true; RES[1] = false; }
		if ((VTC[5] <= VTC[2]) && (VTC[5] >= 0 - VTC[2])) { RES[0] = false; RES[1] = false; }
		if ((VTC[5] < 0 - VTC[2]) && (VTC[5] > 0 - VTC[0])){ VTC[5] = VTC[5] + VTC[2]; RES[0] = false; RES[1] = true; }
		if (VTC[5] == 0 - VTC[0])  { RES[0] = false; RES[1] = true; }
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
return;}

void BMS::R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PT - Fonction de transformation d'un pourcentage en commande 3-Points                            (Compatible IFS) 24/04/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG)                                                                      *
 *                VPC : [00] Signal de commande de r�gulation en pourcentage � convertir                                                     *
 *                      [01] Signal de commande retranscrit sur l'actionneur 3-points en fin du cycle courant                                *
 *                VTC : [00] Dur�e du cycle de l'action 3-Points (s)                                                                         *
 *                      [01] Dur�e totale de la pleine course de l'actionneur (s)                                                            *
 *                      [02] Calcul du temps de cycle automate (s)                                                                           *
 *                      [03] Compteur de dur�e depuis la derni�re ex�cution de la fonction (s)                                               *
 *                      [04] M�moire des millis() de la derni�re ex�cution de la fonction                                                    *
 *                      [05] Dur�e de l'action sur les sorties pour le cycle d'ex�cution en cours                                            *
 *                VNO : Num�ro de la voie de la sortie de commande d'ouverture  de l'actionneur 3-points                                     *
 *                VNF : Num�ro de la voie de la sortie de commande de fermeture de l'actionneur 3-points                                     *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : VPC[0] maintenu � 0                                                                                             *
 *                      0  : VPC[0] fig� dans sa derni�re valeur connue                                                                      *
 *                      1  : VPC[0] est fix� par le calcul de r�gulation PID                                                                 *
 *                      2  : VPC[0] maintenue � VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue � 100                                                                                          *
 *                      >3 : VPC[0] maintenu � 0                                                                                             *
 *       Interne  NUM : [00] M�moire dans l'ex�cution de l'�cart du cycle pr�c�dent                                                          *
 *                      [01] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *                      [02] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 25/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[3];
VTC[2] =  max(0, (millis() - VTC[4])) / 1000;
VTC[3] =  VTC[3] + VTC[2];
VTC[4] =  millis();
switch (CFG) {
	case 0:
		return;
	case 1 :
		if (VTC[3] >= VTC[0]) {
			VTC[3] = 0;
			NUM[0] = VPC[0] - VPC[1];
			NUM[1] = NUM[0] * VTC[1] / 100;
			NUM[2] = abs(NUM[1]);
			if ((NUM[2] >= VTC[0]) || (VPC[0] < 0.1) || (VPC[0] > 99.9)) {
				if ((NUM[1] < 0) || (VPC[0] < 0.1)) {
					VTC[5] = 0 - VTC[0]; 
					VPC[1] = max(VPC[1] - (VTC[0] * 100 / VTC[1]), 0);
				}
				if ((NUM[1] > 0) || (VPC[0] > 99.9)) {
					VTC[5] = VTC[0];
					VPC[1] = min(VPC[1] + (VTC[0] * 100 / VTC[1]), 100);
				}
			} else {
				VTC[5] = NUM[1]; 
				if (abs(VTC[5]) >= VTC[2]) { VPC[1] = VPC[0]; }
			}
		}
		if (VTC[5] == VTC[0])  { digitalWrite(VNO, HIGH); digitalWrite(VNF, LOW); }
		if ((VTC[5] < VTC[0]) && (VTC[5] > VTC[2]))  { VTC[5] = VTC[5] - VTC[2]; digitalWrite(VNO, HIGH); digitalWrite(VNF, LOW); }
		if ((VTC[5] <= VTC[2]) && (VTC[5] >= 0 - VTC[2])) { digitalWrite(VNO, LOW); digitalWrite(VNF, LOW);  }
		if ((VTC[5] < 0 - VTC[2]) && (VTC[5] > 0 - VTC[0])){ VTC[5] = VTC[5] + VTC[2]; digitalWrite(VNO, LOW); digitalWrite(VNF, HIGH);  }
		if (VTC[5] == 0 - VTC[0])  { digitalWrite(VNO, LOW); digitalWrite(VNF, HIGH);  }
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
return;}

void BMS::R_ANATOL(float VPC[], float VTC[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANATOL - Fonction de transformation d'un pourcentage en commande PWM                                 (Compatible IFS) 08/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANATOL(float VPC[], int VTC[], boolean RES[], int CFG)                                                                           *
 *                VPC : [00] Signal de commande de r�gulation en pourcentage � convertir                                                     *
 *                VTC : [00] Dur�e totale de la pleine course de l'actionneur (s)                                                            *
 *                      [01] Calcul du temps de cycle automate (s)                                                                           *
 *                      [02] Compteur de dur�e depuis la derni�re ex�cution de la fonction (s)                                               *
 *                      [03] M�moire des millis() de la derni�re ex�cution de la fonction                                                    *
 *                      [04] Dur�e de l'action sur la sortie pour le cycle d'ex�cution en cours                                              *
 *                RES : [00] Commande bool�enne de l'actionneur PWM                                                                          *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : VPC[0] maintenu � 0                                                                                             *
 *                      0  : VPC[0] fig� dans sa derni�re valeur connue                                                                      *
 *                      1  : VPC[0] est fix� par le calcul de r�gulation PID                                                                 *
 *                      2  : VPC[0] maintenue � VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue � 100                                                                                          *
 *                      >3 : VPC[0] maintenu � 0                                                                                             *
 *       Interne  NUM : [00] M�moire dans l'ex�cution de l'�cart du cycle pr�c�dent                                                          *
 *                      [01] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *                      [02] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
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
 *        R_ANATOR(float VPC[], int VTC[], int VNO, int CFG)                                                                                 *
 *                VPC : [00] Signal de commande de r�gulation en pourcentage � convertir                                                     *
 *                VTC : [00] Dur�e totale de la pleine course de l'actionneur (s)                                                            *
 *                      [01] Calcul du temps de cycle automate (s)                                                                           *
 *                      [02] Compteur de dur�e depuis la derni�re ex�cution de la fonction (s)                                               *
 *                      [03] M�moire des millis() de la derni�re ex�cution de la fonction                                                    *
 *                      [04] Dur�e de l'action sur la sortie pour le cycle d'ex�cution en cours                                              *
 *                VNC : Num�ro de la voie de la sortie de commande d'ouverture  de l'actionneur PWM                                          *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : VPC[0] maintenu � 0                                                                                             *
 *                      0  : VPC[0] fig� dans sa derni�re valeur connue                                                                      *
 *                      1  : VPC[0] est fix� par le calcul de r�gulation PID                                                                 *
 *                      2  : VPC[0] maintenue � VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue � 100                                                                                          *
 *                      >3 : VPC[0] maintenu � 0                                                                                             *
 *       Interne  NUM : [00] M�moire dans l'ex�cution de l'�cart du cycle pr�c�dent                                                          *
 *                      [01] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *                      [02] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
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
 * SE2R : R_PIDITR - Fonction de r�gulation par correction PID it�rative                                         (Compatible IFS) 13/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG)                                                                  *
 *                MES : Mesure de la grandeur de r�gulation                                                                                  *
 *                CSG : Consigne de r�gulation                                                                                               *
 *                VPC : [00] Signal de commande de r�gulation                                                                                *
 *                      [01] M�moire de l'�cart du cycle pr�c�dent                                                                           *
 *                      [02] M�moire de la variation d'�cart du cycle pr�c�dent                                                              *
 *                      [03] Valeur de repli de la commande (CFG = 2)                                                                        *
 *                VKR : [00] Dur�e du cycle du r�gulateur (s)                                                                                *
 *                      [01] Coefficient de transfert (Unit�s)                                                                               *
 *                      [02] Bande proportionnelle (%)                                                                                       *
 *                      [03] Temps int�gral (s)                                                                                              *
 *                      [04] Temps d�riv� (s)                                                                                                *
 *                      [05] Ecart (Unit�s)                                                                                                  *
 *                      [06] Dur�e calcul�e du cycle automate (s)                                                                            *
 *                      [07] Compteur de dur�e depuis la derni�re ex�cution de la fonction (s)                                               *
 *                      [08] M�moire des millis() de la derni�re ex�cution de la fonction                                                    *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : VPC[0] maintenu � 0                                                                                             *
 *                      0  : VPC[0] fig� dans sa derni�re valeur connue                                                                      *
 *                      1  : VPC[0] est fix� par le calcul de r�gulation PID                                                                 *
 *                      2  : VPC[0] maintenue � VPC[3]                                                                                       *
 *                      3  : VPC[0] maintenue � 100                                                                                          *
 *                      >3 : VPC[0] maintenu � 0                                                                                             *
 *       Interne  NUM : [00] M�moire dans l'ex�cution de l'�cart du cycle pr�c�dent                                                          *
 *                      [01] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *                      [02] M�moire dans l'ex�cution de la variation d'�cart du cycle pr�c�dent                                             *
 *                      [03] Calcul de la correction � appliquer � VPC[0] pour le cycle en cours                                             *
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
 *        S_SCANTS(float CPT[])                                                                                                              *
 *                CPT : [00]   : Temps de cycle automate calcul� par la fonction                                                             *
 *                      [01]   : M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                 *
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
 * SE2R : S_TIMERS - Fonction de d�finition de temps de cycle � intervalles r�guliers                                             31/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        S_TIMERS(float CPT[6], boolean FCO[4])                                                                                             *
 *                CPT : [00]   : Compteur de secondes, reset toutes les secondes                                                             *
 *                      [01]   : Compteur de secondes, reset toutes les 10 secondes                                                          *
 *                      [02]   : Compteur de secondes, reset toutes les 30 secondes                                                          *
 *                      [03]   : Compteur de secondes, reset toutes les minutes                                                              *
 *                      [04]   : Compteur de secondes, reset toutes les  5 minutes                                                           *
 *                      [05]   : Compteur de secondes, reset toutes les 30 minutes                                                           *
 *                      [06]   : Compteur de secondes, reset toutes les heures                                                               *
 *                      [07]   : Temps de cycle automate calcul� par la fonction                                                             *
 *                      [08]   : M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                 *
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
 * SE2R : T_APPRLN - Fonction d'approximation lin�aire a segmnts multiples                                       (Compatible IFS) 31/03/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_APPRLN(float MES, int VTE[], float VTS[], int CFG)                                                                               *
 *                MES : Valeur � convertir                                                                                                   *
 *                VTE : [00] Nombre de bornes de segments d'entr�es et de sorties                                                            *
 *                      [01] Borne inf�rieure du premier segment d'entr�e                                                                    *
 *                      [02] Borne sup�rieure du premier segment d'entr�e et borne inf�rieure du 2e segment d'entr�e                         *
 *                      ...    Ce tableau est compos� d'integer forc�ment strictement croissants                                             *
 *                      [NN] Borne sup�rieure du dernier segment d'entr�e. NN = VTE[0]                                                       *
 *                VTS : [00] 	R�sultat de la conversion                                                                                       *
 *                      [01] Borne inf�rieure du premier segment de sortie                                                                   *
 *                      [02] Borne sup�rieure du premier segment d'entr�e et borne inf�rieure du 2e segment de sortie                        *
 *                      ...  Ce tableau est compos� de float. Il n'est pas n�cessairement croissant mais �viter les �galit�s � la suite      *
 *                      [NN] Borne sup�rieure du dernier segment de sortie. NN = VTE[0]                                                      *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : VTS[0] maintenu � 0                                                                                             *
 *                      0  : VTS[0] fig� dans sa derni�re valeur connue                                                                      *
 *                      1  : VTS[0] suit la courbe d�finie par VTE[] et VTS[]                                                                *
 *                      2  : VTS[0] maintenue � VTS[1]                                                                                       *
 *                      3  : VTS[0] maintenue � VTS[VTE[0]]                                                                                  *
 *                      >3 : VTS[0] maintenu � 0                                                                                             *
 *       Interne  NUM : [00] Index pour les diverses it�rations                                                                              *
 *                      [01] Index de la table d'entr�e : borne sup�rieure du segment de la table VTE o� se trouve MES                       *
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
		if (MES <= VTE[1]) { VTS[0] = VTS[1]; break; }
		if ((MES > VTE[1]) && (MES < VTE[VTE[0]])) {
			for(NUM[0] = 2;  NUM[0] <= VTE[0] ; NUM[0]++) { if (MES > VTE[NUM[0] - 1]) { NUM[1] = NUM[0]; } }
			if (VTE[NUM[1]] == VTE[NUM[1] - 1]) { 
				VTS[0] =  VTS[NUM[1] - 1];
			} else {
				VTS[0] = VTS[NUM[1] - 1] + (((MES - VTE[NUM[1] - 1]) * (VTS[NUM[1]] - VTS[NUM[1] - 1])) / (VTE[NUM[1]] - VTE[NUM[1] - 1])); 
			}
			break;
		}
		if (MES >= VTE[VTE[0]])	{ VTS[0] = VTS[VTE[0]]; break; }
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
 * SE2R : T_CLINEB - Fonction de conversion lin�aire born�e de valeur num�rique                                  (Compatible IFS) 18/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_CLINEB(float CDE[], float RES[], int CFG)                                                                                        *
 *                CDE : [00] Commande analogique d'entr�e, qui doit subir la conversion                                                      *
 *                      [01] Borne 1 sur la commande d'entr�e, correspond � une valeur convertie de BS1. Peut �tre > ou < BE2                *
 *                      [02] Borne 2 sur la commande d'entr�e, correspond � une valeur convertie de BS2. Peut �tre > ou < BE1                *
 *                RES : [00] R�sultat de la conversion : si CDE hors [BE1-BE2], BS1 ou BS2. Sinon conversion lin�aire BE1-BE2 / BS1-BS2      *
 *                      [01] Borne 1 de conversion, correspond � une valeur d'entr�e de BE1. Peut �tre > ou < BS2                            *
 *                      [02] Borne 2 de conversion, correspond � une valeur d'entr�e de BE2. Peut �tre > ou < BS1                            *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : RES maintenue � 0                                                                                               *
 *                      0  : RES fig�e dans son dernier �tat connu                                                                           *
 *                      1  : RES est le r�sultat de la conversion lin�aire                                                                   *
 *                      2  : RES maintenue � BS1                                                                                             *
 *                      3  : RES maintenue � BS2                                                                                             *
 *                      >3 : RES maintenue � 0                                                                                               *
 *       Interne  NUM : Calcul interm�diaire de la valeur convertie                                                                          *
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
 * SE2R : T_COMULT - Fonction de multiplexage d'un tableau de 16 bool�ens dans un entier non sign�               (Compatible IFS) 22/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_COMULT(unsigned int MPX[], int RNG, Boolean FLG[], int CFG)                                                                      *
 *                MPX : [00] : Entier, Variable porteuse des bool�ens multiplex�s, forc�ment un tableau de taille au moins 1                 *
 *                RNG : Entier, rang de la variable porteuse dans le tableau des variables porteuses                                         *
 *                FLG : [00] Bool�en de rang 0 dans le tableau de taille 16                                                                  *
 *                      [..] Suite du tableau de bool�ens � multiplexer                                                                      *
 *                      [15] Dernier bool�en du tableau � multiplexer                                                                        *
 *                CFG : Commande de for�age                                                                                                  *
 *                      <0  : la sortie MPX[RNG] reste nulle quelle que soit la valeur des FLG[...]                                          *
 *                      0   : la sortie MPX[RNG] reste fig�e � sa valeur courante                                                            *
 *                      1   : la sortie MPX[RNG] suit les valeurs de FLG[...] encod�s suivant les puissances de 2                            *
 *                      2   : la sortie MPX[RNG] prend pour valeur 65535                                                                     *
 *                      3   : la sortie MPX[RNG] prend pour valeur 65535                                                                     *
 *                      >3  : la sortie MPX[RNG] reste nulle quelle que soit la valeur des FLG[...]                                          *
 *       Interne  NUM : [00] Calcul interm�diaire d'addition des puissances de 2                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 06/04/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 22/10/2016 : DD, SE2R : Si m�morisation EEPROM de MPX[RNG], toujours retirer 1 � MPX[RNG] juste apr�s le T_COMULT (pb de bit 0 EEPROM)    *
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
 * SE2R : T_DEMULT - Fonction de d�multiplexage d'un entier non sign� dans un tableau de bool�ens                (Compatible IFS) 22/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DEMULT(int MPX[], int RNG, Boolean FLG[], int CFG)                                                                               *
 *                MPX : [00] : Entier, Variable porteuse des bool�ens multiplex�s, forc�ment un tableau de taille au moins 1                 *
 *                RNG : Entier, rang de la variable porteuse dans le tableau des variables porteuses                                         *
 *                FLG : [00] : Bool�en de rang 0 dans le tableau                                                                             *
 *                      [..] : Suite du tableau de bool�ens � multiplexer                                                                    *
 *                      [15] : Dernier bool�en du tableau � multiplexer                                                                      *
 *                CFG : Commande de for�age                                                                                                  *
 *                      <0  : les bool�ens FLG[xx] restent false quelle que soit la valeur de MPX[RNG]                                       *
 *                      0   : les bool�ens FLG[xx] restent fig�s � leur valeur courante                                                      *
 *                      1   : les bool�ens FLG[xx] suivent les valeurs de MPX[RNG] d�cod� suivant les puissances de 2                        *
 *                      2   : les bool�ens FLG[xx] restent true quelle que soit la valeur de MPX[RNG]                                        *
 *                      3   : les bool�ens FLG[xx] restent true quelle que soit la valeur de MPX[RNG]                                        *
 *                      >3  : les bool�ens FLG[xx] restent false quelle que soit la valeur de MPX[RNG]                                       *
 *       Interne  NUM : [00] Calcul interm�diaire de d�codage des puissances de 2                                                            *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 06/04/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 22/10/2016 : DD, SE2R : Test�e et valid�e sur banc 32U8, 1286, Arduino Leonardo. Si m�morisation EEPROM, toujours red�marrer apr�s charge *
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
 * SE2R : T_DMULTV - Fonction de d�multiplexage 5DI / 1 AI sur une entr�e en tension                             (Compatible IFS) 07/01/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG)                                                     *
 *                MES : float, lecture en tension 0-10V vers un pourcentage (V10_p) de l'entr�e analogique                                   *
 *                OHM : [00] Valeur de la r�sistance de charge de base                                                                       *
 *                      [01] Valeur de la r�sistance de charge de l'entr�e digitale 1 du multiplexeur                                        *
 *                      [02] Valeur de la r�sistance de charge de l'entr�e digitale 2 du multiplexeur                                        *
 *                      [03] Valeur de la r�sistance de charge de l'entr�e digitale 3 du multiplexeur                                        *
 *                      [04] Valeur de la r�sistance de charge de l'entr�e digitale 4 du multiplexeur                                        *
 *                      [05] Valeur de la r�sistance de charge de l'entr�e digitale 5 du multiplexeur                                        *
 *                TNS : [00] Calcul de la tension en aval de la r�sistance de charge RES[0] selon la configuration des RES[1] � [5]          *
 *                      [01] Tension d'alimentation de r�f�rence : 24000 mV                                                                  *
 *                      [02] Diff�rentiel de tension de compensation des impr�cisions de r�sistances : 150 � 200 mV                          *
 *                      [03] Tension en aval de la r�sistance de charge RES[0] mesur�e � vide pour une alimentation de r�f�rence 24Vdc       *
 *                      [04] Tension en aval de la r�sistance de charge RES[0] mesur�e � vide en conditions r�elles                          *
 *                      [05] Tension d'alimentation r�elle recalcul�e � partir de TNS[3] et TNS[4]                                           *
 *                      [06] Somme des r�sistances de charge en ligne � chaque instant de [00] � [05] au format float                        *
 *                VNC : [00] Nombre de cycles de confirmation antirebond d'une transition d'une des entr�es RES                              *
 *                      [01] Compteur de nombre de cycles de diff�rence continue entre RES[1] et RES[6]                                      *
 *                      [02] Compteur de nombre de cycles de diff�rence continue entre RES[2] et RES[7]                                      *
 *                      [03] Compteur de nombre de cycles de diff�rence continue entre RES[3] et RES[8]                                      *
 *                      [04] Compteur de nombre de cycles de diff�rence continue entre RES[4] et RES[9]                                      *
 *                      [05] Compteur de nombre de cycles de diff�rence continue entre RES[5] et RES[10]                                     *
 *                RES : [00] Commande de m�morisation de la tension � vide du multiplexeur � la calibration                                  *
 *                      [01] Etat calcul� de l'entr�e digitale 1 du multiplexeur apr�s traitement antirebond                                 *
 *                      ...                                                                                                                  *
 *                      [05] Etat calcul� de l'entr�e digitale 5 du multiplexeur apr�s traitement antirebond                                 *
 *                      [06] Etat calcul� de l'entr�e digitale 1 du multiplexeur avant traitement antirebond                                 *
 *                      ...                                                                                                                  *
 *                      [10] Etat calcul� de l'entr�e digitale 5 du multiplexeur avant traitement antirebond                                 *
 *                CFG : Commande de for�age                                                                                                  *
 *                      <0  : les bool�ens RES[1] � [5] restent false quelle que soit la valeur de MPX[RNG]                                  *
 *                      0   : les bool�ens RES[1] � [5] restent fig�s � leur valeur courante                                                 *
 *                      1   : les bool�ens RES[1] � [5] suivent les valeurs de MES d�cod� suivant les valeurs de tensions                    *
 *                      2   : les bool�ens RES[1] � [5] restent true quelle que soit la valeur de MPX[RNG]                                   *
 *                      3   : les bool�ens RES[1] � [5] restent true quelle que soit la valeur de MPX[RNG]                                   *
 *                      >3  : les bool�ens RES[1] � [5] restent false quelle que soit la valeur de MPX[RNG]                                  *
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
 * SE2R : T_HYSTPO - Fonction d'hyst�r�sis (diff�rentiel) avec tempo de confirmation d'activation et d�sactivation  (IFS partiel) 19/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG)                                                                         *
 *                MES : [00] Valeur analogique de base de la commande                                                                        *
 *                      [01] Seuil bas  de commande. Doit imp�rativement �tre inf�rieur � SHT                                                *
 *                      [02] Seuil haut de commande. Doit imp�rativement �tre sup�rieur � SBS                                                *
 *                CPT : [00] Compteur de secondes de d�passement des seuils bas et haut (R�Z entre les seuils)                               *
 *                      [01] Tempo de confirmation de d�passement du seuil bas  avant action sur la commande                                 *
 *                      [02] Tempo de confirmation de d�passement du seuil haut avant action sur la commande                                 *
 *                      [03] Temps de cycle automate calcul� par la fonction                                                                 *
 *                      [04] M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Commande de l'�quipement - forc�ment un tableau de taille au moins 1                                            *
 *                CFG : Commande de for�age et de sens d'action des seuils                                                                   *
 *                      <-1: sortie RES d�sactiv�e quelle que soit la valeur de MES[0]                                                       *
 *                      -1 : sortie RES d�sactiv�e si MES[0] > MES[2] confirm� par CPT[2], activ�e si MES[0] < MES[1] confirm� par CPT[1]    *
 *                      0  : sortie RES fig�e � sa valeur courante                                                                           *
 *                      1  : sortie RES activ�e si MES[0] > MES[2] confirm� par CPT[2], d�sactiv�e si MES[0] < MES[1] confirm� par CPT[1]    *
 *                      2  : sortie RES activ�e quelle que soit la valeur de MES[0]                                                          *
 *                      3  : sortie RES activ�e quelle que soit la valeur de MES[0]                                                          *
 *                      >3 : sortie RES d�sactiv�e quelle que soit la valeur de MES[0]                                                       *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 19/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 26/05/2015 : SE2R, DD : MES[1] doit �tre inf�rieur � MES[2], sinon la fonction commute sans tempo ni diff�rentiel au niveau d'un seuil    *
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
 *        T_LIMVAR(float MES[], float CPT[], int RES[], int CFG)                                                                             *
 *                MES : [00] Valeur analogique de base de la commande                                                                        *
 *                      [01] Vitesse maximum de diminution de VPC_XXXX                                                                       *
 *                      [02] Vitesse maximum d'augmentation de VPC_XXXX                                                                      *
 *                      [03] M�moire de RES en d�but d'ex�cution de la fonction                                                              *
 *                CPT : [00] Temps de cycle automate calcul� par la fonction                                                                 *
 *                      [01] M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Commande de l'�quipement - forc�ment un tableau de taille au moins 1                                            *
 *                CFG : Commande de for�age et de sens d'action des seuils                                                                   *
 *                      <0 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 *                      0  : sortie RES[0] fig�e � sa valeur courante                                                                        *
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
 *        T_MOYDYN(float MES, float RES[], int CFG)                                                                                          *
 *                MES : Valeur analogique de base de la commande                                                                             *
 *                RES : [00] Commande de l'�quipement - forc�ment un tableau de taille au moins 2                                            *
 *                      [01] Nombre de cycles sur lequel la moyenne est effectu�e                                                            *
 *                CFG : Commande de for�age                                                                                                  *
 *                      <0 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 *                      0  : sortie RES[0] fig�e � sa valeur courante                                                                        *
 *                      1  : sortie RES[0] suit MES[0] en respectant les limites d'augmentation et de diminution                             *
 *                      2  : sortie RES[0] suit MES[0] sans amortissement                                                                    *
 *                      3  : sortie RES[0] suit MES[0] sans amortissement                                                                    *
 *                      >3 : sortie RES[0] nulle quelle que soit la valeur de MES[0]                                                         *
 *       Interne  NUM : [00] Nombre d'it�rations avec d�tection de mauvais param�trage de RES[1]                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *                         Si RES[1] n'est pas renseign�, il prend par d�faut la valeur 10                                                   *
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
 * SE2R : T_MULSEC - Fonction de commmande d'�quipements multiples avec secours mutuel et gestion de priorit�    (Compatible IFS) 12/07/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_MULSEC(float VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG)                                                *
 *                VNC : [00] Nombre d'�quipements demand�s en marche                                                                         *
 *                      [01] Nombre total des �quipements command�s par la fonction                                                          *
 *                VTP : [00] Inutilis�, maintenu � 0                                                                                         *
 *                      [01] Ordre de priorit� de l'�quipement 1 lors de la prochaine transition (d�faut ou nb demand� VNC[0])               *
 *                      ...  L'ordre de priorit� le plus bas est prioritaire � la marche, le plus haut prioritaire � l'arr�t                 *
 *                      [NN] Ordre de priorit� du dernier �quipement lors de la prochaine transition (d�faut ou nb demand� VNC[0])           *
 *                VTD : [00] Inutilis�, maintenu � false                                                                                     *
 *                      [01] D�faut de l'�quipement 1. True signale un �quipement en �tat normal. False signale un d�faut.                   *
 *                      ...  Ce tableau est compos� de bool�ens                                                                              *
 *                      [NN] D�faut du dernier �quipement. True signale un �quipement en �tat normal. False signale un d�faut.               *
 *                VMC : [00] M�moire de NUM[1] : rang de la derni�re sortie command�e                                                        *
 *                      [01] M�moire de VMC[4] : Nombre de sorties � command�es lors du cycle pr�c�dent                                      *
 *                      [02] M�moire de NUM[2] : Nombre de commandes actives dans le cycle courant                                           *
 *                      [03] M�moire de NUM[3] : Nombre d'�quipements en �tat normal                                                         *
 *                      [04] M�moire du nombre d'�quipements � commander dans le cycle                                                       *
 *                FTC : [00] Information de synth�se de demande de marche : un au moins des �quipements est command�                         *
 *                      [01] Commande de l'�quipement 1. True signale un �quipement demand� en marche.                                       *
 *                      ...  Ce tableau est compos� de bool�ens                                                                              *
 *                      [NN] Commande du dernier �quipement. True signale un �quipement demand� en marche.                                   *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : Les FTC[x] sont maintenus � false                                                                               *
 *                      0  : Tous les FTC[x] fig�s dans leur dernier �tat connu                                                              *
 *                      1  : Les FTC[x] sont command�s selon VNC[0], des priorit�s VTP[x], et leurs d�fauts respectifs VTD[x]                *
 *                      2  : Les FTC[x] sont maintenus � true                                                                                *
 *                      3  : Les FTC[x] sont maintenus � true                                                                                *
 *                      >3 : Les FTC[x] sont maintenus � false                                                                               *
 *       Interne  NUM : [00] Index pour les it�rations                                                                                       *
 *                      [01] Rang de l'�quipement prioritaire � la marche ou l'arr�t dans les tableaux                                       *
 *                      [02] Somme du contenu du tableau FTC des commandes (nombre de commandes actives)                                     *
 *                      [03] Somme du contenu du tableau VTD des d�fauts (nombre d'�quipements en �tat normal)                               *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 30/06/2015 : DD, SE2R : Ajout du for�age diff�renci� en 2 et 3 dans le cas de deux �quipements command�s                                  *
 * 12/07/2015 : DD, SE2R : Correction d'un bug de fonctionnement � 2 �quipements                                                             *
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
 * SE2R : T_SEQCSR - Fonction de commmande de charges multiples en s�quence � partir d'un % de commande          (Compatible IFS) 29/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG)                                                                           *
 *                VPE : [00] Commande de charge 0-100%                                                                                       *
 *                      [01] Sur la plage relative 0-100% de chaque charge, % d'arr�t de la charge                                           *
 *                      [02] Sur la plage relative 0-100% de chaque charge, % de marche de la charge                                         *
 *                VNC : [00] Nombre d'�quipements demand�s en marche                                                                         *
 *                      [01] Nombre total des �quipements command�s par la fonction                                                          *
 *                FTC : [00] Inutilis�, maintenu � false                                                                                     *
 *                      [01] Commande de l'�quipement 1. True signale un �quipement demand� en marche.                                       *
 *                      ...  Ce tableau est compos� de bool�ens                                                                              *
 *                      [NN] Commande du dernier �quipement. True signale un �quipement demand� en marche.                                   *
 *                CFG : Commande de for�age.                                                                                                 *
 *                      <0 : RE1 et RE2 maintenus � LOW                                                                                      *
 *                      0  : RE1 et RE2 fig�es dans son dernier �tat connu                                                                   *
 *                      1  : RE1 et RE2 command�es selon les �tats de FCO, de la priorit� FP2, et de leurs d�fauts respectifs                *
 *                      2  : RE1 maintenue � HIGH, RE2 maintenue � LOW                                                                       *
 *                      3  : RE1 maintenue � LOW, RE2 maintenue � HIGH                                                                       *
 *                      >3 : RE1 et RE2 maintenus � LOW                                                                                      *
 *       Interne  NUM : [00] Index pour les it�rations                                                                                       *
 *                      [01] Rang de l'�quipement ptioritaire � la marche ou l'arr�t dans les tableaux                                       *
 *                      [02] Somme du contenu du tableau FTC des commandes (nombre de commandes actives)                                     *
 *                      [03] Somme du contenu du tableau VTD des d�fauts (nombre d'�quipements en �tat normal)                               *
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
 * SE2R : T_SEQDEM - Fonction de d�marrage en s�quence d'�quipements sur seuils haut et bas temporis�s           (Compatible IFS) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_SEQDEM(float VPE[], float CPT[], int RES[], int CFG)                                                                             *
 *                VPE : [00] Valeur analogique de base de la commande                                                                        *
 *                      [01] Seuil bas  de commande. Doit imp�rativement �tre inf�rieur � SHT                                                *
 *                      [02] Seuil haut de commande. Doit imp�rativement �tre sup�rieur � SBS                                                *
 *                CPT : [00] Compteur de secondes de d�passement des seuils bas et haut (R�Z entre les seuils)                               *
 *                      [01] Tempo de confirmation de d�passement du seuil bas  avant action sur la commande                                 *
 *                      [02] Tempo de confirmation de d�passement du seuil haut avant action sur la commande                                 *
 *                      [03] Tempo de stabilisation apr�s chaque changement du nombre d'�quipements demand�s                                 *
 *                      [04] Temps de cycle automate calcul� par la fonction                                                                 *
 *                      [05] M�morisation de la variable syst�me Millis pour le calcul du temps de cycle                                     *
 *                RES : [00] Nombre d'�quipements demand�s                                                                                   *
 *                      [01] Nombre minimum d'�quipements demand�s                                                                           *
 *                      [02] Nombre minimum d'�quipements demand�s                                                                           *
 *                CFG : Commande de for�age et de sens d'action des seuils                                                                   *
 *                      <-1: sortie RES[0] d�sactiv�e quelle que soit la valeur de VPE[0]                                                    *
 *                      -1 : sortie RES[0] diminue si VPE[0] > VPE[2] confirm� par CPT[2], augmente si VPE[0] < VPE[1] confirm� par CPT[1]   *
 *                      0  : sortie RES[0] fig�e � sa valeur courante                                                                        *
 *                      1  : sortie RES[0] augmente si VPE[0] > VPE[2] confirm� par CPT[2], diminue si VPE[0] < VPE[1] confirm� par CPT[1]   *
 *                      2  : sortie RES[0] activ�e quelle que soit la valeur de VPE[0]                                                       *
 *                      3  : sortie RES[0] activ�e quelle que soit la valeur de VPE[0]                                                       *
 *                      >3 : sortie RES[0] d�sactiv�e quelle que soit la valeur de VPE[0]                                                    *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/05/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *                         VPE[1] doit �tre inf�rieur � VPE[2], sinon la fonction commute sans tempo ni diff�rentiel au niveau d'un seuil    *
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