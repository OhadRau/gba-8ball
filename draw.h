#ifndef DRAW_H
#define DRAW_H

#include "logic.h"

// This function will be used to draw everything about the state of your app
// including the background and whatnot.
void fullDrawAppState(AppState *state);

// This function draws the title screen
void fullDrawTitleScreen(void);

// This function draws the game over screen
void fullDrawGameOverScreen(AppState *state);

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state);

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state);

// This is kinda arguable whether it belongs in draw.c, but it basically is in charge
// of rendering for sprites, and does so by writing updated sprites to the object buffer
void updateSprites(AppState *state, OBJ_ATTR *buffer);

#endif
