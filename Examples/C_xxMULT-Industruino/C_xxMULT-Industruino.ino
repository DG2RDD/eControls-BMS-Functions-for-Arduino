/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_COMULT - Fonction de multiplexage d'un tableau de 16 booléens dans un entier non signé               (Compatible IFS) 19/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_COMULT(int MPX[], int RNG, Boolean FLG[], int CFG)                                                                    19/10/2016 *
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
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 19/10/2016 : DD, SE2R : Testée et validée sur banc 32U8, 1286, Arduino Leonardo                                                           *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 
 /* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DEMULT - Fonction de démultiplexage d'un entier non signé dans un tableau de booléens                (Compatible IFS) 19/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DEMULT(int MPX[], int RNG, Boolean FLG[], int CFG)                                                                    19/10/2016 *
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
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 19/10/2016 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <Wire.h>			// Biblio   - Gestion des ports de communication
#include <Indio.h>			// Biblio   - Industruino
#include <DIS.h>                        // Biblio   - SE2R - Fonctions de dialogue et d'affichage sur l'écran UC1701 Industruino
static DIS DIS;                         // Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage
static UC1701 lcd;
#include <BMS.h>                        // Biblio   - SE2R - Fonctions de contrôle et régulation
static BMS BMS;                         // Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage

// Déclaration des variables nécessaires à l'exécution de la fonction -----------------------------------------------------------------------*
boolean FTM_BOOL[16];			// EEPROM  -  Table commune de codage / décodage des booléens savegardés en EEPROM
boolean FCX_INPT[16];			// Général  - [00] Valeur digitale d'entrée  0
					//	      [01] Valeur digitale d'entrée  1
					//	      [02] Valeur digitale d'entrée  2
					//	      [03] Valeur digitale d'entrée  3
					//	      [04] Valeur digitale d'entrée  4
					//	      [05] Valeur digitale d'entrée  5
					//	      [06] Valeur digitale d'entrée  6
					//	      [07] Valeur digitale d'entrée  7
					//	      [08] Valeur digitale d'entrée  8
					//	      [09] Valeur digitale d'entrée  9
					//	      [10] Valeur digitale d'entrée 10
					//	      [11] Valeur digitale d'entrée 11
					//	      [12] Valeur digitale d'entrée 12
					//	      [13] Valeur digitale d'entrée 13
					//	      [14] Valeur digitale d'entrée 14
					//	      [15] Valeur digitale d'entrée 15
boolean FCX_OUTP[16];			// Général  - [00] Valeur digitale de sortie  0
					//	      [01] Valeur digitale de sortie  1
					//	      [02] Valeur digitale de sortie  2
					//	      [03] Valeur digitale de sortie  3
					//	      [04] Valeur digitale de sortie  4
					//	      [05] Valeur digitale de sortie  5
					//	      [06] Valeur digitale de sortie  6
					//	      [07] Valeur digitale de sortie  7
					//	      [08] Valeur digitale de sortie  8
					//	      [09] Valeur digitale de sortie  9
					//	      [10] Valeur digitale de sortie 10
					//	      [11] Valeur digitale de sortie 11
					//	      [12] Valeur digitale de sortie 12
					//	      [13] Valeur digitale de sortie 13
					//	      [14] Valeur digitale de sortie 14
					//	      [15] Valeur digitale de sortie 15
unsigned int VTM_BOOL[4];		// EEPROM   - Table de sauvegarde des booléens (par série de 16)
unsigned int VCM_MUIN[1];		// EEPROM   - [00] Valeur de lecture de variables de type insigned int en EEPROM
int VCO_XXXX[1];                        // C_DISCOR - [00] Commande de forçage par l'extérieur

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
boolean FCO_PRUP[5];                    // EcranLCD - Information de touche "Up" appuyée
boolean FCO_PREN[5];                    // EcranLCD - Information de touche "Enter" appuyée
boolean FCO_PRDN[5];                    // EcranLCD - Information de touche "Down" appuyée
int VNT_MENU[3];                        // EcranLCD - Table d'adressage des menus
int VNC_MENU[25];                       // EcranLCD - Table des numéros de menus
int VNC_PRUP[25];                       // EcranLCD - Table des numéros de menus de destination par la touche "Up"
int VNC_PREN[25];                       // EcranLCD - Table des numéros de menus de destination par la touche "Enter"
int VNC_PRDN[25];                       // EcranLCD - Table des numéros de menus de destination par la touche "Down"
float VTT_SCRN[10];                     // EcranLCD - Table des temporisations
int VNX_MN02[12];                       // EcranLCD - Table des menus de destimation et des index du menu 02 - Principal
boolean FCM_IN00[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  0
boolean FCM_IN01[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  1
boolean FCM_IN02[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  2
boolean FCM_IN03[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  3
boolean FCM_IN04[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  4
boolean FCM_IN05[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  5
boolean FCM_IN06[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  6
boolean FCM_IN07[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  7
boolean FCM_IN08[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  8
boolean FCM_IN09[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée  9
boolean FCM_IN10[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée 10
boolean FCM_IN11[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée 11
boolean FCM_IN12[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée 12
boolean FCM_IN13[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée 13
boolean FCM_IN14[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée 14
boolean FCM_IN15[1];                    // G_MODV1B - Mémoire de la commande du bit d'entrée 15

void setup() {                          // Initialisation -----------------------------------------------------------------------------------*

EEPROM.get(18, VTM_BOOL[0]);
BMS.T_DEMULT(VTM_BOOL, 0, FTM_BOOL, 1);
FCX_INPT[0] = FTM_BOOL[0];
FCX_INPT[1] = FTM_BOOL[1];
FCX_INPT[2] = FTM_BOOL[2];
FCX_INPT[3] = FTM_BOOL[3];
FCX_INPT[4] = FTM_BOOL[4];
FCX_INPT[5] = FTM_BOOL[5];
FCX_INPT[6] = FTM_BOOL[6];
FCX_INPT[7] = FTM_BOOL[7];
FCX_INPT[8] = FTM_BOOL[8];
FCX_INPT[9] = FTM_BOOL[9];
FCX_INPT[10] = FTM_BOOL[10];
FCX_INPT[11] = FTM_BOOL[11];
FCX_INPT[12] = FTM_BOOL[12];
FCX_INPT[13] = FTM_BOOL[13];
FCX_INPT[14] = FTM_BOOL[14];
FCX_INPT[15] = FTM_BOOL[15];

VTT_SCRN[1] = 180;                      // G_TOUCHx - Temporisation d'inactivité sur le rétroéclairage
VTT_SCRN[2] = 240;                      // G_TOUCHx - Temporisation d'inactivité de retour à l'écran principal
VTT_SCRN[3] = 20;                       // G_TOUCHx - Temporisation d'antirebond de la touche "Enter"
VTT_SCRN[4] = 15;                       // G_TOUCHx - Temporisation d'antiredond des touches "Up" et "Down"
VNT_MENU[0] = 22;                       // G_NAVIGM - Taille des tables d'index et de destinations
VNT_MENU[1] = 1;                        // G_NAVIGM - Initialisation du pointeur de destinations

//                                      // G_NAVIGM - Tables d'adressage de l'écran LCD
VNC_MENU[0] = 1;
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_MENU[3] = 11;
VNC_MENU[4] = 12;
VNC_MENU[5] = 13;
VNC_MENU[6] = 14;
VNC_MENU[7] = 15;
VNC_MENU[8] = 16;
VNC_MENU[9] = 17;
VNC_MENU[10] = 18;
VNC_MENU[11] = 19;
VNC_MENU[12] = 20;
VNC_MENU[13] = 21;
VNC_MENU[14] = 22;
VNC_MENU[15] = 23;
VNC_MENU[16] = 24;
VNC_MENU[17] = 25;
VNC_MENU[18] = 26;
VNC_MENU[19] = 27;
VNC_MENU[20] = 28;
VNC_MENU[21] = 29;
VNC_MENU[22] = 30;
VNC_PRUP[1] =   1; VNC_PREN[1] =    2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   2; VNC_PREN[2] =    2; VNC_PRDN[2] =   2;
VNC_PRUP[3] =   11; VNC_PREN[3] =  12; VNC_PRDN[3] =  11;
VNC_PRUP[4] =   12; VNC_PREN[4] =  13; VNC_PRDN[4] =  12;
VNC_PRUP[5] =   13; VNC_PREN[5] =  14; VNC_PRDN[5] =  13;
VNC_PRUP[6] =   14; VNC_PREN[6] =  15; VNC_PRDN[6] =  14;
VNC_PRUP[7] =   15; VNC_PREN[7] =  16; VNC_PRDN[7] =  15;
VNC_PRUP[8] =   16; VNC_PREN[8] =  17; VNC_PRDN[8] =  16;
VNC_PRUP[9] =   17; VNC_PREN[9] =  18; VNC_PRDN[9] =  17;
VNC_PRUP[10] =  18; VNC_PREN[10] =  2; VNC_PRDN[10] = 18;
VNC_PRUP[11] =   2; VNC_PREN[11] =  2; VNC_PRDN[11] =  2;
VNC_PRUP[12] =   2; VNC_PREN[12] =  2; VNC_PRDN[12] =  2;
VNC_PRUP[13] =  21; VNC_PREN[13] = 22; VNC_PRDN[13] = 21;
VNC_PRUP[14] =  22; VNC_PREN[14] = 23; VNC_PRDN[14] = 22;
VNC_PRUP[15] =  23; VNC_PREN[15] = 24; VNC_PRDN[15] = 23;
VNC_PRUP[16] =  24; VNC_PREN[16] = 25; VNC_PRDN[16] = 24;
VNC_PRUP[17] =  25; VNC_PREN[17] = 26; VNC_PRDN[17] = 25;
VNC_PRUP[18] =  26; VNC_PREN[18] = 27; VNC_PRDN[18] = 26;
VNC_PRUP[19] =  27; VNC_PREN[19] = 28; VNC_PRDN[19] = 27;
VNC_PRUP[20] =  28; VNC_PREN[20] =  2; VNC_PRDN[20] = 28;
VNC_PRUP[21] =   2; VNC_PREN[21] =  2; VNC_PRDN[21] =  2;
VNC_PRUP[22] =   2; VNC_PREN[22] =  2; VNC_PRDN[22] =  2;

VNX_MN02[10] = 1;			// G_CHOIX8 - Menu 02 - Initialisation index

//                                      // EcranLCD - Paramétrage spécifique Industruino 1286
pinMode(23, INPUT);                     // EcranLCD - Spécifique 1286 - Touche flèche basse
pinMode(24, INPUT);                     // EcranLCD - Spécifique 1286 - Touche entrée
pinMode(25, INPUT);                     // EcranLCD - Spécifique 1286 - Touche flèche haute
pinMode(26, OUTPUT);                    // EcranLCD - Spécifique 1286 - Sortie de commande du rétroéclairage

/*                                      // EcranLCD - Paramétrage spécifique Industruino 32U8
pinMode(13, OUTPUT);                    // EcranLCD - Spécifique 32U8 - Sortie de commande du rétroéclairage
*/
lcd.begin();                            // EcranLCD - Initialisation de l'écran LCD
lcd.clear();                            // EcranLCD - Effacement de l'écran LCD
}

void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
FTM_BOOL[0] = FCX_INPT[0];
FTM_BOOL[1] = FCX_INPT[1];
FTM_BOOL[2] = FCX_INPT[2];
FTM_BOOL[3] = FCX_INPT[3];
FTM_BOOL[4] = FCX_INPT[4];
FTM_BOOL[5] = FCX_INPT[5];
FTM_BOOL[6] = FCX_INPT[6];
FTM_BOOL[7] = FCX_INPT[7];
FTM_BOOL[8] = FCX_INPT[8];
FTM_BOOL[9] = FCX_INPT[9];
FTM_BOOL[10] = FCX_INPT[10];
FTM_BOOL[11] = FCX_INPT[11];
FTM_BOOL[12] = FCX_INPT[12];
FTM_BOOL[13] = FCX_INPT[13];
FTM_BOOL[14] = FCX_INPT[14];
FTM_BOOL[15] = FCX_INPT[15];
BMS.T_COMULT(VTM_BOOL, 0, FTM_BOOL, 1);

EEPROM.get(18, VTM_BOOL[1]);
if (VTM_BOOL[1] != VTM_BOOL[0]) { EEPROM.put(18, VTM_BOOL[0]); }
EEPROM.get(18, VTM_BOOL[1]);

BMS.T_DEMULT(VTM_BOOL, 1, FTM_BOOL, 1);
FCX_OUTP[0] = FTM_BOOL[0];
FCX_OUTP[1] = FTM_BOOL[1];
FCX_OUTP[2] = FTM_BOOL[2];
FCX_OUTP[3] = FTM_BOOL[3];
FCX_OUTP[4] = FTM_BOOL[4];
FCX_OUTP[5] = FTM_BOOL[5];
FCX_OUTP[6] = FTM_BOOL[6];
FCX_OUTP[7] = FTM_BOOL[7];
FCX_OUTP[8] = FTM_BOOL[8];
FCX_OUTP[9] = FTM_BOOL[9];
FCX_OUTP[10] = FTM_BOOL[10];
FCX_OUTP[11] = FTM_BOOL[11];
FCX_OUTP[12] = FTM_BOOL[12];
FCX_OUTP[13] = FTM_BOOL[13];
FCX_OUTP[14] = FTM_BOOL[14];
FCX_OUTP[15] = FTM_BOOL[15];

//                                      // EcranLCD - Gestion des touches spécifique Industruino 1286
DIS.G_TOUCH2(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);
/*                                      // EcranLCD - Gestion des touches spécifique Industruino 32U8
DIS.G_TOUCH1(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN, VNT_MENU);
*/

if (VNT_MENU[2] != VNC_MENU[0]) { lcd.clear(); }
VNT_MENU[2] = VNC_MENU[0];

if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 0); lcd.clearLine(); }
DIS.G_MESS1L(VNC_MENU, 1, 1, 0, "   www.eControls.fr  ");
DIS.G_MESS1L(VNC_MENU, 1, 2, 0, "    SE2R Softwares   ");
DIS.G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  1286  ");
// DIS.G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  32U8  ");
DIS.G_MESS1L(VNC_MENU, 1, 4, 0, "Fonction BMS.T_COMULT");
DIS.G_MESS1L(VNC_MENU, 1, 5, 0, "Fonction BMS.T_DEMULT");
DIS.G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
DIS.G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.02     19/10/2016");

VNX_MN02[0] = 0;
VNX_MN02[1] = 1;
VNX_MN02[2] = 11;
VNX_MN02[3] = 21;
VNX_MN02[4] = 0;
VNX_MN02[5] = 0;
VNX_MN02[6] = 0;
VNX_MN02[7] = 0;
VNX_MN02[8] = 1;
VNX_MN02[9] = 3;
DIS.G_CHOIX8(VNC_MENU, 2, FCO_PRUP, FCO_PREN, FCO_PRDN, VNX_MN02, VNT_MENU);
DIS.G_MESS1L(VNC_MENU, 2, 0, 0, "- Etats & commandes -");
DIS.G_MESS1L(VNC_MENU, 2, 1, 6,  "Retour a l'accueil  ");
DIS.G_MESS1L(VNC_MENU, 2, 2, 6,  "Bits d'entree 00-07 ");
DIS.G_MESS1L(VNC_MENU, 2, 3, 6,  "Bits d'entree 08-15 ");
DIS.G_MESS1L(VNC_MENU, 2, 4, 0, "In   ");
DIS.G_MESS1L(VNC_MENU, 2, 5, 0, "Out  ");
DIS.G_MESS1L(VNC_MENU, 2, 6, 0, "Porteuse             ");
DIS.G_MESS1L(VNC_MENU, 2, 7, 0, "EEPROM               ");
if (VNC_MENU[0] == 2) {
  lcd.setCursor(122, 4); if (FCX_INPT[0] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(116, 4); if (FCX_INPT[1] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(110, 4); if (FCX_INPT[2] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(104, 4); if (FCX_INPT[3] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 98, 4); if (FCX_INPT[4] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 92, 4); if (FCX_INPT[5] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 86, 4); if (FCX_INPT[6] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 80, 4); if (FCX_INPT[7] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 74, 4); if (FCX_INPT[8] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 68, 4); if (FCX_INPT[9] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 62, 4); if (FCX_INPT[10] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 56, 4); if (FCX_INPT[11] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 50, 4); if (FCX_INPT[12] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 44, 4); if (FCX_INPT[13] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 38, 4); if (FCX_INPT[14] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 32, 4); if (FCX_INPT[15] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(122, 5); if (FCX_OUTP[0] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(116, 5); if (FCX_OUTP[1] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(110, 5); if (FCX_OUTP[2] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor(104, 5); if (FCX_OUTP[3] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 98, 5); if (FCX_OUTP[4] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 92, 5); if (FCX_OUTP[5] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 86, 5); if (FCX_OUTP[6] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 80, 5); if (FCX_OUTP[7] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 74, 5); if (FCX_OUTP[8] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 68, 5); if (FCX_OUTP[9] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 62, 5); if (FCX_OUTP[10] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 56, 5); if (FCX_OUTP[11] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 50, 5); if (FCX_OUTP[12] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 44, 5); if (FCX_OUTP[13] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 38, 5); if (FCX_OUTP[14] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 32, 5); if (FCX_OUTP[15] == true) { lcd.print("1"); } else {lcd.print("0"); }
  lcd.setCursor( 80, 6); lcd.print(VTM_BOOL[0]);
  lcd.setCursor( 80, 7); lcd.print(VTM_BOOL[1]);
  FCM_IN00[0] = FCX_INPT[0];
  FCM_IN01[0] = FCX_INPT[1];
  FCM_IN02[0] = FCX_INPT[2];
  FCM_IN03[0] = FCX_INPT[3];
  FCM_IN04[0] = FCX_INPT[4];
  FCM_IN05[0] = FCX_INPT[5];
  FCM_IN06[0] = FCX_INPT[6];
  FCM_IN07[0] = FCX_INPT[7];
  FCM_IN08[0] = FCX_INPT[8];
  FCM_IN09[0] = FCX_INPT[9];
  FCM_IN10[0] = FCX_INPT[10];
  FCM_IN11[0] = FCX_INPT[11];
  FCM_IN12[0] = FCX_INPT[12];
  FCM_IN13[0] = FCX_INPT[13];
  FCM_IN14[0] = FCX_INPT[14];
  FCM_IN15[0] = FCX_INPT[15];
  }

DIS.G_MODV1B(VNC_MENU, 11, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  0, FCM_IN00, "-  Bit d'entree 00  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 12, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  1, FCM_IN01, "-  Bit d'entree 01  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 13, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  2, FCM_IN02, "-  Bit d'entree 02  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 14, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  3, FCM_IN03, "-  Bit d'entree 03  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 15, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  4, FCM_IN04, "-  Bit d'entree 04  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 16, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  5, FCM_IN05, "-  Bit d'entree 05  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 17, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  6, FCM_IN06, "-  Bit d'entree 06  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 18, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  7, FCM_IN07, "-  Bit d'entree 07  -" , "- Changer de valeur -", " Off ", "  On ", true);

DIS.G_MODV1B(VNC_MENU, 21, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  8, FCM_IN08, "-  Bit d'entree 08  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 22, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT,  9, FCM_IN09, "-  Bit d'entree 09  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 23, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT, 10, FCM_IN10, "-  Bit d'entree 10  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 24, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT, 11, FCM_IN11, "-  Bit d'entree 11  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 25, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT, 12, FCM_IN12, "-  Bit d'entree 12  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 26, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT, 13, FCM_IN13, "-  Bit d'entree 13  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 27, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT, 14, FCM_IN14, "-  Bit d'entree 14  -" , "- Changer de valeur -", " Off ", "  On ", true);
DIS.G_MODV1B(VNC_MENU, 28, FCO_PRUP, FCO_PREN, FCO_PRDN, FCX_INPT, 15, FCM_IN15, "-  Bit d'entree 15  -" , "- Changer de valeur -", " Off ", "  On ", true);

DIS.G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
}
