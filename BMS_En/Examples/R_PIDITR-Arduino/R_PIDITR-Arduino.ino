/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_PIDITR - Full numeric iterative PID loop                                                             (SOF compatible) 13/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG)                                                                  *
 *                MES : Measure of controlled value (Measure units)                                                                          *
 *                CSG : Setpoint of the PID loop (Measure units)                                                                             *
 *                VPC : [00] Control signal to actuator (%)                                                                                  *
 *                      [01] Memory of last cycle's measure / setpoint offset (Measure units)                                                *
 *                      [02] Memory of last cycle's measure / setpoint offset variation (Measure units)                                      *
 *                      [03] Fallback override value of the control signal (CFG = 2)  (%)                                                    *
 *                VKR : [00] PID loop time base (s)                                                                                          *
 *                      [01] Full-scale action on measure (Measure units)                                                                    *
 *                      [02] Proportionnal band (%)                                                                                          *
 *                      [03] Integral time (s)                                                                                               *
 *                      [04] Derived time (s)                                                                                                *
 *                      [05] Minimum offset (Measure units)                                                                                  *
 *                      [06] Scan time of the Arduino in seconds                                                                             *
 *                      [07] Time counter in seconds since last execution of this function                                                   *
 *                      [08] Memory of Millis system variable for cycle time calculation                                                     *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : VPC[0] set to 0 %                                                                                               *
 *                      0  : VPC[0] frozen at its current value at the time of the freeze                                                    *
 *                      1  : VPC[0] follows PID action                                                                                       *
 *                      2  : VPC[0] set to fallback value VPC[3] %                                                                           *
 *                      3  : VPC[0]  set to 100 %                                                                                            *
 *                      >3 : VPC[0] set to 0 %                                                                                               *
 *       Internal NUM : [00] Memory last cycle's measure / setpoint offset                                                                   *
 *                      [01] Memory of last cycle's measure / setpoint offset variation                                                      *
 *                      [02] Calculation of the PID effective scan time                                                                      *
 *                      [03] Calculated change on VPC[0] for the current cycle                                                               *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 27/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

// Inclusion of libraries requested to execute the program ----------------------------------------------------------------------------------*
#include <BMS.h>                         // Biblio   - SE2R - Fonctions BMS
BMS BMS;

// Input, Output and variables declaration --------------------------------------------------------------------------------------------------*
float E00_TDR_XXXX;                      // R_PIDITR - PID loop measure
float VCR_XXXX;                          // R_PIDITR - PID loop setpoint
float VPC_XXXX[4];                       // R_PIDITR - PID loog output table
float VKR_XXXX[9];                       // R_PIDITR - PID loop parameters
int VCO_XXXX;                            // R_PIDITR - Override SOF command
void setup() {                           // Initialisation ----------------------------------------------------------------------------------*
  VPC_XXXX[3] = 50;                      // R_PIDITR - Fallback ofverride value for VCO = 2
  VKR_XXXX[0] = 1;                       // R_PIDITR - PID parameter : Time cycle of the loop (s)
  VKR_XXXX[1] = 90;                      // R_PIDITR - PID parameter : Full-scale action on measure (Measure units)
  VKR_XXXX[2] = 150;                     // R_PIDITR - PID parameter : Proportionnal band (%)
  VKR_XXXX[3] = 300;                     // R_PIDITR - PID parameter : Integral time (s)
  VKR_XXXX[4] = 0;                       // R_PIDITR - PID parameter : Derived time (s)
  VKR_XXXX[5] = 0;                       // R_PIDITR - PID parameter : Minimum offset (Measure units)
  VCO_XXXX = 1;                          // R_PIDITR - Override SOF command in Auto position
  Serial.begin(9600);                    // COM Port - Serial comm port speed set to 9600 Bd
}
void loop() {                            // Exécution ---------------------------------------------------------------------------------------*
  E00_TDR_XXXX = analogRead(0) / 10.23;                                              // O-100% read of analog input 0
  BMS.R_PIDITR(E00_TDR_XXXX, VCR_XXXX, VPC_XXXX, VKR_XXXX, VCO_XXXX);
  Serial.print("Measure : ");
  Serial.print("\t");
  Serial.print(E00_TDR_XXXX);
  Serial.print("\t");
  Serial.print("Setpoint : ");
  Serial.print("\t");
  Serial.print(VCR_XXXX);
  Serial.print("\t");
  Serial.print("PID Output : ");
  Serial.print("\t");
  Serial.println(VPC_XXXX[0]);
}
