#ifndef UART_H
#define UART_H

#define F_CPU 16000000UL
#define BAUD 1000000

// https://cache.amobbs.com/bbs_upload782111/files_22/ourdev_508497.html

#include <avr/io.h>
#include "my_utils.h"
#include <util/setbaud.h>
#include "stdio.h"
#include "mcp2515.h"

void uart_init(void);
void uart_putchar(uint8_t u8Data);
void uart_putchar_ptr(uint8_t *ptr, uint8_t size);
void uart_print_packet(struct CAN_FRAME *frame);

#endif //UART_H
