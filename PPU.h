#if !defined _PPU_H_
#define _PPU_H_

#include <stdint.h>

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
