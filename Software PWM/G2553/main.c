//G2553 Software PWM
#include <msp430g2553.h>

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer
    P1DIR |= BIT6; //Set pin RED LED to OUT
    P1DIR |= BIT0; //sets green LED to out for debug

    P1DIR &= ~BIT3;         //set P1.3 as input
    P1IE |=  BIT3;         // P1.3 interrupt enabled
    P1IES |= BIT3;         // P1.3 falling edge
    P1REN |= BIT3;        // Enable Pull Up on SW2 (P1.3)
    P1OUT |= BIT3;        //sets pull up resistor
    P1IFG &= ~BIT3;      // P1.3 IFG cleared

    TA0CCR0 = 255; //period
    TA0CCR1 = 127; //set duty cycle to 50%
    TA0CCTL1 = CCIE; //enable capture compare interrupt
    TA0CTL = TASSEL_2 + MC_1 + ID_2 + TAIE; //SMCLK, up mode
    __bis_SR_register(GIE);   //enable global interrupt
    while(1);
}

#pragma vector =PORT1_VECTOR
__interrupt void PORT1(void)
{
    if (P1IES & BIT3) //if falling edge detected 
    {
    P1OUT |= BIT0; //turn on LED
    P1IES &= ~BIT3; //change to falling edge detect 
    }
    else //if falling edge detected 
    {
    P1OUT &= ~BIT0; //turn off LED
    P1IES |= BIT3; //change to rising edge detect 
    }

    if(TA0CCR1 < 255 && TA0CCR1 >= 0) //if CCR1 is between 0 and 255
    {
    TA0CCR1 +=25; //increase duty cycle by 10%
    }
    else //if duty cycle is 100%
    {
        TA0CCR1 = 0; //reset duty cycle 
    }
    P1IFG &= ~BIT3; //clear button interrupt flag 
}

#pragma vector = TIMER0_A1_VECTOR //timer interrupt vector 
__interrupt void TIMER1 (void)
{
    switch(TA0IV) //switch case for timer0 A interrupt vector 
      {
        case  2:  P1OUT &= ~BIT6;  break;        // CCR1 for duty cycle
        case  4:  break;                        // CCR2 not used
        case 10:  P1OUT |= BIT6;                // overflow for period
                  break;
      }
}
