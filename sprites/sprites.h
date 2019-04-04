/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=4 --for_bitmap --transparent=FFFFFF sprites ./cue_ball.png ./cue.png ./eight_ball.png ./eleven_ball.png ./fifteen_ball.png ./five_ball.png ./four_ball.png ./fourteen_ball.png ./nine_ball.png ./one_ball.png ./seven_ball.png ./six_ball.png ./ten_ball.png ./thirteen_ball.png ./three_ball.png ./twelve_ball.png ./two_ball.png 
 * Time-stamp: Thursday 04/04/2019, 05:37:00
 * 
 * Image Information
 * -----------------
 * ./cue_ball.png 8@8
 * ./cue.png 64@64
 * ./eight_ball.png 8@8
 * ./eleven_ball.png 8@8
 * ./fifteen_ball.png 8@8
 * ./five_ball.png 8@8
 * ./four_ball.png 8@8
 * ./fourteen_ball.png 8@8
 * ./nine_ball.png 8@8
 * ./one_ball.png 8@8
 * ./seven_ball.png 8@8
 * ./six_ball.png 8@8
 * ./ten_ball.png 8@8
 * ./thirteen_ball.png 8@8
 * ./three_ball.png 8@8
 * ./twelve_ball.png 8@8
 * ./two_ball.png 8@8
 * Transparent color: (255, 255, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (0 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[256];
#define SPRITES_PALETTE_SIZE 512
#define SPRITES_PALETTE_LENGTH 256

extern const unsigned short sprites[1280];
#define SPRITES_SIZE 2560
#define SPRITES_LENGTH 1280

#define CUE_PALETTE_ID (0 << 12)
#define CUE_SPRITE_SHAPE (0 << 14)
#define CUE_SPRITE_SIZE (3 << 14)
#define CUE_ID 512

#define TWO_BALL_PALETTE_ID (0 << 12)
#define TWO_BALL_SPRITE_SHAPE (0 << 14)
#define TWO_BALL_SPRITE_SIZE (0 << 14)
#define TWO_BALL_ID 576

#define TWELVE_BALL_PALETTE_ID (0 << 12)
#define TWELVE_BALL_SPRITE_SHAPE (0 << 14)
#define TWELVE_BALL_SPRITE_SIZE (0 << 14)
#define TWELVE_BALL_ID 577

#define THREE_BALL_PALETTE_ID (0 << 12)
#define THREE_BALL_SPRITE_SHAPE (0 << 14)
#define THREE_BALL_SPRITE_SIZE (0 << 14)
#define THREE_BALL_ID 578

#define THIRTEEN_BALL_PALETTE_ID (0 << 12)
#define THIRTEEN_BALL_SPRITE_SHAPE (0 << 14)
#define THIRTEEN_BALL_SPRITE_SIZE (0 << 14)
#define THIRTEEN_BALL_ID 579

#define TEN_BALL_PALETTE_ID (0 << 12)
#define TEN_BALL_SPRITE_SHAPE (0 << 14)
#define TEN_BALL_SPRITE_SIZE (0 << 14)
#define TEN_BALL_ID 580

#define SIX_BALL_PALETTE_ID (0 << 12)
#define SIX_BALL_SPRITE_SHAPE (0 << 14)
#define SIX_BALL_SPRITE_SIZE (0 << 14)
#define SIX_BALL_ID 581

#define SEVEN_BALL_PALETTE_ID (0 << 12)
#define SEVEN_BALL_SPRITE_SHAPE (0 << 14)
#define SEVEN_BALL_SPRITE_SIZE (0 << 14)
#define SEVEN_BALL_ID 582

#define ONE_BALL_PALETTE_ID (0 << 12)
#define ONE_BALL_SPRITE_SHAPE (0 << 14)
#define ONE_BALL_SPRITE_SIZE (0 << 14)
#define ONE_BALL_ID 583

#define NINE_BALL_PALETTE_ID (0 << 12)
#define NINE_BALL_SPRITE_SHAPE (0 << 14)
#define NINE_BALL_SPRITE_SIZE (0 << 14)
#define NINE_BALL_ID 584

#define FOURTEEN_BALL_PALETTE_ID (0 << 12)
#define FOURTEEN_BALL_SPRITE_SHAPE (0 << 14)
#define FOURTEEN_BALL_SPRITE_SIZE (0 << 14)
#define FOURTEEN_BALL_ID 585

#define FOUR_BALL_PALETTE_ID (0 << 12)
#define FOUR_BALL_SPRITE_SHAPE (0 << 14)
#define FOUR_BALL_SPRITE_SIZE (0 << 14)
#define FOUR_BALL_ID 586

#define FIVE_BALL_PALETTE_ID (0 << 12)
#define FIVE_BALL_SPRITE_SHAPE (0 << 14)
#define FIVE_BALL_SPRITE_SIZE (0 << 14)
#define FIVE_BALL_ID 587

#define FIFTEEN_BALL_PALETTE_ID (0 << 12)
#define FIFTEEN_BALL_SPRITE_SHAPE (0 << 14)
#define FIFTEEN_BALL_SPRITE_SIZE (0 << 14)
#define FIFTEEN_BALL_ID 588

#define ELEVEN_BALL_PALETTE_ID (0 << 12)
#define ELEVEN_BALL_SPRITE_SHAPE (0 << 14)
#define ELEVEN_BALL_SPRITE_SIZE (0 << 14)
#define ELEVEN_BALL_ID 589

#define EIGHT_BALL_PALETTE_ID (0 << 12)
#define EIGHT_BALL_SPRITE_SHAPE (0 << 14)
#define EIGHT_BALL_SPRITE_SIZE (0 << 14)
#define EIGHT_BALL_ID 590

#define CUE_BALL_PALETTE_ID (0 << 12)
#define CUE_BALL_SPRITE_SHAPE (0 << 14)
#define CUE_BALL_SPRITE_SIZE (0 << 14)
#define CUE_BALL_ID 591

#endif

