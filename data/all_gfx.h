//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char numbers_Sprite[11264] __attribute__ ((aligned (4))) ;  // Pal : numbers_Pal
extern const unsigned char numbers2_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : numbers2_Pal
extern const unsigned char icons_Sprite[1536] __attribute__ ((aligned (4))) ;  // Pal : icons_Pal

// Background files : 
extern const unsigned short screen1_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short screen0_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageSeq5_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageSeq4_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageSeq3_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageSeq2_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageSeq1_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageSeq0_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet8_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet7_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet6_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet5_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet4_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet3_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet2_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet1_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet0_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pagebas3_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pagebas2_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pagebas1_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pagebas0_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const unsigned short pageMet3b_Bitmap[49152] __attribute__ ((aligned (4))) ;  // 16bit bitmap

extern const int font_Info[3]; // BgMode, Width, Height
extern const unsigned short font_Map[768] __attribute__ ((aligned (4))) ;  // Pal : font_Pal
extern const unsigned char font_Tiles[12544] __attribute__ ((aligned (4))) ;  // Pal : font_Pal
extern PAGfx_struct font; // background pointer


// Palette files : 
extern const unsigned short numbers_Pal[16] __attribute__ ((aligned (4))) ;
extern const unsigned short numbers2_Pal[16] __attribute__ ((aligned (4))) ;
extern const unsigned short icons_Pal[19] __attribute__ ((aligned (4))) ;
extern const unsigned short font_Pal[2] __attribute__ ((aligned (4))) ;


#endif

