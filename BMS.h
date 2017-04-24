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
 *        T_DEMULT - Fonction de d�multiplexage d'un entier non sign� dans un tableau de bool�ens                (Compatible IFS) 22/10/2016 *
 *        T_DMULTV - Fonction de d�multiplexage 5DI / 1 AI sur une entr�e en tension                             (Compatible IFS) 07/01/2017 *
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

 /**
 * @ file	BMS.h
 * @brief	Biblioth�que de fonctions de contr�le-commande pour syst�mes de GTB.
 * @author	SE2R, Didier DORGUIN
 * @date	24/04/2017
 * @version	1.03
 */
 
#ifndef BMS_h
#define BMS_h
#include "Arduino.h"

class BMS
{
public :
	BMS();
	void C_CDELEM(boolean CDE, boolean DFT, boolean RES[], int CFG);
	void C_CDETPO(boolean CDE, boolean DFT, float CPT[], boolean RES[], int CFG);
	void C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[]);
	void C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG);
	void C_TFONCT(boolean MAR, float RES[], int CFG);
	void R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG);
	void R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG);
	void R_ANATOL(float VPC[], float VTC[], boolean RES[], int CFG);
	void R_ANATOR(float VPC[], float VTC[], int VNC, int CFG);
	void R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG);
	void S_SCANTS(float CPT[]);
    void S_TIMERS(float CPT[9], boolean FCO[7]);
	void T_APPRLN(float MES, int VTE[], float VTS[], int CFG);
	void T_CLINEB(float CDE[], float RES[], int CFG);
	void T_COMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG);
	void T_DEMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG);
	void T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG);
	void T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG);
	void T_LIMVAR(float MES[], float CPT[], int RES[], int CFG);
	void T_MOYDYN(float MES, float RES[], int CFG);
	void T_MULSEC(int VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG);
	void T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG);
	void T_SEQDEM(float MES[], float CPT[], int RES[], int CFG);
};
#endif