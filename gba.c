#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
   while (*SCANLINECOUNTER > 160);
   while (*SCANLINECOUNTER < 160);
   vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void) {
    __qran_seed = 1664525 * __qran_seed + 1013904223;
    return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, u16 color) {
    videoBuffer[OFFSET(y, x, WIDTH)] = color;
}

void drawRectDMA(int x, int y, int width, int height, volatile u16 color) {
    DMA[3].cnt = 0; // Clear control to stop DMA transfer
    DMA[3].src = (const void *) &color;
    for (int i = 0; i < height; i++) {
        DMA[3].dst = &videoBuffer[OFFSET(y + i, x, WIDTH)];
        DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
    }
}

void drawFullScreenImageDMA(u16 *image) {
    DMA[3].cnt = 0; // Clear control to stop DMA transfer
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = (WIDTH * HEIGHT) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
}

void drawImageDMA(int x, int y, int width, int height, u16 *image) {
    DMA[3].cnt = 0; // Clear control to stop DMA transfer
    for (int i = 0; i < height; i++) {
        // Is i * width the right calculation? What if the image is wider than width
        DMA[3].src = &image[i * width];
        DMA[3].dst = &videoBuffer[OFFSET(y + i, x, WIDTH)];
        DMA[3].cnt = width | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
    }
}

void fillScreenDMA(volatile u16 color) {
    DMA[3].cnt = 0; // Clear control to stop DMA transfer
    DMA[3].src = (const void *) &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = (WIDTH * HEIGHT) | DMA_SOURCE_FIXED | DMA_ON;
}

void drawChar(int col, int row, char ch, u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}

// From https://www.coranac.com/tonc/text/regobj.htm

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE *) obj_buffer;

void oam_init(OBJ_ATTR *obj, u32 count)
{
    u32 nn = count;
    u32 *dst = (u32 *) obj;

    // Hide each object
    while(nn--)
    {
        *dst++ = ATTR0_HIDE;
        *dst++ = 0;
    }
    // init oam
    oam_copy(oam_mem, obj, count);
}

void oam_copy(OBJ_ATTR *dst, const OBJ_ATTR *src, u32 count)
{

// NOTE: while struct-copying is the Right Thing to do here, 
//   there's a strange bug in DKP that sometimes makes it not work
//   If you see problems, just use the word-copy version.
#if 1
    while(count--)
        *dst++ = *src++;
#else
    u32 *dstw = (u32 *) dst, *srcw = (u32 *) src;
    while(count--)
    {
        *dstw++ = *srcw++;
        *dstw++ = *srcw++;
    }
#endif

}

inline void obj_set_pos(OBJ_ATTR *obj, int x, int y) {
    BF_SET(obj->attr0, y, ATTR0_Y);
    BF_SET(obj->attr1, x, ATTR1_X);
}

inline void obj_hide(OBJ_ATTR *obj) {
    obj->attr0 |= ATTR0_HIDE;
}

inline void obj_aff_hide(OBJ_ATTR *obj) {
    obj->attr0 &= ~ATTR0_AFF;
    obj->attr0 |= ATTR0_HIDE;
}

inline void obj_unhide(OBJ_ATTR *obj) {
    obj->attr0 &= ~ATTR0_HIDE;
}

inline void obj_aff_unhide(OBJ_ATTR *obj) {
    obj->attr0 |= ATTR0_AFF;
    obj->attr0 &= ~ATTR0_HIDE;
}
