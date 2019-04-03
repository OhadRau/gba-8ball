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

static int check_pocket_collision(ball_t *ball) {
    if (ball->y <= INT_TO_FIXED(20 + 8)) {
        if (ball->x <= INT_TO_FIXED(20 + 8)) {
            return 1;
        }
        if (ball->x >= INT_TO_FIXED((WIDTH >> 1) - 8) && ball->x <= INT_TO_FIXED((WIDTH >> 1) + 8)) {
            return 1;
        }
        if (ball->x >= INT_TO_FIXED(WIDTH - 20 - 8)) {
            return 1;
        }
    } else if (ball->y >= INT_TO_FIXED(HEIGHT - 20 - 8)) {
        if (ball->x <= INT_TO_FIXED(20 + 8)) {
            return 1;
        }
        if (ball->x >= INT_TO_FIXED((WIDTH >> 1) - 8) && ball->x <= INT_TO_FIXED((WIDTH >> 1) + 8)) {
            return 1;
        }
        if (ball->x >= INT_TO_FIXED(WIDTH - 20 - 8)) {
            return 1;
        }
    }
    return 0;
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

static int no_balls_moving(AppState *state) {
    if (state->cue_ball->vx != 0 || state->cue_ball->vy != 0) {
        return 0;
    }
    for (int i = 0; i < 15; i++) {
        // Doesn't matter if it's moving if the ball is in a pocket
        if (state->balls[i]->alive == ENTITY_DEAD) {
            continue;
        }
        if (state->balls[i]->vx != 0 || state->balls[i]->vy != 0) {
            return 0;
        }
    }
    return 1;
}

// Basic collision resolution (get balls unstuck)
static void collide_static(ball_t *a, ball_t *b) {
    // Delta (distance) between balls
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;
    fixed_t d = fixed_sqrt(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));
    fixed_t overlap = d - a->radius - b->radius;

    a->x -= FIXED_MULT(overlap, FIXED_DIV(dx, d)) >> 1;
    a->y -= FIXED_MULT(overlap, FIXED_DIV(dy, d)) >> 1;

    b->x += FIXED_MULT(overlap, FIXED_DIV(dx, d)) >> 1;
    b->y += FIXED_MULT(overlap, FIXED_DIV(dy, d)) >> 1;
}

// Dynamic collision resolution (bounce balls)
static void collide_dynamic(ball_t *a, ball_t *b) {
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
    appState->score = 0;
    appState->turns = 0;

    cue_t *cue = malloc(sizeof(cue_t));
    cue->color = YELLOW;
    cue->angle = INT_TO_FIXED(0);
    cue->dist_from_ball = INT_TO_FIXED(0);
    cue->alive = ENTITY_ALIVE;
    appState->cue = cue;

    ball_t *cue_ball = malloc(sizeof(ball_t));
    cue_ball->color = WHITE;
    cue_ball->radius = INT_TO_FIXED(5);
    cue_ball->x = INT_TO_FIXED(WIDTH >> 3);
    cue_ball->y = INT_TO_FIXED((HEIGHT >> 1) - 5);
    // Velocities past 6.0 don't work well (overflow?)... Wonder if we can fix this?
    cue_ball->vx = INT_TO_FIXED(0);
    cue_ball->vy = INT_TO_FIXED(0);
    cue_ball->alive = ENTITY_ALIVE;
    appState->cue_ball = cue_ball;

    // Positions to place balls at to form a triangle:
    // (10 pixels per ball, 5 columns + 4 inner columns, 5 rows)
    int xs[] = {
        0,
        10, 10,
        20, 20, 20,
        30, 30, 30, 30,
        40, 40, 40, 40, 40
    };

    int ys[] = {
        20,
        15, 25,
        10, 20, 30,
        05, 15, 25, 35,
        00, 10, 20, 30, 40
    };

    for (int i = 0; i < 15; i++) {
        appState->balls[i] = malloc(sizeof(ball_t));
        appState->balls[i]->color = BLUE;
        appState->balls[i]->radius = INT_TO_FIXED(5);
        appState->balls[i]->x = INT_TO_FIXED((WIDTH >> 1) + (WIDTH >> 3) + xs[i]);
        appState->balls[i]->y = INT_TO_FIXED((HEIGHT >> 1) - 25 + ys[i]);
        appState->balls[i]->vx = INT_TO_FIXED(0);
        appState->balls[i]->vy = INT_TO_FIXED(0);
        appState->balls[i]->alive = ENTITY_ALIVE;
    }
}

// Free all dynamically allocated memory from the app state
void cleanupAppState(AppState *state) {
    free(state->cue_ball);

    for (int i = 0; i < 15; i++) {
        free(state->balls[i]);
    }

    free(state->cue);
}

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
void processAppState(AppState *state, u32 keysPressedBefore, u32 keysPressedNow) {
    // If nothing is moving
    if (no_balls_moving(state)) {
        state->cue->alive = ENTITY_ALIVE;
        if (KEY_JUST_RELEASED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
            fixed_t strength = FIXED_MULT(MAX_CUE_STRENGTH, FIXED_DIV(state->cue->dist_from_ball, MAX_CUE_DISTANCE));
            fixed_t hit_angle = INT_TO_FIXED(360) - state->cue->angle;

            fixed_t vx = FIXED_MULT(strength, FIXED_COS(hit_angle));
            fixed_t vy = FIXED_MULT(strength, FIXED_SIN(hit_angle));

            state->cue_ball->vx = vx;
            state->cue_ball->vy = vy;

            state->cue->dist_from_ball = 0;
            state->cue->alive = ENTITY_DEAD;
            state->turns++; // Increment turns counter
        } else if (KEY_DOWN(BUTTON_A, keysPressedNow)) {
            if (state->cue->dist_from_ball < MAX_CUE_DISTANCE) {
                state->cue->dist_from_ball += FIXED_ONE;
            }
        } else if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
            if (state->cue->angle <= 0) {
                state->cue->angle += INT_TO_FIXED(360);
            }
            state->cue->angle -= FIXED_ONE;
        } else if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
            if (state->cue->angle >= INT_TO_FIXED(360)) {
                state->cue->angle -= INT_TO_FIXED(360);
            }
            state->cue->angle += FIXED_ONE;
        }
    } else {
        // Just calculate motion for all the balls
        update_ball(state->cue_ball);

        for (int i = 0; i < 15; i++) {
            update_ball(state->balls[i]);
        }

        // Check collision of all the balls
        // There's probably a way to cut out repeated comparisons
        for (int i = 0; i < 15; i++) {
            // Skip balls that are in pockets
            if (state->balls[i]->alive == ENTITY_DEAD) {
                continue;
            }

            // Check to see if the ball is in a pocket
            if (check_pocket_collision(state->balls[i])) {
                state->balls[i]->alive = ENTITY_DEAD;
                state->score++;
                continue; // Don't check collision
            }

            // Check against the cue ball
            if (check_collision(state->cue_ball, state->balls[i])) {
                collide_static(state->cue_ball, state->balls[i]);
                collide_dynamic(state->cue_ball, state->balls[i]);
            }

            // Check against otehr balls
            for (int j = 0; j < 15; j++) {
                // Skip balls that are in pockets
                if (state->balls[j]->alive == ENTITY_DEAD) {
                    continue;
                }

                // Don't compare to itself
                if (i == j)
                    continue;
                
                if (check_collision(state->balls[i], state->balls[j])) {
                    collide_static(state->balls[i], state->balls[j]);
                    collide_dynamic(state->balls[i], state->balls[j]);
                }
            }
        }
    }
}
