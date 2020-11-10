// font.c

#include "font.h"

const struct glyph font[] = {
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x00
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x01
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x02
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x03
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x04
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x05
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x06
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x07
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x08
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x09
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x0a
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x0b
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x0c
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x0d
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x0e
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x0f

    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x10
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x11
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x12
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x13
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x14
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x15
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x16
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x17
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x18
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x19
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x1a
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x1b
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x1c
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x1d
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x1e
    { 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // 0x1f

    { 4, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // (space)
    { 2, { 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // !
    { 4, { 0x07, 0x00, 0x07, 0x00, 0x00, 0x00 } }, // "
    { 6, { 0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00 } }, // #
    { 6, { 0x26, 0x49, 0x7f, 0x49, 0x32, 0x00 } }, // $
    { 6, { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 } }, // %
    { 6, { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 } }, // &
    { 3, { 0x05, 0x03, 0x00, 0x00, 0x00, 0x00 } }, // '
    { 4, { 0x1c, 0x22, 0x41, 0x00, 0x00, 0x00 } }, // (
    { 4, { 0x41, 0x22, 0x1c, 0x00, 0x00, 0x00 } }, // )
    { 6, { 0x08, 0x2a, 0x1c, 0x2a, 0x08, 0x00 } }, // *
    { 6, { 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00 } }, // +
    { 3, { 0x50, 0x30, 0x00, 0x00, 0x00, 0x00 } }, // ,
    { 6, { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 } }, // -
    { 3, { 0x60, 0x60, 0x00, 0x00, 0x00, 0x00 } }, // .
    { 6, { 0x60, 0x10, 0x08, 0x04, 0x03, 0x00 } }, // /

    { 6, { 0x3e, 0x41, 0x49, 0x41, 0x3e, 0x00 } }, // 0
    { 4, { 0x42, 0x7f, 0x40, 0x00, 0x00, 0x00 } }, // 1
    { 6, { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 } }, // 2
    { 6, { 0x22, 0x41, 0x49, 0x49, 0x36, 0x00 } }, // 3
    { 6, { 0x18, 0x14, 0x12, 0x7f, 0x10, 0x00 } }, // 4
    { 6, { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 } }, // 5
    { 6, { 0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00 } }, // 6
    { 6, { 0x01, 0x71, 0x09, 0x05, 0x03, 0x00 } }, // 7
    { 6, { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 } }, // 8
    { 6, { 0x06, 0x49, 0x49, 0x49, 0x3e, 0x00 } }, // 9
    { 3, { 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00 } }, // :
    { 3, { 0x56, 0x36, 0x00, 0x00, 0x00, 0x00 } }, // ;
    { 6, { 0x08, 0x08, 0x14, 0x22, 0x22, 0x00 } }, // <
    { 6, { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 } }, // =
    { 6, { 0x22, 0x22, 0x14, 0x08, 0x08, 0x00 } }, // >
    { 6, { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 } }, // ?

    { 6, { 0x32, 0x49, 0x79, 0x41, 0x3e, 0x00 } }, // @
    { 6, { 0x7e, 0x09, 0x09, 0x09, 0x7e, 0x00 } }, // A
    { 6, { 0x7f, 0x49, 0x49, 0x49, 0x36, 0x00 } }, // B
    { 6, { 0x3e, 0x41, 0x41, 0x41, 0x22, 0x00 } }, // C
    { 6, { 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00 } }, // D
    { 6, { 0x7f, 0x49, 0x49, 0x41, 0x41, 0x00 } }, // E
    { 6, { 0x7f, 0x09, 0x09, 0x01, 0x01, 0x00 } }, // F
    { 6, { 0x3e, 0x41, 0x41, 0x49, 0x39, 0x00 } }, // G
    { 6, { 0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00 } }, // H
    { 4, { 0x41, 0x7f, 0x41, 0x00, 0x00, 0x00 } }, // I
    { 6, { 0x20, 0x40, 0x41, 0x3f, 0x01, 0x00 } }, // J
    { 6, { 0x7f, 0x08, 0x14, 0x22, 0x41, 0x00 } }, // K
    { 6, { 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00 } }, // L
    { 6, { 0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00 } }, // M
    { 6, { 0x7f, 0x02, 0x04, 0x08, 0x7f, 0x00 } }, // N
    { 6, { 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00 } }, // O

    { 6, { 0x7f, 0x09, 0x09, 0x09, 0x06, 0x00 } }, // P
    { 6, { 0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00 } }, // Q
    { 6, { 0x7f, 0x09, 0x19, 0x29, 0x46, 0x00 } }, // R
    { 6, { 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 } }, // S
    { 6, { 0x01, 0x01, 0x7f, 0x01, 0x01, 0x00 } }, // T
    { 6, { 0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00 } }, // U
    { 6, { 0x0f, 0x10, 0x60, 0x10, 0x0f, 0x00 } }, // V
    { 6, { 0x7f, 0x20, 0x18, 0x20, 0x7f, 0x00 } }, // W
    { 6, { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 } }, // X
    { 6, { 0x03, 0x04, 0x78, 0x04, 0x03, 0x00 } }, // Y
    { 6, { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 } }, // Z
    { 4, { 0x7f, 0x41, 0x41, 0x00, 0x00 } }, // [
    { 6, { 0x03, 0x04, 0x08, 0x10, 0x60, 0x00 } }, // "\"
    { 4, { 0x41, 0x41, 0x7f, 0x00, 0x00, 0x00 } }, // ]
    { 6, { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 } }, // ^
    { 6, { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 } }, // _

    { 3, { 0x03, 0x05, 0x00, 0x00, 0x00, 0x00 } }, // `
    { 6, { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 } }, // a
    { 6, { 0x7f, 0x48, 0x44, 0x44, 0x38, 0x00 } }, // b
    { 6, { 0x38, 0x44, 0x44, 0x44, 0x48, 0x00 } }, // c
    { 6, { 0x38, 0x44, 0x44, 0x48, 0x7f, 0x00 } }, // d
    { 6, { 0x38, 0x54, 0x54, 0x54, 0x58, 0x00 } }, // e
    { 5, { 0x08, 0x7e, 0x09, 0x01, 0x02, 0x00 } }, // f
    { 6, { 0x38, 0x44, 0x44, 0x54, 0x34, 0x00 } }, // g
    { 6, { 0x7f, 0x08, 0x04, 0x04, 0x78, 0x00 } }, // h
    { 4, { 0x44, 0x7d, 0x40, 0x00, 0x00, 0x00 } }, // i
    { 5, { 0x20, 0x40, 0x44, 0x3d, 0x00, 0x00 } }, // j
    { 5, { 0x7f, 0x10, 0x28, 0x44, 0x00, 0x00 } }, // k
    { 4, { 0x41, 0x7f, 0x40, 0x00, 0x00, 0x00 } }, // l
    { 6, { 0x7c, 0x04, 0x18, 0x04, 0x78, 0x00 } }, // m
    { 6, { 0x7c, 0x08, 0x04, 0x04, 0x78, 0x00 } }, // n
    { 6, { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 } }, // o

    { 6, { 0x7c, 0x14, 0x14, 0x14, 0x08, 0x00 } }, // p
    { 6, { 0x38, 0x44, 0x54, 0x24, 0x58, 0x00 } }, // q
    { 6, { 0x7c, 0x08, 0x04, 0x04, 0x08, 0x00 } }, // r
    { 6, { 0x48, 0x54, 0x54, 0x54, 0x24, 0x00 } }, // s
    { 6, { 0x04, 0x3f, 0x44, 0x40, 0x20, 0x00 } }, // t
    { 6, { 0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00 } }, // u
    { 6, { 0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00 } }, // v
    { 6, { 0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00 } }, // w
    { 6, { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 } }, // x
    { 6, { 0x0c, 0x50, 0x50, 0x50, 0x3c, 0x00 } }, // y
    { 6, { 0x44, 0x64, 0x54, 0x4c, 0x44, 0x00 } }, // z
    { 4, { 0x08, 0x36, 0x41, 0x00, 0x00, 0x00 } }, // {
    { 2, { 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00 } }, // |
    { 4, { 0x41, 0x36, 0x08, 0x00, 0x00, 0x00 } }, // }
    { 6, { 0x08, 0x04, 0x08, 0x10, 0x08, 0x00 } }, // ~
    { 6, { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x00 } }  // DEL
};