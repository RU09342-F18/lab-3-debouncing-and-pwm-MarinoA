//F5529 Debounce

#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // stop watchdog timer
    P1DIR |= BIT0;                          //set P1.0 as out
    P2DIR &= ~BIT1;                        //set P2.1 as in
    P2IE |=  BIT1;                            // P2.1 interrupt enabled
    P2IES |= BIT1;                            // P2.1 falling edge
    P2REN |= BIT1;                            // Enable Pull Up on SW1 (P2.1)
    P2OUT |= BIT1;                              //sets pull up resistor
    P2IFG &= ~BIT1;                           // P2.1 IFG cleared

    //set up timer
    TB0CTL = TBSSEL_1 + ID_3 + MC_0; //Timer 0 control = ACLK, divide by 8, timer stopped

    //set up CCR1
    TB0CCTL0 =CCIE;  //enable capture compare interrupt
    TB0CCR0 = 600; //change bounce delay

    _BIS_SR(LPM0_bits + GIE);   //enable global interrupt
    return 0;
}


#pragma vector =PORT2_VECTOR            //button interrupt vector
__interrupt void Button_down(void)
{
    TB0CTL = TBSSEL_1 + ID_3 + MC_1; //Timer 0 control = ACLK, divide by 8, count up to CCR0
    P2IE &= ~BIT1;                     //disable button interrupt
    P2IFG &= ~BIT1;                     //clear button interrupt flag
}

#pragma vector =TIMER0_B0_VECTOR    //timer interrupt vector
__interrupt void TIMER0_B1(void)
{

    P1OUT ^= BIT0;                  //toggle P1.0 LED
    TB0CTL = TBSSEL_1 + ID_3 + MC_0; //stop timer
    TBCTL |= TBCLR;                  //clear timer
    P2IFG &= ~BIT1;                   //clear button interrupt flag
    P2IE |=  BIT1;                    //enable button interrupt
}
