#if !defined _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "CPU.h"

typedef void instruction_func_t(Nes*);


void init_instructions(void);

// LDA
void LDA_Immediate(Nes*);
void LDA_Zeropage(Nes*);
void LDA_Zeropage_X(Nes*);
void LDA_Absolute(Nes*);
void LDA_Absolute_X(Nes*);
void LDA_Absolute_Y(Nes*);
void LDA_Indirect_X(Nes*);
void LDA_Indirect_Y(Nes*);

// LDX
void LDX_Immediate(Nes*);
void LDX_Zeropage(Nes*);
void LDX_Zeropage_Y(Nes*);
void LDX_Absolute(Nes*);
void LDX_Absolute_Y(Nes*);

// LDY
void LDY_Immediate(Nes*);
void LDY_Zeropage(Nes*);
void LDY_Zeropage_X(Nes*);
void LDY_Absolute(Nes*);
void LDY_Absolute_X(Nes*);

// STA
void STA_Zeropage(Nes*);
void STA_Zeropage_X(Nes*);
void STA_Absolute(Nes*);
void STA_Absolute_X(Nes*);
void STA_Absolute_Y(Nes*);
void STA_Indirect_X(Nes*);
void STA_Indirect_Y(Nes*);

// STX
void STX_Zeropage(Nes*);
void STX_Zeropage_Y(Nes*);
void STX_Absolute(Nes*);

// STY
void STY_Zeropage(Nes*);
void STY_Zeropage_X(Nes*);
void STY_Absolute(Nes*);

// TAX
void TAX_Implied(Nes*);

// TAY
void TAY_Implied(Nes*);

// TSX
void TSX_Implied(Nes*);

// TXA
void TXA_Implied(Nes*);

// TXS
void TXS_Implied(Nes*);

// TYA
void TYA_Implied(Nes*);

// ADC
void ADC_Immediate(Nes*);
void ADC_Zeropage(Nes*);
void ADC_Zeropage_X(Nes*);
void ADC_Absolute(Nes*);
void ADC_Absolute_X(Nes*);
void ADC_Absolute_Y(Nes*);
void ADC_Indirect_X(Nes*);
void ADC_Indirect_Y(Nes*);

// AND
void AND_Immediate(Nes*);
void AND_Zeropage(Nes*);
void AND_Zeropage_X(Nes*);
void AND_Absolute(Nes*);
void AND_Absolute_X(Nes*);
void AND_Absolute_Y(Nes*);
void AND_Indirect_X(Nes*);
void AND_Indirect_Y(Nes*);

// ASL
void ASL_Accumulator(Nes*);
void ASL_Zeropage(Nes*);
void ASL_Zeropage_X(Nes*);
void ASL_Absolute(Nes*);
void ASL_Absolute_X(Nes*);

// BIT
void BIT_Zeropage(Nes*);
void BIT_Absolute(Nes*);

// CMP
void CMP_Immediate(Nes*);
void CMP_Zeropage(Nes*);
void CMP_Zeropage_X(Nes*);
void CMP_Absolute(Nes*);
void CMP_Absolute_X(Nes*);
void CMP_Absolute_Y(Nes*);
void CMP_Indirect_X(Nes*);
void CMP_Indirect_Y(Nes*);

// CPX
void CPX_Immediate(Nes*);
void CPX_Zeropage(Nes*);
void CPX_Absolute(Nes*);

// CPY
void CPY_Immediate(Nes*);
void CPY_Zeropage(Nes*);
void CPY_Absolute(Nes*);

// DEC
void DEC_Zeropage(Nes*);
void DEC_Zeropage_X(Nes*);
void DEC_Absolute(Nes*);
void DEC_Absolute_X(Nes*);

// DEX
void DEX_Implied(Nes*);

// DEY
void DEY_Implied(Nes*);

// EOR
void EOR_Immediate(Nes*);
void EOR_Zeropage(Nes*);
void EOR_Zeropage_X(Nes*);
void EOR_Absolute(Nes*);
void EOR_Absolute_X(Nes*);
void EOR_Absolute_Y(Nes*);
void EOR_Indirect_X(Nes*);
void EOR_Indirect_Y(Nes*);

// INC
void INC_Zeropage(Nes*);
void INC_Zeropage_X(Nes*);
void INC_Absolute(Nes*);
void INC_Absolute_X(Nes*);

// INX
void INX_Implied(Nes*);

// INY
void INY_Implied(Nes*);

// LSR
void LSR_Accumulator(Nes*);
void LSR_Zeropage(Nes*);
void LSR_Zeropage_X(Nes*);
void LSR_Absolute(Nes*);
void LSR_Absolute_X(Nes*);

// ORA
void ORA_Immediate(Nes*);
void ORA_Zeropage(Nes*);
void ORA_Zeropage_X(Nes*);
void ORA_Absolute(Nes*);
void ORA_Absolute_X(Nes*);
void ORA_Absolute_Y(Nes*);
void ORA_Indirect_X(Nes*);
void ORA_Indirect_Y(Nes*);

// ROL
void ROL_Accumulator(Nes*);
void ROL_Zeropage(Nes*);
void ROL_Zeropage_X(Nes*);
void ROL_Absolute(Nes*);
void ROL_Absolute_X(Nes*);

// ROR
void ROR_Accumulator(Nes*);
void ROR_Zeropage(Nes*);
void ROR_Zeropage_X(Nes*);
void ROR_Absolute(Nes*);
void ROR_Absolute_X(Nes*);

// SBC
void SBC_Immediate(Nes*);
void SBC_Zeropage(Nes*);
void SBC_Zeropage_X(Nes*);
void SBC_Absolute(Nes*);
void SBC_Absolute_X(Nes*);
void SBC_Absolute_Y(Nes*);
void SBC_Indirect_X(Nes*);
void SBC_Indirect_Y(Nes*);

// PHA
void PHA_Implied(Nes*);

// PHP
void PHP_Implied(Nes*);

// PLA
void PLA_Implied(Nes*);

// PLP
void PLP_Implied(Nes*);

// JMP
void JMP_Absolute(Nes*);
void JMP_Indirect(Nes*);

// JSR
void JSR_Absolute(Nes*);

// RTS
void RTS_Implied(Nes*);

// RTI
void RTI_Implied(Nes*);

// BCS
void BCS_Relative(Nes*);

// BEQ
void BEQ_Relative(Nes*);

// BMI
void BMI_Relative(Nes*);

// BNE
void BNE_Relative(Nes*);

// BPL
void BPL_Relative(Nes*);

// BVC
void BVC_Relative(Nes*);

// BVS
void BVS_Relative(Nes*);

// CLC
void CLC_Implied(Nes*);

// CLD
void CLD_Implied(Nes*);

// CLI
void CLI_Implied(Nes*);

// CLV
void CLV_Implied(Nes*);

// SEC
void SEC_Implied(Nes*);

// SED
void SED_Implied(Nes*);

// SEI
void SEI_Implied(Nes*);

// BRK
void BRK_Implied(Nes*);

// NOP
void NOP_Implied(Nes*);

#endif
