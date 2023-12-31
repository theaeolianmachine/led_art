#ifndef FONTS_5PT_H
#define FONTS_5PT_H

//=== F O N T ===//
#define FONT_ARRAY_LENGTH 199
#define FONT_LETTER_NUM_BYTES 6

byte font[FONT_ARRAY_LENGTH][FONT_LETTER_NUM_BYTES]  = {
{2, 0x00,0x00,0x00,0x00,0x00 }, //char32
{2, 0x20,0x20,0x20,0x00,0x20 }, //char33
{5, 0x00,0x00,0x00,0xA0,0xA0 }, //char34
{5, 0x50,0xF8,0x50,0xF8,0x50 }, //char35
{5, 0xF8,0xA0,0xF8,0x28,0xF8 }, //char36
{5, 0x88,0x10,0x20,0x40,0x88 }, //char37
{5, 0x60,0x90,0x68,0x90,0x68 }, //char38
{5, 0x00,0x00,0x00,0x40,0x80 }, //char39
{5, 0x10,0x20,0x20,0x20,0x10 }, //char40
{5, 0x40,0x20,0x20,0x20,0x40 }, //char41
{5, 0xA8,0x70,0xF8,0x70,0xA8 }, //char42
{5, 0x00,0x00,0x40,0xE0,0x40 }, //char43
{5, 0x00,0x00,0x00,0x40,0x80 }, //char44
{5, 0x00,0x00,0x00,0x00,0xE0 }, //char45
{5, 0x00,0x00,0x00,0x00,0x80 }, //char46
{5, 0x10,0x20,0x20,0x20,0x40 }, //char47
{5, 0x70,0x98,0xA8,0xC8,0x70 }, //char48
{5, 0x20,0x60,0x20,0x20,0x70 }, //char49
{5, 0x70,0x10,0x70,0x40,0x70 }, //char50
{5, 0x70,0x10,0x30,0x10,0x70 }, //char51
{5, 0x40,0x40,0x50,0x70,0x10 }, //char52
{5, 0x70,0x40,0x70,0x10,0x70 }, //char53
{5, 0x60,0x40,0x70,0x50,0x70 }, //char54
{5, 0x70,0x10,0x30,0x10,0x10 }, //char55
{5, 0x70,0x50,0x70,0x50,0x70 }, //char56
{5, 0x70,0x50,0x70,0x10,0x70 }, //char57
{2, 0x00,0x00,0x80,0x00,0x80 }, //char58
{5, 0x00,0x40,0x00,0x40,0x80 }, //char59
{5, 0x10,0x20,0x40,0x20,0x10 }, //char60
{5, 0x00,0x00,0xE0,0x00,0xE0 }, //char61
{5, 0x40,0x20,0x10,0x20,0x40 }, //char62
{5, 0x70,0x88,0x30,0x00,0x20 }, //char63
{5, 0x70,0x90,0xA0,0x80,0x70 }, //char64
{5, 0x60,0x90,0x88,0xF8,0x88 }, //char65 //A
{5, 0xE0,0x90,0xF0,0x88,0xF0 }, //char66 //B
{5, 0x70,0x88,0x80,0x88,0x70 }, //char67 //C
{5, 0xF0,0x98,0x88,0x88,0xF0 }, //char68 //D
{5, 0xF8,0x80,0xF0,0x80,0xF8 }, //char69 //E
{5, 0xF8,0x80,0xF0,0x80,0x80 }, //char70 //F
{5, 0x70,0x80,0x98,0x88,0x70 }, //char71 //G
{5, 0x90,0x88,0xF8,0x88,0x88 }, //char72 //H
{5, 0x70,0x20,0x20,0x20,0x70 }, //char73 //I
{5, 0x30,0x10,0x10,0x50,0x70 }, //char74 //J
{5, 0x90,0xA0,0xC0,0xA0,0x90 }, //char75 //K
{5, 0x80,0x80,0x80,0x80,0xF0 }, //char76 //L
{5, 0x88,0xD8,0xA8,0x88,0x88 }, //char77 //M
{5, 0x88,0xC8,0xA8,0x98,0x88 }, //char78 //N
{5, 0x70,0x98,0x88,0x88,0x70 }, //char79 //O
{5, 0xE0,0x90,0xE0,0x80,0x80 }, //char80 //P
{5, 0x70,0x88,0x88,0x98,0x78 }, //char81 //Q
{5, 0xE0,0x90,0xE0,0xA0,0x90 }, //char82 //R
{5, 0x78,0x80,0x70,0x08,0xF0 }, //char83 //S
{5, 0xF8,0x20,0x20,0x20,0x20 }, //char84 //T
{5, 0x90,0x88,0x88,0x88,0x70 }, //char85 //U
{5, 0x50,0x50,0x50,0x50,0x20 }, //char86 //V
{5, 0x90,0x88,0xA8,0xA8,0x50 }, //char87 //W
{5, 0x88,0x50,0x20,0x50,0x88 }, //char88 //X
{5, 0x88,0x50,0x20,0x20,0x20 }, //char89 //Y
{5, 0xF8,0x10,0x20,0x40,0xF8 }, //char90 //Z
{5, 0x30,0x20,0x20,0x20,0x30 }, //char91
{5, 0x40,0x20,0x20,0x20,0x10 }, //char92
{5, 0x60,0x20,0x20,0x20,0x60 }, //char93
{5, 0x00,0x00,0x00,0x40,0xA0 }, //char94
{5, 0x00,0x00,0x00,0x00,0xE0 }, //char95
{5, 0x00,0x00,0x00,0x80,0x40 }, //char96
{5, 0x60,0x90,0x88,0xF8,0x88 }, //char97  //a
{5, 0xE0,0x90,0xF0,0x88,0xF0 }, //char98  //b
{5, 0x70,0x88,0x80,0x88,0x70 }, //char99  //c
{5, 0xF0,0x98,0x88,0x88,0xF0 }, //char100  //d
{5, 0xF8,0x80,0xF0,0x80,0xF8 }, //char101  //e
{5, 0xF8,0x80,0xF0,0x80,0x80 }, //char102  //f
{5, 0x70,0x80,0x98,0x88,0x70 }, //char103  //g
{5, 0x90,0x88,0xF8,0x88,0x88 }, //char104  //h
{5, 0x70,0x20,0x20,0x20,0x70 }, //char105  //i
{5, 0x30,0x10,0x10,0x50,0x70 }, //char106  //j
{5, 0x48,0x50,0x60,0x50,0x48 }, //char107  //k
{5, 0x80,0x80,0x80,0x80,0xF0 }, //char108  //l
{5, 0x88,0xD8,0xA8,0x88,0x88 }, //char109  //m
{5, 0x88,0xC8,0xA8,0x98,0x88 }, //char110  //n
{5, 0x70,0x98,0x88,0x88,0x70 }, //char111  //o
{5, 0xE0,0x90,0xE0,0x80,0x80 }, //char112  //p
{5, 0x70,0x88,0x88,0x98,0x78 }, //char113  //q
{5, 0xE0,0x90,0xE0,0xA0,0x90 }, //char114  //r
{5, 0x78,0x80,0x70,0x08,0xF0 }, //char115  //s
{5, 0xF8,0x20,0x20,0x20,0x20 }, //char116  //t
{5, 0x90,0x88,0x88,0x88,0x70 }, //char117  //u
{5, 0x50,0x50,0x50,0x50,0x20 }, //char118  //v
{5, 0x90,0x88,0xA8,0xA8,0x50 }, //char119  //w
{5, 0x88,0x50,0x20,0x50,0x88 }, //char120  //x
{5, 0x88,0x50,0x20,0x20,0x20 }, //char121  //y
{5, 0xF8,0x10,0x20,0x40,0xF8 }, //char122  //z
{5, 0x30,0x20,0x40,0x20,0x30 }, //char123
{5, 0x20,0x20,0x20,0x20,0x20 }, //char124
{5, 0x60,0x20,0x10,0x20,0x60 }, //char125
{5, 0x00,0x00,0x00,0x50,0xA0 }, //char126
{5, 0x00,0x40,0xA0,0xA0,0xE0 }, //char127
{5, 0x20,0x00,0x20,0x20,0x20 }, //char161
{5, 0x20,0x48,0x90,0x48,0x20 }, //char171
{5, 0x00,0x00,0xE0,0x20,0x20 }, //char172
{5, 0x20,0x70,0x20,0x00,0x70 }, //char177
{5, 0x00,0x00,0x00,0x00,0x80 }, //char183
{5, 0x20,0x90,0x48,0x90,0x20 }, //char187
{5, 0x20,0x00,0x60,0x88,0x70 }, //char191
{5, 0x50,0x00,0x70,0x50,0x78 }, //char196
{5, 0x50,0x00,0x70,0x50,0x70 }, //char214
{5, 0x50,0x00,0x50,0x50,0x70 }, //char220
{5, 0x60,0x90,0xB0,0x88,0xB0 }, //char223
{5, 0x50,0x00,0x70,0x50,0x78 }, //char228
{5, 0x50,0x00,0x70,0x50,0x70 }, //char246
{5, 0x20,0x00,0x70,0x00,0x20 }, //char247
{5, 0x50,0x00,0x50,0x50,0x70 }, //char252
};//==end of font array==//

#endif
