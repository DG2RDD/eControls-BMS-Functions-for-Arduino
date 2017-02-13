/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_TFONCT - Runtime calculation                                                                         (SOF compatible) 12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_TFONCT(boolean MAR, float RES[], int CFG)                                                                                        *
 *                MAR : Running state of the device (True mens run)                                                                          *
 *                RES : [00] Cumulated runtime hours                                                                                         *
 *                      [01] Memory of Millis system variable for cycle time calculation                                                     *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES[0] set to 0                                                                                                 *
 *                      0  : RES[0] frozen at its current value at the time of the freeze                                                    *
 *                      1  : RES[0] cumulates the duration MAR is on in hours                                                                *
 *                      >1 : RES[0] set to 0                                                                                                 *
 *       Internal SCN : [00] Scan time of the Arduino in seconds                                                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
boolean ED2_MGL_XXXX;                    // C_TFONCT - Run information
int VCO_XXXX;                            // C_TFONCT - Override SOF command
float VTO_XXXX[2];                       // C_TFONCT - Equipment runtime calculation
void setup() {                           // Initialization ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_TFONCT - Declaration of digital input  2 (Run state)
  VCO_XXXX = 1;                          // C_TFONCT - Override SOF command in Auto position
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}

void loop() {                            // Execution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == HIGH) { ED2_MGL_XXXX = true; } else { ED2_MGL_XXXX = false; } // NO run state (shunt = run)
  BMS.C_TFONCT(ED2_MGL_XXXX, VTO_XXXX, VCO_XXXX);
  Serial.print("Run state : ");
  Serial.print("\t");
  Serial.print(ED2_MGL_XXXX);
  Serial.print("\t");
  Serial.print("Runtime counter : ");
  Serial.print("\t");
  Serial.println(VTO_XXXX[0]);
}