#if !defined _CPU_H_
#define _CPU_H_

#include <stdint.h>

#define STATUS_CARRY     0x01
#define STATUS_ZERO      0x02
#define STATUS_IRQ       0x04
#define STATUS_DEC       0x08
#define STATUS_BRK       0x10
#define STATUS_RCV       0x20
#define STATUS_OVERFLOW  0x40
#define STATUS_NEG       0x80

// [N, V, R, B, D, I, Z, C]
typedef struct{
    uint8_t car : 1; // LSB
    uint8_t zer : 1;
    uint8_t irq : 1;
    uint8_t dec : 1;
    uint8_t brk : 1;
    uint8_t rcv : 1;
    uint8_t ovf : 1;
    uint8_t neg : 1; // MSB
}StatusReg;

typedef union{
    uint8_t   statusByte;
    StatusReg statusBit; 
}Status;

typedef struct {
    uint16_t mem[0xFFFF];
    uint16_t pc;
    uint16_t cycle;
    Status status;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint16_t S;
    //uint8_t P; // ステータスレジスタ
}Nes;

#endif
