/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : 24/04/2017 - BMS Functions for Arduino - Source code file                                                           Version 1.03   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Functions in this library                                                                                                                 *
 *        C_CDELEM - On/Off boolean control taking account of a fault, and standardized override interface (SOI) (SOF compatible) 12/06/2015 *
 *        C_CDETPO - On/Off boolean control with set/reset delay taking account of a fault                       (SOF compatible) 03/06/2015 *
 *        C_COMMUT - Produces a standardized override command from boolean sets of conditions                                     12/06/2015 *
 *        C_DISCOR - Delayed differential detection between two values,                                          (SOF compatible) 29/06/2015 *
 *        C_TFONCT - On-time hours counter                                                                       (SOF compatible) 12/06/2015 *
 *        R_ANA3PL - Analog-to-tristate converter, output in a boolean array size 2 [0] open [1] close           (SOF compatible) 24/04/2017 *
 *        R_ANA3PT - Analog-to-tristate converter, output on 2 digital output channels to open or close          (SOF compatible) 24/04/2017 *
 *        R_ANATOL - Analog-to-Pulse Width Modulation (PWM), output in a boolean                                 (SOF compatible) 08/06/2016 *
 *        R_ANATOR - Analog-to-Pulse Width Modulation (PWM)                                                      (SOF compatible) 25/06/2015 *
 *        R_PIDITR - Full-numeric Proportionnal-Integral-Derivative (PID) loop                                   (SOF compatible) 24/06/2015 *
 *        S_SCANTS - Scan-time calculation in seconds                                                                             07/08/2015 *
 *        S_TIMERS - Generates pulses on booleans at regular intervals (clock time base)                                          31/05/2016 *
 *        T_APPRLN - Multi-segment linear conversion of a float value to another                                 (SOF compatible) 31/03/2017 *
 *        T_CLINEB - Two-points limited linear conversion of a float value to another                            (SOF compatible) 18/05/2015 *
 *        T_COMULT - Multiplexes (powers of 2 encoding) a 16-bit boolean array to an unsigned integer            (SOF compatible) 06/04/2016 *
 *        T_DEMULT - Demultiplexes (powers of 2 decoding) an unsigned integer to a 16-bit boolean array          (SOF compatible) 06/04/2016 *
 *        T_DMULTV - 5DI / 1 AI Demultiplexer on a tension analog input - Stand-alone version                    (SOF Compatible) 07/09/2016 *
 *        T_HYSTPO - Hysteresis (differential) function with set and reset delays                      (Partially SOF compatible) 19/05/2015 *
 *        T_LIMVAR - Variation speed limiter on a float value (independant increase an decrease speed limits     (SOF compatible) 26/05/2015 *
 *        T_MOYDYN - Dynamic mean value on last X scans (first order filter) on a float value                    (SOF compatible) 26/05/2015 *
 *        T_MULSEC - On/Off boolean control of a set of redundant equipments with mutual replacement if faulty   (SOF compatible) 12/07/2015 *
 *        T_SEQCSR - Multiple loads on/off boolean control from a percentage input                               (SOF compatible) 29/06/2015 *
 *        T_SEQDEM - Sequential inc/dec of a number of loads from a percentage input between two limits          (SOF compatible) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 * You may not use this file except in compliance with the License. You may obtain a copy of the License at :                                *
 *                                        http://www.apache.org/licenses/LICENSE-2.0                                                         *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,     *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions  *
 * and limitations under the License.                                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

#include "Arduino.h"
#include "BMS.h"

BMS::BMS() {}

void BMS::C_CDELEM(boolean CDE, boolean DFT, boolean RES[], int CFG) {
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

switch (CFG) {
    case 0:
		break;
    case 1:
		if (CDE && DFT) {RES[0] = false;} else {RES[0] = true;}
		break;
    case 2:
		if (DFT) {RES[0] = true;} else {RES[0] = false;}
		break;
    case 3:
		RES[0] = true;
		break;
    default:
		RES[0] = false;
		break;
	}
return;}

void BMS::C_CDETPO(boolean CDE, boolean DFT, float CPT[], boolean RES[], int CFG) {
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

CPT[3] =  max(0, (millis() - CPT[4])) / 1000;
CPT[4] =  millis();
switch (CFG) {
	case 0:
		CPT[0] = 0;
		break;
    case 1:
		if ((CDE && RES[0]) || (!CDE && !RES[0]) || !DFT) { CPT[0] = 0; } else { CPT[0] = CPT[0] + CPT[3]; }
		if ((CDE && !RES[0]) && (CPT[0] > CPT[1]) && DFT) { RES[0] = true; }
		if (((!CDE && RES[0]) && (CPT[0] > CPT[2])) || !DFT) { RES[0] = false; }
		break;
    case 2:
		if (DFT) { RES[0] = true; } else { RES[0] = false; }
		CPT[0]= 0;
		break;
    case 3:
		RES[0] = true;
		CPT[0] = 0;
		break;
    default:
		RES[0] = false;
		CPT[0] = 0;
		break;
	}
return;}

void BMS::C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[]) {
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

int NUM = FX1 + (2 * FX2) + (4 * FX3) + (8 * FX0);
if (NUM < 1) { RES[0] = 0; }
if ((NUM < 2) && (NUM >= 1)) { RES[0] = 1; }
if ((NUM < 4) && (NUM >= 2)) { RES[0] = 2; }
if ((NUM < 8) && (NUM >= 4)) { RES[0] = 3; }
if (NUM >= 8) { RES[0] = -1; }
return;}

void BMS::C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG) {
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
float NUM[0];
CPT[2] =  max(0, (millis() - CPT[3])) / 1000;
CPT[3] =  millis();
switch (CFG) {
	case 0:
		CPT[0] = 0;
		break;
	case 1:
		NUM[0] = IN1 - IN2;
		if (abs(NUM[0]) <= DIF)  { CPT[0] = 0; } else { CPT[0] = CPT[0] + CPT[2]; }
		if (CPT[0] > CPT[1]) { RES[0] = false; } else { RES[0] = true; }
		break;
	case 2:
		RES[0] = true;
		CPT[0]= 0;
		break;
	case 3:
		RES[0] = true;
		CPT[0] = 0;
		break;
	default:
		RES[0] = false;
		CPT[0] = 0;
		break;
	}
return;}

void BMS::C_TFONCT(boolean MAR, float RES[], int CFG) {
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
float SCN;
SCN =  max(0, (millis() - RES[1])) / 1000;
RES[1] =  millis();
switch (CFG) {
	case 0:
		break;
	case 1:
		if (MAR) {RES[0] = RES[0] + (SCN/3600);} else  { RES[0] = RES[0]; }
		break;
	default:
		RES[0] = 0;
		RES[1] =  millis();
		break;
	}
return;}

void BMS::R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PL - Translates a percentage command to tristate open/close timed action on booleans             (SOF compatible) 24/04/2017 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG)                                                                         *
 *                VPC : [00] Percentage to translate to tristate action                                                                      *
 *                      [01] Calculated position of the tristate actuator at the end of the last cycle                                       *
 *                VTC : [00] Action cycle of tristate action duration (s) : The actuator is repositionned every VTC[0] seconds               *
 *                      [01] Full range action duration of the tristate actuator                                                             *
 *                      [02] Scan time of the Arduino in seconds                                                                             *
 *                      [03] Time counter in seconds since last execution of this function                                                   *
 *                      [04] Memory of Millis system variable for cycle time calculation                                                     *
 *                      [05] Action duration on booleans for the current execution cycle                                                     *
 *                RES : [00] Boolean action to open  the tristate actuator                                                                   *
 *                      [01] Boolean action to close the tristate actuator                                                                   *
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
 * 30/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[3];
VTC[2] =  max(0, (millis() - VTC[4])) / 1000;
VTC[3] =  VTC[3] + VTC[2];
VTC[4] =  millis();
switch (CFG) {
	case 0:
		return;
	case 1 :
		if (VTC[3] >= VTC[0]) {
			VTC[3] = 0;
			NUM[0] = VPC[0] - VPC[1];
			NUM[1] = NUM[0] * VTC[1] / 100;
			NUM[2] = abs(NUM[1]);
			if ((NUM[2] >= VTC[0]) || (VPC[0] < 0.1) || (VPC[0] > 99.9)) {
				if ((NUM[1] < 0) || (VPC[0] < 0.1)) {
					VTC[5] = 0 - VTC[0]; 
					VPC[1] = max(VPC[1] - (VTC[0] * 100 / VTC[1]), 0);
				}
				if ((NUM[1] > 0) || (VPC[0] > 99.9)) {
					VTC[5] = VTC[0];
					VPC[1] = min(VPC[1] + (VTC[0] * 100 / VTC[1]), 100);
				}
			} else {
				VTC[5] = NUM[1];
				if (abs(VTC[5]) >= VTC[2]) { VPC[1] = VPC[0]; }
			}
		}
		if (VTC[5] == VTC[0])  { RES[0] = true; RES[1] = false; }
		if ((VTC[5] < VTC[0]) && (VTC[5] > VTC[2]))  { VTC[5] = VTC[5] - VTC[2]; RES[0] = true; RES[1] = false; }
		if ((VTC[5] <= VTC[2]) && (VTC[5] >= 0 - VTC[2])) { RES[0] = false; RES[1] = false; }
		if ((VTC[5] < 0 - VTC[2]) && (VTC[5] > 0 - VTC[0])){ VTC[5] = VTC[5] + VTC[2]; RES[0] = false; RES[1] = true; }
		if (VTC[5] == 0 - VTC[0])  { RES[0] = false; RES[1] = true; }
		return;
	case 2 :
		RES[0] = true;
		RES[1] = false;
		VPC[1] = 100;
		return;
	case 3 :
		RES[0] = true;
		RES[1] = false;
		VPC[1] = 100;
		return;
	default:
		RES[0] = false;
		RES[1] = true;
		VPC[1] = 0;
		return;
	}
return;}

void BMS::R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANA3PT - Translates a percentage command to tristate open/close timed action on outputs              (SOF compatible) 29/03/2017 *
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
float NUM[3];
VTC[2] =  max(0, (millis() - VTC[4])) / 1000;
VTC[3] =  VTC[3] + VTC[2];
VTC[4] =  millis();
switch (CFG) {
	case 0:
		return;
	case 1 :
		if (VTC[3] >= VTC[0]) {
			VTC[3] = 0;
			NUM[0] = VPC[0] - VPC[1];
			NUM[1] = NUM[0] * VTC[1] / 100;
			NUM[2] = abs(NUM[1]);
			if ((NUM[2] >= VTC[0]) || (VPC[0] < 0.1) || (VPC[0] > 99.9)) {
				if ((NUM[1] < 0) || (VPC[0] < 0.1)) {
					VTC[5] = 0 - VTC[0]; 
					VPC[1] = max(VPC[1] - (VTC[0] * 100 / VTC[1]), 0);
				}
				if ((NUM[1] > 0) || (VPC[0] > 99.9)) {
					VTC[5] = VTC[0];
					VPC[1] = min(VPC[1] + (VTC[0] * 100 / VTC[1]), 100);
				}
			} else {
				VTC[5] = NUM[1]; 
				if (abs(VTC[5]) >= VTC[2]) { VPC[1] = VPC[0]; }
			}
		}
		if (VTC[5] == VTC[0])  { digitalWrite(VNO, HIGH); digitalWrite(VNF, LOW); }
		if ((VTC[5] < VTC[0]) && (VTC[5] > VTC[2]))  { VTC[5] = VTC[5] - VTC[2]; digitalWrite(VNO, HIGH); digitalWrite(VNF, LOW); }
		if ((VTC[5] <= VTC[2]) && (VTC[5] >= 0 - VTC[2])) { digitalWrite(VNO, LOW); digitalWrite(VNF, LOW);  }
		if ((VTC[5] < 0 - VTC[2]) && (VTC[5] > 0 - VTC[0])){ VTC[5] = VTC[5] + VTC[2]; digitalWrite(VNO, LOW); digitalWrite(VNF, HIGH);  }
		if (VTC[5] == 0 - VTC[0])  { digitalWrite(VNO, LOW); digitalWrite(VNF, HIGH);  }
		return;
	case 2 :
		digitalWrite(VNO, HIGH);
		digitalWrite(VNF, LOW);
		VPC[1] = 100;
		return;
	case 3 :
		digitalWrite(VNO, HIGH);
		digitalWrite(VNF, LOW);
		VPC[1] = 100;
	default:
		digitalWrite(VNO, LOW);
		digitalWrite(VNF, HIGH);
		VPC[1] = 0;
		return;
	}
return;}

void BMS::R_ANATOL(float VPC[], float VTC[], boolean RES[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : R_ANATOL - Translates a percentage command to PWM action on a boolean                                  (SOF compatible) 08/06/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        R_ANATOL(float VPC[], int VTC[], boolean RES[], int CFG)                                                                           *
 *                VPC : [00] Percentage to translate to tristate action                                                                      *
 *                VTC : [00] Full range action duration of the tristate actuator                                                             *
 *                      [01] Scan time of the Arduino in seconds                                                                             *
 *                      [02] Time counter in seconds since last execution of this function                                                   *
 *                      [03] Memory of Millis system variable for cycle time calculation                                                     *
 *                      [04] Action duration on booleans for the current execution cycle                                                     *
 *                RES : [00] Boolean action to digital PWM actuator                                                                          *
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
 
VTC[1] =  max(0, (millis() - VTC[3])) / 1000;
VTC[2] =  VTC[2] + VTC[1];
VTC[3] =  millis();
if (VTC[2] >= VTC[4]) { RES[0] = false; }
if (VTC[2] >= VTC[0]) {
	VTC[2] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			VTC[4] = VPC[0] * VTC[0] / 100;
			if (VTC[4] > 0) { RES[0] = true; }
			return;
		case 2 :
			RES[0] = true;
			return;
		case 3 :
			RES[0] = true;
			return;
		default:
			RES[0] = false;
			return;
		}
	} else {
		return;
	}
return;}

void BMS::R_ANATOR(float VPC[], float VTC[], int VNC, int CFG) {
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
 
VTC[1] =  max(0, (millis() - VTC[3])) / 1000;
VTC[2] =  VTC[2] + VTC[1];
VTC[3] =  millis();
if (VTC[2] >= VTC[4]) { digitalWrite(VNC, LOW); }
if (VTC[2] >= VTC[0]) {
	VTC[2] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			VTC[4] = VPC[0] * VTC[0] / 100;
			if (VTC[4] > 0) { digitalWrite(VNC, HIGH); }
			return;
		case 2 :
			digitalWrite(VNC, HIGH);
			return;
		case 3 :
			digitalWrite(VNC, HIGH);
			return;
		default:
			digitalWrite(VNC, LOW);
			return;
		}
	} else {
		return;
	}
return;}

void BMS::R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG) {
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
 *                      3  : VPC[0] set to 100 %                                                                                             *
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

 float NUM[4];
VKR[6] =  max(0, (millis() - VKR[8])) / 1000;
VKR[7] =  VKR[7] + VKR[6];
VKR[8] =  millis();
if (VKR[7] >= VKR[0]) {
	VKR[7] = 0;
	switch (CFG) {
		case 0:
			return;
		case 1 :
			NUM[0] = VPC[1];
			NUM[1] = VPC[2];
			NUM[2] = max(VKR[0], VKR[6]);
			if (VKR[1] == 0) { VKR[1] = 1000; }
			if (VKR[2] == 0) { VKR[2] = 1000; }
			if (VKR[3] == 0) { VKR[3] = 10000; }
			if (VKR[5] == 0) { VPC[1] = CSG - MES; } else { VPC[1] = min(VKR[5], max((0-VKR[5]), (CSG - MES))); }
			VPC[2] = VPC[1] - NUM[0];
			NUM[3] = 100 * VPC[2] / (VKR[1] * (VKR[2] / 100) * NUM[2]);
			NUM[3] = NUM[3] + (100 * VPC[1] / (VKR[1] * VKR[3]));
			NUM[3] = NUM[3] + (100 * (VPC[2] - NUM[1]) * VKR[4] / (VKR[1] * NUM[2] * NUM[2]));
			VPC[0] = min(100, max(0, VPC[0] + (NUM[3] * NUM[2])));
			return;
		case 2 :
			VPC[0] = VPC[3];
			return;
		case 3 :
			VPC[0] = 100;
			return;
		default:
			VPC[0] = 0;
			return;
		}
	}
return; }

void BMS::S_SCANTS(float CPT[2]) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : S_SCANTS - Scan time calculation in seconds                                                                             07/08/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        S_SCANTS(float CPT[])                                                                                                              *
 *                CPT : [00] Scan time of the Arduino in seconds                                                                             *
 *                      [01] Memory of Millis system variable for cycle time calculation                                                     *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/08/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[0] =  max(0, (millis() - CPT[1])) / 1000;
CPT[1] =  millis();
return;}

void BMS::S_TIMERS(float CPT[9], boolean FCO[7]) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : S_TIMERS - Timed actions on different cycle times                                                                       31/05/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        S_TIMERS(float CPT[6], boolean FCO[4])                                                                                             *
 *                CPT : [00] Seconds counter, resets every    second                                                                         *
 *                      [01] Seconds counter, resets every 10 seconds                                                                        *
 *                      [02] Seconds counter, resets every    minute                                                                         *
 *                      [03] Seconds counter, resets every  5 minute                                                                         *
 *                      [04] Seconds counter, resets every 10 minutes                                                                        *
 *                      [05] Seconds counter, resets every 30 minutes                                                                        *
 *                      [06] Seconds counter, resets every    hour                                                                           *
 *                      [07] Scan time of the Arduino in seconds                                                                             *
 *                      [08] Memory of Millis system variable for cycle time calculation                                                     *
 *                FCO : [00] Cyclic action : Activates during 1 CPU scan every    second                                                     *
 *                      [01] Cyclic action : Activates during 1 CPU scan every 10 seconds                                                    *
 *                      [02] Cyclic action : Activates during 1 CPU scan every    minute                                                     *
 *                      [03] Cyclic action : Activates during 1 CPU scan every  5 minute                                                     *
 *                      [04] Cyclic action : Activates during 1 CPU scan every 10 minutes                                                    *
 *                      [05] Cyclic action : Activates during 1 CPU scan every    hour                                                       *
 *                      [06] Indicateur de cycle : s'active 1 cycle automate toutes les heures                                               *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 31/05/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[7] =  max(0, (millis() - CPT[8])) / 1000;
CPT[8] = millis();
if (CPT[0] < 1) { FCO[0] = false; CPT[0] = CPT[0] + CPT[7];} else { FCO[0] = true; CPT[0] =  0;} 
if (CPT[1] < 10) { FCO[1] = false; CPT[1] = CPT[1] + CPT[7];} else { FCO[1] = true; CPT[1] =  0;} 
if (CPT[2] < 60) { FCO[2] = false; CPT[2] = CPT[2] + CPT[7];} else { FCO[2] = true; CPT[2] =  0;} 
if (CPT[3] < 300) { FCO[3] = false; CPT[3] = CPT[3] + CPT[7];} else { FCO[3] = true; CPT[3] =  0;} 
if (CPT[4] < 600) { FCO[4] = false; CPT[4] = CPT[4] + CPT[7];} else { FCO[4] = true; CPT[4] =  0;} 
if (CPT[5] < 1800) { FCO[5] = false; CPT[5] = CPT[5] + CPT[7];} else { FCO[5] = true; CPT[5] =  0;} 
if (CPT[6] < 3600) { FCO[6] = false; CPT[6] = CPT[6] + CPT[7];} else { FCO[6] = true; CPT[6] =  0;} 
return;}

void BMS::T_APPRLN(float MES, int VTE[], float VTS[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_APPRLN - Linear multiple-segment conversion                                                          (SOF Compatible) 23/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_APPRLN(float MES, int VTE[], float VTS[], int CFG)                                                                               *
 *                MES : Value to convert                                                                                                     *
 *                VTE : [00] Number of input segment points (VTE[0] = NN)                                                                    *
 *                      [01] Low end of first input segment                                                                                  *
 *                      [02] High end of the first input segment and low end of the second input segment                                     *
 *                      ...    This array consists of integers that have to be of rising values                                              *
 *                      [NN] High end of the last input segment                                                                              *
 *                VTS : [00] Conversion result                                                                                               *
 *                      [01] Low end of first output segment                                                                                 *
 *                      [02] High end of the first output segment and low end of the second output segment                                   *
 *                      ...  This array consists of floats that do not have to be of rising values, but avoid two contiguous equal values    *
 *                      [NN] High end of the last output segment NN = VTE[0]                                                                 *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : VTS[0] set to 0                                                                                                 *
 *                      0  : VTS[0] frozen at its current value at the time of the freeze                                                    *
 *                      1  : VTS[0] follows the curve defined by VTE[] as X and VTS[] as Y                                                   *
 *                      2  : VTS[0] set to fallback value VPC[3] %                                                                           *
 *                      3  : VTS[0]  set to VTS[1]                                                                                           *
 *                      >3 : VTS[0] set to VTS[VTE[0]]                                                                                       *
 *       Internal NUM : [00] Index for diverse interations                                                                                   *
 *                      [01] Index in the input table : high end fo the segment in which MES is situated                                     *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 23/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[2];
if (VTE[0] < 2) { return; }
switch (CFG) {
	case 0:
		break;
    case 1 :
		NUM[0] = 2;
		NUM[1] = 0;
		if (MES <= VTE[1]) { VTS[0] = VTS[1]; break; }
		if ((MES > VTE[1]) && (MES < VTE[VTE[0]])) {
			for(NUM[0] = 2;  NUM[0] <= VTE[0] ; NUM[0]++) { if (MES > VTE[NUM[0] - 1]) { NUM[1] = NUM[0]; } }
			if (VTE[NUM[1]] == VTE[NUM[1] - 1]) { 
				VTS[0] =  VTS[NUM[1] - 1];
			} else {
				VTS[0] = VTS[NUM[1] - 1] + (((MES - VTE[NUM[1] - 1]) * (VTS[NUM[1]] - VTS[NUM[1] - 1])) / (VTE[NUM[1]] - VTE[NUM[1] - 1])); 
			}
			break;
		}
		if (MES >= VTE[VTE[0]])	{ VTS[0] = VTS[VTE[0]]; break; }
	case 2 :
		VTS[0] = VTS[1];
		break;
	case 3 :
		VTS[0] = VTS[VTE[0]];
	default:
		VTS[0] = 0;
		break;
	}
return;}

void BMS::T_CLINEB(float CDE[], float RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_CLINEB - Float to float linear limited conversion                                                    (SOF Compatible) 18/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_CLINEB(float CDE[], float RES[], int CFG)                                                                                        *
 *                CDE : [00] Analog input to convert                                                                                         *
 *                      [01] Input value 1, corresponds to converted value RES[1]. May be > or < CDE[2]                                      *
 *                      [02] Input value 2, corresponds to converted value RES[2]. May be > or < CDE[1]                                      *
 *                RES : [00] Conversion result : If CDE[0] outside [CDE[1]-CDE[2]], RES[1] or RES[2]. Else linear conv. CDE[1]-[2]/RES[1]-[2]*
 *                      [01] Output value 1, corresponds to input value CDE[1]. May be > or < RES[2]                                         *
 *                      [02] Output value 2, corresponds to input value CDE[2]. May be > or < RES[1]                                         *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : RES[0] set to 0                                                                                                 *
 *                      0  : RES[0] frozen at its current value at the time of the freeze                                                    *
 *                      1  : RES[0] follows the curve defined by VTE[] as X and VTS[] as Y                                                   *
 *                      2  : RES[0] set to VTS[1]                                                                                            *
 *                      3  : RES[0] set to VTS[2]                                                                                            *
 *                      >3 : RES[0] set to 0                                                                                                 *
 *       Internal NUM : Intermediary calculation of the converted value                                                                      *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 18/05/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

float NUM;
switch (CFG) {
	case 0:
		break;
	case 1:
		if (CDE[1] == CDE[2]) { RES[0] = RES[1]; }
		NUM = ((RES[2] - RES[1]) / (CDE[2] - CDE[1]));
		if (CDE[2] > CDE[1]) {
			if (CDE[0] < CDE[1]) { NUM = RES[1]; }
			if (CDE[0] > CDE[2]) { RES[0] = RES[2]; }
			if ((CDE[0] >= CDE[1]) && (CDE[0] <= CDE[2])) { RES[0] = ((CDE[0] - CDE[1]) * NUM) + RES[1]; }
		}
		if (CDE[2] < CDE[1]) {
			if (CDE[0] < CDE[2]) { RES[0] = RES[2]; }
			if (CDE[0] > CDE[1]) { RES[0] = RES[1]; }
			if ((CDE[0] <= CDE[1]) && (CDE[0] >= CDE[2])) { RES[0] = ((CDE[0] - CDE[1]) * NUM) + RES[1]; }
		}
		break;
    case 2:
		RES[0] = RES[1];
		break;
    case 3:
		RES[0] = RES[2];
		break;
    default:
		RES[0] = 0;
		break;
	}
return;}

void BMS::T_COMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_COMULT - Multiplexes a 16-boolean array to an unsigned integer                                       (SOF Compatible) 22/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_COMULT(unsigned int MPX[], int RNG, Boolean FLG[], int CFG)                                                                      *
 *                MPX : [00] Unsigned integer in which the booleans will be encoded, Has to be an array, size 1 at least                     *
 *                RNG : Integer, place in the array wher the booleans will be encoded                                                        *
 *                FLG : [00] Rank 0 boolean in the 16-boolean array                                                                          *
 *                      [..] Other booleans to multiplex                                                                                     *
 *                      [15] Rank 15 and last boolean in the 16-boolean array                                                                *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : MPX[RNG] set to 0                                                                                               *
 *                      0  : MPX[RNG] frozen at its current value at the time of the freeze                                                  *
 *                      1  : MPX[RNG] contains the multiplexed value of the 16-bit array                                                     *
 *                      2  : MPX[RNG] set to 65535                                                                                           *
 *                      3  : MPX[RNG] set to 65535                                                                                           *
 *                      >3 : MPX[RNG] set to 0                                                                                               *
 *       Internal NUM : Intermediary calculation of the multiplexed value                                                                    *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 06/04/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 22/10/2016 : SE2R, DD : Si EEPROM memo of MPX[RNG], always substract 1 to MPX[RNG] just after the T_COMULT (pb of bit 0 in EEPROM)        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

 float NUM[1];
switch (CFG) {
	case 0:
		break;
	case 1:
  		NUM[0] = float(FLG[0]);
		for (int i = 1; i < 16; i++) { NUM[0] = NUM[0] + (FLG[i] * pow(2,i)); }
		MPX[RNG] = int(NUM[0]) + 1; 
		break;
	case 2:
		MPX[RNG] = 65535;
		break;
	case 3:
		MPX[RNG] = 65535;
		break;
	default:
		MPX[RNG] = 0;
		break;
	}
return;}

void BMS::T_DEMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DEMULT - Demultiplexes an unsigned integer to a 16-boolean array                                     (SOF Compatible) 22/10/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DEMULT(int MPX[], int RNG, Boolean FLG[], int CFG)                                                                               *
 *                MPX : [00] Unsigned integer in which the booleans are encoded, Has to be an array, size 1 at least                         *
 *                RNG : Integer, place in the array wher the booleans are encoded                                                            *
 *                FLG : [00] Rank 0 boolean in the 16-boolean array                                                                          *
 *                      [..] Other booleans to demultiplex to                                                                                *
 *                      [15] Rank 15 and last boolean in the 16-boolean array                                                                *
 *                CFG : Standardized override format                                                                                         *
 *                      <0 : All FLG[xx] set to false, whatever the value of MPX[RNG]                                                        *
 *                      0  : All FLG[xx] frozen at their current value at the time of the freeze                                             *
 *                      1  : FLG[xx] follow the values demultiplexed from MPX[RNG]                                                           *
 *                      2  : All FLG[xx] set to true, whatever the value of MPX[RNG]                                                         *
 *                      3  : All FLG[xx] set to true, whatever the value of MPX[RNG]                                                         *
 *                      >3 : All FLG[xx] set to false, whatever the value of MPX[RNG]                                                        *
 *       Internal NUM : Intermediary calculation of the multiplexed value                                                                    *
  * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 22/10/2016 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/

float NUM[1];
switch (CFG) {
	case 0:
		break;
	case 1:
		NUM[0] = float(MPX[RNG]);
		for (int i = 15; i >= 0; i--) {
			if (NUM[0] >= pow(2,i)) { FLG[i] = true; } else { FLG[i] = false;}
			NUM[0] = NUM[0] - (FLG[i] * pow(2,i));
        }
		break;
	case 2:
		for (int i = 15; i >= 0; i--) { FLG[i] = true; }
		break;
	case 3:
		for (int i = 15; i >= 0; i--) { FLG[i] = true; }
		break;
	default:
		for (int i = 15; i >= 0; i--) { FLG[i] = false; }
		break;
	}
return;}
  
void BMS::T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_DMULTV - 5DI / 1 AI Demultiplexer on a tension analog input - Stand-alone version                    (SOF Compatible) 07/09/2016 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_DMULTV(float MES, float TNS[], float VNC[], boolean RES[], int CFG)                                                   07/09/2016 *
 *                MES : float, 0-10V reading to percentage (V10_p) of the analog input                                                       *
 *                OHM : [00] Common load resistance value in Ohms                                                                            *
 *                      [01] Load resistance value of multiplexers dry contact input 1 in Ohms                                               *
 *                      [02] Load resistance value of multiplexers dry contact input 2 in Ohms                                               *
 *                      [03] Load resistance value of multiplexers dry contact input 3 in Ohms                                               *
 *                      [04] Load resistance value of multiplexers dry contact input 4 in Ohms                                               *
 *                      [05] Load resistance value of multiplexers dry contact input 5 in Ohms                                               *
 *                      [06] Instant value of unshunted load resistances [00] to [05]                                                        *
 *                TNS : [00] Calculated tension on OHM[0] depending on the shunting configuration of OHM[1] to [5]                           *
 *                      [01] Reference power supply voltage : 24000 mV                                                                       *
 *                      [02] Voltage differential to compensate resistance deviations : 150 to 200 mV                                        *
 *                      [03] Theoretical voltage on RES[0], with no OHM[x] shunted, and for a 24000 mVdc power supply                        *
 *                      [04] Measured voltage on RES[0], with no OHM[x] shunted, and for actual power supply                                 *
 *                      [05] Actual power supply voltage according to TNS[3] and TNS[2]                                                      *
 *                VNC : [00] Number of controller scan cycles to confirm RES[x] transitions (rebound and interference filtering)             *
 *                      [01] Scan cycles counter of different RES[1] and RES[6] to confirm RES[1] transition                                 *
 *                      [02] Scan cycles counter of different RES[2] and RES[7] to confirm RES[2] transition                                 *
 *                      [03] Scan cycles counter of different RES[3] and RES[8] to confirm RES[3] transition                                 *
 *                      [04] Scan cycles counter of different RES[4] and RES[9] to confirm RES[4] transition                                 *
 *                      [05] Scan cycles counter of different RES[6] and RES[10] to confirm RES[5] transition                                *
 *                RES : [00] Operator validation to set TNS[4] to current input value (Voltage initialization)                               *
 *                      [01] Filtered demultiplexed digital input 1                                                                          *
 *                      ...                                                                                                                  *
 *                      [05] Filtered demultiplexed digital input 5                                                                          *
 *                      [06] Raw demultiplexed digital input 1                                                                               *
 *                      ...                                                                                                                  *
 *                      [10] Raw demultiplexed digital input 1                                                                               *
 *                CFG : Standardized override format                                                                                         *
 *                      <0  : RES[1] to [5] stay false whatever the value of MES is                                                          *
 *                      0   : RES[1] to [5] keep their current value, that can be changed from outside the function                          *
 *                      1   : RES[1] to [5] are actuated according to the values of MES                                                      *
 *                      2   : RES[1] to [5] stay true whatever the value of MES is                                                           *
 *                      3   : RES[1] to [5] stay true whatever the value of MES is                                                           *
 *                      >3  : RES[1] to [5] stay false whatever the value of MES is                                                          *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R under the APACHE Licence                                                                                                   *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested hardware : Industruino 32U8, 1286                                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * 07/09/2016 : SE2R, DD : Bench tested and validated                                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
TNS[1] = 24000;
TNS[6] = float(OHM[5] + OHM[4] + OHM[3] + OHM[2]  + OHM[1] + OHM[0]);
TNS[3] = TNS[1] * (1 - (OHM[0] / TNS[6]));
if (RES[0] == true) {TNS[4] = MES * 100; RES[0] = false; }
TNS[5] = TNS[1] * TNS[4] / TNS[3];
switch (CFG) {
	case 0:
		break;
    case 1:
		TNS[6] = float(OHM[4] + OHM[3] + OHM[2]  + OHM[1] + OHM[0]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[6] = true; } else { RES[6] = false ; }
		TNS[6] = TNS[6] + (float(OHM[5]) * (1 - float(RES[6] == true))) - float(OHM[4]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[7] = true;  } else { RES[7] = false ; }
		TNS[6] = TNS[6] + (float(OHM[4]) * (1 - float(RES[7] == true))) - float(OHM[3]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[8] = true;  } else { RES[8] = false ; }
		TNS[6] = TNS[6] + (float(OHM[3]) * (1 - float(RES[8] == true))) - float(OHM[2]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[9] = true;  } else { RES[9] = false ; }
		TNS[6] = TNS[6] + (float(OHM[2]) * (1 - float(RES[9] == true))) - float(OHM[1]);
		TNS[0] = TNS[2] + TNS[5] - (float(OHM[0]) * TNS[5] / TNS[6]);
		if (MES * 100 < TNS[0]) {RES[10] = true;  } else { RES[10] = false ; }
		for (int I = 1; I < 6; I++) {
			if (RES[I] < RES[I + 5]) { VNC[I] = min(VNC[I] + 1, VNC[0]); }
			if (RES[I] > RES[I + 5]) { VNC[I] = max(VNC[I] - 1, 0); }
			if ((RES[I] == RES[I + 5]) && (VNC[I] != 0) && (VNC[I] != VNC[0])) {VNC[I] = VNC[0] * float(RES[I]); }
			if (VNC[I] == 0) { RES[I] = false; }
			if (VNC[I] == VNC[0]) { RES[I] = true; }
		}
		break;
    case 2:
		for (int i = 1; i < 6; i++) { RES[i] = true; }
		break;
    case 3:
		for (int i = 1; i < 6; i++) { RES[i] = true; }
		break;
    default:
		for (int i = 1; i < 6; i++) { RES[i] = false; }
		break;
	}
return;}

void BMS::T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_HYSTPO - Time-based confirmed hysteresis                                                                (partial SOF) 19/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG)                                                                         *
 *                MES : [00] Analog value to produce hysteresis                                                                              *
 *                      [01] Low  threshold. Has to be inferior to MES[2]                                                                    *
 *                      [02] High threshold. Has to be superior to MES[1]                                                                    *
 *                CPT : [00] Counter of elapsed time outside of [MES[1] - MES[2]] interval. Resets when MES[0] is between MES[1] and MES[2]  *
 *                      [01] Duration to confirm low  threshold action in seconds                                                            *
 *                      [02] Duration to confirm high threshold action in seconds                                                            *
 *                      [03] Scan time of the Arduino in seconds                                                                             *
 *                      [04] Memory of Millis system variable for cycle time calculation                                                     *
 *                RES : [00] Hysteresis command - Has to be an array of size at least 1                                                      *
 *                CFG : Standardized override format                                                                                         *
 *                      <-1 : RES[0] stays false whatever the value of MES[0] is                                                             *
 *                      -1  : RES[0] disactivates if MES[0] > MES[2] confirmed by CPT[2], activates if MES[0] < MES[1] confirmed by CPT[1]   *
 *                      0   : RES[0] keeps its current value, that can be changed from outside the function                                  *
 *                      1   : RES[0] activates if MES[0] > MES[2] confirmed by CPT[2], disactivates if MES[0] < MES[1] confirmed by CPT[1]   *
 *                      2   : RES[0] stays true whatever the value of MES[0] is                                                              *
 *                      3   : RES[0] stays true whatever the value of MES[0] is                                                              *
 *                      >3  : RES[0] stays false whatever the value of MES[0] is                                                             *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 19/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 26/05/2015 : SE2R, DD : MES[1] Has to be inferior to MES[2], or the function will commute around a threshold, not timed, no differential  *
 *                         MES[0] has to take a value between MES[1] and MES[2] for counter CPT[0] to reset                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[3] =  max(0, (millis() - CPT[4])) / 1000;
CPT[4] =  millis();
switch (CFG) {
	case 0:
		CPT[0]= 0;
		break;
	case 1:
		if ((MES[0]>= MES[1]) && (MES[0]<= MES[2])) { CPT[0]= 0; } else { CPT[0]= CPT[0] + CPT[3]; }
		if ((MES[0] > MES[2]) && (CPT[0]>= CPT[2])) { RES[0] = true; }
		if ((MES[0] < MES[1]) && (CPT[0]>= CPT[1])) { RES[0] = false; }
		break;
	case -1:
		if ((MES[0]>= MES[1]) && (MES[0]<= MES[2])) { CPT[0]= 0; } else { CPT[0]= CPT[0] + CPT[3]; }
		if ((MES[0] > MES[2]) && (CPT[0]>= CPT[2])) { RES[0] = false; }
		if ((MES[0] < MES[1]) && (CPT[0]>= CPT[1])) { RES[0] = true; }
		break;
	case 2:
		RES[0] = true;
		CPT[0]= 0;
		break;
	case 3:
		RES[0] = true;
		CPT[0]= 0;
		break;
	default:
		RES[0] = false;
		CPT[0]= 0;
		break;
	}
return;}

void BMS::T_LIMVAR(float MES[], float CPT[], int RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_LIMVAR - Variation speed limiter                                                                     (SOF Compatible) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_LIMVAR(float MES[], float CPT[], int RES[], int CFG)                                                                             *
 *                MES : [00] Analog value to be limited                                                                                      *
 *                      [01] Maximum decrease speed allowed for MES[0] (units / s)                                                           *
 *                      [02] Maximum increase speed allowed for MES[0] (units / s)                                                           *
 *                      [03] Mémoire de RES en début d'exécution de la fonction                                                              *
 *                CPT : [00] Scan time of the Arduino in seconds                                                                             *
 *                      [01] Memory of Millis system variable for cycle time calculation                                                     *
 *                RES : [00] Limited command - Has to be an array of size at least 1                                                         *
 *                CFG : Standardized override format                                                                                         *
 *                      <0  : RES[0] set to 0 whatever the value of MES[0] is                                                                *
 *                      0   : RES[0] keeps its current value, that can be changed from outside the function                                  *
 *                      1   : RES[0] is the limited value of MES[0]                                                                          *
 *                      2   : RES[0] equals MES[0] with no speed variation limitation                                                        *
 *                      3   : RES[0] equals MES[0] with no speed variation limitation                                                        *
 *                      >3  : RES[0] set to 0 whatever the value of MES[0] is                                                                *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[0] =  max(0, (millis() - CPT[1])) / 1000;
CPT[1] =  millis();
switch (CFG) {
	case 0:
		break;
	case 1:
		if (RES[0] < MES[3]) {
			if (MES[1] == 0) {
				RES[0] = MES[0];
			} else {
				if ((MES[0] - RES[0]) < (MES[2] * CPT[0])) {
					RES[0] = MES[0];
				} else {
					RES[0] = RES[0] + (MES[2] * CPT[0]);
				}
			}
		} else {
			if (MES[2] == 0) { 
				RES[0] = MES[0];
			} else {
				if ((RES[0] - MES[0]) < (MES[1] * CPT[0])) {
					RES[0] = MES[0];
				} else {
					RES[0] = RES[0] - (MES[1] * CPT[0]);
				}
			}
		}
		MES[3] = MES[0];
		break;
	case 2:
		RES[0] = MES[0];
		break;
	case 3:
		RES[0] = MES[0];
		break;
	default:
		RES[0] = 0;
		break;
	}
return;}

void BMS::T_MOYDYN(float MES, float RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_MOYDYN - Dynamic average (first order filter)                                                        (SOF Compatible) 07/09/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_MOYDYN(float MES, float RES[], int CFG)                                                                                          *
 *                MES : Analog value to be averaged                                                                                          *
 *                RES : [00] Limited command - Has to be an array of size at least 2                                                         *
 *                      [01] Number of averaging cycles (number of scan times of averaging)                                                  *
 *                CFG : Standardized override format                                                                                         *
 *                      <0  : RES[0] set to 0 whatever the value of MES is                                                                   *
 *                      0   : RES[0] keeps its current value, that can be changed from outside the function                                  *
 *                      1   : RES[0] is the dynamically averaged value of MES                                                                *
 *                      2   : RES[0] equals MES with no averaging                                                                            *
 *                      3   : RES[0] equals MES with no averaging                                                                            *
 *                      >3  : RES[0] set to 0 whatever the value of MES is                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *                         If RES[1] is set to 0 or not set, it takes default value 10                                                       *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
float NUM[1];
switch (CFG) {
    case 0:
		break;
    case 1:
		if (RES[1] < 1) { NUM[0] = 10; } else { NUM[0] = RES[1]; } 
		RES[0] = (MES + (RES[0] * ( NUM[0] - 1))) / NUM[0];
		break;
    case 2:
		RES[0] = MES;
		break;
    case 3:
		RES[0] = MES;
		break;
    default:
		RES[0] = 0;
		break;
    }
return; }

void BMS::T_MULSEC(int VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_MULSEC - Multaple load command with mutual replacement and priority order                            (SOF Compatible) 12/07/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_MULSEC(float VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG)                                                *
 *                VNC : [00] Number of loads called                                                                                          *
 *                      [01] Total number of loads                                                                                           *
 *                VTP : [00] Unused, set to 0                                                                                                *
 *                      [01] Priority order of load 1 for next transition (Load default or change in VNC[0])                                 *
 *                      ...  The lower order means the load has priority to run. The highest order in running loads means priority to stop   *
 *                      [NN] Priority order of last listed load for next transition (Load default or change in VNC[0])                       *
 *                VTD : [00] Unused, set to false                                                                                            *
 *                      [01] Load 1 default. True means a normal situation. False means load is faulty.                                      *
 *                      ...  This array is composed of booleans                                                                              *
 *                      [NN] Last listed load default. True means a normal situation. False means load is faulty.                            *
 *                VMC : [00] Memory of NUM[1] : rank of last boolean on which an action was taken                                            *
 *                      [01] Memory of VMC[4] : Number of loads called during last execution                                                 *
 *                      [02] Memory of NUM[2] : Number of active loads during the current cycle                                              *
 *                      [03] Memory of NUM[3] : Number of loads in a normal state                                                            *
 *                      [04] Memory of the number of loads called during the current cycle                                                   *
 *                FTC : [00] Running synthesis information : True if at least one load is active                                             *
 *                      [01] Load 1 command. True means the load is active                                                                   *
 *                      ...  This array is composed of booleans                                                                              *
 *                      [NN] Last listed load command. True means the load is active                                                         *
 *                CFG : Standardized override format                                                                                         *
 *                      <0  : All FTC[x] set to false                                                                                        *
 *                      0   : All FTC[x] keep their current value, that can be changed from outside the function                             *
 *                      1   : All FTC[x] are set according to VNC[0], priorities VTP[x], and defaults VTD[x]                                 *                                                     *
 *                      2   : All FTC[x] set to true                                                                                         *
 *                      3   : All FTC[x] set to true                                                                                         *
 *                      >3  : All FTC[x] set to false                                                                                        *
 *       Internal NUM : [00] Iteration index                                                                                                 *
 *                      [01] Rank of prioritary load (run or stop) for current cycle                                                         *
 *                      [02] Sum of the FTC array content (Number of active loads)                                                           *
 *                      [03] Sum of the VTD array content (Number of loads in normal state)                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 12/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * 30/06/2015 : SE2R, DD : in case of 2 to 3 or more loads, override actions change                                                          *
 * 12/07/2015 : SE2R, DD : Bug in the 2-loads configuration corrected                                                                        *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[4];
int VTC[int(VNC[1])];
VTP[0] = 0;
VTD[0] = true;
if (VNC[1] < 1) { return; }
switch (CFG) {
    case 0:
		return;
    case 1 :
		if (VNC[0] < VMC[1]) { VMC[4] = VMC[4] - 1; }
		if (VNC[0] > VMC[1]) { VMC[4] = VMC[4] + 1; }
		NUM[0] = 1;
		NUM[2] = 0;
		NUM[3] = 0;
		for(NUM[0] = 1;  NUM[0] <= VNC[1] ; NUM[0]++) {
			if (VTD[NUM[0]] < 0.5) { FTC[NUM[0]] = false; }
			NUM[2] = NUM[2] + FTC[NUM[0]] ;
			NUM[3] = NUM[3] + VTD[NUM[0]] ;
		}
		if (NUM[2] > VMC[4]) { VMC[1] = VMC[1] + 1; }
		if (NUM[2] < VMC[4]) { VMC[1] = VMC[1] - 1; }
		if ((NUM[3] < VMC[3]) && (NUM[2] < VMC[2]))  { VMC[1] = VMC[1] - 1; } 
		if (VMC[4] > VMC[1]) {
			NUM[0] = 1;
			NUM[1] = 1; 
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
				VTC[NUM[0]] = VTP[NUM[0]] + (1000 * (FTC[NUM[0]] > 0.5)) + (10000 * (VTD[NUM[0]] < 0.5));
				if (VTC[NUM[0]] < VTC[NUM[1]]) { NUM[1] = NUM[0]; }
			}
		if (VTD[NUM[1]] > 0.5) { FTC[NUM[1]] = true; } else { FTC[NUM[1]] = false; }
		}
		if (VMC[4] < VMC[1]) {
			NUM[0] = 1;
			NUM[1] = 1; 
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
				VTC[NUM[0]] = VTP[NUM[0]] + (1000 * (VTD[NUM[0]] < 0.5)) + (10000 * (FTC[NUM[0]] > 0.5));
				if (VTC[NUM[0]] > VTC[NUM[1]]) { NUM[1] = NUM[0]; }
			}
			FTC[NUM[1]] = false;
		}
		VMC[0] = NUM[1];
		VMC[1] = VMC[4];
		VMC[2] = NUM[2];
		VMC[3] = NUM[3];
		return;
	case 2 :
		NUM[0] = 1;
		if (VNC[1] = 2) {
			FTC[1] = true;
			FTC[2] = false;
		} else {
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) { FTC[NUM[0]] = true; }
		}
		VMC[0] = NUM[1];
		return;
	case 3 :
		NUM[0] = 1;
		if (VNC[1] = 2) {
			FTC[1] = false;
			FTC[2] = true;
		} else {
			for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) { FTC[NUM[0]] = true; }
		}
		VMC[0] = NUM[1];
		return;
	default:
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) { FTC[NUM[0]] = false; }
		VMC[0] = NUM[1];
		return;
	}
return; }

void BMS::T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG) {
/*-------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_SEQCSR - Multiple and identical loads sequencer from a global load percentage command                (SOF Compatible) 29/06/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG)                                                                           *
 *                VPE : [00] Global load percentage command                                                                                  *
 *                      [01] On every 0-100% span of every load, Stop % of the load (has to be lower than VPE[2]                             *
 *                      [02] On every 0-100% span of every load, Run  % of the load (has to be greater than VPE[1]                           *
 *                VNC : [00] Calculated number of loads running according to VPE[0]                                                          *
 *                      [01] Total number of loads available                                                                                 *
 *                FTC : [00] Unused, set to 0                                                                                                *
 *                      [01] Load 1 command. True means the load is active                                                                   *
 *                      ...  This array is composed of booleans                                                                              *
 *                      [NN] Last listed load command. True means the load is active                                                         *
 *                CFG : Standardized override format                                                                                         *
 *                      <0  : All FTC[x] set to false                                                                                        *
 *                      0   : All FTC[x] keep their current value, that can be changed from outside the function                             *
 *                      1   : All FTC[x] are set according to VPE[0]                                                                         *
 *                      2   : All FTC[x] set to true                                                                                         *
 *                      3   : All FTC[x] set to true                                                                                         *
 *                      >3  : All FTC[x] set to false                                                                                        *
 *       Internal NUM : [00] Iteration index                                                                                                 *
 *                      [01] Sum of the FTC array content (Number of active loads)                                                           *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 29/06/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
int NUM[2];
if (VNC[1] < 1) { return; }
switch (CFG) {
    case 0:
		return;
    case 1 :
		NUM[0] = 1;
		NUM[1] = 0;
		for(NUM[0] = 1;  NUM[0] <= VNC[1] ; NUM[0]++) {
			if (((VPE[0] / VNC[1]) - (NUM[0] - 1)) < (VPE[1] /VNC[1])) { FTC[NUM[0]] = false; }
			if (((VPE[0] / VNC[1]) - (NUM[0] - 1)) > (VPE[2] /VNC[1])) { FTC[NUM[0]] = true; }
			NUM[1] = NUM[1] + FTC[NUM[0]];
		}
		VNC[0] = NUM[1];
		if (NUM[1] > 0) { FTC[0] = true; } else { FTC[0] = false; }
		return;
	case 2 :
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
			FTC[NUM[0]] = true;
		}
		VNC[0] = VNC[1];
		return;
	case 3 :
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
			FTC[NUM[0]] = true;
		}
		VNC[0] = VNC[1];
		return;
	default:
		NUM[0] = 1;
		for(NUM[0] = 1; NUM[0] <= VNC[1]; NUM[0]++) {  
			FTC[NUM[0]] = false;
		}
		VNC[0] = 0;
		return;
	}
return; }

void BMS::T_SEQDEM(float VPE[], float CPT[], int RES[], int CFG) {
/* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : T_SEQDEM - Fonction de démarrage en séquence d'équipements sur seuils haut et bas temporisés           (Compatible IFS) 26/05/2015 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 *        T_SEQDEM(float VPE[], float CPT[], int RES[], int CFG)                                                                             *
 *                VPE : [00] Analog value to be averaged (percentage in general)                                                             *
 *                      [01] Low  command threshold. has to be inferior to VPE[2]                                                            *
 *                      [02] High command threshold. has to be superior to VPE[1]                                                            *
 *                CPT : [00] Counter of elapsed time outside of [VPE[1] - VPE[2]] interval. Resets when VPE[0] is between VPE[1] and VPE[2]  *
 *                      [01] Duration to confirm low  threshold action in seconds                                                            *
 *                      [02] Duration to confirm high threshold action in seconds                                                            *
 *                      [03] Stabilization duration : neutralizes counters after any change in RES[0]                                        *
 *                      [04] Scan time of the Arduino in seconds                                                                             *
 *                      [05] Memory of Millis system variable for cycle time calculation                                                     *
 *                RES : [00] Calculated number of loads called                                                                               *
 *                      [01] Minimum number of loads to call                                                                                 *
 *                      [02] Maximum number of loads to call                                                                                 *
 *                CFG : Standardized override format                                                                                         *
 *                      <-1 : RES[0] stays to 0 whatever the value of VPE[0] is                                                              *
 *                      -1  : RES[0] disactivates if VPE[0] > VPE[2] confirmed by CPT[2], activates if VPE[0] < VPE[1] confirmed by CPT[1]   *
 *                      0   : RES[0] keeps its current value, that can be changed from outside the function                                  *
 *                      1   : RES[0] activates if VPE[0] > VPE[2] confirmed by CPT[2], disactivates if VPE[0] < VPE[1] confirmed by CPT[1]   *
 *                      2   : RES[0] stays to RES[1] whatever the value of VPE[0] is                                                         *
 *                      3   : RES[0] stays to RES[2] whatever the value of VPE[0] is                                                         *
 *                      >3  : RES[0] stays to 0 whatever the value of VPE[0] is                                                              *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Copyright SE2R , Licensed under the Apache License, Version 2.0 (the "License");                                                          *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * Tested supports : Arduino Leonardo, Arduino Uno, Industruino 32U8 and 1286                                                                *
 *-------------------------------------------------------------------------------------------------------------------------------------------*
 * 26/05/2015 : SE2R, DD : Tested et validated on benchmark                                                                                  *
 *                         VPE[1] Has to be inferior to VPE[2], or the function will commute around a threshold, not timed, no differential  *
 *                         VPE[0] has to take a value between VPE[1] and VPE[2] for counter CPT[0] to reset                                  *
 *                         Stabilization duration acts by setting a negative value to CPT[0]                                                 *
 * ------------------------------------------------------------------------------------------------------------------------------------------*/
CPT[4] =  max(0, (millis() - CPT[5])) / 1000;
CPT[5] =  millis();
switch (CFG) {
    case 0:
		CPT[0]= 0;
		break;
    case 1:
		if ((VPE[0]>= VPE[1]) && (VPE[0]<= VPE[2])) { CPT[0]= min(0, CPT[0]); } else { CPT[0]= CPT[0] + CPT[4]; }
		if ((VPE[0] > VPE[2]) && (CPT[0]>= CPT[2])) { 
			RES[0] = max(RES[1], min(RES[2], (RES[0] + 1)));
			CPT[0] = 0 - CPT[3];
        }
		if ((VPE[0] < VPE[1]) && (CPT[0]>= CPT[1])) {
			RES[0] = max(RES[1], min(RES[2], (RES[0] - 1))) ;
			CPT[0] = 0 - CPT[3];
        }
      break;
    case -1:
		if ((VPE[0]>= VPE[1]) && (VPE[0]<= VPE[2])) { CPT[0]= 0; } else { CPT[0]= CPT[0] + CPT[4]; }
		if ((VPE[0] > VPE[2]) && (CPT[0]>= CPT[2])) {
			RES[0] = max(RES[1], min(RES[2], (RES[0] - 1)));
			CPT[0] = 0 - CPT[3];
        }
		if ((VPE[0] < VPE[1]) && (CPT[0]>= CPT[1])) {
			RES[0] = max(RES[1], min(RES[2], (RES[0] + 1)));
			CPT[0] = 0 - CPT[3];
        }
      break;
    case 2:
		RES[0] = RES[1];
		CPT[0]= 0;
		break;
    case 3:
		RES[0] = RES[2];
		CPT[0]= 0;
		break;
    default:
		RES[0] = 0;
		CPT[0]= 0;
		break;
    }
return; }