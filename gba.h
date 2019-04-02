#ifndef GBA_H
#define GBA_H

// ---------------------------------------------------------------------------
//                       USEFUL TYPEDEFS
// ---------------------------------------------------------------------------
/** An unsigned 32-bit (4-byte) type */
typedef unsigned int u32;

/** An unsigned 16-bit (2-byte) type */
typedef unsigned short u16;

/** An unsigned 8-bit (1-byte) type. Note that this type cannot be written onto RAM directly. */
typedef unsigned char u8;

/** A signed 32-bit (4-byte) type */
typedef signed int s32;

/** A signed 16-bit (2-byte) type */
typedef signed short s16;

/** A signed 8-bit (1-byte) type. Note that this type cannot be written onto RAM directly. */
typedef signed char s8;

// ---------------------------------------------------------------------------
//                       MODE3 MACROS
// ---------------------------------------------------------------------------
#define OFFSET(r, c, rowlen) ((c)+(rowlen)*(r))

#define REG_DISPCNT  *(volatile unsigned short *) 0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define SPRITES_ENABLE (1 << 12)
#define ONE_DIM_MAPPING (1 << 6)

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0
#define GRAY COLOR(5, 5, 5)

// The size of the GBA Screen
#define WIDTH 240
#define HEIGHT 160

// This is initialized in gba.c
extern volatile unsigned short *videoBuffer;

// ---------------------------------------------------------------------------
//                       BUTTON INPUT
// ---------------------------------------------------------------------------
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define ANY_KEY			((1 << 10) - 1)

#define BUTTONS *(volatile u32 *) 0x4000130
#define KEY_DOWN(key, buttons) (~(buttons) & (key))

// Remember that a button is recently pressed if it wasn't pressed in the last
// input (oldButtons) but is pressed in the current input. Use the KEY_DOWN
// macro to check if the button was pressed in the inputs.
#define KEY_JUST_PRESSED(key, buttons, oldbuttons) (KEY_DOWN(key, buttons) && !KEY_DOWN(key, oldbuttons))

// ---------------------------------------------------------------------------
//                       DMA
// ---------------------------------------------------------------------------
typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

// ---------------------------------------------------------------------------
//                       VBLANK
// ---------------------------------------------------------------------------
#define SCANLINECOUNTER (volatile unsigned short *)0x4000006

// Use this variable to count vBlanks. Initialized in gba.c and to be
// manipulated by waitForVBlank()
extern u32 vBlankCounter;

/**
 * Runs a blocking loop until the start of next VBlank.
 */
void waitForVBlank(void);

// ---------------------------------------------------------------------------
//                       MISC
// ---------------------------------------------------------------------------
#define UNUSED(param) ((void)((param)))

/**
 * Generates a pseudo-random number between min and max, inclusive.
 *
 * @param  min bottom end of range.
 * @param  max top end of range.
 * @return random number in the given range.
 */
int randint(int min, int max);

// ---------------------------------------------------------------------------
//                       DRAWING
// ---------------------------------------------------------------------------
void setPixel(int x, int y, u16 color);
void drawRectDMA(int x, int y, int width, int height, volatile u16 color);
void drawFullScreenImageDMA(u16 *image);
void drawImageDMA(int x, int y, int width, int height, u16 *image);
void fillScreenDMA(volatile u16 color);
void drawChar(int col, int row, char ch, u16 color);
void drawString(int col, int row, char *str, u16 color);
void drawCenteredString(int col, int row, int width, int height, char *str, u16 color);

// ---------------------------------------------------------------------------
//                       SPRITES
// ---------------------------------------------------------------------------

// See https://www.coranac.com/tonc/text/objbg.htm#ssec-img-cbb
// tile 8x8@4bpp: 32bytes; 8 ints
typedef struct { u32 data[8];  } TILE, TILE4;
// d-tile: double-sized tile (8bpp)
typedef struct { u32 data[16]; } TILE8;
// tile block: 32x16 tiles, 16x16 d-tiles
typedef TILE  CHARBLOCK[512];
typedef TILE8 CHARBLOCK8[256];

#define tile_mem  ((CHARBLOCK*) 0x06000000)
#define tile8_mem ((CHARBLOCK8*) 0x06000000)

#define MEM_PAL (0x05000000)
#define PAL_BG_SIZE (0x00200)
#define MEM_PAL_OBJ (MEM_PAL + PAL_BG_SIZE)
#define pal_obj_mem ((u16 *) MEM_PAL_OBJ)

// See https://www.coranac.com/tonc/text/regobj.htm
typedef struct {
	u16 attr0; // Y coord (8 bits) + attr0 flags
	u16 attr1; // X coord (9 bits) + affine index (5 bits << 9) + attr1 flags
	u16 attr2; // ID (10 bits) + priority (2 bits << 10) + palette bank (4 bits << 0xC)
	s16 fill;
} __attribute__(( aligned(4) )) OBJ_ATTR;

#define ATTR0_Y(Y) (Y)
#define ATTR0_Y_MASK (0xFF)
#define ATTR0_Y_SHIFT (0)

#define ATTR0_MODE_MASK (0x3)
#define ATTR0_MODE_SHIFT (8)
#define ATTR0_REG (0 << 8)
#define ATTR0_AFF (1 << 8)
#define ATTR0_HIDE (2 << 8)
#define ATTR0_AFF_DBL (3 << 8)

#define ATTR0_BLEND (1 << 0xA)
#define ATTR0_WIN (2 << 0xA)

#define ATTR0_MOSAIC (1 << 0xC)

#define ATTR0_4BPP (0 << 0xD)
#define ATTR0_8BPP (1 << 0xD)

#define ATTR0_SHAPE_MASK
#define ATTR0_SQUARE (0 << 0xE)
#define ATTR0_WIDE (1 << 0xE)
#define ATTR0_TALL (2 << 0xE)

#define ATTR1_X(X) (X)
#define ATTR1_X_MASK (0x1FF)
#define ATTR1_X_SHIFT (0)

#define ATTR1_AFFIDX(IDX) ((IDX) << 9)
#define ATTR1_HFLIP (1 << 0xC)
#define ATTR1_VFLIP (1 << 0xD)

#define ATTR1_TINY (0 << 0xE)
#define ATTR1_NORMAL (1 << 0xE)
#define ATTR1_LARGE (2 << 0xE)
#define ATTR1_HUGE (3 << 0xE)

#define ATTR2_ID(ID) (ID)
#define ATTR2_PRI(PRI) ((PRI) << 0xA)
#define ATTR2_PALBANK(PAL) ((PAL) << 0xC)

/*
The affine matrix:
[ pa pb ] = [  s_x * cos(a)  s_y * sin(a) ]
[ pc pd ] = [ -s_x * sin(a)  s_y * cos(a) ]
*/
typedef struct {
	u16 fill0[3];
	s16 pa; // 8.8 fixed
	u16 fill1[3];
	s16 pb; // 8.8 fixed
	u16 fill2[3];
	s16 pc; // 8.8 fixed
	u16 fill3[3];
	s16 pd; // 8.8 fixed
} __attribute__(( aligned(4) )) OBJ_AFFINE;

#define MEM_OAM 0x07000000
#define oam_mem ((OBJ_ATTR *) MEM_OAM)

extern OBJ_ATTR obj_buffer[128];
extern OBJ_AFFINE *obj_aff_buffer;

void oam_init(OBJ_ATTR *obj, u32 count);
void oam_copy(OBJ_ATTR *dst, const OBJ_ATTR *src, u32 count);

// bit field set and get routines
#define BF_PREP(x, name)         ( ((x)<<name##_SHIFT)& name##_MASK  )
#define BF_GET(x, name)          ( ((x) & name##_MASK)>> name##_SHIFT )
#define BF_SET(y, x, name)       (y = ((y)&~name##_MASK) | BF_PREP(x,name) )

#define BF_PREP2(x, name)        ( (x) & name##_MASK )
#define BF_GET2(y, name)         ( (y) & name##_MASK )
#define BF_SET2(y, x, name)      (y = ((y)&~name##_MASK) | BF_PREP2(x, name) )

void obj_set_pos(OBJ_ATTR *obj, int x, int y);
void obj_hide(OBJ_ATTR *obj);
void obj_unhide(OBJ_ATTR *obj);

/** Contains the pixels of each character from a 6x8 font */
// This is in the font.c file. You can replace the font if you want.
extern const unsigned char fontdata_6x8[12288];

#endif
