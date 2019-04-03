#include "logic.h"
#include <stdlib.h>
#include "genlut/lut.h"

#define FRICTION 8

/* Would love to use a LUT for this but there's some constraints:
   - Seems like precision of 1.0 doesn't cut it?
   - Hard to constrain to a certain range of values
   - Doesn't wrap around like sin/cos, so hard to say what to do for OoB input
   - Mostly, I just can't get this stuff to work with a LUT for some reason */
static fixed_t fixed_sqrt(fixed_t f) {
  if (f < INT_TO_FIXED(2)) {
    return f;
  }
  fixed_t next = 2 * fixed_sqrt(f >> 2);
  if (FIXED_MULT((next + FIXED_ONE), (next + FIXED_ONE)) > f) {
    return next;
  } else {
    return next + FIXED_ONE;
  }
}

static int check_collision(ball_t *a, ball_t *b) {
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;

    fixed_t max_dist = a->radius + b->radius;

    // Calculating the sqrt is slow, so try to rule out false collisions early
    if (dx > max_dist || dy > max_dist) {
        return 0;
    }

    // Now make sure the Manhattan distance is < 4r
    if (abs(dx) + abs(dy) > 2 * max_dist) {
        return 0;
    }

    fixed_t d = fixed_sqrt(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));
    
    return d <= max_dist;
}

static void update_ball(ball_t *ball) {
    ball->x += ball->vx;
    ball->y += ball->vy;

    // Remember, velocities are fixed point
    // vx, vy decay from friction
    fixed_t oldvx = ball->vx, oldvy = ball->vy;
    ball->vx = FIXED_MULT(ball->vx, FIXED_ONE - FRICTION);
    ball->vy = FIXED_MULT(ball->vy, FIXED_ONE - FRICTION);
    // If friction calculation didn't work, just stop the balls 
    if (ball->vx == oldvx && ball->vy == oldvy) {
        ball->vx = 0;
        ball->vy = 0;
    }

    // Now reflect if we hit a wall
    
    if (ball->x - ball->radius < INT_TO_FIXED(20)) { // Hit left wall
        ball->x = INT_TO_FIXED(20) + ball->radius + 1;
        ball->vx = -ball->vx;
    } else if (ball->x + ball->radius > INT_TO_FIXED(WIDTH - 20)) { // Hit right wall
        ball->x = INT_TO_FIXED(WIDTH - 20) - ball->radius + 1;
        ball->vx = -ball->vx;
    }

    if (ball->y - ball->radius < INT_TO_FIXED(20)) { // Hit top wall
        ball->y = INT_TO_FIXED(20) + ball->radius + 1;
        ball->vy = -ball->vy;
    } else if (ball->y + ball->radius > INT_TO_FIXED(HEIGHT - 20)) { // Hit top wall
        ball->y = INT_TO_FIXED(HEIGHT - 20) - ball->radius + 1;
        ball->vy = -ball->vy;
    }
}

static void collide(ball_t *a, ball_t *b) {
    // Delta (distance) between balls
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;
    fixed_t d = fixed_sqrt(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));

    // Normal to collision
    // Division is slow, can we get rid of this?
    fixed_t nx = FIXED_DIV(-dx, d);
    fixed_t ny = FIXED_DIV(-dy, d);

    // Tangent
    fixed_t tx = -ny;
    fixed_t ty = nx;

    fixed_t dpTanA = FIXED_MULT(a->vx, tx) + FIXED_MULT(a->vy, ty);
    fixed_t dpTanB = FIXED_MULT(b->vx, tx) + FIXED_MULT(b->vy, ty);

    fixed_t dpNormA = FIXED_MULT(a->vx, nx) + FIXED_MULT(a->vy, ny);
    fixed_t dpNormB = FIXED_MULT(b->vx, nx) + FIXED_MULT(b->vy, ny);

    // We don't need to calculate momentum since mass = 1 for both balls. mA, mB = dpNormB, dpNormA
    a->vx = FIXED_MULT(tx, dpTanA) + FIXED_MULT(nx, dpNormB);
    a->vy = FIXED_MULT(ty, dpTanA) + FIXED_MULT(ny, dpNormB);
    
    b->vx = FIXED_MULT(tx, dpTanB) + FIXED_MULT(nx, dpNormA);
    b->vy = FIXED_MULT(ty, dpTanB) + FIXED_MULT(ny, dpNormA);
}

void initializeAppState(AppState *appState) {
    appState->gameOver = 0;

    cue_t *cue = malloc(sizeof(cue_t));
    cue->color = YELLOW;
    cue->angle = INT_TO_FIXED(0);
    cue->dist_from_ball = INT_TO_FIXED(0);
    cue->alive = ENTITY_ALIVE;
    appState->cue = cue;

    ball_t *cue_ball = malloc(sizeof(ball_t));
    cue_ball->color = WHITE;
    cue_ball->radius = INT_TO_FIXED(5);
    cue_ball->x = INT_TO_FIXED(50);
    cue_ball->y = INT_TO_FIXED(50);
    // Velocities past 6.0 don't work well (overflow?)... Wonder if we can fix this?
    cue_ball->vx = INT_TO_FIXED(0);
    cue_ball->vy = INT_TO_FIXED(0);
    cue_ball->alive = ENTITY_ALIVE;
    appState->cue_ball = cue_ball;

    // Positions to place balls at to form a triangle:
    // (10 pixels per ball, 5 columns + 4 inner columns, 5 rows)
    int xs[] = {
        20,
        15, 25,
        10, 20, 30,
        05, 15, 25, 35,
        00, 10, 20, 30, 40
    };

    int ys[] = {
        0,
        10, 10,
        20, 20, 20,
        30, 30, 30, 30,
        40, 40, 40, 40, 40
    };

    for (int i = 0; i < 15; i++) {
        appState->balls[i] = malloc(sizeof(ball_t));
        appState->balls[i]->color = BLUE;
        appState->balls[i]->radius = INT_TO_FIXED(5);
        appState->balls[i]->x = INT_TO_FIXED(120 + xs[i]);
        appState->balls[i]->y = INT_TO_FIXED(55 + ys[i]);
        appState->balls[i]->vx = INT_TO_FIXED(0);
        appState->balls[i]->vy = INT_TO_FIXED(0);
        appState->balls[i]->alive = ENTITY_ALIVE;
    }
}

void cleanupAppState(AppState *appState) {
    free(appState->cue_ball);

    for (int i = 0; i < 15; i++) {
        free(appState->balls[i]);
    }

    free(appState->cue);
    // Uh... apparently the appState is on the stack oof
    //free(appState);
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */
    AppState nextAppState = *currentAppState;

    // Copy the cue ball into the new state (without reusing the same pointer)
    nextAppState.cue_ball = malloc(sizeof(ball_t));
    *(nextAppState.cue_ball) = *(currentAppState->cue_ball);

    // Copy over the other balls
    for (int i = 0; i < 15; i++) {
        nextAppState.balls[i] = malloc(sizeof(ball_t));
        *(nextAppState.balls[i]) = *(currentAppState->balls[i]);
    }

    // Copy over the cue
    nextAppState.cue = malloc(sizeof(cue_t));
    *(nextAppState.cue) = *(currentAppState->cue);

    // If nothing is moving
    if (nextAppState.cue_ball->vx == 0 && nextAppState.cue_ball->vy == 0) {
        nextAppState.cue->alive = ENTITY_ALIVE;
        if (KEY_JUST_RELEASED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
            fixed_t strength = FIXED_MULT(MAX_CUE_STRENGTH, FIXED_DIV(currentAppState->cue->dist_from_ball, MAX_CUE_DISTANCE));
            fixed_t hit_angle = INT_TO_FIXED(360) - currentAppState->cue->angle;

            fixed_t vx = FIXED_MULT(strength, FIXED_COS(hit_angle));
            fixed_t vy = FIXED_MULT(strength, FIXED_SIN(hit_angle));

            nextAppState.cue_ball->vx = vx;
            nextAppState.cue_ball->vy = vy;

            nextAppState.cue->dist_from_ball = 0;
            nextAppState.cue->alive = ENTITY_DEAD;
        } else if (KEY_DOWN(BUTTON_A, keysPressedNow)) {
            if (currentAppState->cue->dist_from_ball < MAX_CUE_DISTANCE) {
                nextAppState.cue->dist_from_ball += FIXED_ONE;
            }
        } else if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
            if (nextAppState.cue->angle <= 0) {
                nextAppState.cue->angle += INT_TO_FIXED(360);
            }
            nextAppState.cue->angle -= FIXED_ONE;
        } else if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
            if (nextAppState.cue->angle >= INT_TO_FIXED(360)) {
                nextAppState.cue->angle -= INT_TO_FIXED(360);
            }
            nextAppState.cue->angle += FIXED_ONE;
        }
    } else {
        // Just calculate motion for all the balls
        update_ball(nextAppState.cue_ball);

        for (int i = 0; i < 15; i++) {
            update_ball(nextAppState.balls[i]);
        }

        // Check collision of all the balls
        // There's probably a way to cut out repeated comparisons
        for (int i = 0; i < 15; i++) {
            if (check_collision(nextAppState.cue_ball, nextAppState.balls[i])) {
                collide(nextAppState.cue_ball, nextAppState.balls[i]);
            }
            for (int j = 0; j < 15; j++) {
                if (i == j)
                    break;
                if (check_collision(nextAppState.balls[i], nextAppState.balls[j])) {
                    collide(nextAppState.balls[i], nextAppState.balls[j]);
                }
            }
        }

    }

    return nextAppState;
}
