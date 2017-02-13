/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_CDELEM - On/Off boolean control taking account of a fault, and standardized override interface (SOI) (SOF compatible) 12/06/2015 *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 *                CDE : Boolean : On/Off demand from the operator (can be the result of another function or a logical expression             *
 *                DFT : Boolean : Fault on the controlled device. True means a fonctionnal device. false means a faulty device               *
 *                RES : [00] Boolean array, size >= 1. On/Off command to the device                                                          *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES kept to false                                                                                               *
 *                      0  : RES frozen in its current state athe time of the freeze                                                         *
 *                      1  : RES set to true if CDE is set and DFT is normal, false instead                                                  *
 *                      2  : RES set to true if DFT is normal, false if DFT is faulty (As if CDE was permanently true)                       *
 *                      3  : RES set to true, no questions asked                                                                             *
 *                      >3 : RES set to false, no questions asked                                                                            *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *-------------------------------------------------------------------------------------------------------------------------------------------*/
 
// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - BMS and industrial controls library
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
boolean ED2_DGL_XXXX;                    // C_CDELEM - Fault information
boolean ED3_ZCL_XXXX;                    // C_CDELEM - Run command
boolean FCC_XXXX[1];                     // C_CDELEM - Command to send to the output
int VCO_XXXX;                            // C_CDELEM - Override SOF command
void setup() {                           // Initialization ----------------------------------------------------------------------------------*
  pinMode(0, INPUT);                     // C_CDELEM - Declaration of digital input  2 (Fault)
  pinMode(1, INPUT);                     // C_CDELEM - Declaration of digital input  3 (Run command)
  pinMode(7, OUTPUT);                    // C_CDELEM - Declaration of digital output 7 (Run output)
  VCO_XXXX = 1;                          // C_CDELEM - Override SOF command in Auto position
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}
void loop() {                            // Execution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { ED2_DGL_XXXX = true; } else { ED2_DGL_XXXX = false; }  // NC fault information (shunt = normal)
  if (digitalRead(3) == LOW) { ED3_ZCL_XXXX = true; } else { ED3_ZCL_XXXX = false; }  // NO run demand (shunt = demand)
  BMS.C_CDELEM(ED3_ZCL_XXXX, ED2_DGL_XXXX, FCC_XXXX, VCO_XXXX);
  if (FCC_XXXX[0]) { digitalWrite(7, HIGH); } else { digitalWrite(7, LOW); }
  Serial.print("Demand : ");
  Serial.print("\t");
  Serial.print(ED3_ZCL_XXXX);
  Serial.print("\t");
  Serial.print("Fault : ");
  Serial.print("\t");
  Serial.print(ED2_DGL_XXXX);
  Serial.print("\t");
  Serial.print("Command : ");
  Serial.print("\t");
  Serial.println(FCC_XXXX[0]);
}
