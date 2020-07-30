

#include <string.h>
#include "mcp2515.h"
#include "uart.h"

//void SPI_TransferBegin() { __asm__ __volatile__("cbi  0x05, 2"); } // CS line LOW
//void SPI_TransferEnd() { __asm__ __volatile__("sbi  0x05, 2"); } // CS line HIGH

void SPI_TransferBegin() { bit_clear(PORTB, BIT(PORTB2)); }
void SPI_TransferEnd() { bit_set(PORTB, BIT(PORTB2)); }

void SPI_init(SPI_SPEED speed){
    /* Set MOSI and SCK output, all others input */
    // D13 SCK D12 MISO D11 MOSI D10 CS
    DDRB = (1<<DDB2)|(1<<DDB3)|(1<<DDB5);
    bit_set(PORTB, BIT(PORTB2)); // safety

    /* Enable SPI, Master, set clock rate fck/16 */
    switch (speed) {
        case SPI_250kHz:
            SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); //250 kHz
            break;
        case SPI_1MHz:
            SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); // 1 Mhz
            break;
        case SPI_4MHz:
            SPCR = (1<<SPE)|(1<<MSTR);
            break;
        case SPI_8MHz:
            SPCR = (1<<SPE)|(1<<MSTR);
            SPSR |= (1<<SPI2X);
            break;
    }
}

void SPI_TransferReceive(uint8_t txByte, uint8_t *rxByte){
    // Assembly speed
    //register uint8_t myR1 asm("r14");
    //myR1 = 0xA1;
    //__asm__ __volatile__("out 0x2e, r14");

    //__asm__ __volatile__("cbi  0x05, 2"); // CS line LOW
    SPDR = txByte; //Start transmission
    while(!(SPSR & (1<<SPIF))); //Wait for byte transmission complete
    *rxByte = SPDR;

    //__asm__ __volatile__("sbi  0x05, 2"); // CS line HIGH
}
void SPI_Transfer(uint8_t txByte){

    SPDR = txByte;
    while(!(SPSR & (1<<SPIF))); //Wait for transmission complete
}

// MCP2515 commands
void setRegister(const uint8_t reg, const uint8_t val) {
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_WRITE);
    SPI_Transfer(reg);
    SPI_Transfer(val);
    SPI_TransferEnd();
}

void setRegisters(const uint8_t reg, const uint8_t val[], const uint8_t size) {
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_WRITE);
    SPI_Transfer(reg);

    for (uint8_t i = 0; i < size; i++) {
        SPI_Transfer(val[i]);
    }

    SPI_TransferEnd();
}

uint8_t readRegister(const uint8_t reg) {
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_READ);
    SPI_Transfer(reg);
    uint8_t r;
    SPI_TransferReceive(0x00, &r);
    SPI_TransferEnd();
    return r;
}

void readRegisters(const uint8_t reg, uint8_t val[], const uint8_t size) {
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_READ);
    SPI_Transfer(reg);
    for (uint8_t i = 0; i < size; i++) {
        SPI_TransferReceive(0x00, &val[i]);
    }
    SPI_TransferEnd();

}

uint8_t readStatusRegister() {
    uint8_t status;
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_READ_STATUS);
    SPI_TransferReceive(0x00, &status);
    SPI_TransferEnd();
    return status;
}

void modifyRegister(const uint8_t reg, const uint8_t mask, const uint8_t val) {
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_BITMOD);
    SPI_Transfer(reg);
    SPI_Transfer(mask);
    SPI_Transfer(val);
    SPI_TransferEnd();
}

ERROR reset() {
    SPI_TransferBegin();
    SPI_Transfer(INSTRUCTION_RESET);
    SPI_TransferEnd();

    uint8_t zero[14];
    memset(zero,0,14);
    setRegisters(MCP_TXB0CTRL, zero, 14);
    setRegisters(MCP_TXB1CTRL, zero, 14);
    setRegisters(MCP_TXB2CTRL, zero, 14);

    setRegister(MCP_RXB0CTRL, 0x60); //0x60 means accept all message, default -> 0
    setRegister(MCP_RXB1CTRL, 0);

    setRegister(MCP_CANINTE, 0x00);
    setRegister(MCP_CANINTF, 0x00); // clear interrupts

    return ERROR_OK;
}

ERROR setCanSpeed(CANSPEED speed) {

    // We have 8 Mhz clock
    // https://www.kvaser.com/support/calculators/bit-timing-calculator/
    uint8_t cfg1, cfg2, cfg3;
    switch (speed) {
        case CAN_1000kbps:
            cfg1 = 0x00;
            cfg2 = 0x80;
            cfg3 = 0x01;
            break;
        case CAN_500kbps:
            cfg1 = 0x00; //0x01
            cfg2 = 0x91; //0x80
            cfg3 = 0x01; //0x80
            break;
        case CAN_250kbps:
            cfg1 = 0x00;
            cfg2 = 0xB1;
            cfg3 = 0x85;
            break;
        case CAN_200kbps:
            cfg1 = 0x00;
            cfg2 = 0xB4;
            cfg3 = 0x86;
            break;
        case CAN_125kbps:
            cfg1 = 0x01;
            cfg2 = 0xB1;
            cfg3 = 0x85;
            break;
        case CAN_100kbps:
            cfg1 = 0x01;
            cfg2 = 0xB4;
            cfg3 = 0x86;
            break;
    }
    setRegister(MCP_CNF1, cfg1);
    setRegister(MCP_CNF2, cfg2);
    setRegister(MCP_CNF3, cfg3);

    return ERROR_OK;
}

ERROR setCanMode(CANMODE mode) {

    uint8_t bits;
    switch (mode) {
        case CANMODE_CONFIG:
            bits = 0x80;
            break;
        case CANMODE_LISTEN_ONLY:
            bits = 0x60;
            break;
        case CANMODE_SLEEP:
            bits = 0x20;
            break;
        case CANMODE_LOOPBACK:
            bits = 0x40;
            break;
        case CANMODE_NORMAL:
            bits = 0x00;
            break;
    }


    modifyRegister(MCP_CANCTRL, 0xE0, bits);
    modifyRegister(MCP_CANCTRL, 0x08, 0x08); // one shot mode

    uint8_t reg_state = 0xFF; // Dummy state

    while ( (reg_state & 0xE0) != bits) {
        reg_state = readRegister(MCP_CANSTAT);
    }

    return ERROR_OK;

}

ERROR sendMessage(const struct CAN_FRAME *frame) {

    modifyRegister(MCP_TXB0CTRL, 0x08, 0x00);

    /*
    if (frame->can_extended) {
        // Todo this part unreliable in loopback mode
        setRegister(MCP_TXB0EID8, (uint8_t) (frame->can_id >> 8));
        setRegister(MCP_TXB0EID0, (uint8_t) (frame->can_id & 0xFF));

        uint16_t _temp_id = (uint16_t)(frame->can_id >> 16);
        uint8_t SIDL = (uint8_t) (_temp_id & 0x03) + (uint8_t) ((_temp_id & 0x1C) << 3);
        SIDL |= 0x08; //EXIDE bit
        setRegister(MCP_TXB0SIDL, SIDL);
        setRegister(MCP_TXB0SIDH, (uint8_t) (_temp_id >> 5));

    } else { */
        setRegister(MCP_TXB0SIDH, (uint8_t) (frame->can_id >> 3));
        setRegister(MCP_TXB0SIDL, (uint8_t) ((frame->can_id & 0x07 ) << 5));
        setRegister(MCP_TXB0EID8, 0x00);
        setRegister(MCP_TXB0EID0, 0x00);
    //}

    setRegister(MCP_TXB0DLC, frame->can_dlc);
    setRegisters(MCP_TXB0DATA, frame->can_data, frame->can_dlc);
    modifyRegister(MCP_TXB0CTRL, 0x08, 0x08);

    //Check if message was sent
    uint8_t check_msg_ok = readRegister(MCP_TXB0CTRL);
    while ( (check_msg_ok & 0x08) != 0x00 ) {
        check_msg_ok = readRegister(MCP_TXB0CTRL);
    }

    return ERROR_OK;
}

ERROR readMessage(struct CAN_FRAME *frame) {


    uint8_t status = readStatusRegister();

    /* polling method */
    while ((status & 0x1) != 0x1) {
        status = readStatusRegister();
    }

    uint8_t frame_info[5];
    readRegisters(MCP_RXB0SIDH, frame_info,5);

    uint16_t id = (frame_info[MCP_SIDH] << 3) + (frame_info[MCP_SIDL] >> 5);
    uint8_t dlc = (frame_info[MCP_DLC] & 0x0F);

    // check extended frame
    /*if ((frame_info[1] & 0x08) == 0x08) {
        id = (id << 3) +  (frame_info[MCP_SIDL] & 0x03);
        id = (id << 8) +  (frame_info[MCP_EID8]);
        id = (id << 8) +  (frame_info[MCP_EID0]);

        id |= CAN_EFF_FLAG;
    }*/
    frame->can_id = id;
    frame->can_dlc = dlc;
    readRegisters(MCP_RXB0DATA, frame->can_data, dlc);

    /*
    if (enable_debug == 1) {
        uart_putchar(0x55);
        uart_putchar(0xA8);

        uint8_t *ptr = &frame->can_id;
        for (uint8_t x=2; x-- > 0;) {
            uart_putchar(*(ptr + x));
        }
        //uart_putchar_ptr((uint8_t *)&frame->can_id, 2); // write id todo (2 should be 4 but extended can not working yet)
        uart_putchar(frame->can_dlc);
        uart_putchar_ptr((uint8_t *)&frame->can_data, dlc);
    }*/


    modifyRegister(MCP_CANINTF, 0x01, 0); // clear interrupt

    return ERROR_OK;
}

void setFilter(uint32_t addr) {

    setRegister(MCP_RXF0SIDH, (uint8_t)(addr >> 2));
    setRegister(MCP_RXF0SIDL, (uint8_t)(addr << 5));

    setRegister(MCP_RXM0SIDH, (uint8_t)(addr >> 2));
    setRegister(MCP_RXM0SIDL, (uint8_t)(addr << 5));
}