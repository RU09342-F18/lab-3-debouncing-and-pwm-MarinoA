//FR2311 Hardware PWM

#include <msp430fr2311.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //unlock GPIO
    P2DIR |= BIT0; //Set P2.0 to OUT
    P1DIR |= BIT0; //Set P1.0 to OUT
    P2SEL0 |= BIT0; //select bit 0 = 1 for P2.0
    P2SEL1 &= ~BIT0; //select bit 1 = 0 for P2.0 (PWM)

    P1DIR &= ~BIT1;         //set P1.1 as input
    P1IE |=  BIT1;         // P1.1 interrupt enabled
    P1IES |= BIT1;         // P1.1 falling edge
    P1REN |= BIT1;        // Enable Pull Up on P1.1
    P1OUT |= BIT1;        //sets pull up resistor
    P1IFG &= ~BIT1;      // P1.1 IFG cleared

    TB1CCR0 = 1000; //Set period 
    TB1CCTL1 = OUTMOD_7; //set PWM to reset/set
    TB1CCR1 = 500; //ON period 
    TB1CTL = TBSSEL_2 + MC_1 + ID_2; //SMCLK, up mode, divide by 4
    __bis_SR_register(LPM0_bits + GIE); //Low power mode 0 and globel interrupt enable 
}

#pragma vector =PORT1_VECTOR //button interrupt 
__interrupt void PORT1(void)
{
    if (P1IES & BIT1)//if falling edge 
    {
    P1OUT |= BIT0; //turn on LED
    P1IES &= ~BIT1;//change to detect falling edge 
    }
    else //if rising edge 
    {
    P1OUT &= ~BIT0;//turn off LED
    P1IES |= BIT1;//change to rising edge detect 
    }

    if(TB1CCR1 < 1000 && TB1CCR1 >= 0) //if CCR1 is between 0 and 1000
    {
    TB1CCR1 +=100; //increase duty cycle by 10%
    }
    else if (TB1CCR1 >= 1000) //if duty cyle is 100%
    {
        TB1CCR1 = 0; //clear duty cycle to 0%
    }
    else
    {
        TB1CCR1 = 100; //value outside range gets 10%
    }

    P1IFG &= ~BIT1; //clear button interrupt flag 
}

