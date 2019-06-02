#ifndef _uart_h
#define _uart_h 1

#include <stdio.h>

void uart_init(int interrupt = 1);
int uart_putchar(char c, FILE * f = NULL);
int uart_getchar(FILE * f = NULL);
void set_receive_interrupt_handler(void (*handler)(unsigned char));

#endif /* defined _uart_h */
