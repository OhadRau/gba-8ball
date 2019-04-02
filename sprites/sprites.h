/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites sprites ./cue_ball.png ./cue.png ./eight_ball.png ./eleven_ball.png ./fifteen_ball.png ./five_ball.png ./four_ball.png ./fourteen_ball.png ./nine_ball.png ./one_ball.png ./seven_ball.png ./six_ball.png ./ten_ball.png ./thirteen_ball.png ./three_ball.png ./twelve_ball.png ./two_ball.png 
 * Time-stamp: Tuesday 04/02/2019, 05:24:14
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
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (1 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[11];
#define SPRITES_PALETTE_SIZE 22
#define SPRITES_PALETTE_LENGTH 11

extern const unsigned short sprites[2560];
#define SPRITES_SIZE 5120
#define SPRITES_LENGTH 2560

#define CUE_BALL_PALETTE_ID (0 << 12)
#define CUE_BALL_SPRITE_SHAPE (0 << 14)
#define CUE_BALL_SPRITE_SIZE (0 << 14)
#define CUE_BALL_ID 0

#define CUE_PALETTE_ID (0 << 12)
#define CUE_SPRITE_SHAPE (0 << 14)
#define CUE_SPRITE_SIZE (3 << 14)
#define CUE_ID 2

#define EIGHT_BALL_PALETTE_ID (0 << 12)
#define EIGHT_BALL_SPRITE_SHAPE (0 << 14)
#define EIGHT_BALL_SPRITE_SIZE (0 << 14)
#define EIGHT_BALL_ID 130

#define ELEVEN_BALL_PALETTE_ID (0 << 12)
#define ELEVEN_BALL_SPRITE_SHAPE (0 << 14)
#define ELEVEN_BALL_SPRITE_SIZE (0 << 14)
#define ELEVEN_BALL_ID 132

#define FIFTEEN_BALL_PALETTE_ID (0 << 12)
#define FIFTEEN_BALL_SPRITE_SHAPE (0 << 14)
#define FIFTEEN_BALL_SPRITE_SIZE (0 << 14)
#define FIFTEEN_BALL_ID 134

#define FIVE_BALL_PALETTE_ID (0 << 12)
#define FIVE_BALL_SPRITE_SHAPE (0 << 14)
#define FIVE_BALL_SPRITE_SIZE (0 << 14)
#define FIVE_BALL_ID 136

#define FOUR_BALL_PALETTE_ID (0 << 12)
#define FOUR_BALL_SPRITE_SHAPE (0 << 14)
#define FOUR_BALL_SPRITE_SIZE (0 << 14)
#define FOUR_BALL_ID 138

#define FOURTEEN_BALL_PALETTE_ID (0 << 12)
#define FOURTEEN_BALL_SPRITE_SHAPE (0 << 14)
#define FOURTEEN_BALL_SPRITE_SIZE (0 << 14)
#define FOURTEEN_BALL_ID 140

#define NINE_BALL_PALETTE_ID (0 << 12)
#define NINE_BALL_SPRITE_SHAPE (0 << 14)
#define NINE_BALL_SPRITE_SIZE (0 << 14)
#define NINE_BALL_ID 142

#define ONE_BALL_PALETTE_ID (0 << 12)
#define ONE_BALL_SPRITE_SHAPE (0 << 14)
#define ONE_BALL_SPRITE_SIZE (0 << 14)
#define ONE_BALL_ID 144

#define SEVEN_BALL_PALETTE_ID (0 << 12)
#define SEVEN_BALL_SPRITE_SHAPE (0 << 14)
#define SEVEN_BALL_SPRITE_SIZE (0 << 14)
#define SEVEN_BALL_ID 146

#define SIX_BALL_PALETTE_ID (0 << 12)
#define SIX_BALL_SPRITE_SHAPE (0 << 14)
#define SIX_BALL_SPRITE_SIZE (0 << 14)
#define SIX_BALL_ID 148

#define TEN_BALL_PALETTE_ID (0 << 12)
#define TEN_BALL_SPRITE_SHAPE (0 << 14)
#define TEN_BALL_SPRITE_SIZE (0 << 14)
#define TEN_BALL_ID 150

#define THIRTEEN_BALL_PALETTE_ID (0 << 12)
#define THIRTEEN_BALL_SPRITE_SHAPE (0 << 14)
#define THIRTEEN_BALL_SPRITE_SIZE (0 << 14)
#define THIRTEEN_BALL_ID 152

#define THREE_BALL_PALETTE_ID (0 << 12)
#define THREE_BALL_SPRITE_SHAPE (0 << 14)
#define THREE_BALL_SPRITE_SIZE (0 << 14)
#define THREE_BALL_ID 154

#define TWELVE_BALL_PALETTE_ID (0 << 12)
#define TWELVE_BALL_SPRITE_SHAPE (0 << 14)
#define TWELVE_BALL_SPRITE_SIZE (0 << 14)
#define TWELVE_BALL_ID 156

#define TWO_BALL_PALETTE_ID (0 << 12)
#define TWO_BALL_SPRITE_SHAPE (0 << 14)
#define TWO_BALL_SPRITE_SIZE (0 << 14)
#define TWO_BALL_ID 158

#endif

