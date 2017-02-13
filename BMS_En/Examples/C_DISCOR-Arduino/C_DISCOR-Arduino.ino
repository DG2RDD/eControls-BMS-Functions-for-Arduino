/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_DISCOR - Delayed difference detection between 2 boolean or analog values                             (SOF compatible) 03/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG)                                                     *
 *                IN1 : First value to compare                                                                                               *
 *                IN2 : Second value to compare                                                                                              *
 *                DIF : Maximum tolerated difference between IN1 and IN2                                                                     *
 *                CPT : [00] Timer counter                                                                                                   *
 *                      [01] Delay to confirm the difference                                                                                 *
 *                      [02] Scan time of the Arduino in seconds                                                                             *
 *                      [03] Memory of Millis system variable for cycle time calculation                                                     *
 *                RES : [00] Boolean array, size >= 1. Difference confirmed if false                                                         *
 *                CFG : Commande de forçage.                                                                                                 *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES kept to false                                                                                               *
 *                      0  : RES frozen in its current state athe time of the freeze                                                         *
 *                      1  : RES set to true after CPT[01] seconds if CDE is set and DFT is normal, false after CPT[02] seconds instead      *
 *                      2  : RES set to true, no questions asked                                                                             *
 *                      3  : RES set to true, no questions asked                                                                             *
 *                      >3 : RES set to false, no questions asked                                                                            *
 *       Internal NUM : [00] Current difference between IN1 and IN2                                                                          *
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
boolean ED2_MGL_XXXX;                    // C_DISCOR - Run state
boolean ED3_ZCL_XXXX;                    // C_DISCOR - Run command
boolean FCO_REAR;                        // C_DISCOR - Reset commmand
boolean FDM_XXXX[1];                     // C_DISCOR - Discordance information
float VTT_XXXX[4];                       // C_DISCOR - Time table to determine discordance

void setup() {                           // Initialization ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_DISCOR - Declaration of digital input  2 (Run state)
  pinMode(3, INPUT);                     // C_DISCOR - Declaration of digital input  3 (Run command)
  pinMode(4, INPUT);                     // C_DISCOR - Declaration of digital input  4 (Reset button)
  VTT_XXXX[1] = 10;                      // C_DISCOR - Discordance confirmation delay
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}

void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { ED2_MGL_XXXX = true; } else { ED2_MGL_XXXX = false; } // NO run information (shunt = run)
  if (digitalRead(3) == LOW) { ED3_ZCL_XXXX = true; } else { ED3_ZCL_XXXX = false; } // NO run demand (shunt = demand)
  if (digitalRead(4) == LOW) { FCO_REAR = true; } else { FCO_REAR = false; }         // NO reset demand (shunt = resets)
  if (FCO_REAR) { FDM_XXXX[0] = true; }
  BMS.C_DISCOR(ED3_ZCL_XXXX, ED2_MGL_XXXX, 0.5, VTT_XXXX, FDM_XXXX, 1 - (2 * (FDM_XXXX[0] == false)));
  Serial.print("Demand : ");
  Serial.print("\t");
  Serial.print(ED3_ZCL_XXXX);
  Serial.print("\t");
  Serial.print("State : ");
  Serial.print("\t");
  Serial.print(ED2_MGL_XXXX);
  Serial.print("\t");
  Serial.print("Reset : ");
  Serial.print("\t");
  Serial.print(FCO_REAR);
  Serial.print("\t");
  Serial.print("Time counter : ");
  Serial.print("\t");
  Serial.print(VTT_XXXX[0]);
  Serial.print("\t");
  Serial.print("Discordance : ");
  Serial.print("\t");
  Serial.println(FDM_XXXX[0]);
}