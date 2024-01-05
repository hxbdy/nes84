#include <stdio.h>
#include <stdint.h>

#include "common.h"
#include "CPU.h"
#include "PPU.h"
#include "render.h"
#include "instruction.h"

#define INES_HEADER_SIZE 0x10

extern instruction_func_t* instructions[];

Nes Cpu;
uint8_t VRAM[1024*2]; // 2KB


typedef struct{
    uint16_t PPUADDR;
    uint16_t PPUDATA;
}PPU_T;
PPU_T PPU;

const uint8_t cycleTbl[] = {
 /* 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F */
    7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6, // 0x00
    2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7, // 0x10
    6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6, // 0x20
    2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7, // 0x30
    6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6, // 0x40
    2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7, // 0x50
    6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6, // 0x60
    2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7, // 0x70
    2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4, // 0x80
    2, 6, 2, 6, 4, 4, 4, 4, 2, 4, 2, 5, 5, 4, 5, 5, // 0x90
    2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4, // 0xA0
    2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, // 0xB0
    2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, // 0xC0
    2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, // 0xD0
    2, 6, 3, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, // 0xE0
    2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, // 0xF0
};

typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}COLOR_T;
const COLOR_T color_table[] = {
  {0x80, 0x80, 0x80}, {0x00, 0x3D, 0xA6}, {0x00, 0x12, 0xB0}, {0x44, 0x00, 0x96},
  {0xA1, 0x00, 0x5E}, {0xC7, 0x00, 0x28}, {0xBA, 0x06, 0x00}, {0x8C, 0x17, 0x00},
  {0x5C, 0x2F, 0x00}, {0x10, 0x45, 0x00}, {0x05, 0x4A, 0x00}, {0x00, 0x47, 0x2E},
  {0x00, 0x41, 0x66}, {0x00, 0x00, 0x00}, {0x05, 0x05, 0x05}, {0x05, 0x05, 0x05},
  {0xC7, 0xC7, 0xC7}, {0x00, 0x77, 0xFF}, {0x21, 0x55, 0xFF}, {0x82, 0x37, 0xFA},
  {0xEB, 0x2F, 0xB5}, {0xFF, 0x29, 0x50}, {0xFF, 0x22, 0x00}, {0xD6, 0x32, 0x00},
  {0xC4, 0x62, 0x00}, {0x35, 0x80, 0x00}, {0x05, 0x8F, 0x00}, {0x00, 0x8A, 0x55},
  {0x00, 0x99, 0xCC}, {0x21, 0x21, 0x21}, {0x09, 0x09, 0x09}, {0x09, 0x09, 0x09},
  {0xFF, 0xFF, 0xFF}, {0x0F, 0xD7, 0xFF}, {0x69, 0xA2, 0xFF}, {0xD4, 0x80, 0xFF},
  {0xFF, 0x45, 0xF3}, {0xFF, 0x61, 0x8B}, {0xFF, 0x88, 0x33}, {0xFF, 0x9C, 0x12},
  {0xFA, 0xBC, 0x20}, {0x9F, 0xE3, 0x0E}, {0x2B, 0xF0, 0x35}, {0x0C, 0xF0, 0xA4},
  {0x05, 0xFB, 0xFF}, {0x5E, 0x5E, 0x5E}, {0x0D, 0x0D, 0x0D}, {0x0D, 0x0D, 0x0D},
  {0xFF, 0xFF, 0xFF}, {0xA6, 0xFC, 0xFF}, {0xB3, 0xEC, 0xFF}, {0xDA, 0xAB, 0xEB},
  {0xFF, 0xA8, 0xF9}, {0xFF, 0xAB, 0xB3}, {0xFF, 0xD2, 0xB0}, {0xFF, 0xEF, 0xA6},
  {0xFF, 0xF7, 0x9C}, {0xD7, 0xE8, 0x95}, {0xA6, 0xED, 0xAF}, {0xA2, 0xF2, 0xDA},
  {0x99, 0xFF, 0xFC}, {0xDD, 0xDD, 0xDD}, {0x11, 0x11, 0x11}, {0x11, 0x11, 0x11},
};

typedef struct{
    uint8_t no : 1;
}PaletteNo;

typedef union{
    uint64_t  palette_64;
    PaletteNo palette_array[64]; 
}Palette;

// 描画
void draw(uint8_t cycles);

// utils
void dump16(uint16_t address);
void statusCheck(uint8_t check, uint8_t reg);
void dumpCROM(uint16_t start_addr, uint16_t end_addr);




// 0x2006 PPU アドレス書き込み
// 実行するたびに書き込み先を下位8bit->上位8bitと入れ替える
void write_ppu_address(uint8_t add)
{
    // 8 -> 0 -> 8 -> ...
    static uint16_t mask = 0x0000;
    mask ^= 0x0008;

    uint16_t shit_add = (uint16_t)add << mask;
    PPU.PPUADDR &= (0x00ff << (8 - mask));
    PPU.PPUADDR |= shit_add;
}

// 0x2007 PPU データ書き込み
// 実行するたびに書き込み先アドレスをインクリメントする
void write_ppu_data(uint8_t data)
{
    printf("PPU[%04x] = %02x\n", PPU.PPUADDR, data);
    VRAM[PPU.PPUADDR] = data;
    PPU.PPUADDR++;
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("usage> nes84.exe path_to.nes\n");
        return -1;
    }
    if(!sdl_init()){
        printf("SDL int error !!!\n");
        return -1;
    }
    
    // 初期化
    Cpu.S = 0x01FD; // スタックポインタ 0x0100～0x01FF(上位8bitは固定)

    // iNESヘッダー読み込み
    FILE* fp = NULL;
    uint8_t cassette[0xA010]; // 今のところ決め打ち。可変にしたい。

    fopen_s(&fp, argv[1], "rb");
    fread(cassette, sizeof(uint8_t), sizeof(cassette)/sizeof(cassette[0]), fp);
    printf("===CASSETTE INFO===\n");
    printf("iNES header : ");
    for(int i=0; i<INES_HEADER_SIZE; i++){
        printf("%02X ", cassette[i]);
    }
    printf("\n");
    fclose(fp);

    // .nesからプログラムROMとキャラクタROMのアドレスを算出する
    const uint8_t chrRomPage = cassette[5];
    const uint16_t chrRomStartAddr = INES_HEADER_SIZE + cassette[4] * 0x4000; // Size of PRG ROM in 16 KB units
    const uint16_t chrRomEndAddr = chrRomStartAddr + cassette[5] * 0x2000;    // Size of CHR ROM in  8 KB units
    printf("PROM 0x%04X - 0x%04X\n",INES_HEADER_SIZE, chrRomStartAddr - 1);
    printf("CROM 0x%04X - 0x%04X\n",chrRomStartAddr, chrRomEndAddr - 1);

    // エミュのメモリにPROMをマッピングする
    for(int i=INES_HEADER_SIZE;i<chrRomStartAddr - 1; i++){
        Cpu.mem[0x8000 + i - INES_HEADER_SIZE] = cassette[i];
    }

    // エミュのメモリにCROMをマッピングする
    for(int i=chrRomStartAddr;i<chrRomEndAddr - 1; i++){
        VRAM[i - chrRomStartAddr] = cassette[i];
        //printf("[%04X] %02X\n", i - chrRomStartAddr, VRAM[i - chrRomStartAddr]);
    }


    // リセット。基本は0x8000になるはず
    Cpu.pc = 0x8000; // Cpu.mem[0xFFFC] | (Cpu.mem[0xFFFD] << 8);
    printf("RESET : PC <- 0x%04X\n", Cpu.pc);

    // 命令テーブル初期化    
    init_instructions();
    
    printf("===PC LOG===\n");
    uint8_t opcode;
    bool result;
    while(true){
        // fetch
        opcode = Cpu.mem[Cpu.pc];

        // 命令実行
        if(instructions[opcode] == NULL){
            printf("Unknown opcode : 0x%02X\n", opcode);
            break;
        }
        instructions[opcode](&Cpu);

        // 積算実行サイクル
        Cpu.cycle += cycleTbl[opcode];

        // 描画
        draw(cycleTbl[opcode] * 3);
    }

    sdl_finalize();

    return 0;
}

int ppu_cycle = 0;
int lines = 0;
void draw(uint8_t cycles)
{
    ppu_cycle += cycles;
    if(ppu_cycle >= 341){
        ppu_cycle -= 341;
        lines++;

        if((lines <= 240) && ((lines % 8) == 0)){
            // 描画
            int y = (lines / 8) - 1;
            for(int x=0;x<32;x++){

                // パレット番号取得
                uint8_t pal_byte = VRAM[PPU_MEMMAP_ATTR_TABLE_1_START + 8 * (y / 4) + (x / 4)];
                {
                    uint8_t bit_shift;
                    if(((x % 2) == 0) && ((y % 2) == 0)){
                        bit_shift = 0;
                    }
                    else if(((x % 2) == 0) && ((y % 2) != 0)){
                        bit_shift = 2;
                    }
                    else if(((x % 2) != 0) && ((y % 2) == 0)){
                        bit_shift = 1;
                    }
                    else{
                        bit_shift = 3;
                    }
                    pal_byte = (pal_byte >> (2*bit_shift)) & 3;
                }

                uint8_t pal[8*8] = {0};
                {
                    uint16_t chr = (uint16_t)VRAM[PPU_MEMMAP_NAME_TABLE_1_START + 32 * y + x] * 16;

                    // 8*8の色番号をpal配列に格納
                    // 最初の8バイトが各1bit目、次の8バイトが2bit目
                    for(int i=0; i<16; i++){
                        uint8_t color_no = VRAM[chr+i];
                        for(uint8_t j=0;j<8;j++){
                            uint8_t tmp = (color_no >> (7 - j)) & 0x01;
                            pal[(i%8)*8 + j] |= (tmp << (i/8));
                        }
                    }
                }

                // RGB読み出し
                for(uint8_t bg_y=0; bg_y<8; bg_y++){
                    for(uint8_t bg_x=0; bg_x<8; bg_x++){
                        uint8_t color = VRAM[PPU_MEMMAP_BG_PALETTE_TABLE_START + 4 * pal_byte + pal[8*bg_y + bg_x]];
                        COLOR_T rgb = color_table[color];
                        sdl_dot((x*8)+bg_x, (y*8)+bg_y, rgb.red, rgb.green, rgb.blue, 0);
                    }
                }
            }
            sdl_refresh();
        }

        // 240 lines + VBlank (20 lines) + post-render/pre-render scanline (2 lines)
        // = 262 lines
        if(lines >= 240){
            // VBlank set
            Cpu.mem[0x2002] |= 0x80;
        }
        
        if(lines == 262){
            printf("lines == 262 !!!\n");
            lines = 0;

            // VBlank clr
            Cpu.mem[0x2002] &= 0x7F;
            sdl_wait();
        }
    }
}

// 指定アドレスから16バイトメモリから読んで出力する
int offsetPrintFlg = 0;
void dump16(uint16_t address)
{
    if(offsetPrintFlg == 0){
        offsetPrintFlg = 1;
        for(int blank=0; blank<9; blank++){
            printf(" ");
        }
        for(int offset = 0; offset < 16; offset++){
            printf("%02X ", offset);
        }
        printf("\n");
    }
    uint16_t startAddress = address & 0xFFF0;
    printf("[0x%04X] ", startAddress);
    for(int i=0; i<16; i++){
        printf("%02X ", Cpu.mem[startAddress + i]);
    }
    printf("\n");
}

// 指定ステータスを更新する
void statusCheck(uint8_t check, uint8_t reg)
{
    if(check & STATUS_CARRY){
        // MSBからの桁上げが発生したとき
        // この関数内では跨いだか判断できないため今は呼び出し元で実現している
        //printf("\n[!!!] NOP STATUS_CARRY\n");
    }

    if(check & STATUS_ZERO){
        if(reg == 0){
            Cpu.status.statusBit.zer = 1;
        }
        else{
            Cpu.status.statusBit.zer = 0;
        }
    }

    if(check & STATUS_IRQ){
        Cpu.status.statusBit.irq = reg;
    }
    
    if(check & STATUS_DEC){
        
    }
    
    if(check & STATUS_BRK){
        
    }

    if(check & STATUS_RCV){
        
    }

    if(check & STATUS_OVERFLOW){
        // 0x7f - 0x80 を跨いだときにセットする
        // この関数内では跨いだか判断できないため今は呼び出し元で実現している
        //printf("\n[!!!] NOP STATUS_OVERFLOW\n");
    }

    if(check & STATUS_NEG){
        Cpu.status.statusBit.neg = (reg & 0x80) >> 7;
    }
}

// CROMをダンプして描画してみる
void dumpCROM(uint16_t start_addr, uint16_t end_addr)
{
    
}
