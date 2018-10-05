Software PWM by Alex Marino
Embedded Systems Section 3
Due by October 5, 2018
Created October 3, 2018
Version 1.0 - Lasted updated October 5, 2018
Supported Boards: MSP430G2553, MSP430FR2311
###############################################
Included files:
main.c
README.txt
Also required: msp430.h
###############################################
The software PWM replicates the functionality of the hardware PWM but uses timer interrupts instead of 
the built in PWM module. This would allow any pins to be driven with the given duty cycle. For this 
program, interrupt needed to be enabled for the button and the timer. Capture compare interrupt was also
needed for the capture compare control of the timer. Then, the reset/set functionality was emulated using
an interrupt triggered by the timer. When the timer interrupt vector was set to 2, this signified a CCR1 
value, and so the LED was turned off. When the timer interrupt vector was 10, this signified an overflow, 
meaning CCR0 had been reached and the LED was toggled on. The same button interrupt routine as hardware 
PWM was also used. 
The G2553 uses P1.0 and P1.6 as the LED outputs and P1.3 as the switch input. 
The FR2311 uses P2.0 and P1.0 as the output LEDs and P1.1 as the switch input. The GPIO pins also need to be 
unlocked for this board. 
