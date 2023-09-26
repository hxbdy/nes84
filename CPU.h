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

#define CPU_MEMMAP_WRAM_SIZE            0x0800
#define CPU_MEMMAP_WRAM_START           0x0000
#define CPU_MEMMAP_WRAM_END             (CPU_MEMMAP_WRAM_START + CPU_MEMMAP_WRAM_SIZE)

#define CPU_MEMMAP_WRAM_MIRROR_START    0x0800
#define CPU_MEMMAP_WRAM_MIRROR_END      0x2000

#define CPU_MEMMAP_PPU_REG_SIZE         0x0008
#define CPU_MEMMAP_PPU_REG_START        0x2000
#define CPU_MEMMAP_PPU_REG_END          (CPU_MEMMAP_PPU_REG_START + CPU_MEMMAP_PPU_REG_SIZE)

#define CPU_MEMMAP_PPU_REG_MIRROR_START 0x2008
#define CPU_MEMMAP_PPU_REG_MIRROR_END   0x4000

#define CPU_MEMMAP_APU_IO_SIZE          0x0020
#define CPU_MEMMAP_APU_IO_START         0x4000
#define CPU_MEMMAP_APU_IO_END           (CPU_MEMMAP_APU_IO_START + CPU_MEMMAP_APU_IO_SIZE)

#define CPU_MEMMAP_EXPAND_ROM_SIZE      0x1FE0
#define CPU_MEMMAP_EXPAND_ROM_START     0x4020
#define CPU_MEMMAP_EXPAND_ROM_END       (CPU_MEMMAP_EXPAND_ROM_START + CPU_MEMMAP_EXPAND_ROM_SIZE)

#define CPU_MEMMAP_EXPAND_RAM_SIZE      0x2000
#define CPU_MEMMAP_EXPAND_RAM_START     0x6000
#define CPU_MEMMAP_EXPAND_RAM_END       (CPU_MEMMAP_EXPAND_RAM_START + CPU_MEMMAP_EXPAND_RAM_SIZE)

#define CPU_MEMMAP_PRG_ROM1_SIZE        0x4000
#define CPU_MEMMAP_PRG_ROM1_START       0x8000
#define CPU_MEMMAP_PRG_ROM1_END         (CPU_MEMMAP_PRG_ROM1_START + CPU_MEMMAP_PRG_ROM1_SIZE)

#define CPU_MEMMAP_PRG_ROM2_START       0xC000
#define CPU_MEMMAP_PRG_ROM2_END         0xFFFF

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
