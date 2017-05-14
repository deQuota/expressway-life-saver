//#include "custom_char.h"
#include "lcd_hd44780_pic16.h"
#include "myutils.h"
#include "usart_pic16.h"
#include "pic16f877a.h"
#include <xc.h>
//#include <string.h>
#include "UART1.h"
//#include <stdio.h>
//#include <stdlib.h>
//#define GSMON PORTCbits_t.RC4
#define _XTAL_FREQ 20000000
#define k 1.25
#pragma config FOSC = HS            // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF           // Watchdog Timer Enable bit (WDT Disabled)
#pragma config PWRTE = OFF          // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF             // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = ON           // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF            // Low Voltage In-Circuit Serial Programming Disable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF            // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = OFF            // FLASH Program Memory Write Disable (Unprotected program memory may be written to by EECON control)
void fireIndiSend();
char sendS[7];
char sendLa[9];
char sendLo[10];
int checkSpd, checkZero;
int co;
unsigned int warNo = 0;


//void InitUART(int BAUDRATE)
//{
//	TRISC6 = 0;   					// TX Pin
//	TRISC7 = 1;   					// RX Pin
//
//	SPBRG = ((_XTAL_FREQ/16)/BAUDRATE) - 1;
//	BRGH  = 1;                   	// Fast baudrate
//	SYNC  = 0;						// Asynchronous
//	SPEN  = 1;						// Enable serial port pins
//	CREN  = 1;						// Enable reception
//	SREN  = 0;						// No effect
//	TXIE  = 0;						// Disable tx interrupts
//	RCIE  = 1;						// Enable rx interrupts
//	TX9   = 0;						// 8-bit transmission
//	RX9   = 0;						// 8-bit reception
//	TXEN  = 0;						// Reset transmitter
//	TXEN  = 1;						// Enable the transmitter
//}
//
//char BusyUART(void)
//{
//    if (!TXIF)
//        return 1;
//    else
//    {
//        TXIF = 0; //Clear busy flag
//        return 0;
//    }
//}
//
//

void SendByteSerially(unsigned char Byte) // Writes a character to the serial port
{
    while (!TXIF); // wait for previous transmission to finish
    TXREG = Byte;
}

void SendStringSerially(const unsigned char* st) {
    while (*st)
        SendByteSerially(*st++);
}

void sendByGPRS() {



    __delay_ms(1500);
    //  InitUART(9600);
    //  USARTInit(9600);
    // LCDInit(LS_NONE);
    LCDClear();
    LCDWriteStringXY(0, 0, "Sending Data via");
    LCDWriteStringXY(0, 1, "GPRS.....");



    SendStringSerially("AT+CSQ");
    SendByteSerially((char) 13);
    __delay_ms(100);

    SendStringSerially("AT+CGATT?");
    SendByteSerially((char) 13);
    __delay_ms(100);

    SendStringSerially("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    SendByteSerially((char) 13);
    __delay_ms(1000);

    SendStringSerially("AT+SAPBR=3,1,\"APN\",\"CMNET\"");
    SendByteSerially((char) 13);
    __delay_ms(4000);

    SendStringSerially("AT+SAPBR=1,1");
    SendByteSerially((char) 13);
    __delay_ms(2000);


    SendStringSerially("AT+HTTPINIT");
    SendByteSerially((char) 13);
    __delay_ms(2000);
    // rom char* j="VA";
    // const unsigned char *j="VA";


    SendStringSerially("AT+HTTPPARA=\"URL\",\"http://192.248.12.9/~s144186/test.php?speed=");
    SendStringSerially(sendS);
    SendStringSerially("&lata=");
    SendStringSerially(sendLa);
    SendStringSerially("&longa=");
    SendStringSerially(sendLo);
    SendStringSerially("\"");

    SendByteSerially((char) 13);
    __delay_ms(2000);

    SendStringSerially("AT+HTTPACTION=0");
    SendByteSerially((char) 13);
    __delay_ms(10000);

    LCDClear();
    LCDWriteStringXY(0, 0, "Sending Over");
    __delay_ms(1000);

}

//void callGSM(){
//    USARTInit(9600);
//    __delay_ms(2000);
//    SendStringSerially("ATD94775860941;");
//
//
//}

void POSITION() {
    //  USARTInit(9600);

    char lat[9], latNS;
    char longi[10], longiEW;


    int count = 0, i = 0;
    // LCDClear();
    while (i < 32900) {
        i++;
        if (USARTReadData() == '$') {
            __delay_ms(k);
            if (USARTReadData() == 'G') {
                __delay_ms(k);
                if (USARTReadData() == 'P') {
                    __delay_ms(k);
                    if (USARTReadData() == 'G') {
                        __delay_ms(k);
                        if (USARTReadData() == 'G') {
                            __delay_ms(k);
                            if (USARTReadData() == 'A') {
                                __delay_ms(k);

                                // __delay_ms(k);

                                for (int i = 0; i < 70; i++) {
                                    __delay_ms(k);
                                    if (USARTReadData() == ',') {
                                        ++count;
                                        if (count == 2) {
                                            for (int l = 0; l < 9; l++) {
                                                __delay_ms(k);
                                                lat[l] = USARTReadData();

                                            }
                                        }
                                        if (count == 3) {
                                            __delay_ms(k);
                                            latNS = USARTReadData();

                                        }
                                        if (count == 4) {
                                            for (int l = 0; l < 10; l++) {
                                                __delay_ms(k);
                                                longi[l] = USARTReadData();

                                            }

                                        }
                                        if (count == 5) {
                                            __delay_ms(k);
                                            longiEW = USARTReadData();

                                        }
                                    }


                                }
                            }
                        }

                    }
                }
            }



        } else {
            continue;

        }
        count = 0;
        LCDWriteStringXY(0, 2, "Lat :");
        LCDWriteStringXY(0, 3, "Lon :");
        LCDWriteCharXY(17, 2, latNS);
        LCDWriteCharXY(17, 3, longiEW);

        for (int l = 0; l < 9; l++) {
            LCDWriteCharXY(l + 5, 2, lat[l]);
        }
        for (int l = 0; l < 9; l++) {
            LCDWriteCharXY(l + 5, 3, longi[l]);
        }
        // __delay_ms(100);

        if ((i % 100) == 0) {
            __delay_ms(2000);
            for (int i = 0; i < 10; i++) {
                if (i < 9)
                    sendLa[i] = lat[i];
                sendLo[i] = longi[i];

            }
            //  fireIndiSend();
            sendByGPRS();
            break;
        }

    }
}

void SPEED() {
    USARTInit(9600);


    int count1 = 0, i = 0;
    char kmph[7];
    char knot[7];
    for (int i = 0; i < 7; i++) {
        kmph[i] = '0';
        knot[i] = '0';
    }
    LCDClear();
    while (i < 32900) {
        i++;
        if (USARTReadData() == '$') {
            __delay_ms(k);
            if (USARTReadData() == 'G') {
                __delay_ms(k);
                if (USARTReadData() == 'P') {
                    __delay_ms(k);
                    if (USARTReadData() == 'V') {
                        __delay_ms(k);
                        if (USARTReadData() == 'T') {
                            __delay_ms(k);
                            if (USARTReadData() == 'G') {
                                __delay_ms(k);

                                // __delay_ms(k);

                                for (int i = 0; i < 70; i++) {
                                    __delay_ms(k);
                                    if (USARTReadData() == ',') {
                                        ++count1;
                                        if (count1 == 5) {
                                            for (int l = 0; l < 7; l++) {
                                                __delay_ms(k);
                                                knot[l] = USARTReadData();

                                            }
                                        }

                                        if (count1 == 6) {
                                            for (int l = 0; l < 7; l++) {
                                                __delay_ms(k);
                                                kmph[l] = USARTReadData();

                                            }

                                        }

                                    }


                                }
                            }
                        }

                    }
                }
            }



        } else {
            continue;

        }
        count1 = 0;
        // LCDClear();

        //        int spd;
        //        for (int l = 0;kmph[l]!=','&&l<7; l++) {
        //            if(kmph[l]=='.')
        //            {
        //                spd=(kmph[l-1]-48)+(kmph[l-2]-48)*10+(kmph[l-3]-48)*100;
        //            }
        //
        //        }
        //        if(spd>1)
        //        {
        //            LCDWriteCharXY(14,0,spd+48)
        //           // __delay_ms(2000);
        //        }

        // sendByGPRS(kmph);
        // LCDClear();

        LCDClear();
        LCDWriteStringXY(0, 0, "Kmph:");
        LCDWriteStringXY(0, 1, "Knot:");
        for (int l = 0; kmph[l] != ',' && l < 7; l++) {
            LCDWriteCharXY(l + 5, 0, kmph[l]);
        }
        for (int l = 0; knot[l] != ',' && l < 7; l++) {
            LCDWriteCharXY(l + 5, 1, knot[l]);
        }
        for (int l = 0; l < 7; l++) {
            if (kmph[l] == '.' && (l > 0))
                co = l;
            //  zo=1;
        }

        if (co == 2) {
            checkSpd = ((kmph[co - 2] - 48)+(kmph[co - 1] - 48)*10);
            if (checkSpd > 30) {
                warNo++;
                __delay_ms(1000);
                LCDClear();
                LCDWriteStringXY(0, 0, "OOPS!");
                LCDWriteStringXY(1, 1, "What's the hurry!");
                LCDWriteStringXY(2, 2, "Let's slow down");
                LCDWriteStringXY(2, 3, "Warning No. :");
                LCDWriteIntXY(15, 3, warNo, 1);
                if (warNo > 4) {
                    SendStringSerially("AT+CMGF=1");
                    SendByteSerially((char) 13);
                    __delay_ms(1000);
                    SendStringSerially("AT+CMGS=\"+94712419618\"\r");
                    __delay_ms(1000);
                    SendStringSerially("High Speeed indicated in Device_01");
                    //SendByteSerially('\032');
                    // EmeNoInput();
                    SendByteSerially((char) 26);
                    //  SendByteSerially(13);
                    __delay_ms(1000);
                }
                //  LCDWriteIntXY(11,0,checkSpd,1);
                __delay_ms(1000);
                break;
            }
        }
        if (co == 1) {
            if (checkSpd < 5) {
                LCDClear();
                LCDWriteStringXY(0, 0, "You are too slow");
                LCDWriteStringXY(0, 1, "Having Trouble?");
                LCDWriteStringXY(0, 2, "Wait for Emergency");
                LCDWriteStringXY(0, 3, "Units");
                SendStringSerially("AT+CMGF=1");
                SendByteSerially((char) 13);
                __delay_ms(1000);
                SendStringSerially("AT+CMGS=\"+94712419618\"\r");
                __delay_ms(1000);
                SendStringSerially("Vehicle Stop indicated in Device_01");
                //SendByteSerially('\032');
                // EmeNoInput();
                SendByteSerially((char) 26);
                //  SendByteSerially(13);
                __delay_ms(1000);
                break;

            }
        }
        // __delay_ms(2000);
        if ((i % 100) == 0) {
            __delay_ms(2000);
            for (int i = 0; i < 7; i++) {
                sendS[i] = kmph[i];
            }
            //  sendByGPRS();
            fireIndiSend();
            POSITION();

        }

        //  LCDClear();
        //  __delay_ms(1000);
    }


}

void buzzerTone() {
    TRISB6 = 0;
    for (int i = 0; i < 3; i++) {
        RB6 = 1;
        __delay_ms(100);
        RB6 = 0;
        __delay_ms(100);
    }
}

void ADC_Init() {
    ADCON0 = 0x81; //Turn ON ADC and Clock Selection
    ADCON1 = 0x00; //All pins as Analog Input and setting Reference Voltages
}

unsigned int ADC_Read(unsigned char channel) {
    if (channel > 7) //Channel range is 0 ~ 7
        return 0;

    ADCON0 &= 0xC5; //Clearing channel selection bits
    ADCON0 |= channel << 3; //Setting channel selection bits
    __delay_ms(2); //Acquisition time to charge hold capacitor
    GO_nDONE = 1; //Initializes A/D conversion
    while (GO_nDONE); //Waiting for conversion to complete
    return ((ADRESH << 8) + ADRESL); //Return result
}

void fireIndiSend() {
    unsigned int a;
    TRISA = 0xFF;
    ADC_Init();
    do {
        a = ADC_Read(0);
        //  LCDWriteIntXY(0,0,a,10);
        __delay_ms(100); //Read Analog Channel 0
        if (a > 8000) {

            RB6 = 1;
            __delay_ms(2000);
            RB6 = 0;
            // __delay_ms(2000);
            LCDClear();
            LCDWriteStringXY(2, 0, "Fire Indicated!!");
            __delay_ms(100);
            LCDWriteStringXY(0, 1, "Sending Message");
            LCDWriteStringXY(0, 2, "To Emergency");
            LCDWriteStringXY(0, 3, "Units");

            SendStringSerially("AT+CMGF=1");
            SendByteSerially((char) 13);
            __delay_ms(1000);
            SendStringSerially("AT+CMGS=\"+94712419618\"\r");
            __delay_ms(1000);
            SendStringSerially("Fire Indicated in Device_01");
            //SendByteSerially('\032');
            // EmeNoInput();
            SendByteSerially((char) 26);
            //  SendByteSerially(13);
            __delay_ms(1000);
            LCDWriteStringXY(0, 0, "Message Sent");
            LCDWriteStringXY(0, 1, "Wait For Assistance");
            LCDWriteStringXY(0, 2, "Please Stay Out");
            LCDWriteStringXY(0, 3, "From Vehicle");
            LCDClear();
            __delay_ms(2000);

            break;

        } else {
            break;
        }
        //Delay
    } while (1);
}

void main() {
    //  callGSM();

    //  sendByGPRS();
    // USARTInit(9600);

    LCDInit(LS_NONE);
    __delay_ms(100);
    LCDClear();
    buzzerTone();
    LCDWriteStringXY(6, 0, "Welcome");
    LCDWriteStringXY(9, 1, "To");
    LCDWriteStringXY(5, 2, "Expressway");
    LCDWriteStringXY(5, 3, "Life Saver");
    __delay_ms(3000);
    LCDClear();
    LCDWriteStringXY(2, 2, "Turning GSM ON");
    TRISB0 = 0;
    RB0 = 1;
    __delay_ms(2000);
    RB0 = 0;
    LCDClear();
    LCDWriteStringXY(4, 2, "GSM Running");
    __delay_ms(500);
    LCDClear();
    LCDWriteStringXY(0, 0, "1.Hope you are not");
    LCDWriteStringXY(0, 1, "drunk");
    LCDWriteStringXY(0, 2, "2.Don't forget seat");
    LCDWriteStringXY(0, 3, "belts");
    __delay_ms(3000);
    LCDClear();
    LCDWriteStringXY(0, 0, "Sleepy?");
    buzzerTone();
    __delay_ms(100);
    LCDWriteStringXY(0, 1, "Let's listen to");
    LCDWriteStringXY(0, 2, "the Radio");
    __delay_ms(3000);
    LCDClear();
    LCDWriteStringXY(0, 0, "Have a safe drive");
    LCDWriteStringXY(0, 1, ":)");
    LCDWriteStringXY(0, 2, "ELS will take care");
    LCDWriteStringXY(0, 3, "of you");
    __delay_ms(3000);
    for (int i = 30; i > 0; i--) {
        LCDClear();
        LCDWriteStringXY(1, 0, "Device will start");
        LCDWriteStringXY(7, 1, "in ");
        LCDWriteIntXY(10, 1, i, 2);
        LCDWriteStringXY(6, 2, "seconds");
        __delay_ms(1000);
        if (i == 1) {
            buzzerTone();
        }
    }
    //  Initialize USART with baud rate 9600
    LCDClear();


    //Analog pins as Input
    //Port B as Output
    //    TRISB7=1;
    //    TRISA2=0;


    //    while(1){
    //
    //     if(RB7==1){
    //     RA2=1;
    //            __delay_ms(3000);
    //           RA2=0;
    //        __delay_ms(3000);
    //
    //     }
    //    }


    while (1) {


        //        POSITION();
        //        __delay_ms(1000);

        SPEED();
        // POSITION();
        __delay_ms(1000);





        //  LCDClear();

    }





    //    for(int i=0;i<96;i++)
    //    {
    //    temp[i]=USARTReadData();
    //    }
    //    for(int i=0;i<96;i++)
    //    {
    ////        if(temp[i]=='$'&&temp[i+1]=='G'&&temp[i+2]=='P'&&temp[i+3]=='V'&&temp[i+4]=='T'&&temp[i+5]=='G')
    ////        {
    ////            for(int j=0;j<6;j++)
    ////            {
    ////                LCDWriteStringXY(0,1,"Knots:");
    ////                LCDWriteCharXY(j+5,1,temp[i+j+1]);
    ////            }
    ////            for(int j=0;j<6;j++)
    ////            {
    ////                LCDWriteStringXY(0,0,"Kmph:");
    ////                LCDWriteCharXY(j+5,0,temp[i+j+21]);
    ////            }
    ////         if(temp[i]=='$'&&temp[i+1]=='G'&&temp[i+2]=='P'&&temp[i+3]=='V'&&temp[i+4]=='T'&&temp[i+5]=='G')
    ////         {
    ////              for(int j=0;j<16;j++)
    ////              {
    ////                   LCDWriteCharXY(j,0,temp[i+j+13]);
    ////              }
    ////
    ////         }
    //            if(temp[i]=='$'&&temp[i+1]=='G'&&temp[i+2]=='P'&&temp[i+3]=='G'&&temp[i+4]=='G'&&temp[i+5]=='A')
    //            {
    //
    //            for(int j=0;j<8;j++)
    //            {
    //
    //                LCDWriteCharXY(j,0,temp[i+j+7]);
    //            }
    //
    //            }
    //
    //
    //
    //
    //
    //    }
    //    __delay_ms(65);
    //    }












}