/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_DISCOR - Fonction de détection temporisée de discordance entre deux valeurs ToR ou analogiques                        29/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG)                                          29/06/2015 *
 *                IN1 : Première valeur à comparer                                                                                           *
 *                IN2 : Deuxième valeur à comparer                                                                                           *
 *                DIF : Différence maximum tolérée entre IN1 et IN2                                                                          *
 *                TPO : [0] Compteur de temporisation                                                                                        *
 *                      [1] Tempo de confirmation de la discordance                                                                          *
 *                      [2] Temps de cycle automate calculé par la fonction                                                                  *
 *                      [3] Mémorisation de la variable système Millis pour le calcul du temps de cycle                                      *
 *                RES : [0] Discordance de l'équipement - forcément un tableau de taille au moins 1                                          *
 *                CFG : Commande de forçage.                                                                                                 *
 *                      <0 : RES maintenu à false donc en discordance                                                                        *
 *                      0  : RES figée dans son dernier état connu                                                                           *
 *                      1  : RES est true sauf si IN1 et IN2 diffèrent de plus de DIF pendant plus de CPT[1] sans discontinuer               *
 *                      2  : RES maintenu à true en permanence                                                                               *
 *                      3  : RES maintenu à true en permanence                                                                               *
 *                      >3 : RES maintenu à false                                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U8 et 1286                                                                                           *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 29/06/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <DIS.h>                        // Biblio   - SE2R - Fonctions de dialogue et d'affichage sur l'écran UC1701 Industruino
static DIS DIS;                         // Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage
static UC1701 lcd;
#include <BMS.h>                        // Biblio   - SE2R - Fonctions de contrôle et régulation
static BMS BMS;                         // Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage

// Déclaration des variables nécessaires à l'exécution de la fonction -----------------------------------------------------------------------*
boolean FCO_REAR[1];                    // C_DISCOR - [00] Commande de réarmement depuis l'extérieur
boolean EDx_TEL_XXXX[9];                // Système  - [00] Inutilisée
                                        //            [01] Etat de la lecture de la voie digitale 1 si elle est une entrée
                                        //            [02] Etat de la lecture de la voie digitale 2 si elle est une entrée
                                        //            [03] Etat de la lecture de la voie digitale 3 si elle est une entrée
                                        //            [04] Etat de la lecture de la voie digitale 4 si elle est une entrée
                                        //            [05] Etat de la lecture de la voie digitale 5 si elle est une entrée
                                        //            [06] Etat de la lecture de la voie digitale 6 si elle est une entrée
                                        //            [07] Etat de la lecture de la voie digitale 7 si elle est une entrée
                                        //            [08] Etat de la lecture de la voie digitale 8 si elle est une entrée
boolean SDx_TSL_XXXX[9];                // Système  - [00] Mémoire temporaire utilisée pour les calculs
                                        //            [01] Etat de la commande de la voie digitale 1 si elle est une sortie
                                        //            [02] Etat de la commande de la voie digitale 1 si elle est une sortie
                                        //            [04] Etat de la commande de la voie digitale 1 si elle est une sortie
                                        //            [05] Etat de la commande de la voie digitale 1 si elle est une sortie
                                        //            [07] Etat de la commande de la voie digitale 1 si elle est une sortie
                                        //            [08] Etat de la commande de la voie digitale 1 si elle est une sortie
boolean FDM_XXXX[1];                    // C_DISCOR - [00] Etat de la commande à transmettre à la sortie
float VTT_XXXX[5];                      // C_DISCOR - [00] Compteur des temporisations
                                        //            [01] Tempo de confirmation de la discordance
                                        //            [02] Temps de cycle automate calculé par la fonction
                                        //            [03] Mémorisation de la variable système Millis pour le calcul du temps de cycle
int VCO_XXXX[1];                        // C_DISCOR - [00] Commande de forçage par l'extérieur

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[21] = {                 // EcranLCD - Table de chaînes de caractères de présentation
  "                     ",  // 00
  "-  Menu  Principal  -",
  "Retour Accueil      ",
  "Rearmer             ",
  "Chrono              ",
  "Entree 1            ",
  "Entree 2            ",
  "Discorde            ",
  "- Selection + Enter -",
  "-    Rearmement     -",
  "- Tempo discordance -",  // 10
  " ",
  " ",
  " ",
  " ",
  " ",
  " ",
  " ",
  " ",
  " ",
  " "};                      // 20


String TCM_UNIT[21] = {                 // EcranLCD - Table de chaînes de caractères d'unités standard
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
  "   Arret ",
  "  Marche ",
  "Discorde ",
  "  Normal ",
  "  Active ", // 15
  " Rearmer ",
  " ",
  " ",
  " ",
  "  " };      // 20

boolean FCO_PRUP[5];                    // EcranLCD - Information de touche "Up" appuyée
boolean FCO_PREN[5];                    // EcranLCD - Information de touche "Enter" appuyée
boolean FCO_PRDN[5];                    // EcranLCD - Information de touche "Down" appuyée
int VNT_MENU[2];                        // EcranLCD - Table d'adressage des menus
int VNC_MENU[10];                       // EcranLCD - Table des numéros de menus
int VNC_PRUP[11];                       // EcranLCD - Table des numéros de menus de destination par la touche "Up"
int VNC_PREN[10];                       // EcranLCD - Table des numéros de menus de destination par la touche "Enter"
int VNC_PRDN[10];                       // EcranLCD - Table des numéros de menus de destination par la touche "Down"
float VTT_SCRN[10];                     // EcranLCD - Table des temporisations
int VNX_MN02[12];                       // EcranLCD - Table des menus de destimation et des index du menu 02 - Principal
boolean FCM_REAR[1];                    // G_MODV1B - Mémoire de la demande de réarmement
float VTM_TPAM[11];                     // G_MODV1F - Mémoire de calcul de la commande de la fonction pour VTT_XXXX[1] - Tempo à la marche
float VTM_TPMA[11];                     // G_MODV1F - Mémoire de calcul de la commande de la fonction pour VTT_XXXX[2] - Tempo à l'arrêt

void setup() {                          // Initialisation -----------------------------------------------------------------------------------*
Indio.digitalMode(1,INPUT);             // Système  - Affectation de la voie Dx1 en tant qu'entrée digitale
Indio.digitalMode(2,INPUT);             // Système  - Affectation de la voie Dx2 en tant qu'entrée digitale
VCO_XXXX[0] = 1;                        // C_CDELEM - Passage du forçage en position auto

VTT_SCRN[1] = 180;                      // G_TOUCHx - Temporisation d'inactivité sur le rétroéclairage
VTT_SCRN[2] = 240;                      // G_TOUCHx - Temporisation d'inactivité de retour à l'écran principal
VTT_SCRN[3] = 20;                       // G_TOUCHx - Temporisation d'antirebond de la touche "Enter"
VTT_SCRN[4] = 15;                       // G_TOUCHx - Temporisation d'antiredond des touches "Up" et "Down"
VNT_MENU[0] = 4;                        // G_NAVIGM - Taille des tables d'index et de destinations
VNT_MENU[1] = 1;                        // G_NAVIGM - Initialisation du pointeur de destinations

//                                      // G_NAVIGM - Tables d'adressage de l'écran LCD
VNC_MENU[0] = 1;
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_MENU[3] = 3;
VNC_MENU[4] = 4;
VNC_PRUP[1] =   1; VNC_PREN[1] =   2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   2; VNC_PREN[2] =   2; VNC_PRDN[2] =   2;
VNC_PRUP[3] =   3; VNC_PREN[3] =   2; VNC_PRDN[3] =   3;
VNC_PRUP[4] =   4; VNC_PREN[4] =   2; VNC_PRDN[4] =   4;
VNC_PRUP[5] =   2;

VNX_MN02[0] = 0;						// G_CHOIX8 - Table des menus de destination et des index du menu 11 - Initialisation index
VNX_MN02[1] = 1;
VNX_MN02[2] = 3;
VNX_MN02[3] = 4;
VNX_MN02[4] = 0;
VNX_MN02[5] = 0;
VNX_MN02[6] = 0;
VNX_MN02[7] = 0;
VNX_MN02[8] = 1;
VNX_MN02[9] = 3;
VNX_MN02[10] = 1;

VTM_XXXX[1] = 0; VTM_XXXX[2] = 600; VTM_XXXX[3] = 1;  VTM_XXXX[4] = VTT_XXXX[1];  VTM_XXXX[8] = 100;

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
if (Indio.digitalRead(1) == HIGH) { EDx_TEL_XXXX[1] = false; } else { EDx_TEL_XXXX[1] = true; }
if (Indio.digitalRead(2) == HIGH) { EDx_TEL_XXXX[2] = false; } else { EDx_TEL_XXXX[2] = true; }
if (FCO_REAR[0]) { FDM_XXXX[0] = true; }
BMS.C_DISCOR(EDx_TEL_XXXX[1], EDx_TEL_XXXX[2], 0.5, VTT_XXXX, FDM_XXXX, 1 - (2 * (FDM_XXXX[0] == false)));

//                                      // EcranLCD - Gestion des touches spécifique Industruino 1286
DIS.G_TOUCH2(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN);
/*                                      // EcranLCD - Gestion des touches spécifique Industruino 32U8
DIS.G_TOUCH1(VNC_MENU, VTT_SCRN, FCO_PRUP, FCO_PREN, FCO_PRDN);
*/

if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 0); lcd.clearLine(); }
DIS.G_MESS1L(VNC_MENU, 1, 1, 0, "   www.eControls.eu  ");
DIS.G_MESS1L(VNC_MENU, 1, 2, 0, "    SE2R Softwares   ");
DIS.G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  1286  ");
// DIS.G_MESS1L(VNC_MENU, 1, 3, 0, "  Industruino  32U8  ");
if (VNC_MENU[0] ==  1) { lcd.setCursor(0, 4); lcd.clearLine(); }
DIS.G_MESS1L(VNC_MENU, 1, 5, 0, "Fonction BMS.C_DISCOR");
DIS.G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
DIS.G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.01     29/06/2015");

DIS.G_CHOIX8(VNC_MENU, 2, FCO_PRUP, FCO_PREN, FCO_PRDN, VNX_MN02);
DIS.G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
DIS.G_MESS1L(VNC_MENU, 2, 1, 6, TCM_PRES[2]);
DIS.G_VISV1B(VNC_MENU, 2, 2, 6, FCO_REAR[0], TCM_PRES[3], TCM_UNIT[15], TCM_UNIT[16]);
DIS.G_VISV1F(VNC_MENU, 2, 3, 6, VTT_XXXX[0], TCM_PRES[4], TCM_UNIT[3]);
DIS.G_VISV1B(VNC_MENU, 2, 4, 0, EDx_TEL_XXXX[1], TCM_PRES[6], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1B(VNC_MENU, 2, 5, 0, EDx_TEL_XXXX[2], TCM_PRES[6], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1B(VNC_MENU, 2, 6, 0, FDM_XXXX[0], TCM_PRES[7], TCM_UNIT[13], TCM_UNIT[14]);
DIS.G_MESS1L(VNC_MENU, 2, 7, 0, TCM_PRES[8]);

DIS.G_MODV1B(VNC_MENU, 3, FCO_PRUP, FCO_PREN, FCO_PRDN, FCO_REAR, 0, FCM_REAR, TCM_PRES[9] , "Rearme discorde a On", TCM_UNIT[13], TCM_UNIT[14], true);
DIS.G_MODV1F(VNC_MENU, 5, FCO_PRUP, FCO_PREN, FCO_PRDN, VTT_XXXX, 1, VTM_TPAM, TCM_PRES[10], "Avant  passage a Off", TCM_UNIT[3], true, 0);

DIS.G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
}