#define TMR2PRESCALE 16
//#define OSCCON 0x30

#include <xc.h>
#include <pic18f4520.h>
// BEGIN CONFIG
#pragma config OSC = INTIO67   // Oscillator Selection bits (HS oscillator)
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)

#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

//END CONFIG
int i = 0;
int flag = 0;
PWM1_Init(long setDuty)
{
  PR2 = setDuty;
}

PWM1_Duty(unsigned int duty)
{
    CCP1CONbits.DC1B1 = duty % 2;
    CCP1CONbits.DC1B0 = (duty % 4) / 2;
    CCPR1L = duty / 4;
//set duty to CCPR1L , CCP1X(CCP1CON:5) and CCP1Y(CCP1CON:4)
}

PWM1_Start()
{

  //set CCP1CON
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M3 = 1;
   
  //set timer2 on
    //IPEN=0x01;
    T2CON = 0x7F;       //pre1:16 post 1:16 ==>500k/4/16/16
    /*PIE1bits.TMR2IE = 1;
    IPR1bits.TMR2IP = 1;
    PIR1bits.TMR2IF = 0;*/
  //set rc2 output
    
    TRISCbits.RC2 = 0;
    //RC2 = 1;
    
  /*if (TMR2PRESCALE == 1)
  {
	
  }
  else if(TMR2PRESCALE == 4)
  {  
 
  }
  else if (TMR2PRESCALE == 16)
  {

  }*/
 
}

void main()
{
    OSCCON = 0x30;//500kHz
    PWM1_Init(155);
    PWM1_Start();
   // TRISDbits.RD0 = 1;
  //set trisD
  
    RCONbits.IPEN=0x01;
    T1CON=0x95;     //SELECT 1:2prescator
    PIE1bits.TMR1IE=1;  //timer1 enable
    IPR1bits.TMR1IP=1;  //timer1 priority
    PIR1bits.TMR1IF=0;  //timer1 flag
    INTCONbits.GIE=1;  // Enable all High Priority Interrupt
    TMR1 = 65535-15625;
  
  PWM1_Duty(0);
  
  do{
    if(i<=150) {
        PWM1_Duty(i);
    }
    //__delay_ms(50);
    
  }while(1);
}
void interrupt tc_int(void)     // High priority interrupt
{
    PIE1bits.TMR1IE=0;
    if(TMR1IF == 1){
        if(flag==0)
            i = i+10;
        if(flag==1) {
            i = i-10;
        }
        if(i==160) {
            i = 150;
            flag = 1;
        }
        if(i==-10&&flag==1) {
            i = 0;
            flag = 0;
        }
        TMR1IF=0;
    }
    PIE1bits.TMR1IE=1;
    TMR1 = 65535-15625;
}
