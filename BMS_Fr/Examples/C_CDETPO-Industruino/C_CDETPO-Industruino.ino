/*-------------------------------------------------------------------------------------------------------------------------------------------*
* SE2R : C_CDETPO - Fonction de temporisation de commande à l'action et à l'arrêt                                                03/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_CDETPO(boolean CDE, boolean DFT, float CPT[], int RES[], int CFG)                                                     03/06/2015 *
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
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Plate-formes testées : Industruino 32U8 et 1286                                                                                           *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 27/05/2015 : DD, SE2R : Testée et validée sur banc                                                                                        *
 * 27/05/2015 : DD, SE2R : Cette fonction nécessite l'utilisation de S_SCANTS pour accéder à la valeur du cycle automate                     *
 * 03/06/2015 : DD, SE2R : Intégration du calcul de temps de cycle, passage de la commande en boolean et simplification du code              *
 * 03/06/2015 : DD, SE2R : Passage en mode void et ajout des commentaires d'appartenance à C_CDETPO                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Déclaration des bibliothèques nécessaires à l'exécution du programme ---------------------------------------------------------------------*
#include <DIS.h>                        // Biblio   - SE2R - Fonctions de dialogue et d'affichage sur l'écran UC1701 Industruino
static DIS DIS;                         // Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage
static UC1701 lcd;
#include <BMS.h>                        // Biblio   - SE2R - Fonctions de contrôle et régulation
static BMS BMS;                         // Biblio   - Activation du constructeur de la bibliothèque de fonctions d'affichage

// Déclaration des variables nécessaires à l'exécution de la fonction -----------------------------------------------------------------------*
boolean FCO_XXXX[1];                    // C_CDELEM - [00] Commande de marche depuis l'extérieur
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
float VTT_XXXX[5];                      // C_CDETPO - [00] Compteur des temporisations
                                        //            [01] Tempo entre le front montant de la demande CDE avant l'action sur la commande               
                                        //            [02] Tempo entre le front descendant de la demande CDE avant la désactivation de la commande
                                        //            [03] Temps de cycle automate calculé par la fonction
                                        //            [04] Mémorisation de la variable système Millis pour le calcul du temps de cycle
int VCO_XXXX[1];                        // C_CDETPO - [00] Commande de forçage par l'extérieur

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[21] = {                 // EcranLCD - Table de chaînes de caractères de présentation
  "                     ",  // 00
  "-  Menu  Principal  -",
  "Retour Accueil      ",
  "Demande             ",
  "forcage             ",
  "Chrono              ",
  "Defaut               ",
  "Cde M/A              ",
  "- Selection + Enter -",
  "- Commande  Mar/Arr -",
  "-      Forcage      -",  // 10
  "- Tempo a la marche -",
  "-  Tempo a l'arret  -",
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
  " F.Arret ",
  " Debraye ",
  "    Auto ", // 15
  "F.Marche ",
  "V.Marche ",
  "  Defaut ",
  "  Normal ",
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
boolean FCM_XXXX[1];                    // G_MODV1B - Mémoire de la demande de marche
int VTM_XXXX[7];                        // G_MODV1M - Mémoire de calcul de la commande de la fonction pour VCO_XXXX - Forçage
float VTM_TPAM[11];                     // G_MODV1F - Mémoire de calcul de la commande de la fonction pour VTT_XXXX[1] - Tempo à la marche
float VTM_TPMA[11];                     // G_MODV1F - Mémoire de calcul de la commande de la fonction pour VTT_XXXX[2] - Tempo à l'arrêt

void setup() {                          // Initialisation -----------------------------------------------------------------------------------*
Indio.digitalMode(1,INPUT);             // Système  - Affectation de la voie Dx1 en tant qu'entrée digitale
Indio.digitalMode(5,OUTPUT);            // Système  - Affectation de la voie Dx5 en tant que sortie digitale
VCO_XXXX[0] = 1;                        // C_CDELEM - Passage du forçage en position auto

VTT_SCRN[1] = 180;                      // G_TOUCHx - Temporisation d'inactivité sur le rétroéclairage
VTT_SCRN[2] = 240;                      // G_TOUCHx - Temporisation d'inactivité de retour à l'écran principal
VTT_SCRN[3] = 20;                       // G_TOUCHx - Temporisation d'antirebond de la touche "Enter"
VTT_SCRN[4] = 15;                       // G_TOUCHx - Temporisation d'antiredond des touches "Up" et "Down"
VNT_MENU[0] = 6;                        // G_NAVIGM - Taille des tables d'index et de destinations
VNT_MENU[1] = 1;                        // G_NAVIGM - Initialisation du pointeur de destinations

//                                      // G_NAVIGM - Tables d'adressage de l'écran LCD
VNC_MENU[0] = 1;
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_MENU[3] = 3;
VNC_MENU[4] = 4;
VNC_MENU[5] = 5;
VNC_MENU[6] = 6;
VNC_PRUP[1] =   1; VNC_PREN[1] =   2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   2; VNC_PREN[2] =   2; VNC_PRDN[2] =   2;
VNC_PRUP[3] =   3; VNC_PREN[3] =   2; VNC_PRDN[3] =   3;
VNC_PRUP[4] =   4; VNC_PREN[4] =   2; VNC_PRDN[4] =   4;
VNC_PRUP[5] =   5; VNC_PREN[5] =   6; VNC_PRDN[5] =   5;
VNC_PRUP[6] =   6; VNC_PREN[6] =   2; VNC_PRDN[6] =   6;
VNC_PRUP[7] =   2;

VNX_MN02[0] = 0;						// G_CHOIX8 - Table des menus de destination et des index du menu 11 - Initialisation index
VNX_MN02[1] = 1;
VNX_MN02[2] = 3;
VNX_MN02[3] = 4;
VNX_MN02[4] = 5;
VNX_MN02[5] = 0;
VNX_MN02[6] = 0;
VNX_MN02[7] = 0;
VNX_MN02[8] = 1;
VNX_MN02[9] = 4;
VNX_MN02[10] = 1;

VTM_XXXX[1] = 0; VTM_XXXX[2] = 2; VTM_XXXX[3] = 1; VTM_XXXX[4] = VCO_XXXX;
VTM_TPAM[1] = 0; VTM_TPAM[2] = 600; VTM_TPAM[3] = 1;  VTM_TPAM[4] = VTT_XXXX[1];  VTM_TPAM[8] = 100;
VTM_TPMA[1] = 0; VTM_TPMA[2] = 600; VTM_TPMA[3] = 1;  VTM_TPMA[4] = VTT_XXXX[2];  VTM_TPMA[8] = 100;

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
BMS.C_CDETPO(FCO_XXXX[0], EDx_TEL_XXXX[1], VTT_XXXX, SDx_TSL_XXXX, VCO_XXXX[0]);
if (SDx_TSL_XXXX[0] == true ) { SDx_TSL_XXXX[5] = true; } else  { SDx_TSL_XXXX[5] = false; } 
if (SDx_TSL_XXXX[5] == true ) { Indio.digitalWrite(5,HIGH); } else {Indio.digitalWrite(5,LOW); }

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
DIS.G_MESS1L(VNC_MENU, 1, 5, 0, "Fonction BMS.C_CDETPO");
DIS.G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
DIS.G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.01     03/06/2015");

DIS.G_CHOIX8(VNC_MENU, 2, FCO_PRUP, FCO_PREN, FCO_PRDN, VNX_MN02);
DIS.G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
DIS.G_MESS1L(VNC_MENU, 2, 1, 6, TCM_PRES[2]);
DIS.G_VISV1B(VNC_MENU, 2, 2, 6, FCO_XXXX[0], TCM_PRES[3], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1M(VNC_MENU, 2, 3, 6, VCO_XXXX[0], TCM_PRES[4], TCM_UNIT[14]);
DIS.G_VISV1F(VNC_MENU, 2, 4, 6, VTT_XXXX[0], TCM_PRES[4], TCM_UNIT[3]);
DIS.G_VISV1B(VNC_MENU, 2, 5, 0, EM1_MII_MPLX[4], TCM_PRES[6], TCM_UNIT[18], TCM_UNIT[19]);
DIS.G_VISV1B(VNC_MENU, 2, 6, 0, SDx_MPL_XXXX[5], TCM_PRES[7], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_MESS1L(VNC_MENU, 2, 7, 0, TCM_PRES[8]);

DIS.G_MODV1B(VNC_MENU, 3, FCO_PRUP, FCO_PREN, FCO_PRDN, FCO_XXXX, 0, FCM_XXXX, TCM_PRES[9] , " Commande operateur ", TCM_UNIT[13], TCM_UNIT[14], true);
DIS.G_MODV1M(VNC_MENU, 4, FCO_PRUP, FCO_PREN, FCO_PRDN, VCO_XXXX, 0, VTM_XXXX, TCM_PRES[10], "Arr/Deb/Auto/Mar/VMa", TCM_UNIT, 14, true, 0);
DIS.G_MODV1F(VNC_MENU, 5, FCO_PRUP, FCO_PREN, FCO_PRDN, VTT_XXXX, 1, VTM_TPAM, TCM_PRES[11], "de Demande a  Marche", TCM_UNIT[3], true, 0);
DIS.G_MODV1F(VNC_MENU, 6, FCO_PRUP, FCO_PREN, FCO_PRDN, VTT_XXXX, 2, VTM_TPMA, TCM_PRES[12], "de Demande a   Arret", TCM_UNIT[3], true, 0);

DIS.G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
}
