/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANATOR - Translates a percentage command to PWM action on an output                                  (SOF compatible) 08/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANATOR(float VPC[], int VTC[], int VNC, int CFG)                                                                                 *
 *                VPC : [00] Percentage to translate to tristate action                                                                      *
 *                VTC : [00] Full range action duration of the tristate actuator                                                             *
 *                      [01] Scan time of the Arduino in seconds                                                                             *
 *                      [02] Time counter in seconds since last execution of this function                                                   *
 *                      [03] Memory of Millis system variable for cycle time calculation                                                     *
 *                      [04] Action duration on booleans for the current execution cycle                                                     *
 *                VNC : Number of the output to digital PWM actuator                                                                         *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES[0] set to false                                                                                             *
 *                      0  : RES[0] frozen at its current value at the time of the freeze                                                    *
 *                      1  : RES[0] follows open/close orders to match VPC[0] on the VTC[0] time basis                                       *
 *                      2  : RES[0] set to true                                                                                              *
 *                      3  : RES[0] set to true                                                                                              *
 *                      >3 : RES[0] set to false                                                                                             *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 08/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
float VPC_XXXX[1];                       // R_ANATOR - R_ANATOR percentage input
float VTC_XXXX[5];                       // R_ANATOR - Timetable of delays and counters for the PWM action
int VCO_XXXX;                            // R_ANATOR - Override SOF command
void setup() {                           // Initialization ----------------------------------------------------------------------------------*
  VTC_XXXX[0] = 60;                      // R_ANATOR - Time cycle of the PWM action
  VCO_XXXX = 1;                          // R_ANATOR - Override SOF command in Auto position
  pinMode(7, OUTPUT);                    // Système  - DIO 7 is declared as the PWM output
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}
void loop() {                            // Execution ---------------------------------------------------------------------------------------*
  VPC_XXXX[0] = analogRead(0) / 10.23; // O-100% read of analog input 0
  BMS.R_ANATOR(VPC_XXXX, VTC_XXXX, 7, VCO_XXXX);
  Serial.print("% Input : ");
  Serial.print("\t");
  Serial.print(VPC_XXXX[0]);
  Serial.print("\t");
  Serial.print("Time base : ");
  Serial.print("\t");
  Serial.print(VTC_XXXX[0]);
  Serial.print("\t");
  Serial.print("Action time : ");
  Serial.print("\t");
  Serial.print(VTC_XXXX[4]);
  Serial.print("\t");
  Serial.print("PWM Command : ");
  Serial.print("\t");
  Serial.println(digitalRead(7));
}
