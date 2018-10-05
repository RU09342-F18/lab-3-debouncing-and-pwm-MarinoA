//G2553 Debounce

#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	                // stop watchdog timer
    BCSCTL3 = LFXT1S_2;                        //tells MC to use internal crystal
    P1DIR |= BIT0;                          //set P1.0 as out
    P1DIR &= ~BIT3;                        //set P1.3 as in
    P1IE |=  BIT3;                            // P1.3 interrupt enabled
    P1IES |= BIT3;                            // P1.3 falling edge
    P1REN |= BIT3;                            // Enable Pull Up on SW2 (P1.3)
    P1OUT |= BIT3;                              //sets pull up resistor
    P1IFG &= ~BIT3;                           // P1.3 IFG cleared

    //set up timer
    TA0CTL = TASSEL_1 + ID_1 + MC_0; //Timer 0 control = ACLK, divide by 2, timer stopped

    //set up CCR1
    CCTL0 =CCIE;  //enable capture compare interrupt
    CCR0 = 200; //change bounce delay

    _BIS_SR(LPM0_bits + GIE);   //enable global interrupt
    return 0;
}


#pragma vector =PORT1_VECTOR            //button interrupt vector
__interrupt void Button_down(void)
{
    TA0CTL = TASSEL_1 + ID_1 + MC_1; //Timer 0 control = ACLK, divide by 2, count up to CCR0
    P1IE &= ~BIT3;                     //disable button interrupt
    P1IFG &= ~BIT3;                     //clear button interrupt flag
}

#pragma vector =TIMER0_A0_VECTOR        //timer interrupt vector
__interrupt void TIMER0_A1(void)
{

    P1OUT ^= BIT0;                    //toggle P1.0 LED
    TA0CTL = TASSEL_1 + ID_1 + MC_0; //stop timer
    TACTL |= TACLR;                  //clear timer
    P1IFG &= ~BIT3;                 //clear button interrupt flag
    P1IE |= BIT3;                   //enable button interrupt
}
