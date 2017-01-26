/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_COMMUT - Fonction de calcul de variable de forçage                                                                    12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[])                                                 12/06/2015 *
 *                FX1 : Demande de marche en auto (RES = 1) si true, débrayé (RES = 0) si false                                              *
 *                FX2 : Demande de forçage intermédiaire (RES = 2), prioritaire sur FX1                                                      *
 *                FX3 : Demande de forçage en marche renforcé (RES = 3), prioritaire sur FX1 et FX2                                          *
 *                FX0 : Demande de forçage à l'arrêt (RES = -1), prioritaire sur tout autre forçage                                          *
 *                RES : [0] Commande de l'équipement - forcément un tableau de taille au moins 1                                             *
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
int VCO_XXXX[1];                        // C_COMMUT - [00] Commande de forçage par l'extérieur

// Déclaration des variables nécessaires au fonctionnement de l'écran -----------------------------------------------------------------------*
String TCM_PRES[11] = {                 // EcranLCD - Table de chaînes de caractères de présentation
  "                     ",  // 00
  "-  Menu  Principal  -",
  "Pos.Auto            ",
  "Force Mar           ",
  "Verr. Mar           ",
  "Force Arr           ",
  "Resultat            ",
  "Forcage             ",
  "-Enter,Haut: Accueil-",
  " ",
  " "};                      // 10

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
  " Inactif ",
  "   Actif ",
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

void setup() {                          // Initialisation -----------------------------------------------------------------------------------*
Indio.digitalMode(1,INPUT);             // Système  - Affectation de la voie Dx1 en tant qu'entrée digitale
Indio.digitalMode(5,OUTPUT);            // Système  - Affectation de la voie Dx5 en tant que sortie digitale
VCO_XXXX[0] = 1;                        // C_CDELEM - Passage du forçage en position auto

VTT_SCRN[1] = 180;                      // G_TOUCHx - Temporisation d'inactivité sur le rétroéclairage
VTT_SCRN[2] = 240;                      // G_TOUCHx - Temporisation d'inactivité de retour à l'écran principal
VTT_SCRN[3] = 20;                       // G_TOUCHx - Temporisation d'antirebond de la touche "Enter"
VTT_SCRN[4] = 15;                       // G_TOUCHx - Temporisation d'antiredond des touches "Up" et "Down"
VNT_MENU[0] = 2;                        // G_NAVIGM - Taille des tables d'index et de destinations
VNT_MENU[1] = 1;                        // G_NAVIGM - Initialisation du pointeur de destinations

//                                      // G_NAVIGM - Tables d'adressage de l'écran LCD
VNC_MENU[0] = 1;
VNC_MENU[1] = 1;
VNC_MENU[2] = 2;
VNC_PRUP[1] =   1; VNC_PREN[1] =   2; VNC_PRDN[1] =   2;
VNC_PRUP[2] =   1; VNC_PREN[2] =   1; VNC_PRDN[2] =   2;
VNC_PRUP[3] =   2;

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
if (Indio.digitalRead(3) == HIGH) { EDx_TEL_XXXX[3] = false; } else { EDx_TEL_XXXX[3] = true; }
if (Indio.digitalRead(4) == HIGH) { EDx_TEL_XXXX[4] = false; } else { EDx_TEL_XXXX[4] = true; }
BMS.C_COMMUT(EDx_TEL_XXXX[1], EDx_TEL_XXXX[2], EDx_TEL_XXXX[3], EDx_TEL_XXXX[4], VCO_XXXX[0]);

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
DIS.G_MESS1L(VNC_MENU, 1, 5, 0, "Fonction BMS.C_COMMUT");
DIS.G_MESS1L(VNC_MENU, 1, 6, 0, "A# TESTS     C# 00001");
DIS.G_MESS1L(VNC_MENU, 1, 7, 0, "V.1.01     12/06/2015");

DIS.G_MESS1L(VNC_MENU, 2, 0, 0, TCM_PRES[1]);
DIS.G_VISV1B(VNC_MENU, 2, 1, 0, EDx_TEL_XXXX[1], TCM_PRES[2], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1B(VNC_MENU, 2, 2, 0, EDx_TEL_XXXX[2], TCM_PRES[3], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1B(VNC_MENU, 2, 3, 0, EDx_TEL_XXXX[3], TCM_PRES[4], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1B(VNC_MENU, 2, 4, 0, EDx_TEL_XXXX[4], TCM_PRES[5], TCM_UNIT[11], TCM_UNIT[12]);
DIS.G_VISV1F(VNC_MENU, 2, 5, 0, VCO_XXXX[0], TCM_PRES[6], TCM_UNIT[0]);
DIS.G_VISV1M(VNC_MENU, 2, 6, 0, VCO_XXXX[0], TCM_PRES[7], TCM_UNIT[14]);
DIS.G_MESS1L(VNC_MENU, 2, 7, 0, TCM_PRES[8]);

DIS.G_NAVIGM(VNC_MENU, VNT_MENU, FCO_PRUP, FCO_PREN, FCO_PRDN , VNC_PRUP, VNC_PREN, VNC_PRDN);
}