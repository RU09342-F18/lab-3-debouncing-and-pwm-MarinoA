Button Debounce by Alex Marino
Embedded Systems Section 3
Due by October 5, 2018
Created October 3, 2018
Version 1.0 - Lasted updated October 5, 2018
Supported Boards: MSP430G2553, MSP430F5529
###############################################
Included files:
main.c
README.txt
Also required: msp430.h
###############################################
The purpose of this program is to debounce a button which controls an LED. Bouncing occurs when a single 
input results in multiple triggers of the output. In this case, a bounce would make the LED change states 
multiple times on a single button press. The G2553 has built in debounce circuitry, and is therefore not 
as affected, but the F5529 bounced significantly and this program does aid in lowering the occurances of 
bounce on that board. The program uses a button interrupt, which would normally trigger the LED, to instead
trigger a timer to begin counting and disable button interrupt. Once the timer, in up mode, reaches its 
CCR0, a second interrupt is triggered which stops the timer and toggles the LED. It also re-enables button
interrupt so that additional button presses can once again be recognized. This makes it so that, 
while the timer is counting, no additional button presses will trigger an interrupt. The timer was set
such that the delay was long enough to reduce accidental triggers, but not so long that the LED output 
was visually delayed from the input as perceptible by the naked eye. A final value of 200, with an input 
of ACLK divided by 2, was used. 
For the G2553, P1.3 was the switch input, and P1.0 was the LED output. 
On the F5529, P1.0 is still the LED output but P2.1 was used as the switch input. 
