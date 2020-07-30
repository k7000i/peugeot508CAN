//
// Created by Rácz Attila on 2019-03-12.
//

#ifndef MY_AVR_PROJECT_MCP2515_H
#define MY_AVR_PROJECT_MCP2515_H
#include <avr/io.h>
#include "my_utils.h"

// Commands
#define INSTRUCTION_WRITE       (0x02)
#define INSTRUCTION_READ        (0x03)
#define INSTRUCTION_BITMOD      (0x05)
#define INSTRUCTION_LOAD_TX0    (0x40)
#define INSTRUCTION_LOAD_TX1    (0x42)
#define INSTRUCTION_LOAD_TX2    (0x44)
#define INSTRUCTION_RTS_TX0     (0x81)
#define INSTRUCTION_RTS_TX1     (0x82)
#define INSTRUCTION_RTS_TX2     (0x84)
#define INSTRUCTION_RTS_ALL     (0x87)
#define INSTRUCTION_READ_RX0    (0x90)
#define INSTRUCTION_READ_RX1    (0x94)
#define INSTRUCTION_READ_STATUS (0xA0)
#define INSTRUCTION_RX_STATUS   (0xB0)
#define INSTRUCTION_RESET       (0xC0)

// Registers
#define MCP_RXF0SIDH (0x00)
#define MCP_RXF0SIDL (0x01)
#define MCP_RXF0EID8 (0x02)
#define MCP_RXF0EID0 (0x03)
#define MCP_RXF1SIDH (0x04)
#define MCP_RXF1SIDL (0x05)
#define MCP_RXF1EID8 (0x06)
#define MCP_RXF1EID0 (0x07)
#define MCP_RXF2SIDH (0x08)
#define MCP_RXF2SIDL (0x09)
#define MCP_RXF2EID8 (0x0A)
#define MCP_RXF2EID0 (0x0B)
#define MCP_CANSTAT  (0x0E)
#define MCP_CANCTRL  (0x0F)
#define MCP_RXF3SIDH (0x10)
#define MCP_RXF3SIDL (0x11)
#define MCP_RXF3EID8 (0x12)
#define MCP_RXF3EID0 (0x13)
#define MCP_RXF4SIDH (0x14)
#define MCP_RXF4SIDL (0x15)
#define MCP_RXF4EID8 (0x16)
#define MCP_RXF4EID0 (0x17)
#define MCP_RXF5SIDH (0x18)
#define MCP_RXF5SIDL (0x19)
#define MCP_RXF5EID8 (0x1A)
#define MCP_RXF5EID0 (0x1B)
#define MCP_TEC      (0x1C)
#define MCP_REC      (0x1D)
#define MCP_RXM0SIDH (0x20)
#define MCP_RXM0SIDL (0x21)
#define MCP_RXM0EID8 (0x22)
#define MCP_RXM0EID0 (0x23)
#define MCP_RXM1SIDH (0x24)
#define MCP_RXM1SIDL (0x25)
#define MCP_RXM1EID8 (0x26)
#define MCP_RXM1EID0 (0x27)
#define MCP_CNF3     (0x28)
#define MCP_CNF2     (0x29)
#define MCP_CNF1     (0x2A)
#define MCP_CANINTE  (0x2B)
#define MCP_CANINTF  (0x2C)
#define MCP_EFLG     (0x2D)
#define MCP_TXB0CTRL (0x30)
#define MCP_TXB0SIDH (0x31)
#define MCP_TXB0SIDL (0x32)
#define MCP_TXB0EID8 (0x33)
#define MCP_TXB0EID0 (0x34)
#define MCP_TXB0DLC  (0x35)
#define MCP_TXB0DATA (0x36)
#define MCP_TXB1CTRL (0x40)
#define MCP_TXB1SIDH (0x41)
#define MCP_TXB1SIDL (0x42)
#define MCP_TXB1EID8 (0x43)
#define MCP_TXB1EID0 (0x44)
#define MCP_TXB1DLC  (0x45)
#define MCP_TXB1DATA (0x46)
#define MCP_TXB2CTRL (0x50)
#define MCP_TXB2SIDH (0x51)
#define MCP_TXB2SIDL (0x52)
#define MCP_TXB2EID8 (0x53)
#define MCP_TXB2EID0 (0x54)
#define MCP_TXB2DLC  (0x55)
#define MCP_TXB2DATA (0x56)
#define MCP_RXB0CTRL (0x60)
#define MCP_RXB0SIDH (0x61)
#define MCP_RXB0SIDL (0x62)
#define MCP_RXB0EID8 (0x63)
#define MCP_RXB0EID0 (0x64)
#define MCP_RXB0DLC  (0x65)
#define MCP_RXB0DATA (0x66)
#define MCP_RXB1CTRL (0x70)
#define MCP_RXB1SIDH (0x71)
#define MCP_RXB1SIDL (0x72)
#define MCP_RXB1EID8 (0x73)
#define MCP_RXB1EID0 (0x74)
#define MCP_RXB1DLC  (0x75)
#define MCP_RXB1DATA (0x76)

#define CAN_MAX_DLEN 8

static const uint8_t MCP_SIDH = 0;
static const uint8_t MCP_SIDL = 1;
static const uint8_t MCP_EID8 = 2;
static const uint8_t MCP_EID0 = 3;
static const uint8_t MCP_DLC  = 4;
static const uint8_t MCP_DATA = 5;

static const enable_debug = 0;

#define CAN_EFF_FLAG 0x80000000UL

typedef enum SPI_SPEED {
    SPI_250kHz,
    SPI_1MHz,
    SPI_4MHz,
    SPI_8MHz
} SPI_SPEED;

typedef enum CANSPEED {
    CAN_1000kbps,
    CAN_500kbps,
    CAN_250kbps,
    CAN_200kbps,
    CAN_125kbps,
    CAN_100kbps
} CANSPEED;

typedef enum ERROR {
    ERROR_OK        = 0,
    ERROR_FAIL      = 1,
    ERROR_ALLTXBUSY = 2,
    ERROR_FAILINIT  = 3,
    ERROR_FAILTX    = 4,
    ERROR_NOMSG     = 5
} ERROR;

typedef enum CANMODE {
    CANMODE_CONFIG,
    CANMODE_LISTEN_ONLY,
    CANMODE_SLEEP,
    CANMODE_LOOPBACK,
    CANMODE_NORMAL
} CANMODE;

struct CAN_FRAME {
    uint16_t    can_id;
    uint8_t     can_extended;
    uint8_t     can_dlc;
    uint8_t     can_data[CAN_MAX_DLEN];
} CAN_FRAME;

void SPI_init(SPI_SPEED speed);
void SPI_Transfer(uint8_t txByte);
void SPI_TransferBegin();
void SPI_TransferEnd();

uint8_t readRegister(const uint8_t reg);
uint8_t readStatusRegister();
ERROR reset();
ERROR setCanSpeed(CANSPEED speed);
ERROR setCanMode(CANMODE mode);
ERROR sendMessage(const struct CAN_FRAME *frame);
ERROR readMessage(struct CAN_FRAME *frame);
void setFilter(uint32_t addr);

#endif //MY_AVR_PROJECT_MCP2515_H
