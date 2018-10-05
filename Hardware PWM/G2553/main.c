//G2553 Hardware PWM

#include <msp430g2553.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer
    P1DIR |= BIT6; //Set pin RED LED to OUT
    P1DIR |= BIT0;
    P1SEL |= BIT6; //Set pin 1.6 as PWM OUT

    P1DIR &= ~BIT3;         //set P1.3 as input
    P1IE |=  BIT3;         // P1.3 interrupt enabled
    P1IES |= BIT3;         // P1.3 falling edge
    P1REN |= BIT3;        // Enable Pull Up on SW2 (P1.3)
    P1OUT |= BIT3;        //sets pull up resistor
    P1IFG &= ~BIT3;      // P1.3 IFG cleared

    TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us.
    TA0CCTL1 = OUTMOD_7;
    TA0CCR1 = 500; //The period in microseconds that the power is ON. It's half the time, which translates to a 50% duty cycle.
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
    __bis_SR_register(LPM0_bits + GIE); //Switch to low power mode 0.
}

#pragma vector =PORT1_VECTOR
__interrupt void PORT1(void)
{
    if (P1IES & BIT3)
    {
    P1OUT |= BIT0;
    P1IES &= ~BIT3;
    }
    else
    {
    P1OUT &= ~BIT0;
    P1IES |= BIT3;
    }

    if(TA0CCR1 < 1000 && TA0CCR1 >= 0)
    {
    TA0CCR1 +=100;
    }
    else if (TA0CCR1 >= 1000)
    {
        TA0CCR1 = 0;
    }
    else
    {
        TA0CCR1 = 100;
    }

    P1IFG &= ~BIT3;
}

