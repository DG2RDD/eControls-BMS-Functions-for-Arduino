/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_CDETPO - On/Off boolean control with set/reset delay taking account of a fault                       (SOF compatible) 03/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *                CDE : Boolean : On/Off demand from the operator (can be the result of another function or a logical expression             *
 *                DFT : Boolean : Fault on the controlled device. True means a fonctionnal device. false means a faulty device               *
 *                CPT : [00] Timer counter                                                                                                   *
 *                      [01] Delay between the rising front of CDE and RES[0] being set to true                                              *
 *                      [02] Delay between the falling front of CDE and RES[0] being set to false                                            *
 *                      [03] Scan time of the Arduino in seconds                                                                             *
 *                      [04] Memory of Millis system variable for cycle time calculation                                                     *
 *                RES : [00] Boolean array, size >= 1. On/Off command to the device                                                          *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES kept to false                                                                                               *
 *                      0  : RES frozen in its current state athe time of the freeze                                                         *
 *                      1  : RES set to true after CPT[01] seconds if CDE is set and DFT is normal, false after CPT[02] seconds instead      *
 *                      2  : RES set to true if DFT is normal, false if DFT is faulty (As if CDE was permanently true)                       *
 *                      3  : RES set to true, no questions asked                                                                             *
 *                      >3 : RES set to false, no questions asked                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 03/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/

// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
boolean ED3_ZCL_XXXX;                    // C_CDETPO - Run command
boolean ED2_DGL_XXXX;                    // C_CDETPO - Fault information
boolean FCC_XXXX[1];                     // C_CDETPO - Run command
float VTT_XXXX[5];                       // C_CDETPO - Timer table
int VCO_XXXX;                            // C_CDETPO - Override SOF command
void setup() {                           // Initialization ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_CDETPO - Declaration of digital input  2 (Fault)
  pinMode(3, INPUT);                     // C_CDETPO - Declaration of digital input  3 (Run command)
  pinMode(7, OUTPUT);                    // C_CDETPO - Declaration of digital output 7 (Run output)
  VTT_XXXX[1] = 2;                       // C_CDETPO - Run delay
  VTT_XXXX[2] = 5;                       // C_CDETPO - Stop delay
  VCO_XXXX = 1;                          // C_CDETPO - Override SOF command in Auto position
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}
void loop() {                            // Execution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { ED2_DGL_XXXX = true; } else { ED2_DGL_XXXX = false; } // NC fault information (shunt = normal)
  if (digitalRead(3) == LOW) { ED3_ZCL_XXXX = true; } else { ED3_ZCL_XXXX = false; } // NO run demand (shunt = demand)
  BMS.C_CDETPO(ED3_ZCL_XXXX, ED2_DGL_XXXX, VTT_XXXX, FCC_XXXX, VCO_XXXX);
  if (FCC_XXXX[0]) { digitalWrite(7, HIGH); } else { digitalWrite(7, LOW); }
  Serial.print("Demande : ");
  Serial.print("\t");
  Serial.print(ED3_ZCL_XXXX);
  Serial.print("\t");
  Serial.print("Defaut : ");
  Serial.print("\t");
  Serial.print(ED2_DGL_XXXX);
  Serial.print("\t");
  Serial.print("Compteur T : ");
  Serial.print("\t");
  Serial.print(VTT_XXXX[0]);
  Serial.print("\t");
  Serial.print("Commande : ");
  Serial.print("\t");
  Serial.println(FCC_XXXX[0]);
}
