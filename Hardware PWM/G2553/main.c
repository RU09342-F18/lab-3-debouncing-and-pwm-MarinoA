//G2553 Hardware PWM

#include <msp430g2553.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer
    P1DIR |= BIT6; //Set RED LED to OUT
    P1DIR |= BIT0; //Set GREEN LED to OUT
    P1SEL |= BIT6; //Set pin 1.6 as PWM OUT

    P1DIR &= ~BIT3;         //set P1.3 as input
    P1IE |=  BIT3;         // P1.3 interrupt enabled
    P1IES |= BIT3;         // P1.3 falling edge
    P1REN |= BIT3;        // Enable Pull Up on SW2 (P1.3)
    P1OUT |= BIT3;        //sets pull up resistor
    P1IFG &= ~BIT3;      // P1.3 IFG cleared

    TA0CCR0 = 1000; //Set the period
    TA0CCTL1 = OUTMOD_7; //set PWM to reset/set
    TA0CCR1 = 500; //ON period 
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //SMCLK, up mode, divide by 4
    __bis_SR_register(LPM0_bits + GIE); //Low power mode 0 and global interrupt enable
}

#pragma vector =PORT1_VECTOR //button interrupt 
__interrupt void PORT1(void)
{
    if (P1IES & BIT3) //if falling edge detected 
    {
    P1OUT |= BIT0; //turn on LED
    P1IES &= ~BIT3; //change to falling edge detect 
    }
    else //if rising edge 
    {
    P1OUT &= ~BIT0; //turn off LED
    P1IES |= BIT3; //change to rising edge detect 
    }

    if(TA0CCR1 < 1000 && TA0CCR1 >= 0) //if current CCR1 is between 0 and 1000
    {
    TA0CCR1 +=100; //increase duty cycle by 10%
    }
    else if (TA0CCR1 >= 1000) //if CCR1 is greater than or equal to 1000 (100% duty cycle) 
    {
        TA0CCR1 = 0; //clear duty cycle to 0%
    }
    else
    {
        TA0CCR1 = 100; //some other value goes to 10%
    }

    P1IFG &= ~BIT3; //clear button interrupt flag
}

