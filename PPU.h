#if !defined _PPU_H_
#define _PPU_H_

#include <stdint.h>

// パターンテーブル0
#define PPU_MEMMAP_PATTERN_TABLE_0_SIZE  0x1000
#define PPU_MEMMAP_PATTERN_TABLE_0_START 0x0000
#define PPU_MEMMAP_PATTERN_TABLE_0_END   (PPU_MEMMAP_PATTERN_TABLE_0_START + PPU_MEMMAP_PATTERN_TABLE_0_SIZE)

// パターンテーブル1
#define PPU_MEMMAP_PATTERN_TABLE_1_SIZE  0x1000
#define PPU_MEMMAP_PATTERN_TABLE_1_START 0x1000
#define PPU_MEMMAP_PATTERN_TABLE_1_END   (PPU_MEMMAP_PATTERN_TABLE_1_START + PPU_MEMMAP_PATTERN_TABLE_1_SIZE)

// 画面1 ネームテーブル
#define PPU_MEMMAP_NAME_TABLE_1_SIZE     0x03C0
#define PPU_MEMMAP_NAME_TABLE_1_START    0x2000
#define PPU_MEMMAP_NAME_TABLE_1_END      (PPU_MEMMAP_NAME_TABLE_1_START + PPU_MEMMAP_NAME_TABLE_1_SIZE)

// 画面1 属性テーブル
#define PPU_MEMMAP_ATTR_TABLE_1_SIZE     0x0040
#define PPU_MEMMAP_ATTR_TABLE_1_START    0x23C0
#define PPU_MEMMAP_ATTR_TABLE_1_END      (PPU_MEMMAP_ATTR_TABLE_1_START + PPU_MEMMAP_ATTR_TABLE_1_SIZE)

// 画面2 ネームテーブル
#define PPU_MEMMAP_NAME_TABLE_2_SIZE     0x03C0
#define PPU_MEMMAP_NAME_TABLE_2_START    0x2400
#define PPU_MEMMAP_NAME_TABLE_2_END      (PPU_MEMMAP_NAME_TABLE_2_START + PPU_MEMMAP_NAME_TABLE_2_SIZE)

// 画面2 属性テーブル
#define PPU_MEMMAP_ATTR_TABLE_2_SIZE     0x0040
#define PPU_MEMMAP_ATTR_TABLE_2_START    0x27C0
#define PPU_MEMMAP_ATTR_TABLE_2_END      (PPU_MEMMAP_ATTR_TABLE_2_START + PPU_MEMMAP_ATTR_TABLE_2_SIZE)

// 画面3 ネームテーブル
#define PPU_MEMMAP_NAME_TABLE_3_SIZE     0x03C0
#define PPU_MEMMAP_NAME_TABLE_3_START    0x2800
#define PPU_MEMMAP_NAME_TABLE_3_END      (PPU_MEMMAP_NAME_TABLE_3_START + PPU_MEMMAP_NAME_TABLE_3_SIZE)

// 画面3 属性テーブル
#define PPU_MEMMAP_ATTR_TABLE_3_SIZE     0x0040
#define PPU_MEMMAP_ATTR_TABLE_3_START    0x2BC0
#define PPU_MEMMAP_ATTR_TABLE_3_END      (PPU_MEMMAP_ATTR_TABLE_3_START + PPU_MEMMAP_ATTR_TABLE_3_SIZE)

// 画面4 ネームテーブル
#define PPU_MEMMAP_NAME_TABLE_4_SIZE     0x03C0
#define PPU_MEMMAP_NAME_TABLE_4_START    0x2C00
#define PPU_MEMMAP_NAME_TABLE_4_END      (PPU_MEMMAP_NAME_TABLE_4_START + PPU_MEMMAP_NAME_TABLE_4_SIZE)

// 画面4 属性テーブル
#define PPU_MEMMAP_ATTR_TABLE_4_SIZE     0x0040
#define PPU_MEMMAP_ATTR_TABLE_4_START    0x2FC0
#define PPU_MEMMAP_ATTR_TABLE_4_END      (PPU_MEMMAP_ATTR_TABLE_4_START + PPU_MEMMAP_ATTR_TABLE_4_SIZE)

// 未使用? 0x2000-0x2EFFのミラー
#define PPU_MEMMAP_MIRROR_1_START       0x3000
#define PPU_MEMMAP_MIRROR_1_END         0x4000

// BGパレットテーブル
#define PPU_MEMMAP_BG_PALETTE_TABLE_SIZE     0x0010
#define PPU_MEMMAP_BG_PALETTE_TABLE_START    0x3F00
#define PPU_MEMMAP_BG_PALETTE_TABLE_END      (PPU_MEMMAP_BG_PALETTE_TABLE_START + PPU_MEMMAP_BG_PALETTE_TABLE_SIZE)

// スプライトパレットテーブル
#define PPU_MEMMAP_SP_PALETTE_TABLE_SIZE     0x0010
#define PPU_MEMMAP_SP_PALETTE_TABLE_START    0x3F10
#define PPU_MEMMAP_SP_PALETTE_TABLE_END      (PPU_MEMMAP_SP_PALETTE_TABLE_START + PPU_MEMMAP_SP_PALETTE_TABLE_SIZE)

// 未使用? 0x3F20-0x3FFFのミラー
#define PPU_MEMMAP_MIRROR_2_START       0x3F20
#define PPU_MEMMAP_MIRROR_2_END         0x4000

// 未使用? 0x4000-0xFFFFのミラー
#define PPU_MEMMAP_MIRROR_3_START       0x4000
#define PPU_MEMMAP_MIRROR_3_END         0x10000

// $2000 (Basic settings / store only)
typedef struct{
    uint8_t NN : 2; // メインスクリーン (00: $2000, 01: $2400, 10: $2800, 11: $2C00)
    uint8_t I  : 1; // VRAM入出力時のアドレス変化値 (0: +1, 1: +32)
    uint8_t S  : 1; // スプライトのキャラクタテーブル番号 (0: $0000, 1: $1000)
    uint8_t B  : 1; // BGのキャラクタテーブル番号 (0: $0000, 1: $1000)
    uint8_t H  : 1; // スプライトのサイズ (0: 8x8, 1: 8x16)
    uint8_t P  : 1; // PPU type (0: master, 1: slave)
    uint8_t V  : 1; // vBlank発生を割り込みで検出 (0: off, 1: on)
}BasicSetting;

typedef union{
    uint8_t      basicSettingPPUByte;
    BasicSetting basicSettingPPUBit;
}BasicSetting_T;

// $2001 (Mask settings / store only)
typedef struct{
    uint8_t C;     // モノクロ表示 (0: color, 1: mono)
    uint8_t m;     // 左端8x8のBG表示 (0: off, 1: on)
    uint8_t M;     // 左端8x8のスプライト表示 (0: off, 1: on)
    uint8_t BG;    // BG表示 (0: off, 1: on)
    uint8_t S;     // スプライト表示 (0: off, 1: on)
    uint8_t red;   // 赤を強調表示 (0: off, 1: on)
    uint8_t green; // 緑を強調表示 (0: off, 1: on)
    uint8_t blue;  // 青を強調表示 (0: off, 1: on)
}MaskSetting;

typedef union{
    uint8_t     maskSettingPPUByte;
    MaskSetting maskSettingPPUBit;
}MaskSetting_T;

#endif
