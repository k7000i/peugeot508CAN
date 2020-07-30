#define __SFR_OFFSET 0          // Use 0 for the I/O register offset
#include <avr/io.h>             // Defines I/O port aliases

//https://www.microchip.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_asmconst.html
//http://msoe.us/taylor/tutorial/ce2810/candasm

.global asmfunc_calledfrom_c    ; Makes asmfunc_calledfrom_c visible in other source files

.section .text                  ; Defines a code section

asmfunc_calledfrom_c:           ; Start of asmfunc_calledfrom_c subroutine
    out  PORTB, r24             ; Send value passed to asmfunc_calledfrom_c to PORTB
    ret