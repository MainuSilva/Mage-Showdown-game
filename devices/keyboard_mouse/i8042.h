#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KBD_IRQ 1 /* Keyboard IRQ Line */
#define MOUSE_IRQ 12 /* Mouse IRQ Line */

/* I/O port addresses */

/* I/O Port Addresses */
#define STATUS_REG    0x64       // status register port
#define OUT_BUF       0x60       // output buffer port
#define IN_BUF        0x64
#define IN_BUF_ARG    0x60

/* Status Register Masks */
#define OUT_BUF_FULL	     BIT(0)    // bit mask to check if output buffer is full
#define IN_BUF_FULL	         BIT(1)    // bit mask to check if input buffer is full
#define TRANSMIT_TIME_OUT    BIT(5)    // bit mask, 1 if mouse data, 0 if keyboard data
#define TIME_OUT             BIT(6)    // bit mask to check for timeout error
#define PARITY_ERROR         BIT(7)    // bit mask to check for parity error
#define SIZE 0xE0

/* KBC Commands */
#define WRITE_CMD_BYTE      0x60       //read command byte
#define READ_CMD_BYTE       0x20       //write command byte

/* Mouse Commands */
#define WRITE_MOUSE         0xD4       //write byte to mouse
#define ENABLE_DATA_REPORT  0xF4
#define DISABLE_DATA_REPORT 0xF5
#define STREAM_MODE         0xEA
#define READ_DATA           0xEB
#define REMOTE_MODE         0xF0

/* Scancodes */
#define MAKE_CODE          BIT(7)
#define ENABLE_INT         BIT(0)
#define INCOMPLETE_CODE    0xE0
#define ESC_MAKE           0x01
#define ESC_BREAK          0x81
#define SPACE_MAKE         0x39
#define SPACE_BREAK        0xB9
#define LEFT_ARROW_MAKE    0x4B
#define LEFT_ARROW_BREAK   0xCB
#define RIGHT_ARROW_MAKE   0x4D
#define RIGHT_ARROW_BREAK  0xCD
#define UP_ARROW_MAKE      0x48
#define UP_ARROW_BREAK     0xC8
#define DOWN_ARROW_MAKE    0x50
#define DOWN_ARROW_BREAK   0xD0
#define ENTER_MAKE         0x1D
#define ENTER_BREAK        0x9D
#define W_MAKE             0x11
#define W_BREAK            0x91
#define Q_MAKE             0x10
#define Q_BREAK            0x90
#define S_MAKE             0x1F
#define S_BREAK            0x9F
#define D_MAKE             0x20
#define D_BREAK            0xA0
#define A_MAKE             0x1E
#define A_BREAK            0x9E
#define SPACEBAR           0xB9
#define E                  0x12

/* Mouse Packet */
#define FIRST_PACKET    BIT(3)   // bit mask to check if we are receiving a packet's first byte

/* Packet struct */
#define LB       BIT(0)
#define RB       BIT(1)
#define MB       BIT(2)
#define DELTA_X  BIT(4)
#define DELTA_Y  BIT(5)
#define X_OV     BIT(6)
#define Y_OV     BIT(7)

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC //Second consecutive invalid byte





#define STAT_REG 0x64 /**< @brief Register for status */
#define KBC_CMD_REG 0x64
#define OBF BIT(0)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define WAIT_KBC 20000

#define OUT_BUF 0x60
#define BREAK BIT(7)
#define ESC 0x81
#define IBF BIT(1)

#define READ_CMDB 0x20
#define WRITE_CMBD 0x60

#define MASK_IBF 0xFD

#define INT BIT(0) //Interrupt enable for keyboard in Command Byte

//MOUSE

#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

#define DIS_MOUSE 0xA7
#define WRITE_B_MOUSE 0xD4
#define DIS_DATA_REP 0xF5
#define ENA_DATA_REP 0xF4
#define STREAM_MODE 0xEA
#define REMOTE_MODE 0xF0
#define READ_DATA 0xEB

//Mouse Controller command responses
#define ACK 0xFA   //Acknowledged command or argument
#define NACK 0xFE  //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte

#endif /* _LCOM_I8042_H */
