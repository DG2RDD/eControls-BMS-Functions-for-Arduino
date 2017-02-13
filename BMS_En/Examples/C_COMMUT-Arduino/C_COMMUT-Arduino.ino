/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : C_COMMUT - Standard override format command calculation from boolean conditions                                         12/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[])                                                            *
 *                      <0 : RES kept to false                                                                                               *
 *                      0  : RES frozen in its current state athe time of the freeze                                                         *
 *                      1  : RES set to true after CPT[01] seconds if CDE is set and DFT is normal, false after CPT[02] seconds instead      *
 *                      2  : RES set to true if DFT is normal, false if DFT is faulty (As if CDE was permanently true)                       *
 *                      3  : RES set to true, no questions asked                                                                             *
 *                      >3 : RES set to false, no questions asked                                                                            *
 *                FX1 : Auto position (RES = 1) if true, Frozen in state (RES = 0) if false                                                  *
 *                FX2 : Intermediary override position (RES = 2) if true, has priority over FX1                                              *
 *                FX3 : Full On override position (RES = 3), has priority over FX1 et FX2                                                    *
 *                FX0 : Full Off override position (RES = -1), has priority over every other override                                        *
 *                RES : [00] Calculated standard override format command - Has to be an array, size 1 at least                               *
 *       Internal NUM : [00] Multiplexed value of FXi                                                                                        *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ----------------------------------------------------------------------------------------------------------------------------------------- */

// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
boolean FCX_XXXX[4];                     // C_COMMUT - External override conditions
int VCO_XXXX[0];                         // C_COMMUT - Calculated override
void setup() {                           // Initialization ----------------------------------------------------------------------------------*
  pinMode(2, INPUT);                     // C_COMMUT - Declaration of digital input  2 (Auto position)
  pinMode(3, INPUT);                     // C_COMMUT - Declaration of digital input  3 (Override 2 position)
  pinMode(4, INPUT);                     // C_COMMUT - Declaration of digital input  4 (Override 3 position)
  pinMode(5, INPUT);                     // C_COMMUT - Declaration of digital input  5 (Override stop position)
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}
void loop() {                            // Execution ---------------------------------------------------------------------------------------*
  if (digitalRead(2) == LOW) { FCX_XXXX[0] = true; } else { FCX_XXXX[0] = false; }
  if (digitalRead(3) == LOW) { FCX_XXXX[1] = true; } else { FCX_XXXX[1] = false; }
  if (digitalRead(4) == LOW) { FCX_XXXX[2] = true; } else { FCX_XXXX[2] = false; }
  if (digitalRead(5) == LOW) { FCX_XXXX[3] = true; } else { FCX_XXXX[3] = false; }
  BMS.C_COMMUT(FCX_XXXX[0], FCX_XXXX[1], FCX_XXXX[2], FCX_XXXX[3], VCO_XXXX);
  Serial.print("Auto : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[0]);
  Serial.print("\t");
  Serial.print("Manual 1 : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[1]);
  Serial.print("\t");
  Serial.print("Manual 2 : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[2]);
  Serial.print("\t");
  Serial.print("Stop : ");
  Serial.print("\t");
  Serial.print(FCX_XXXX[3]);
  Serial.print("\t");
  Serial.print("Override command : ");
  Serial.print("\t");
  Serial.println(VCO_XXXX[0]);
}