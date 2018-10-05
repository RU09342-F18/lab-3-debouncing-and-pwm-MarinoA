//FR2311 Hardware PWM

#include <msp430fr2311.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //unlock GPIO
    P2DIR |= BIT0; //Set pin RED LED to OUT
    P1DIR |= BIT0;
    P2SEL0 |= BIT0; //Set pin 2.0 as PWM OUT
    P2SEL1 &= ~BIT0;

    P1DIR &= ~BIT1;         //set P1.1 as input
    P1IE |=  BIT1;         // P1.1 interrupt enabled
    P1IES |= BIT1;         // P1.1 falling edge
    P1REN |= BIT1;        // Enable Pull Up on SW2 (P1.1)
    P1OUT |= BIT1;        //sets pull up resistor
    P1IFG &= ~BIT1;      // P1.1 IFG cleared

    TB1CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us.
    TB1CCTL1 = OUTMOD_7;
    TB1CCR1 = 500; //The period in microseconds that the power is ON. It's half the time, which translates to a 50% duty cycle.
    TB1CTL = TBSSEL_2 + MC_1 + ID_2; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.
    __bis_SR_register(LPM0_bits + GIE); //Switch to low power mode 0.
}

#pragma vector =PORT1_VECTOR
__interrupt void PORT1(void)
{
    if (P1IES & BIT1)
    {
    P1OUT |= BIT0;
    P1IES &= ~BIT1;
    }
    else
    {
    P1OUT &= ~BIT0;
    P1IES |= BIT1;
    }

    if(TB1CCR1 < 1000 && TB1CCR1 >= 0)
    {
    TB1CCR1 +=100;
    }
    else if (TB1CCR1 >= 1000)
    {
        TB1CCR1 = 0;
    }
    else
    {
        TB1CCR1 = 100;
    }

    P1IFG &= ~BIT1;
}

