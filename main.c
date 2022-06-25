#include <stdio.h>
#include <stdint.h>

#define DEBUG_STEP_MAX 16*12

#define INES_HEADER_SIZE 0x10

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
    uint8_t statusByte;
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
Nes Cpu;

uint8_t cycleTbl[] = {
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

void dump16(uint16_t address);
void statusCheck(uint8_t check, uint8_t reg);

int main(int argc, char* argv[])
{    
    // 初期化
    Cpu.S = 0x01FD; // スタックポインタ 0x0100～0x01FF(上位8bitは固定)

    // iNESヘッダー読み込み
    FILE* fp = NULL;
    uint8_t cassette[0xA010]; // 今のところ決め打ち。可変にしたい。

    fopen_s(&fp, "./sample2/sample2.nes", "rb");
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
    //for(int i=chrRomStartAddr;i<chrRomEndAddr - 1; i++){
    //    Cpu.mem[0x8000 + i - INES_HEADER_SIZE] = cassette[i];
    //}

    // リセット。基本は0x8000になるはず
    Cpu.pc = Cpu.mem[0xFFFC] | (Cpu.mem[0xFFFD] << 8);
    printf("RESET : PC <- 0x%04X\n", Cpu.pc);

    // とりあえず16バイト出力してみる
    printf("===MEM MAP===\n");
    dump16(Cpu.pc);
    dump16(0x8059);
    dump16(0x8060);
    dump16(0x80F9);
    dump16(0x8100);
    dump16(0x814D);
    dump16(0x8150);
    
    printf("===PC LOG===\n");
    uint8_t opcode;
    for(int max = 0; max < DEBUG_STEP_MAX; max++){
        if((0 < max) && (max % 16 == 0)) printf("\n");
        printf("%04X ", Cpu.pc);
        // fetch
        opcode = Cpu.mem[Cpu.pc];
        // decode & exe
        switch(opcode){
            case 0x78:
                // IRQ割込み禁止, 今のところ無視
                statusCheck(STATUS_IRQ, 1);
                Cpu.pc++;
                break;

            case 0xA2:
                // LDX Imm
                Cpu.X = Cpu.mem[++Cpu.pc];
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.X);
                Cpu.pc++;
                break;

            case 0x9A:
                // TXS
                Cpu.S = 0x0100 | Cpu.X;
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.X);
                Cpu.pc++;
                break;

            case 0xA9:
                // LDA Imm
                Cpu.A = Cpu.mem[++Cpu.pc];
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.X);
                Cpu.pc++;
                break;

            case 0x85:
                // STA zpg
                Cpu.mem[Cpu.mem[++Cpu.pc]] = Cpu.A;
                Cpu.pc++;
                break;

            case 0x20:
                // JSR abs
                uint16_t ad;
                ad = Cpu.mem[++Cpu.pc]; // 下位
                ad |= Cpu.mem[++Cpu.pc] << 8; // 上位

                // 現在のPCをスタックに退避
                // printf("SP = 0x%04X\n", Cpu.S);
                Cpu.mem[Cpu.S] = (Cpu.pc & 0xFF00) >> 8; // 上位
                Cpu.S--;
                // printf("SP = 0x%04X\n", Cpu.S);
                Cpu.mem[Cpu.S] = (Cpu.pc & 0x00FF); // 下位
                Cpu.S--;

                Cpu.pc = ad;
                break;

            case 0x98:
                // TYA imp
                Cpu.A = Cpu.Y;
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.A);
                Cpu.pc++;

            case 0xA4:
                // LDY zpg
                Cpu.Y = Cpu.mem[++Cpu.pc];
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.Y);
                Cpu.pc++;
                break;

            case 0xD0:
                // BNE Rel
                if(Cpu.status.statusBit.zer == 0){
                    int8_t offset = Cpu.mem[Cpu.pc + 1];
                    Cpu.pc += offset;
                }
                else{
                    Cpu.pc += 2;
                }
                break;

            case 0xC6:
                // DEC zpg
                Cpu.pc++;
                Cpu.mem[Cpu.pc] = Cpu.mem[Cpu.pc]--;
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.mem[Cpu.pc]);
                Cpu.pc++;
                break;

            case 0x01:
                // ORA (Ind,X)
                {
                    uint16_t address = 0x0000;
                    address = (Cpu.mem[++Cpu.pc] + Cpu.X) & 0x00FF;
                    address = (Cpu.mem[address + 1] << 4) | Cpu.mem[address];
                    Cpu.A |= Cpu.mem[address];
                    Cpu.pc++;
                }
                break;

            case 0x00:
                // BRK imp
                // 今のところ何もしない
                statusCheck(STATUS_BRK, 0);
                Cpu.pc++;
                break;

            case 0xA0:
                // LDY imm
                Cpu.Y += Cpu.mem[++Cpu.pc];
                statusCheck(STATUS_ZERO | STATUS_NEG, Cpu.Y);
                Cpu.pc++;
                break;

            case 0x91:
                // STA (ind),Y
                {
                    uint16_t address = 0x0000;
                    address = Cpu.mem[++Cpu.pc] & 0x00FF;
                    address = (Cpu.mem[address] << 4) | Cpu.mem[address + 1];
                    Cpu.mem[address + Cpu.Y] = Cpu.A;
                    Cpu.pc++;
                }
                break;

            case 0x60:
                // Return from Subroutine
                {
                    uint16_t address = 0x0000;
                    address = (Cpu.mem[Cpu.S + 2] << 8) | Cpu.mem[Cpu.S + 1];
                    Cpu.S += 2;
                    Cpu.pc = address;
                    Cpu.pc++;
                }
                break;

            case 0x38:
                // SEC
                Cpu.status.statusBit.car = 1;
                Cpu.pc++;
                break;

            case 0x65:
                // ADC (A + メモリ + キャリーフラグ) を演算して結果をAへ返します。[N.V.0.0.0.0.Z.C]
                uint8_t beforeA;
                beforeA = Cpu.A;
                Cpu.A = Cpu.A + Cpu.mem[++Cpu.pc] + Cpu.status.statusBit.car;
                if((beforeA < 0x80) && (Cpu.A >= 0x80)){
                    // 0x80を跨いだ
                    Cpu.status.statusBit.ovf = 1;
                }
                else{
                    Cpu.status.statusBit.ovf = 0;
                }
                if(Cpu.A < beforeA){
                    // 0xFFを超えた
                    Cpu.status.statusBit.car = 1;
                }
                statusCheck(STATUS_CARRY | STATUS_NEG | STATUS_OVERFLOW | STATUS_ZERO, Cpu.A);
                Cpu.pc++;
                break;

            case 0xB0:
                // BCS Rel
                if(Cpu.status.statusBit.car == 1){
                    int8_t offset = Cpu.mem[Cpu.pc + 1];
                    Cpu.pc += offset;
                }
                else{
                    Cpu.pc += 2;
                }
                break;

            case 0xB9:
                // LDA Abs, Y
                {
                    uint16_t address;
                    address = Cpu.mem[++Cpu.pc];
                    address |= (Cpu.mem[++Cpu.pc] << 8);
                    address += Cpu.Y;
                    Cpu.A = Cpu.mem[address];
                    statusCheck(STATUS_NEG | STATUS_ZERO, Cpu.A);
                }
                Cpu.pc++;
                break;

            case 0x88:
                // DEY
                Cpu.Y--;
                statusCheck(STATUS_NEG | STATUS_ZERO, Cpu.Y);
                Cpu.pc++;
                break;
            
            case 0x10:
                // BPL Rel
                if(Cpu.status.statusBit.neg == 0){
                    int8_t offset = Cpu.mem[Cpu.pc + 1];
                    Cpu.pc += offset;
                }
                else{
                    Cpu.pc += 2;
                }
                break;

            /*
            case 0x:
                Cpu.pc++;
                break;
            */

            default:
                printf("\n[!!!] Unknown opcode : 0x%02X\n",opcode);
                max = DEBUG_STEP_MAX; // 一旦終了
                break;
        }

        // 積算実行サイクル
        Cpu.cycle += cycleTbl[opcode];
    }

    return 0;
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
    else if (check & STATUS_ZERO){
        Cpu.status.statusBit.zer = reg & 0x01;
    }
    else if (check & STATUS_IRQ){
        Cpu.status.statusBit.irq = reg;
    }
    else if (check & STATUS_DEC){
        
    }
    else if (check & STATUS_BRK){
        
    }
    else if (check & STATUS_RCV){
        
    }
    else if (check & STATUS_OVERFLOW){
        // 0x7f - 0x80 を跨いだときにセットする
        // この関数内では跨いだか判断できないため今は呼び出し元で実現している
        //printf("\n[!!!] NOP STATUS_OVERFLOW\n");
    }
    else if (check & STATUS_NEG){
        Cpu.status.statusBit.neg = (Cpu.A & 0x80) >> 7;
    }
}