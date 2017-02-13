 /* ------------------------------------------------------------------------------------------------------------------------------------------*
 * SE2R : 12/01/2017 - BMS Functions for Arduino - Source code file                                                           Version 1.02   *
 * ------------------------------------------------------------------------------------------------------------------------------------------*
 * Functions in this library                                                                                                                 *
 *        C_CDELEM - On/Off boolean control taking account of a fault, and standardized override interface (SOI) (SOF compatible) 12/06/2015 *
 *        C_CDETPO - On/Off boolean control with set/reset delay taking account of a fault                       (SOF compatible) 03/06/2015 *
 *        C_COMMUT - Produces a standardized override command from boolean sets of conditions                                     12/06/2015 *
 *        C_DISCOR - Delayed differential detection between two values,                                          (SOF compatible) 29/06/2015 *
 *        C_TFONCT - On-time hours counter                                                                       (SOF compatible) 12/06/2015 *
 *        R_ANA3PL - Analog-to-tristate converter, output in a boolean array size 2 [0] open [1] close           (SOF compatible) 30/06/2015 *
 *        R_ANA3PT - Analog-to-tristate converter, output on 2 digital output channels to open or close          (SOF compatible) 30/06/2015 *
 *        R_ANATOR - Analog-to-Pulse Width Modulation (PWM)                                                      (SOF compatible) 25/06/2015 *
 *        R_PIDITR - Full-numeric Proportionnal-Integral-Derivative (PID) loop                                   (SOF compatible) 24/06/2015 *
 *        S_SCANTS - Scan-time calculation in seconds                                                                             07/08/2015 *
 *        T_APPRLN - Multi-segment linear conversion of a float value to another                                 (SOF compatible) 23/06/2015 *
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

 /**
 * @ file	BMS.h
 * @brief	Functions for industrial controls and BMS.
 * @author	SE2R, Didier DORGUIN
 * @date	12/01/2017
 * @version	1.02
 */
 
#ifndef BMS_h
#define BMS_h
#include "Arduino.h"

class BMS
{
public :
	BMS();
	void C_CDELEM(boolean CDE, boolean DFT, boolean RES[], int CFG);
	void C_CDETPO(boolean CDE, boolean DFT, float CPT[], boolean RES[], int CFG);
	void C_COMMUT(boolean FX1, boolean FX2, boolean FX3, boolean FX0, int RES[]);
	void C_DISCOR(float IN1, float IN2, float DIF, float CPT[], boolean RES[], int CFG);
	void C_TFONCT(boolean MAR, float RES[], int CFG);
	void R_ANA3PL(float VPC[], float VTC[], boolean RES[], int CFG);
	void R_ANA3PT(float VPC[], float VTC[], int VNO, int VNF, int CFG);
	void R_ANATOL(float VPC[], float VTC[], boolean RES[], int CFG);
	void R_ANATOR(float VPC[], float VTC[], int VNC, int CFG);
	void R_PIDITR(float MES, float CSG, float VPC[], float VKR[], int CFG);
	void S_SCANTS(float CPT[]);
    void S_TIMERS(float CPT[9], boolean FCO[7]);
	void T_APPRLN(float MES, int VTE[], float VTS[], int CFG);
	void T_CLINEB(float CDE[], float RES[], int CFG);
	void T_COMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG);
	void T_DEMULT(unsigned int MPX[], int RNG, boolean FLG[], int CFG);
	void T_DMULTV(float MES, int OHM[], float TNS[], int VNC[], boolean RES[], int CFG);
	void T_HYSTPO(float MES[], float CPT[], boolean RES[], int CFG);
	void T_LIMVAR(float MES[], float CPT[], int RES[], int CFG);
	void T_MOYDYN(float MES, float RES[], int CFG);
	void T_MULSEC(int VNC[], int VTP[], boolean VTD[], int VMC[], boolean FTC[],  int CFG);
	void T_SEQCSR(float VPE[], int VNC[], boolean FTC[], int CFG);
	void T_SEQDEM(float MES[], float CPT[], int RES[], int CFG);
};
#endif