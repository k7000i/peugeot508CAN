#include "uart.h"


void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    //UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
    UCSR0B = _BV(TXEN0);   /* Enable RX and TX */
}

void uart_putchar(uint8_t u8Data){

    // Wait until last byte has been transmitted
    while((UCSR0A &(1<<UDRE0)) == 0);

    // Transmit data
    UDR0 = u8Data;
}

void uart_putchar_ptr(uint8_t *ptr, uint8_t size) {
    // https://www.microchip.com/forums/m394183.aspx
    for (uint8_t x=0; x<size; x++) {
        uart_putchar(*ptr++);
    }
}

void uart_print_packet(struct CAN_FRAME *frame) {
    uart_putchar(0x55);
    uart_putchar(0xA8);
    uart_putchar_ptr((uint8_t *)&frame->can_id, 2); // write id todo (2 should be 4 but extended can not working yet)
    uart_putchar(frame->can_dlc);
    uart_putchar_ptr((uint8_t *)&frame->can_data, frame->can_dlc);
}