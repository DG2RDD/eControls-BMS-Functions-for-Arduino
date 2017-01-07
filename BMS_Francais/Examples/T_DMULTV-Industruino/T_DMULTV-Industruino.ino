/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - Fonction de démultiplexage 5DI / 1 AI sur une entrée en tension                             (Compatible IFS) 07/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DMULTV(float MES, float TNS[], float VNC[], boolean RES[], int CFG)                                                   07/09/2016 *
 *                MES : float, lecture en tension 0-10V vers un pourcentage (V10_p) de l'entrée analogique                                   *
 *                OHM : [00] Valeur de la résistance de charge de base                                                                       *
 *                      [01] Valeur de la résistance de charge de l'entrée digitale 1 du multiplexeur                                        *
 *                      [02] Valeur de la résistance de charge de l'entrée digitale 2 du multiplexeur                                        *
 *                      [03] Valeur de la résistance de charge de l'entrée digitale 3 du multiplexeur                                        *
 *                      [04] Valeur de la résistance de charge de l'entrée digitale 4 du multiplexeur                                        *
 *                      [05] Valeur de la résistance de charge de l'entrée digitale 5 du multiplexeur                                        *
 *                      [06] Somme des résistances de charge en ligne à chaque instant de [00] à [05]                                        *
 *                TNS : [00] Calcul de la tension en aval de la résistance de charge RES[0] selon la configuration des RES[1] à [5]          *
 *                      [01] Tension d'alimentation de référence : 24000 mV                                                                  *
 *                      [02] Différentiel de tension de compensation des imprécisions de résistances : 200 mV                                *
 *                      [03] Tension en aval de la résistance de charge RES[0] mesurée à vide pour une alimentation de référence 24Vdc       *
 *                      [04] Tension en aval de la résistance de charge RES[0] mesurée à vide en conditions réelles                          *
 *                      [05] Tension d'alimentation réelle recalculée à partir de TNS[3] et TNS[2]                                           *
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
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/09/2016 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Fonctions utilisées - Bibliothèque BMS                                                                                         07/09/2016 *
 * Fonctions utilisées - Bibliothèque DIS                                                                                         08/07/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <Wire.h>					// Biblio   - Gestion des ports de communication
#include <Indio.h>					// Biblio   - Industruino
#include <DIS.h>					// Biblio   - SE2R - Fonctions de dialogue et d'affichage sur l'écran UC1701 Industruino
static DIS DIS;						// Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage
static UC1701 lcd;
#include <BMS.h>					// Biblio   - SE2R - Fonctions de contrôle et régulation
static BMS BMS;						// Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage

// Déclaration des entrées et sorties -------------------------------------------------------------------------------------------------------*
float EA1_MII_MPLX;					// Général  - Mesure de tension en sortie du multiplexeur 1
float VTE_MPLX[7] ;					// Multi_V  - Calculs de tensions dans la chaîne de mesure
int VNC_MPLX[6];					// Multi-V  - Compteurs de confirmation de transition
boolean EM1_MII_MPLX[11];				// Général  - Etats des contacts en entrée du multiplexeur 1

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[11] = {					// EcranLCD - Table de chaînes de caractères de présentation
  "                     ", // 00
  "Haut, Enter : Accueil",
  "EA1                  ",
  "EM1                  ",
  "EM2                  ",
  "EM3                  ",
  "EM4                  ",
  "EM5                  ",
  "- Bas :  Reglage 0  -",
  "-  Reglage du Zero  -",
  " "};                   // 10

String TCM_UNIT[15] = {					// EcranLCD - Table de chaînes de caractères d'unités standard
  "        ",  // 00
  "oC      ",
  "%       ",
  "s       ",
  "V       ",
  "mV      ",  // 05
  "",
  "",
  "",
  "",
  "",          // 10
  "C.Ouvert ",
  "   Shunt ",
  "  Appris ",
  " Apprend "} ;

boolean FCO_PRUP[5];					// EcranLCD - Information de touche "Up" appuyée
boolean FCO_PREN[5];					// EcranLCD - Information de touche "Enter" appuyée
boolean FCO_PRDN[5];					// EcranLCD - Information de touche "Down" appuyée
int VNT_MENU[2];					// EcranLCD - Table d'adressage des menus
int VNC_MENU[30];					// EcranLCD - Table des numéros de menus
int VNC_PRUP[31];					// EcranLCD - Table des numéros de menus de destination par la touche "Up"
int VNC_PREN[30];					// EcranLCD - Table des numéros de menus de destination par la touche "Enter"
int VNC_PRDN[30];					// EcranLCD - Table des numéros de menus de destination par la touche "Down"
float VTT_SCRN[10];					// EcranLCD - Table des temporisations
boolean FCM_MPX1[1];					// G_MODV1B - Mémoire de la demande d'apprentissage de la tension de base du multiplexeur 1

void setup() {						// Initialisation principale -----------------------------------------------------------------------*
I_INPS();
I_DISP();
Serial.begin(9600);
}

void loop() {						// Boucle d'exécution principale -------------------------------------------------------------------*
P_INPS();
P_DISP();
}

void I_INPS() {						// Initialisation des entrées ---------------------------------------------------------------- INIT *
Indio.setADCResolution(14);				// Système  - Résolution des AI. Valeurs : 12bit@240Lps, 14bit@60Lps, 16bit@15Lps and 18bit@3.75Lps.
Indio.analogReadMode(1, V10_p);			        // Système  - Affectation de la voie AI1 en tant que 0-10V   -> 0-100%
VTE_MPLX[2] = 150;					// Offset de tension de détection en mV
VTE_MPLX[3] = VTE_MPLX[1] * (1 - (10110 / 14715));
VTE_MPLX[4] = VTE_MPLX[3];
VNC_MPLX[0] = 3;					// Nombre de lectures consécutives de confirmation
return;
}

void P_INPS() {                                         // Lecture des entrées ----------------------------------------------------------------------- EXEC *
  int RES[6] = {10110, 150, 301, 604, 1210, 2340};
  EA1_MII_MPLX = Indio.analogRead(1) ;
  BMS.T_DMULTV(EA1_MII_MPLX, RES, VTE_MPLX, VNC_MPLX, EM1_MII_MPLX, 1);
  Serial.print(VTE_MPLX[3]);
  Serial.print("\t");
  Serial.print(VTE_MPLX[4]);
  Serial.print("\t");
  Serial.print(EA1_MII_MPLX * 100);
  Serial.print("\t");
  Serial.print(VNC_MPLX[1]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[1]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[2]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[2]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[3]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[3]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[4]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[4]);
  Serial.print("\t");
  Serial.print(VNC_MPLX[5]);
  Serial.print("\t");
  Serial.print(EM1_MII_MPLX[5]);
  Serial.println("\t");
  return;
}

void I_DISP() {						// Initialisation des paramètres afficheur --------------------------------------------------- INIT *
VTT_SCRN[1] = 180;					// G_TOUCHx - Temporisation d'inactivité sur le rétroéclairage
VTT_SCRN[2] = 240;					// G_TOUCHx - Temporisation d'inactivité de retour à l'écran principal
VTT_SCRN[3] = 20;					// G_TOUCHx - Temporisation d'antirebond de la touche "Enter"
VTT_SCRN[4] = 15;					// G_TOUCHx - Temporisation d'antiredond des touches "Up" et "Down"
VNT_MENU[0] = 3;					// G_NAVIGM - Taille des tables d'index et de destinations
VNT_MENU[1] = 1;					// G_NAVIGM - Initialisation du pointeur de destinations
VNC_MENU[0] = 1;					// G_NAVIGM - Tables des adresses de menus
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_MENU[3] = 3;
			  				// G_NAVIGM - Tables des adresses de destination des actions sur les touches
VNC_PRUP[1] =   1; VNC_PREN[1] =   2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   1; VNC_PREN[2] =   1; VNC_PRDN[2] =   3;
VNC_PRUP[3] =   3; VNC_PREN[3] =   2; VNC_PRDN[3] =   3;
VNC_PRUP[4] =   2;
 
pinMode(23, INPUT);					// EcranLCD - Spécifique 1286 - Touche flèche basse
pinMode(24, INPUT);					// EcranLCD - Spécifique 1286 - Touche entrée
pinMode(25, INPUT);					// EcranLCD - Spécifique 1286 - Touche flèche haute
pinMode(26, OUTPUT);					// EcranLCD - Spécifique 1286 - Sortie de commande du rétroéclairage
lcd.begin();						// EcranLCD - Initialisation de l'écran LCD
lcd.clear();						// EcranLCD - Effacement de l'écran LCD
}

void P_DISP() {						// Gestion des menus de l'écran -------------------------------------------------------------- EXEC *
  DIS.G_TOUCH2(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN);
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 0); lcd.clearLine(); }
  DIS.G_MESS1L(VNC_MENU, 1, 1, 0, "    www.DG2R.com     ");
  DIS.G_MESS1L(VNC_MENU, 1, 2, 0, "Controle & Regulation");
  DIS.G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  1286  ");
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 4); lcd.clearLine(); }
  DIS.G_MESS1L(VNC_MENU, 1, 5, 0, "Multiplexeur  5DI/1AI");
  DIS.G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
  DIS.G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.00     07/09/2016");
  
  DIS.G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
  DIS.G_VISV1F(VNC_MENU, 2, 1, 0, EA1_MII_MPLX * 100, TCM_PRES[2], TCM_UNIT[5]);
  DIS.G_VISV1B(VNC_MENU, 2, 2, 0, EM1_MII_MPLX[1], TCM_PRES[3], TCM_UNIT[11], TCM_UNIT[12]);
  DIS.G_VISV1B(VNC_MENU, 2, 3, 0, EM1_MII_MPLX[2], TCM_PRES[4], TCM_UNIT[11], TCM_UNIT[12]);
  DIS.G_VISV1B(VNC_MENU, 2, 4, 0, EM1_MII_MPLX[3], TCM_PRES[5], TCM_UNIT[11], TCM_UNIT[12]);
  DIS.G_VISV1B(VNC_MENU, 2, 5, 0, EM1_MII_MPLX[4], TCM_PRES[6], TCM_UNIT[11], TCM_UNIT[12]);
  DIS.G_VISV1B(VNC_MENU, 2, 6, 0, EM1_MII_MPLX[5], TCM_PRES[7], TCM_UNIT[11], TCM_UNIT[12]);
  DIS.G_MESS1L(VNC_MENU, 2, 7, 0, TCM_PRES[8]);

  DIS.G_MODV1B(VNC_MENU, 3, FCO_PRUP, FCO_PREN, FCO_PRDN, EM1_MII_MPLX, 0, FCM_MPX1, "- Multiplexage 5D1A -", "EA1 - Reglage du Zero", TCM_UNIT[13], TCM_UNIT[14], true);

  DIS.G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
  return;
}