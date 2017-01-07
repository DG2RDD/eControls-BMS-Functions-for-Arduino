# eControls-BMS
Arduino functions library for BMS and industrial controls

Have you ever been trying to write a PID in Arduino code from scratch ? Did your arduino learn how to fly in the process ?

OK, just keep a stiff upper lip and have a look at this repo. My pals and myself have been using these controls functions for a bit more than a quarter of a century (Yeah, call me a dinosaur if you want, I'll manage, but just keep in mind I was probably coding when you were'nt even planned as a part of Life, the Universe, and Whatever else).

What's in there, now, old relative ?
A bunch of C functions originally developped in a C-like proprietary environment, that I have been translating to Arduino C recently. In fact, we are currently using them in an industrial version of Arduino called Industruino, but you may use most of them in a regular Arduino. Hardware compatibility is listed in every function header comment block in the .cpp file.
These, once combined, will help you to quickly create complex controls applications where the volume of the control code you will have to write will be reduced to a minimum.
The initial letter of every function relates it to one of 4 families :
- "C" functions are mainly dedicated to direct on/off control of equipments
- "R" functions are dedicated to PID control and peripherals of the PID loop
- "S" functions are dedicated to system information or calculation
- "T" are analog or boolean calculation functions. In fact this family is some kind of a bag of controls tricks
Among the recurrent problems in controls, overriding the action of a function and returning it to standard operation is quite something. All these functions are at least partly compatible with what I call SOF for "Standard Override Format".
You will find most of them can be overridden through an integer value called CFG in every arguments list : 
- CFG at -1 means action to Off or 0
- CFG at 0 means no action (function neutralized)
- CFG at 1 means auto (function active)
- CFG at 2 means action overriden to a fixed value or On (with security active)
- CFG at 3 means action overriden to max value or On (unconditional).
Of course, there is a function to produce an acceptable CFG value from logical conditions, called C_COMMUT.

Now you're done with your babbling, what about some code ?

"C" family - Commands to obey
- C_CDELEM : sets/resets a boolean command according to a boolean run demand and a boolean default. SOF compatible
- C_TPOCDE : Same as C_CDELEM, but with activation and deactivation delays. SOF compatible
- C_COMMUT : SOF integer calculation function.
- C_DISCOR : A delayed differential between two boolean or analog values sets/resets a boolean. SOF compatible
- C_TFONCT : Runtime calculation function. SOF compatible

"R" family  PID gets in the ring
- R_ANA3PL : Translates a 0-100% action to tristate through 2 booleans. SOF compatible
- R_ANA3PT : Translates a 0-100% action to tristate through 2 binary outputs. SOF compatible
- R_ANATOL : Translates a 0-100% action to PWM action on a boolean. SOF compatible
- R_ANATOR : Translates a 0-100% action to PWM action on a binary output. SOF compatible
- R_PIDITR : Full-digital iterative parallel PID loop. SOF compatible

"S" family - Systems considerations
- S_SCANTS : Calculation of the current CPU scan time. SOF compatible
- S_TIMERS : Sets/Resets booleans at fixed time intervals. SOF compatible

"T" family - Bag of tricks
- T_APPRLN : Translates an analog value into another through a poly-linear curve approximation. SOF compatible
- T_CLINEB : Translates an nalog avalue into another through a linear segment with low/high limits. SOF compatible
- T_COMULT : Multiplexes a 16-boolean array into an unsigned integer. SOF compatible
- T_DEMULT : Demultiplexes an unsigned integer to a 16-boolean array. SOF compatible
- T_DMULTC : Demultiplexes 5 dry contacts from a voltage input (needs some cabling). SOF compatible
- T_HYSTPO : Delayed Set/Reset of a boolean according to and analog value and hi/lo limits. SOF compatible
- T_LIMVAR : Variation speed limiter in an analog value. SOF compatible 
- T_MOYDYN : Dynamic averaging (first-order filter) on an analog value. SOF compatible
- T_MULSEC : Sets/resets commands according to the number demanded, defaults, with prioritties and mutual replacement. SOF compat.
- T_SEQCSR : multiple loads sequencer from an analog command. SOF compatible
- T_SEQDEM : multiple loads sequencer through delayed hi/lo limits on an analog command. SOF compatible
