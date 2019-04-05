#include "draw.h"
#include "gba.h"
#include "myLib.h"

#include "images/table.h"
#include "images/startscreen.h"
#include "images/powermeter.h"

#include "genlut/lut.h"

#include <stdio.h>

#define TABLE_COLOR 0x15c6
#define WOOD_COLOR 0x0066

#define POWERBAR_CLEAR_COLOR 0x2108
#define POWERBAR_FILL_COLOR GREEN

// Represents the *MAXIMUM* area covered by the power bar
#define POWERBAR_X (WIDTH - 6)
#define POWERBAR_Y ((HEIGHT >> 1) - (POWERMETER_HEIGHT >> 1) + 3)
#define POWERBAR_WIDTH (4)
#define POWERBAR_HEIGHT ((POWERMETER_HEIGHT) - 6)

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    drawFullScreenImageDMA((u16 *) table);

    // Draw the power meter image at (WIDTH - 15, HEIGHT / 2 - POWERMETER_HEIGHT/2) -- centers it on the Y axis
    drawImageDMA(WIDTH - 15, (HEIGHT >> 1) - (POWERMETER_HEIGHT >> 1), POWERMETER_WIDTH, POWERMETER_HEIGHT, (u16 *) powermeter);

    char buffer[32];

    // Draw score on top left of screen
    sprintf(buffer, "Score: %d", state->score);
    drawString(2, 2, buffer, TABLE_COLOR);

    // And draw turns on top right of screen
    sprintf(buffer, "Turns: %d", state->turns);
    drawString(WIDTH - 60, 2, buffer, TABLE_COLOR);
}

// Draw the title screen
void fullDrawTitleScreen(void) {
    drawFullScreenImageDMA((u16 *) startscreen);

    drawCenteredString(15, HEIGHT - 40, WIDTH - 30, 25, "Welcome to Ball in Hand!", WHITE);
    drawCenteredString(15, HEIGHT - 25, WIDTH - 30, 25, "Press any button to start!", WHITE);
}

void fullDrawGameOverScreen(AppState *state) {
    drawFullScreenImageDMA((u16 *) startscreen);

    // Figure out what game over message to show the user based on the game over state
    switch (state->gameOver) {
    case GAME_OVER_WIN:
        drawCenteredString(15, HEIGHT - 40, WIDTH - 30, 25, "Congratulations! You win!", WHITE);
        drawCenteredString(15, HEIGHT - 25, WIDTH - 30, 25, "Press SELECT to replay!", WHITE);
        break;
    case GAME_OVER_LOSS:
        drawCenteredString(15, HEIGHT - 40, WIDTH - 30, 25, "Ouch, tough loss.", WHITE);
        drawCenteredString(15, HEIGHT - 25, WIDTH - 30, 25, "Press SELECT to replay!", WHITE);
        break;
    }
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    UNUSED(state);

    // Clear the maximum size of the power bar (DMA may or may not be slower than recalculating it from the state)
    /* BTW, you'll notice I'm clearing with the FILL_COLOR here. This is for efficiency, as it's easier to draw from
    the bottom up than it is to draw from the top down. If the powerbar was unfilled by default, we'd have to basically
    figure out the amount to fill and then rather than using that as the height, we have to offset the bottom of the
    fill rectangle from that. This shouldn't be a problem, but converting from fixed point to int is a little fuzzy and
    this causes the bottom of the bar to shift up and down a few pixels on occasion.
    */
    drawRectDMA(POWERBAR_X, POWERBAR_Y, POWERBAR_WIDTH, POWERBAR_HEIGHT, POWERBAR_FILL_COLOR);
    // And then clear the two HUD texts (the score counter and the turn counter)
    drawRectDMA(2, 2, 60, 10, WOOD_COLOR);
    drawRectDMA(WIDTH - 60, 2, 60, 10, WOOD_COLOR);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // This math is a little funky, but see the note above when I'm clearing the bar.
    fixed_t powerbar_fullness = FIXED_ONE - FIXED_DIV(state->cue->dist_from_ball, MAX_CUE_DISTANCE);
    fixed_t powerbar_height = FIXED_MULT(INT_TO_FIXED(POWERBAR_HEIGHT), powerbar_fullness);
    drawRectDMA(POWERBAR_X, POWERBAR_Y, POWERBAR_WIDTH, FIXED_TO_INT(powerbar_height), POWERBAR_CLEAR_COLOR);

    char buffer[32];

    // Draw score on top left of screen
    sprintf(buffer, "Score: %d", state->score);
    drawString(2, 2, buffer, TABLE_COLOR);

    // And draw turns on top right of screen
    sprintf(buffer, "Turns: %d", state->turns);
    drawString(WIDTH - 60, 2, buffer, TABLE_COLOR);
}

void updateSprites(AppState *state, OBJ_ATTR *buffer) {
    // buffer layout = cue ball, balls 1 .. 15, (AFFINE cue stick) cue stick

    // Update the position for the cue ball
    obj_set_pos(&buffer[0], FIXED_TO_INT(state->cue_ball->x), FIXED_TO_INT(state->cue_ball->y));

    // Update the positions for other balls
    for (int i = 0; i < 15; i++) {
        if (state->balls[i]->alive == ENTITY_ALIVE) {
            // Unhide the ball, it's supposed to be on screen
            obj_unhide(&buffer[i + 1]);
            // And update its position
            obj_set_pos(&buffer[i + 1], FIXED_TO_INT(state->balls[i]->x), FIXED_TO_INT(state->balls[i]->y));
        } else {
            // Hide the ball, it must be in a pocket
            obj_hide(&buffer[i + 1]);
        }
    }

    // If we want the cue on screen
    if (state->cue->alive) {
        // Unhide the cue
        obj_aff_unhide(&buffer[16]);

        // Get an affine object for the cue's transformation matrix so we can rotate the cue according to the angle
        OBJ_AFFINE *acue = (OBJ_AFFINE *) &obj_aff_buffer[4];
        
        // Calculate the rotation matrix for that angle
        acue->pa =  FIXED_COS(state->cue->angle);
        acue->pb = -FIXED_SIN(state->cue->angle);
        acue->pc =  FIXED_SIN(state->cue->angle);
        acue->pd =  FIXED_COS(state->cue->angle);

        // Now draw the cue as if its tip is anchored to the ball
        // Math based on https://www.coranac.com/tonc/text/affobj.htm (11.3)
        int tex_x = FIXED_TO_INT(state->cue->dist_from_ball) + 70, tex_y = 32; // Anchor's texture coord (x, y) on the sprite
        int cnt_x = 32, cnt_y = 32; // Texture coords for center (x, y) of the sprite
        fixed_t x = // Calculate the shift along the X axis
            state->cue_ball->x - cnt_x - (FIXED_COS(state->cue->angle) * (tex_x - cnt_x)) + (FIXED_SIN(state->cue->angle) * (tex_y - cnt_y));
        fixed_t y = // And the shift along the y axis
            state->cue_ball->y - cnt_y - (-FIXED_SIN(state->cue->angle) * (tex_x - cnt_x)) + (FIXED_COS(state->cue->angle) * (tex_y - cnt_y));

        // Subtract (28, 28) because that's where the center of the ball is
        obj_set_pos(&buffer[16], FIXED_TO_INT(x) - 28, FIXED_TO_INT(y) - 28);
    } else {
        // Hide the cue, the player doesn't want it on screen
        obj_aff_hide(&buffer[16]);
    }
}
