#include "draw.h"
#include "gba.h"
#include "myLib.h"

#include "images/table.h"
#include "images/startscreen.h"

#include "genlut/lut.h"

#include <stdio.h>

//#define TABLE_COLOR COLOR(0, 12, 5)
//#define WOOD_COLOR COLOR(6, 2, 0)
#define TABLE_COLOR 0x15c6
#define WOOD_COLOR 0x0066

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    char buffer[32];
    sprintf(buffer, "Score: %d", state->score);
    drawString(2, 2, buffer, TABLE_COLOR);

    sprintf(buffer, "Turns: %d", state->turns);
    drawString(WIDTH - 60, 2, buffer, TABLE_COLOR);

    drawFullScreenImageDMA((u16 *) table);
}

// Draw the title screen
void fullDrawTitleScreen(void) {
    drawFullScreenImageDMA((u16 *) startscreen);

    drawCenteredString(15, HEIGHT - 40, WIDTH - 30, 25, "Welcome to Ball in Hand!", WHITE);
    drawCenteredString(15, HEIGHT - 25, WIDTH - 30, 25, "Press any button to start!", WHITE);
}

void fullDrawGameOverScreen(AppState *state) {
    drawFullScreenImageDMA((u16 *) startscreen);

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
    drawRectDMA(2, 2, 60, 10, WOOD_COLOR);
    drawRectDMA(WIDTH - 60, 2, 60, 10, WOOD_COLOR);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    char buffer[32];
    sprintf(buffer, "Score: %d", state->score);
    drawString(2, 2, buffer, TABLE_COLOR);

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

        // Rotate the cue according to the angle
        OBJ_AFFINE *acue = (OBJ_AFFINE *) &obj_aff_buffer[4];
        
        acue->pa =  FIXED_COS(state->cue->angle);
        acue->pb = -FIXED_SIN(state->cue->angle);
        acue->pc =  FIXED_SIN(state->cue->angle);
        acue->pd =  FIXED_COS(state->cue->angle);

        // Now draw the cue as if its tip is anchored to the ball
        // Math based on https://www.coranac.com/tonc/text/affobj.htm (11.3)
        int tex_x = FIXED_TO_INT(state->cue->dist_from_ball) + 70, tex_y = 32;
        int cnt_x = 32, cnt_y = 32;
        fixed_t x =
            state->cue_ball->x - cnt_x - (FIXED_COS(state->cue->angle) * (tex_x - cnt_x)) + (FIXED_SIN(state->cue->angle) * (tex_y - cnt_y));
        fixed_t y =
            state->cue_ball->y - cnt_y - (-FIXED_SIN(state->cue->angle) * (tex_x - cnt_x)) + (FIXED_COS(state->cue->angle) * (tex_y - cnt_y));

        obj_set_pos(&buffer[16], FIXED_TO_INT(x) - 28, FIXED_TO_INT(y) - 28);
    } else {
        // Hide the cue, the player doesn't want it on screen
        obj_aff_hide(&buffer[16]);
    }
}
