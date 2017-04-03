/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PL - Fonction de transformation d'un pourcentage en commande 3-Points sur deux points binaires                    29/03/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PL(float VPC[], float VTC[], boolean FCC[], int CFG)                                                                         *
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
 * Plate-formes testées : Industruino 32U8 et 1286                                                                                           *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 29/03/2017 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <Wire.h>					// Biblio   - Gestion des ports de communication
#include <Indio.h>					// Biblio   - Industruino
#include <DIS.h>					// Biblio   - SE2R - Fonctions de dialogue et d'affichage sur l'écran UC1701 Industruino
static DIS DIS;						// Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage
static UC1701 lcd;
#include <BMS.h>					// Biblio   - SE2R - Fonctions de contrôle et régulation
static BMS BMS;						// Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage

// Déclaration des variables nécessaires à l'exécution du programme -------------------------------------------------------------------------*
float VPC_XXXX[2];					// R_ANA3PL - Table des commandes en sortie du régulateur
float VTC_XXXX[6];					// R_ANA3PL - Table des temporisations de commande de la vanne 3 points
boolean FCC_XXXX[2];					// R_ANA3PL - Table des commandes de sorties de la vanne 3 points
int VCO_XXXX;						// R_ANA3PL - Commande de forçage, ici en position auto
boolean SD7_SOL_AIRN;					// Général  - Commande d'ouverture  du registre
boolean SD8_SFL_AIRN;					// Général  - Commande de fermeture du registre
// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[11] = {					// EcranLCD - Table de chaînes de caractères de présentation
  "                     ", // 00
  "Haut, Enter : Accueil",
  "% Voulu              ",
  "% Actuel             ",
  "Cde Ouverture        ",
  "Cde Fermeture        ",
  "                     ",
  "                     ",
  "                     ",
  "                     ",
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
  "   Ouvre ",
  "   Ferme ",
  "   Ferme ",
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

void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  Indio.setADCResolution(14);				// Système  - Résolution des AI. Valeurs : 12bit@240Lps, 14bit@60Lps, 16bit@15Lps and 18bit@3.75Lps.
  Indio.analogReadMode(1, V10_p);			// Système  - Affectation de la voie AI1 en tant que 0-10V   -> 0-100%
  Indio.digitalMode(7,OUTPUT);				// Système  - Affectation de la voie Dx7 en tant que sortie digitale
  Indio.digitalMode(8,OUTPUT);				// Système  - Affectation de la voie Dx8 en tant que sortie digitale
  VTC_XXXX[0] = 10;					// R_ANA3PL - Durée du cycle de rafraîchissement de l'action sur la vanne
  VTC_XXXX[1] = 180;					// R_ANA3PL - Durée de la course complète de la vanne
  VCO_XXXX = 1;						// R_ANA3PL - Commande de forçage
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
  Serial.begin(9600);					// Port COM - Paramétrage du port série
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  VPC_XXXX[0] = Indio.analogRead(1);			// Lecture 0-100% de l'entrée analogique 1
  BMS.R_ANA3PL(VPC_XXXX, VTC_XXXX, FCC_XXXX, 1);
  if ( FCC_XXXX[0] ) { SD7_SOL_AIRN = true; } else { SD7_SOL_AIRN = false; }
  if ( FCC_XXXX[1] ) { SD8_SFL_AIRN = true; } else { SD8_SFL_AIRN = false; }
  if ( SD7_SOL_AIRN == true ) { Indio.digitalWrite( 7 , HIGH ); } else { Indio.digitalWrite( 7,  LOW ); }
  if ( SD8_SFL_AIRN == true ) { Indio.digitalWrite( 8 , HIGH ); } else { Indio.digitalWrite( 8,  LOW ); }

  Serial.print(VTC_XXXX[1]);
  Serial.print("\t");
  Serial.print(VTC_XXXX[3]);
  Serial.print("\t");
  Serial.print(VTC_XXXX[5]);
  Serial.print("\t");
  Serial.print(VPC_XXXX[0]);
  Serial.print("\t");
  Serial.print(VPC_XXXX[1]);
  Serial.print("\t");
  Serial.print(FCC_XXXX[0]);
  Serial.print("\t");
  Serial.print(FCC_XXXX[1]);
  Serial.println(" ");

  DIS.G_TOUCH2(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 0); lcd.clearLine(); }
  DIS.G_MESS1L(VNC_MENU, 1, 1, 0, "    www.DG2R.com     ");
  DIS.G_MESS1L(VNC_MENU, 1, 2, 0, "Controle & Regulation");
  DIS.G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  1286  ");
  if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 4); lcd.clearLine(); }
  DIS.G_MESS1L(VNC_MENU, 1, 5, 0, "      R_ANA3PL       ");
  DIS.G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
  DIS.G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.00     30/03/2017");
  
  DIS.G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
  DIS.G_VISV1F(VNC_MENU, 2, 1, 0, VPC_XXXX[0], TCM_PRES[2], TCM_UNIT[2]);
  DIS.G_VISV1F(VNC_MENU, 2, 2, 0, VPC_XXXX[1], TCM_PRES[3], TCM_UNIT[2]);
  DIS.G_VISV1B(VNC_MENU, 2, 3, 0, FCC_XXXX[0], TCM_PRES[4], TCM_UNIT[0], TCM_UNIT[11]);
  DIS.G_VISV1B(VNC_MENU, 2, 4, 0, FCC_XXXX[1], TCM_PRES[5], TCM_UNIT[0], TCM_UNIT[12]);
  if (VNC_MENU[0] ==  2) {
    lcd.setCursor(0, 5); lcd.clearLine();
    lcd.setCursor(0, 6); lcd.clearLine();
    lcd.setCursor(0, 7); lcd.clearLine();
  }

  DIS.G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);

}
