/*
 * File:   lab11.c
 * Author: PADA
 *
 * Created on 2017å¹?2??1?? ä¸‹å? 5:47
 */

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic18f4520.h>
// easy ring buffer 
char mystring[8];
int stringC = 0;
char temp;
int count = 0;
int onesec = 0;
unsigned int fourcheck[4];
unsigned int MyadcValue;
//Some easy function(init...etc)
void Myusartwrite(char);
void MyusartRead();
void Myusartinit();
void ISR_Init();
void Mylab10_2();
void MyadcInit();
void MyadcRead();
void main(void) {
    
    
    Myusartinit();
    MyadcInit();
    ISR_Init();
    int thousand = 0;
    RCONbits.IPEN=0x01;
    T1CON=0x85;     //SELECT 1:2prescator
    PIE1bits.TMR1IE=0;
    while(1)
    {
        //TODO 
        if(mystring[0] == 'p' && mystring[1] == 'r' && mystring[2] == 'i' && mystring[3] == 'n' && mystring[4] == 't') {
            MyadcRead();
            Myusartwrite(' ');
            Myusartwrite((MyadcValue/1000)+'0');
            Myusartwrite(((MyadcValue/100)%10)+'0');
            Myusartwrite(((MyadcValue/10)%10)+'0');
            Myusartwrite((MyadcValue%10)+'0');
            mystring[4] = '\n';
            count = 0;
        }
        else if(mystring[0] == 'a' && mystring[1] == 'u' && mystring[2] == 't' && mystring[3] == 'o') {
           // TRISD = 0;
            MyadcRead();
            Myusartwrite(' ');
            Myusartwrite((MyadcValue/1000)+'0');
            Myusartwrite(((MyadcValue/100)%10)+'0');
            Myusartwrite(((MyadcValue/10)%10)+'0');
            Myusartwrite((MyadcValue%10)+'0');
            mystring[3] = '\n';
            count = 0;
            
            PIE1bits.TMR1IE=1;  //timer1 enable
            IPR1bits.TMR1IP=1;  //timer1 priority
            PIR1bits.TMR1IF=0;  //timer1 flag
            INTCONbits.GIE=1;  // Enable all High Priority Interrupt
            TMR1 = 65535-62500;
        }
            //Mylab10_2();
            
        
    }
    return;
    
}
void ISR_Init()
{
    //TODO
    
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //enable low priority interrupt
    
            
    return ;
}

void MyusartRead()//read word from user scanf
{
    //TODO
    temp = RCREG;
    mystring[count] = RCREG;
    count = (count+1)%5;
    
    return ;
}
void Myusartwrite(char a)
{
    //TODO
    TXREG = a;//show on the window
    int i = 1;
           //write to TXREG will send data 
    return ;
}
void Myusartinit()
{
    
    
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet 
    TRISCbits.TRISC7 = 1;            //  
    TRISD = 0;
    LATD = 0;
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */      
    //  Setting baud rate
    BAUDCONbits.BRG16 = 1 ;          //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 25;      
    
   //   Serial enable
    TXSTAbits.SYNC = 0;             //choose the async moode
    RCSTAbits.SPEN = 1;              //open serial port
    
    // Interrutp Setting 
    
    //setting TX/RX
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 1;
    TXSTAbits.TXEN = 1;             //Enable Tx
    RCSTAbits.CREN = 1;             //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //Tx interrupt
    IPR1bits.TXIP = 1;              //Setting Tx as high/low priority interrupt
    PIE1bits.RCIE = 1;              //Rx interrupt
    IPR1bits.RCIP = 1;              //Setting Rc as high/low priority interrupt
    
    
    return ;
}
void Mylab10_2()
{
    /*if(count%9!=0) {
        return;
    }
    count = 1;
    if(temp == '1'){
        LATDbits.LATD2 = 1;
    }
    
    else if(temp == '2'){
        LATDbits.LATD2 = 0;
    }
    
    else if(temp == '3'){
        LATDbits.LATD3 = 1;
    }
    
    else if(temp == '4'){
        LATDbits.LATD3 = 0;
    }
    */
    return ;
}
void interrupt  Hi_ISR(void)
{
    /*
     * TODO
     * 
    */
    if(TMR1IF == 1&&PIE1bits.TMR1IE==1) {
            PIE1bits.TMR1IE=0;
            TMR1IF=0;
            MyadcRead();
            fourcheck[onesec] = MyadcValue;
            if(onesec==3){
                MyadcValue = (fourcheck[0]+fourcheck[1]+fourcheck[2]+fourcheck[3])/4;
                Myusartwrite(' ');
                Myusartwrite((MyadcValue/1000)+'0');
                Myusartwrite(((MyadcValue/100)%10)+'0');
                Myusartwrite(((MyadcValue/10)%10)+'0');
                Myusartwrite((MyadcValue%10)+'0');
                onesec = -1;
            }
            onesec++;
            TMR1 = 65535-62500;
            PIE1bits.TMR1IE=1;
            return;   
    }
    
    if(RCIF)
    {
        if(RCSTAbits.OERR == 1)
        {
            /*if necessary *handle overrun exception
             * Overrun will set if rcreg fifo is full then third word is recived
             * Then the RCSTA<1> OERR will set
             * how clear?
             * clear CERN OEER will be clear
            */
        }
        MyusartRead();
        Myusartwrite(temp);
    }
    
    return ;
}
void interrupt low_priority Lo_ISR(void)
{
   // process other interrupt sources here, if required
    return;
}
void MyadcInit()
{
    //TODO
    ADCON1bits.VCFG1 = 0 ;  //setting vref-
    ADCON1bits.VCFG0 = 0 ;  //setting vref+
    ADCON1bits.PCFG  = 15 ;  //Setting A/D Port Configuration Control
    ADCON0bits.CHS = 0 ;    //setting input channel(RA0))
    TRISA = 0x01 ;             //setting port as input 
    ADCON2bits.ADFM = 0 ;    //setting Left justified
    //setting acquisition time (ADCON2) ACQT 2:0
    //setting conversion time (ADCON2))
    ADCON2bits.ACQT = 1;
    ADCON2bits.ADCS = 0;
    ADCON0bits.ADON = 1;    //turn on ad module 

    /*setting adc interrupt 
     * 1.clear ADIF
     * 2.set ADIE
     * 3.SET GIE
    */
    
    return ;
}
void MyadcRead()
{
    //waitting acquistion time
    /*start conversion 
     * 1.Set GO/DONE ADCON0
     * 2.ckeck GO/DONE (clear is setting finish)
     * 3.result in ADRESH"ADRESL
     */
    ADCON0bits.GO =1;
    while(ADCON0bits.GO);
    MyadcValue = ADRESH * 4;
    MyadcValue += (ADRESL / 64);
    
    //TODO
    //convert adc value into MyadcValue and show it to ta . describe why the value is
    
    return ;
}
