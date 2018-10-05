//FR2311 Software PWM
#include <msp430fr2311.h>

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //unlock GPIO pins 
    P2DIR |= BIT0; //Set pin RED LED to OUT
    P1DIR |= BIT0; //sets green LED to out for debug

    P1DIR &= ~BIT1;         //set P1.1 as input
    P1IE |=  BIT1;         // P1.1 interrupt enabled
    P1IES |= BIT1;         // P1.1 falling edge
    P1REN |= BIT1;        // Enable Pull Up on SW2 (P1.1)
    P1OUT |= BIT1;        //sets pull up resistor
    P1IFG &= ~BIT1;      // P1.1 IFG cleared

    TB0CCR0 = 255; //period
    TB0CCR1 = 127; //set duty cycle to 50%
    TB0CCTL1 = CCIE; //enable capture compare interrupt
    TB0CTL = TBSSEL_2 + MC_1 + ID_2 + TBIE; //SMCLK, up mode
    __bis_SR_register(GIE);   //enable global interrupt
    while(1);
}

#pragma vector =PORT1_VECTOR //button interrupt 
__interrupt void PORT1(void)
{
    if (P1IES & BIT1) //if falling edge detected 
    {
    P1OUT |= BIT0; //turn on LED
    P1IES &= ~BIT1; //change to falling edge detect 
    }
    else //if falling edge 
    {
    P1OUT &= ~BIT0; //turn off LED
    P1IES |= BIT1; //change to rising edge detect 
    }

    if(TB0CCR1 < 255 && TB0CCR1 >= 0) //if CCR1 is between 0 and 255
    {
    TB0CCR1 +=25; //increase duty cycle by 10%
    }
    else //if duty cycle is 100%
    {
        TB0CCR1 = 0; //reset duty cycle 
    }
    P1IFG &= ~BIT1; //clear button interrupt flag 
}

#pragma vector = TIMER0_B1_VECTOR //timer interrupt vector 
__interrupt void TIMER1 (void)
{
    switch(TB0IV)
      {
        case  2:  P2OUT &= ~BIT0;  break;        // CCR1 for duty cycle 
        case  4:  break;                        // CCR2 not used
        case 0x0E:  P2OUT |= BIT0;                // overflow for period
                  break;
      }
}
