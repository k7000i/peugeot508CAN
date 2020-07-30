#define F_CPU 16000000UL

#include <avr/io.h>
//#include "avr/interrupt.h"
#include <util/delay.h>
#include "my_utils.h"
#include "mcp2515.h"
//#include "uart.h"


/*void led_setup(void){
    //DDRB |= (1 << DDB0);
    DDRB |= 0x20;
}

void led_on(void){
    // PORTB0 is in sink config
    // Meaning when pin is ground
    // LED will turn on
    //PORTB = (0 << PORTB0);
    PORTB |= 0x20;
}

void led_off(void){
    // sink config
    //PORTB = (1 << PORTB0);
    PORTB ^= 0x20;
}*/
//extern void asmfunc_calledfrom_c(uint8_t val);

/*
void delay_ms(unsigned int delaytime){
    int i = 0;
    for(i=0; i<=delaytime; i++){
        _delay_ms(1);
    }
} */

// *0xFF = 1 -> 0xFF memória hely értéke SET 1
// &0xFF -> 0xFF memória hely értéke GET

// & reference return pointer address
// * dereference return content of an address
// https://www.youtube.com/watch?v=47IS8VtAM9E
// https://barrgroup.com/Embedded-Systems/How-To/C-Volatile-Keyword
// https://www.avrfreaks.net/forum/tut-c-bit-manipulation-aka-programming-101?page=all
// http://www.engbedded.com/fusecalc/

//Assembly include
// https://github.com/DarkSector/AVR/blob/master/asm/include/m328Pdef.inc
/*ISR(INT0_vect) {
    readMessage(&canmsg2);
}*/
void sendStartMessageToCluster(uint8_t msg0, uint8_t msg1) {
    struct CAN_FRAME can_read_msg;
    while(1) {
        readMessage(&can_read_msg);
        if (can_read_msg.can_id == 0x1A1) {
            break;
        }
    }
    struct CAN_FRAME canmsg;
    canmsg.can_id = 0x1A1;
    canmsg.can_extended = 0;
    canmsg.can_dlc = 0x08;
    canmsg.can_data[0] = msg0;
    canmsg.can_data[1] = msg1;
    canmsg.can_data[2] = 0xA4;
    canmsg.can_data[3] = 0x00;
    canmsg.can_data[4] = 0x00;
    canmsg.can_data[5] = 0x00;
    canmsg.can_data[6] = 0x00;
    canmsg.can_data[7] = 0x00;
    sendMessage(&canmsg);

}
/*
void clearCluster() {
    struct CAN_FRAME can_read_msg;
    while(1) {
        readMessage(&can_read_msg);
        if (can_read_msg.can_id == 0x1A1) {
            break;
        }
    }
    struct CAN_FRAME canmsg;
    canmsg.can_id = 0x1A1;
    canmsg.can_extended = 0;
    canmsg.can_dlc = 0x7F;
    canmsg.can_data[0] = 0xFF;
    canmsg.can_data[1] = 0x00;
    canmsg.can_data[2] = 0x00;
    canmsg.can_data[3] = 0x00;
    canmsg.can_data[4] = 0x00;
    canmsg.can_data[5] = 0x00;
    canmsg.can_data[6] = 0x00;
    canmsg.can_data[7] = 0x00;
    sendMessage(&canmsg);

}


void test() {
    struct CAN_FRAME canmsg;
    //canmsg.can_id = 0x1A1;
    // 00 B0 01 06 00 40 04 00
    canmsg.can_id = 0x128;
    canmsg.can_extended = 0;
    canmsg.can_dlc = 0x08;
    canmsg.can_data[0] = 0x00;
    canmsg.can_data[1] = 0xB1;
    canmsg.can_data[2] = 0x01;
    canmsg.can_data[3] = 0x06;
    canmsg.can_data[4] = 0x00;
    canmsg.can_data[5] = 0x40;
    canmsg.can_data[6] = 0x04;
    canmsg.can_data[7] = 0x00;

    for (uint8_t i=0; i< 5;i++) {
        uint8_t t = 0;
        while (t < 255) {
            sendMessage(&canmsg);
            t += 1;
            delay_ms(1);
        }
    }
}

void sendEndMessageToCluster(uint8_t msg_part_1, uint8_t msg_part_2) {
    struct CAN_FRAME canmsg;
    canmsg.can_id = 0x1A1;
    canmsg.can_extended = 0;
    canmsg.can_dlc = 0x08;
    canmsg.can_data[0] = msg_part_1;
    canmsg.can_data[1] = msg_part_2;
    canmsg.can_data[2] = 0xA4; // 0xA1 big screen red border, 0xA4 standard message
    canmsg.can_data[3] = 0x00;
    canmsg.can_data[4] = 0x00;
    canmsg.can_data[5] = 0x00;
    canmsg.can_data[6] = 0x00;
    canmsg.can_data[7] = 0x00;

    for (uint8_t i=0; i< 5;i++) {
        uint8_t t = 0;
        while (t < 255) {
            sendMessage(&canmsg);
            t += 1;
            delay_ms(1);
        }
    }

    delay_ms(3000);
    canmsg.can_data[0] = 0x7F;
    canmsg.can_data[1] = 0xFF;
    canmsg.can_data[2] = 0x00;
    sendMessage(&canmsg);
    delay_ms(1);
    sendMessage(&canmsg);
    delay_ms(1);
    sendMessage(&canmsg);
    delay_ms(1);
    sendMessage(&canmsg);
    delay_ms(1);

}
*/

int main(void) {

    // enable interrupt
    //PORTD |= 1<<PD2;   // enable PORTD.2 pin pull up resistor
    //EIMSK |= 1<<INT0;  // enable external interrupt 0
    //EICRA |= 1<<ISC01;
    //sei();


    // WARNING! be sure the correct settings otherwise you could damage your car! Never connect to Engine critical canbus!!!

    //uart_init();
    SPI_init(SPI_4MHz);
    reset();
    //setFilter(0x2AD);
    setCanSpeed(CAN_125kbps); //Peugeot can diag 500 kpbs
    setCanMode(CANMODE_NORMAL);

    struct CAN_FRAME canMsg1;
    struct CAN_FRAME canmsg2;

    //sendStartMessageToCluster();
    //return 0;
    /*canMsg1.can_id = 0x50A;
    canMsg1.can_extended = 0;
    canMsg1.can_dlc = 0x08;
    for (int i=0; i<canMsg1.can_dlc; i++) {
        canMsg1.can_data[i] = 7 * i;
    }*/
    /* CAN MESSAGE FILTER SETUP */
    //sendEndMessageToCluster(0x00);
    //delay_ms(100);
    //sendEndMessageToCluster(0x00);
    //return 0;

    // 81 2E sport mode
    // 82 51 confort mode
    //for (int i=46; i<60; i++) {
    //for (int j=0; j< 15;j++) {
    //    sendStartMessageToCluster((uint8_t)0x00);
    //    delay_ms(1);
    //}
    //    clearCluster();
    //}

    //for (int i=0; i<10; i++) {
    //    clearCluster();
    //}

    //Uncomment this if you want to see the can messages with serial monitor
    while (1) {
        readMessage(&canmsg2);
    }

    uint8_t is_active = 0;
    while (1) {
        readMessage(&canmsg2);
        if (canmsg2.can_id == 0x2AD) {
            if ( (canmsg2.can_data[0] & 0x20) == 0x20 && is_active == 0 ) {
                is_active = 1;
                for (int j=0; j< 15;j++) {
                    sendStartMessageToCluster((uint8_t)0x81, (uint8_t)0x2E);
                    //delay_ms(1);
                    _delay_ms(1);
                }
            } else if( (canmsg2.can_data[0] & 0x20) == 0x00 && is_active == 1) {
                is_active = 0;
                for (int j=0; j< 15;j++) {
                    sendStartMessageToCluster((uint8_t)0x82, (uint8_t)0x33);
                    //delay_ms(1);
                    _delay_ms(1);
                }
            }
        }

    }
    return 0;

    canMsg1.can_id = 0x50A;
    canMsg1.can_extended = 0;
    canMsg1.can_dlc = 0x08;
    for (int i=0; i<canMsg1.can_dlc; i++) {
        canMsg1.can_data[i] = 7 * i;
    }

    uint8_t i = 0;
    while (1) {
        canMsg1.can_data[1] = i;
        canMsg1.can_data[2] = i;
        sendMessage(&canMsg1);
        delay_ms(500);
        //readMessage(&canmsg2);
        i += 1;
    }


    return 0;


    return 0;
    delay_ms(200);
    while (1) {
        delay_ms(200);
    }


    return 0;

    // setup the leds
    //led_setup();
    //bit_set(PRR, BIT(PRSPI));
    /*uart_init();
    uint8_t asd[] = "Niceasdasda \n";
    int i;
    while (1) {
        for (i=0; i< sizeof(asd); i++) {
            uart_putchar(asd[i]);
            delay_ms(50);
        }
    }*/
    //if (PRR & _BV(PRSPI)) {
    if (bit_get(PRR , BIT(PRSPI))) {
        while(1) {
            delay_ms(1500);
            bit_flip(PORTB, BIT(PORTB5));
            //led_on();
            delay_ms(200);
            //led_off();
            //
        }
    }


    return 0;
}
