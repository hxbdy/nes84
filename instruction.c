#include "instruction.h"

instruction_func_t* instructions[0xff];
void init_instructions(void)
{
    memset(instructions, 0, sizeof(instructions));
    instructions[0x78] = SEI_Implied;
    instructions[0xA2] = LDX_Immediate;
    instructions[0x9A] = TXS_Implied;
    instructions[0xA9] = LDA_Immediate;
    instructions[0x85] = STA_Zeropage;
    instructions[0x20] = JSR_Absolute;
    instructions[0x98] = TYA_Implied;
    instructions[0xA4] = LDY_Zeropage;
    instructions[0xD0] = BNE_Relative;
    instructions[0xC6] = DEC_Zeropage;
    instructions[0x01] = ORA_Indirect_X;
    instructions[0x00] = BRK_Implied;
    instructions[0xA0] = LDY_Immediate;
    instructions[0xAD] = LDA_Absolute;
    instructions[0x91] = STA_Indirect_Y;
    instructions[0x60] = RTS_Implied;
    instructions[0x38] = SEC_Implied;
    instructions[0x65] = ADC_Zeropage;
    instructions[0xB0] = BCS_Relative;
    instructions[0xB9] = LDA_Absolute_Y;
    instructions[0x88] = DEY_Implied;
    instructions[0x10] = BPL_Relative;
    instructions[0x8D] = STA_Absolute;
    instructions[0xBD] = LDA_Absolute_X;
    instructions[0xE0] = CPX_Immediate;
    instructions[0xE8] = INX_Implied;
    instructions[0x4C] = JMP_Absolute;
}

void SEI_Implied(Nes *nes)
{
    // IRQ割込み禁止, 今のところ無視
    statusCheck(STATUS_IRQ, 1);
    nes->pc++;
}

void LDX_Immediate(Nes *nes)
{
    // LDX Imm
    nes->X = nes->mem[++nes->pc];
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->X);
    nes->pc++;
    // printf("X = %d\n", nes->X);
}

void TXS_Implied(Nes *nes)
{
    // TXS
    nes->S = 0x0100 | nes->X;
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->S);
    nes->pc++;
}

void LDA_Immediate(Nes *nes)
{
    // LDA Imm
    nes->A = nes->mem[++nes->pc];
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->A);
    nes->pc++;
}

void STA_Zeropage(Nes *nes)
{
    // STA zpg
    nes->mem[nes->mem[++nes->pc]] = nes->A;
    nes->pc++;
}

void JSR_Absolute(Nes *nes)
{
    // JSR abs
    uint16_t ad;
    ad = nes->mem[++nes->pc];       // 下位
    ad |= nes->mem[++nes->pc] << 8; // 上位

    // 現在のPCをスタックに退避
    // printf("SP = 0x%04X\n", nes->S);
    nes->mem[nes->S] = (nes->pc & 0xFF00) >> 8; // 上位
    nes->S--;
    // printf("SP = 0x%04X\n", nes->S);
    nes->mem[nes->S] = (nes->pc & 0x00FF); // 下位
    nes->S--;

    nes->pc = ad;
}

void TYA_Implied(Nes *nes)
{
    // TYA imp
    nes->A = nes->Y;
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->A);
    nes->pc++;
}

void LDY_Zeropage(Nes *nes)
{
    // LDY zpg
    nes->Y = nes->mem[++nes->pc];
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->Y);
    nes->pc++;
}

void BNE_Relative(Nes *nes)
{
    // BNE Rel
    if (nes->status.statusBit.zer == 0)
    {
        int8_t offset = nes->mem[nes->pc + 1];
        nes->pc += offset;
    }
    nes->pc += 2;
}

void DEC_Zeropage(Nes *nes)
{
    // DEC zpg
    nes->pc++;
    nes->mem[nes->pc] = nes->mem[nes->pc]--;
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->mem[nes->pc]);
    nes->pc++;
}
void ORA_Indirect_X(Nes *nes)
{
    uint16_t address = 0x0000;
    address = (nes->mem[++nes->pc] + nes->X) & 0x00FF;
    address = (nes->mem[address + 1] << 4) | nes->mem[address];
    nes->A |= nes->mem[address];
    nes->pc++;
}
void BRK_Implied(Nes *nes)
{
    // BRK imp
    // 今のところ何もしない
    statusCheck(STATUS_BRK, 0);
    nes->pc++;
}
void LDY_Immediate(Nes *nes)
{
    // LDY imm
    nes->Y = nes->mem[++nes->pc];
    statusCheck(STATUS_ZERO | STATUS_NEG, nes->Y);
    nes->pc++;
}
void STA_Indirect_Y(Nes *nes)
{
    // STA (ind),Y
    {
        uint16_t address = 0x0000;
        address = nes->mem[++nes->pc] & 0x00FF;
        address = (nes->mem[address] << 8) | nes->mem[address + 1];
        nes->mem[address + nes->Y] = nes->A;
        nes->pc++;
    }
}
void RTS_Implied(Nes *nes)
{
    // Return from Subroutine
    {
        uint16_t address = 0x0000;
        address = (nes->mem[nes->S + 2] << 8) | nes->mem[nes->S + 1];
        nes->S += 2;
        nes->pc = address;
        nes->pc++;
    }
}
void SEC_Implied(Nes *nes)
{
    // SEC
    nes->status.statusBit.car = 1;
    nes->pc++;
}
void ADC_Zeropage(Nes *nes)
{
    // ADC (A + メモリ + キャリーフラグ) を演算して結果をAへ返します。[N.V.0.0.0.0.Z.C]
    uint8_t beforeA;
    beforeA = nes->A;
    nes->A = nes->A + nes->mem[++nes->pc] + nes->status.statusBit.car;
    if ((beforeA < 0x80) && (nes->A >= 0x80))
    {
        // 0x80を跨いだ
        nes->status.statusBit.ovf = 1;
    }
    else
    {
        nes->status.statusBit.ovf = 0;
    }
    if (nes->A < beforeA)
    {
        // 0xFFを超えた
        nes->status.statusBit.car = 1;
    }
    statusCheck(STATUS_CARRY | STATUS_NEG | STATUS_OVERFLOW | STATUS_ZERO, nes->A);
    nes->pc++;
}
void BCS_Relative(Nes *nes)
{
    // BCS Rel
    if (nes->status.statusBit.car == 1)
    {
        int8_t offset = nes->mem[nes->pc + 1];
        nes->pc += offset;
    }
    nes->pc += 2;
}
void LDA_Absolute_Y(Nes *nes)
{
    // LDA Abs, Y
    {
        uint16_t address;
        address = nes->mem[++nes->pc] & 0x00ff;
        address |= (nes->mem[++nes->pc] << 8);
        address += nes->Y;
        nes->A = nes->mem[address];
        statusCheck(STATUS_NEG | STATUS_ZERO, nes->A);
    }
    nes->pc++;
}
void DEY_Implied(Nes *nes)
{
    // DEY
    nes->Y--;
    statusCheck(STATUS_NEG | STATUS_ZERO, nes->Y);
    nes->pc++;
    // printf("Y = %d\n", nes->Y);
}
void BPL_Relative(Nes *nes)
{
    // BPL Rel
    if (nes->status.statusBit.neg == 0)
    {
        int8_t offset = nes->mem[nes->pc + 1];
        nes->pc += offset;
    }
    nes->pc += 2;
}
void STA_Absolute(Nes *nes)
{
    // STA Abs
    {
        uint16_t address = 0x00;
        address = nes->mem[++nes->pc] & 0x00FF;         // under
        address |= (nes->mem[++nes->pc] << 8) & 0xFF00; // upper
        nes->mem[address] = nes->A;

        switch (address)
        {
        case 0x2003:
            write_oam_address(nes->A);
            break;
        case 0x2004:
            write_oam_access(nes->A);
            break;
        case 0x2006:
            // $2006 (VRAMアドレスレジスタ)
            // このレジスタには2回連続で書き込みます。
            write_ppu_address(nes->A);
            break;
        case 0x2007:
            // $2007 (VRAMアクセスレジスタ)
            // VRAMからのデータ読み込み、VRAMへのデータ書き込みを行います。
            write_ppu_data(nes->A);
            break;
        
        default:
            break;
        }

        nes->pc++;
    }
}
void LDA_Absolute_X(Nes *nes)
{
    // LDA Abs,X
    {
        uint16_t address = 0x00;
        address = nes->mem[++nes->pc] & 0x00FF;         // under
        address |= (nes->mem[++nes->pc] << 8) & 0xFF00; // upper
        nes->A = nes->mem[address + nes->X];
        statusCheck(STATUS_NEG | STATUS_ZERO, nes->A);
        nes->pc++;
    }
}
void INX_Implied(Nes *nes)
{
    // INX
    nes->X++;
    statusCheck(STATUS_NEG | STATUS_ZERO, nes->X);
    nes->pc++;
}
void JMP_Absolute(Nes *nes)
{
    // JMP abs
    {
        uint16_t address = 0x00;
        address = nes->mem[++nes->pc] & 0x00FF;         // under
        address |= (nes->mem[++nes->pc] << 8) & 0xFF00; // upper
        nes->pc = address;
    }
}

void LDA_Absolute(Nes *nes)
{
    {
        uint16_t address = 0x00;
        address = nes->mem[++nes->pc] & 0x00FF;         // under
        address |= (nes->mem[++nes->pc] << 8) & 0xFF00; // upper
        nes->A = nes->mem[address];
        statusCheck(STATUS_NEG | STATUS_ZERO, nes->A);
        nes->pc++;
    }
}

void CPX_Immediate(Nes *nes)
{
    uint8_t val = nes->X - nes->mem[++nes->pc];
    if(val >= 0){
        nes->status.statusBit.car = 1;
    }
    else{
        nes->status.statusBit.car = 0;
    }
    statusCheck(STATUS_NEG | STATUS_ZERO, val);
    nes->pc++;
}
