/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PT - Translates a percentage command to tristate open/close timed action on outputs              (SOF compatible) 25/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG)                                                                      *
 *                VPC : [00] Percentage to translate to tristate action                                                                      *
 *                      [01] Calculated position of the tristate actuator at the end of the last cycle                                       *
 *                VTC : [00] Action cycle of tristate action duration (s) : The actuator is repositionned every VTC[0] seconds               *
 *                      [01] Full range action duration of the tristate actuator                                                             *
 *                      [02] Scan time of the Arduino in seconds                                                                             *
 *                      [03] Time counter in seconds since last execution of this function                                                   *
 *                      [04] Memory of Millis system variable for cycle time calculation                                                     *
 *                      [05] Action duration on outputs for the current execution cycle                                                      *
 *                VNO : Number of the output to open  the tristate actuator                                                                  *
 *                VNF : Number of the output to close the tristate actuator                                                                  *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES[0] set to false, RES[1] set to true                                                                         *
 *                      0  : RES[0] frozen at its current value at the time of the freeze                                                    *
 *                      1  : RES[0] and RES[1] produce open/close orders to match VPC[0] to the tristate actuator position                   *
 *                      2  : RES[0] set to true, RES[1] set to false                                                                         *
 *                      3  : RES[0] set to true, RES[1] set to false                                                                         *
 *                      >3 : RES[0] set to false, RES[1] set to true                                                                         *
 *      Internal  NUM : [00] Offset between ordered position and current position at the beginning of the cycle in percent                   *
 *                      [01] Same as [0] but on a 0-1 scale                                                                                  *
 *                      [02] Absolute value of [1]                                                                                           *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 25/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
float VPC_XXXX[2];                       // R_ANA3PT - Percentage input table
float VTC_XXXX[6];                       // R_ANA3PT - Timetable of delays and counters for the tristate actuator
int VNO_OUVR;                            // R_ANA3PT - Number of the output to command tristate actuator opening
int VNF_FERM;                            // R_ANA3PT - Number of the output to command tristate actuator closing
int VCO_XXXX;                            // R_ANA3PT - Override SOF command
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  VTC_XXXX[0] = 30;                      // R_ANA3PT - Refresh time cycle of the tristate actuator
  VTC_XXXX[1] = 180;                     // R_ANA3PT - Full 0-100% span duration of the tristate actuator
  VCO_XXXX = 1;                          // R_ANA3PT - Override SOF command in Auto position
  pinMode(7, OUTPUT);                    // System   - DIO 07 is declared as opening command of the tristate actuator
  pinMode(8, OUTPUT);                    // System   - DIO 08 is declared as closing command of the tristate actuator
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}
void loop() {                            // Execution ---------------------------------------------------------------------------------------*
  VPC_XXXX[0] = analogRead(0) / 10.23;                                              // O-100% read of analog input 0
  BMS.R_ANA3PT(VPC_XXXX, VTC_XXXX, 7, 8, VCO_XXXX);
  Serial.print("% Input : ");
  Serial.print("\t");
  Serial.print(VPC_XXXX[0]);
  Serial.print("\t");
  Serial.print("Time base : ");
  Serial.print("\t");
  Serial.print(VTC_XXXX[0]);
  Serial.print("\t");
  Serial.print("Actuator full range : ");
  Serial.print("\t");
  Serial.print(VTC_XXXX[1]);
  Serial.print("\t");
  Serial.print("Action time : ");
  Serial.print("\t");
  Serial.print(VTC_XXXX[5]);
  Serial.print("\t");
  Serial.print("Open Command : ");
  Serial.print("\t");
  Serial.print(digitalRead(7));
  Serial.print("\t");
  Serial.print("Close Command : ");
  Serial.print("\t");
  Serial.println(digitalRead(8));
}
