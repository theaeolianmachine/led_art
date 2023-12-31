#ifndef FONT_BRANDO_H
#define FONT_BRANDO_H

//=== F O N T ===//
#define FONT_ARRAY_LENGTH 199
#define FONT_LETTER_NUM_BYTES 6

byte font[FONT_ARRAY_LENGTH][FONT_LETTER_NUM_BYTES]  = {
{2, 0x00,0x00,0x00,0x00,0x00 }, //space
{2, 0x80,0x80,0x80,0x00,0x80 }, //exclam
{4, 0xA0,0xA0,0x00,0x00,0x00 }, //quotedbl
{4, 0xA0,0xE0,0xA0,0xE0,0xA0 }, //numbersign
{4, 0x60,0xC0,0x60,0xC0,0x40 }, //dollar
{4, 0x80,0x20,0x40,0x80,0x20 }, //percent
{4, 0xC0,0xC0,0xE0,0xA0,0x60 }, //ampersand
{2, 0x80,0x80,0x00,0x00,0x00 }, //quotesingle
{4, 0x40,0x80,0x80,0x80,0x40 }, //parenleft
{4, 0x80,0x40,0x40,0x40,0x80 }, //parenright
{4, 0x00,0x00,0xA0,0x40,0xA0 }, //asterisk
{4, 0x00,0x00,0x40,0xE0,0x40 }, //plus
{4, 0x00,0x00,0x00,0x40,0x80 }, //comma
{4, 0x00,0x00,0x00,0x00,0xE0 }, //hyphen
{2, 0x00,0x00,0x00,0x00,0x80 }, //period
{4, 0x20,0x20,0x40,0x80,0x80 }, //slash
{4, 0x60,0xA0,0xA0,0xA0,0xC0 }, //zero
{4, 0x40,0xC0,0x40,0x40,0x40 }, //one
{4, 0xC0,0x20,0x40,0x80,0xE0 }, //two
{4, 0xC0,0x20,0x40,0x20,0xC0 }, //three
{4, 0xA0,0xA0,0xE0,0x20,0x20 }, //four
{4, 0xE0,0x80,0xC0,0x20,0xC0 }, //five
{4, 0x60,0x80,0xE0,0xA0,0xE0 }, //six
{4, 0xE0,0x20,0x40,0x80,0x80 }, //seven
{4, 0xE0,0xA0,0xE0,0xA0,0xE0 }, //eight
{4, 0xE0,0xA0,0xE0,0x20,0xC0 }, //nine
{2, 0x00,0x80,0x00,0x80,0x00 }, //colon
{4, 0x00,0x40,0x00,0x40,0x80 }, //semicolon
{4, 0x20,0x40,0x80,0x40,0x20 }, //less
{4, 0x00,0x00,0xE0,0x00,0xE0 }, //equal
{4, 0x80,0x40,0x20,0x40,0x80 }, //greater
{4, 0xE0,0x20,0x40,0x00,0x40 }, //question
// we swapped uppercase and lowercase, so uppercase is the default, but
// you CAN get lowercase if you want
{4, 0x40,0xA0,0xE0,0x80,0x60 }, //at
{4, 0x00,0xC0,0x60,0xA0,0xE0 }, //a
{4, 0x80,0xC0,0xA0,0xA0,0xC0 }, //b
{4, 0x00,0x60,0x80,0x80,0x60 }, //c
{4, 0x20,0x60,0xA0,0xA0,0x60 }, //d
{4, 0x00,0x60,0xA0,0xC0,0x60 }, //e
{4, 0x20,0x40,0xE0,0x40,0x40 }, //f
{4, 0x60,0xA0,0xE0,0x20,0x40 }, //g
{4, 0x80,0xC0,0xA0,0xA0,0xA0 }, //h
{2, 0x80,0x00,0x80,0x80,0x80 }, //i
{4, 0x20,0x00,0x20,0xA0,0x40 }, //j
{4, 0x80,0xA0,0xC0,0xC0,0xA0 }, //k
{4, 0xC0,0x40,0x40,0x40,0xE0 }, //l
{4, 0x00,0xE0,0xE0,0xE0,0xA0 }, //m
{4, 0x00,0xC0,0xA0,0xA0,0xA0 }, //n
{4, 0x00,0x40,0xA0,0xA0,0x40 }, //o
{4, 0xC0,0xA0,0xA0,0xC0,0x80 }, //p
{4, 0x60,0xA0,0xA0,0x60,0x20 }, //q
{4, 0x00,0x60,0x80,0x80,0x80 }, //r
{4, 0x00,0x60,0xC0,0x60,0xC0 }, //s
{4, 0x40,0xE0,0x40,0x40,0x60 }, //t
{4, 0x00,0xA0,0xA0,0xA0,0x60 }, //u
{4, 0x00,0xA0,0xA0,0xE0,0x40 }, //v
{4, 0x00,0xA0,0xE0,0xE0,0xE0 }, //w
{4, 0x00,0xA0,0x40,0x40,0xA0 }, //x
{4, 0xA0,0xA0,0x60,0x20,0x40 }, //y
{4, 0x00,0xE0,0x60,0xC0,0xE0 }, //z
{4, 0xE0,0x80,0x80,0x80,0xE0 }, //bracketleft
{4, 0x00,0x00,0x80,0x40,0x20 }, //backslash
{4, 0xE0,0x20,0x20,0x20,0xE0 }, //bracketright
{4, 0x00,0x00,0x00,0x40,0xA0 }, //asciicircum
{4, 0x00,0x00,0x00,0x00,0xE0 }, //underscore
{4, 0x00,0x00,0x00,0x80,0x40 }, //grave
{4, 0x40,0xA0,0xE0,0xA0,0xA0 }, //A
{4, 0xC0,0xA0,0xC0,0xA0,0xC0 }, //B
{4, 0x60,0x80,0x80,0x80,0x60 }, //C
{4, 0xC0,0xA0,0xA0,0xA0,0xC0 }, //D
{4, 0xE0,0x80,0xE0,0x80,0xE0 }, //E
{4, 0xE0,0x80,0xE0,0x80,0x80 }, //F
{4, 0x60,0x80,0xE0,0xA0,0x60 }, //G
// {4, 0xA0,0xA0,0xE0,0xA0,0xA0 }, //H
{5, 0x90,0x90,0xF0,0x90,0x90 }, //EDITED H
{4, 0xE0,0x40,0x40,0x40,0xE0 }, //I
{4, 0x20,0x20,0x20,0xA0,0x40 }, //J
// {4, 0xA0,0xA0,0xC0,0xA0,0xA0 }, //K
{5, 0x90,0xA0,0xC0,0xA0,0x90 }, //EDITED K
{4, 0x80,0x80,0x80,0x80,0xE0 }, //L
// {5, 0xB0,0xF0,0xD0,0x90,0x90 }, //EDITED //M
{6, 0x88,0xD8,0xA8,0x88,0x88 }, //EDITED //M
{5, 0x90,0xD0,0xB0,0x90,0x90 }, //EDITED //N
{4, 0x40,0xA0,0xA0,0xA0,0x40 }, //O
{4, 0xC0,0xA0,0xC0,0x80,0x80 }, //P
{4, 0x40,0xA0,0xA0,0xE0,0x60 }, //Q
{4, 0xC0,0xA0,0xE0,0xC0,0xA0 }, //R
{4, 0x60,0x80,0x40,0x20,0xC0 }, //S
{4, 0xE0,0x40,0x40,0x40,0x40 }, //T
{4, 0xA0,0xA0,0xA0,0xA0,0x60 }, //U
// {4, 0xA0,0xA0,0xA0,0xA0,0x60 }, //U
{4, 0xA0,0xA0,0xA0,0xC0,0x80 }, //EDITED V
{6, 0x90,0x88,0xA8,0xA8,0x50 }, //EDITED //W
{4, 0xA0,0xA0,0x40,0xA0,0xA0 }, //X
{4, 0xA0,0xA0,0x40,0x40,0x40 }, //Y
{4, 0xE0,0x20,0x40,0x80,0xE0 }, //Z
{4, 0x60,0x40,0x80,0x40,0x60 }, //braceleft
{4, 0x80,0x80,0x00,0x80,0x80 }, //bar
{4, 0xC0,0x40,0x20,0x40,0xC0 }, //braceright
{4, 0x00,0x00,0x00,0x60,0xC0 }, //asciitilde
{4, 0x80,0x00,0x80,0x80,0x80 }, //exclamdown
{4, 0x40,0xE0,0x80,0xE0,0x40 }, //cent
{4, 0x60,0x40,0xE0,0x40,0xE0 }, //sterling
{4, 0xA0,0x40,0xE0,0x40,0xA0 }, //currency
{4, 0xA0,0xA0,0x40,0xE0,0x40 }, //yen
{4, 0x80,0x80,0x00,0x80,0x80 }, //brokenbar
{4, 0x60,0x40,0xA0,0x40,0xC0 }, //section
{4, 0x00,0x00,0x00,0x00,0xA0 }, //dieresis
{4, 0x00,0x00,0x60,0x80,0x60 }, //copyright
{4, 0x60,0xA0,0xE0,0x00,0xE0 }, //ordfeminine
{4, 0x00,0x00,0x40,0x80,0x40 }, //guillemotleft
{4, 0x00,0x00,0x00,0xE0,0x20 }, //logicalnot
{4, 0x00,0x00,0x00,0x00,0xC0 }, //softhyphen
{4, 0x00,0x00,0xC0,0xC0,0xA0 }, //registered
{4, 0x00,0x00,0x00,0x00,0xE0 }, //macron
{4, 0x00,0x00,0x40,0xA0,0x40 }, //degree
{4, 0x40,0xE0,0x40,0x00,0xE0 }, //plusminus
{4, 0x00,0x00,0xC0,0x40,0x60 }, //twosuperior
{4, 0x00,0x00,0xE0,0x60,0xE0 }, //threesuperior
{4, 0x00,0x00,0x00,0x40,0x80 }, //acute
{4, 0xA0,0xA0,0xA0,0xC0,0x80 }, //mu
{4, 0x60,0xA0,0x60,0x60,0x60 }, //paragraph
{4, 0x00,0x00,0xE0,0xE0,0xE0 }, //periodcentered
{4, 0x00,0x00,0x40,0x20,0xC0 }, //cedilla
{4, 0x00,0x00,0x80,0x80,0x80 }, //onesuperior
{4, 0x40,0xA0,0x40,0x00,0xE0 }, //ordmasculine
{4, 0x00,0x00,0x80,0x40,0x80 }, //guillemotright
{4, 0x80,0x80,0x00,0x60,0x20 }, //onequarter
{4, 0x80,0x80,0x00,0xC0,0x60 }, //onehalf
{4, 0xC0,0xC0,0x00,0x60,0x20 }, //threequarters
{4, 0x40,0x00,0x40,0x80,0xE0 }, //questiondown
{4, 0x40,0x20,0x40,0xE0,0xA0 }, //Agrave
{4, 0x40,0x80,0x40,0xE0,0xA0 }, //Aacute
{4, 0xE0,0x00,0x40,0xE0,0xA0 }, //Acircumflex
{4, 0x60,0xC0,0x40,0xE0,0xA0 }, //Atilde
{4, 0xA0,0x40,0xA0,0xE0,0xA0 }, //Adieresis
{4, 0xC0,0xC0,0xA0,0xE0,0xA0 }, //Aring
{4, 0x60,0xC0,0xE0,0xC0,0xE0 }, //AE
{4, 0x40,0x20,0xE0,0xC0,0xE0 }, //Egrave
{4, 0x40,0x80,0xE0,0xC0,0xE0 }, //Eacute
{4, 0xE0,0x00,0xE0,0xC0,0xE0 }, //Ecircumflex
{4, 0xA0,0x00,0xE0,0xC0,0xE0 }, //Edieresis
{4, 0x40,0x20,0xE0,0x40,0xE0 }, //Igrave
{4, 0x40,0x80,0xE0,0x40,0xE0 }, //Iacute
{4, 0xE0,0x00,0xE0,0x40,0xE0 }, //Icircumflex
{4, 0xA0,0x00,0xE0,0x40,0xE0 }, //Idieresis
{4, 0xC0,0xA0,0xE0,0xA0,0xC0 }, //Eth
{4, 0xC0,0x60,0xA0,0xE0,0xA0 }, //Ntilde
{4, 0x40,0x20,0xE0,0xA0,0xE0 }, //Ograve
{4, 0x40,0x80,0xE0,0xA0,0xE0 }, //Oacute
{4, 0xE0,0x00,0xE0,0xA0,0xE0 }, //Ocircumflex
{4, 0xC0,0x60,0xE0,0xA0,0xE0 }, //Otilde
{4, 0xA0,0x00,0xE0,0xA0,0xE0 }, //Odieresis
{4, 0x00,0x00,0xA0,0x40,0xA0 }, //multiply
{4, 0x60,0xA0,0xE0,0xA0,0xC0 }, //Oslash
{4, 0x80,0x40,0xA0,0xA0,0xE0 }, //Ugrave
{4, 0x20,0x40,0xA0,0xA0,0xE0 }, //Uacute
{4, 0xE0,0x00,0xA0,0xA0,0xE0 }, //Ucircumflex
{4, 0xA0,0x00,0xA0,0xA0,0xE0 }, //Udieresis
{4, 0x20,0x40,0xA0,0xE0,0x40 }, //Yacute
{4, 0x80,0xE0,0xA0,0xE0,0x80 }, //Thorn
{4, 0x40,0x20,0x60,0xA0,0xE0 }, //agrave
{4, 0x40,0x80,0x60,0xA0,0xE0 }, //aacute
{4, 0xE0,0x00,0x60,0xA0,0xE0 }, //acircumflex
{4, 0x60,0xC0,0x60,0xA0,0xE0 }, //atilde
{4, 0xA0,0x00,0x60,0xA0,0xE0 }, //adieresis
{4, 0x60,0x60,0x60,0xA0,0xE0 }, //aring
{4, 0x00,0x60,0xE0,0xE0,0xC0 }, //ae
{4, 0x60,0x80,0x60,0x20,0x40 }, //ccedilla
{4, 0x40,0x20,0x60,0xE0,0x60 }, //egrave
{4, 0x40,0x80,0x60,0xE0,0x60 }, //eacute
{4, 0xE0,0x00,0x60,0xE0,0x60 }, //ecircumflex
{4, 0xA0,0x00,0x60,0xE0,0x60 }, //edieresis
{4, 0x80,0x40,0x80,0x80,0x80 }, //igrave
{4, 0x40,0x80,0x40,0x40,0x40 }, //iacute
{4, 0xE0,0x00,0x40,0x40,0x40 }, //icircumflex
{4, 0xA0,0x00,0x40,0x40,0x40 }, //idieresis
{4, 0x60,0xC0,0x60,0xA0,0x60 }, //eth
{4, 0xC0,0x60,0xC0,0xA0,0xA0 }, //ntilde
{4, 0x40,0x20,0x40,0xA0,0x40 }, //ograve
{4, 0x40,0x80,0x40,0xA0,0x40 }, //oacute
{4, 0xE0,0x00,0x40,0xA0,0x40 }, //ocircumflex
{4, 0xC0,0x60,0x40,0xA0,0x40 }, //otilde
{4, 0xA0,0x00,0x40,0xA0,0x40 }, //odieresis
{4, 0x40,0x00,0xE0,0x00,0x40 }, //divide
{4, 0x00,0x60,0xE0,0xA0,0xC0 }, //oslash
{4, 0x80,0x40,0xA0,0xA0,0x60 }, //ugrave
{4, 0x20,0x40,0xA0,0xA0,0x60 }, //uacute
{4, 0xE0,0x00,0xA0,0xA0,0x60 }, //ucircumflex
{4, 0xA0,0x00,0xA0,0xA0,0x60 }, //udieresis
{4, 0x80,0xC0,0xA0,0xC0,0x80 }, //thorn
{4, 0x00,0x00,0x00,0x00,0x00 }, //gcircumflex
{4, 0x60,0xC0,0xE0,0xC0,0x60 }, //OE
{4, 0x00,0x60,0xE0,0xC0,0xE0 }, //oe
{4, 0xA0,0x60,0xC0,0x60,0xC0 }, //Scaron
{4, 0xA0,0x60,0xC0,0x60,0xC0 }, //scaron
{4, 0xA0,0x00,0xA0,0x40,0x40 }, //Ydieresis
{4, 0xA0,0xE0,0x60,0xC0,0xE0 }, //Zcaron
{4, 0xA0,0xE0,0x60,0xC0,0xE0 }, //zcaron
{4, 0x00,0x00,0x00,0x00,0x00 }, //uni0EA4
{4, 0x00,0x00,0x00,0x00,0x00 }, //uni13A0
{4, 0x00,0x00,0x00,0x00,0x80 }, //bullet
{4, 0x00,0x00,0x00,0x00,0xA0 }, //ellipsis
{4, 0x60,0xE0,0xE0,0xC0,0x60 }, //Euro
};//==end of font array==//

#endif
