/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=lut --func=sin,int.8,0,512,1,1 sin 
 * Time-stamp: Tuesday 04/02/2019, 21:21:28
 * 
 * Look Up Table Information
 * -------------------------
 * Function: sin Type: int.8 Range: [0, 512] Step: 1
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SIN_H
#define SIN_H

extern const int sin_table[513];
#define SIN_TABLE_LENGTH 513
#define SIN_TABLE_FIXED_LENGTH 8
#define SIN_TABLE_BEGIN ((0 << 8) | 0x00)
#define SIN_TABLE_END ((512 << 8) | 0x00)
#define SIN_TABLE_STEP ((1 << 8) | 0x00)

#endif

