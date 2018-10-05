Hardware PWM by Alex Marino
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
This program is designed to enable the PWM, or pulse width modulation, which is built into the development 
boards. This program works by setting the LED select bit (PxSEL) to 1 and the Timer A0 capture compare 
control to OUTMOD_7, which sets that pin to output from the PWM in reset/set mode. This makes it so that
the PWM resets the output when the timer reaches CCR1 and sets the value when it reaches CCR0. By leaving
the value of CCR0 constant and changing the value of CCR1, the duty cycle of the PWM could be changed. In
practice, changing the duty cycle changed the brightness of the LED. Initially, a duty cycle of 50% was 
selected, but upon pressing the button, the duty cycle was increased by 10% each time. Once a 100% duty 
cycle was reached, the subsequent button press would reset it to 0, which would turn the LED off. Any 
additional button presses would continue to increase the duty cycle by 10%. For debugging purposes, while 
the button is pressed to change the duty cycle, the green LED is enabled. The LED is turned back off when 
the button is released. 
The G2553 uses P1.0 and P1.6 as the LED outputs and P1.3 as the switch input. 
The FR2311 uses P2.0 and P1.0 as the output LEDs and P1.1 as the switch input. The GPIO pins also need to be 
unlocked for this board. 
