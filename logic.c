#include "logic.h"
#include <stdlib.h>
#include "genlut/lut.h"
#include "myLib.h"

// Check to see whether two balls are colliding
static int check_collision(ball_t *a, ball_t *b) {
    // Find the deltas of their positions
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;

    // Maximum distance is 2 radii, because if balls are just touching their edges are each 1 radius away from center
    fixed_t max_dist = a->radius + b->radius;

    // Calculating the sqrt is slow, so try to rule out false collisions early
    // Check that both dx and dy are less than the max distance
    if (dx > max_dist || dy > max_dist) {
        return 0;
    }

    // Now make sure the Manhattan distance (|dx| + |dy|) is < 4 radii (which would mean they're diagonal from eachother)
    if (abs(dx) + abs(dy) > 2 * max_dist) {
        return 0;
    }

    // Calculate the square root of the distance
    fixed_t d = FIXED_SQRT(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));
    
    return d <= max_dist;
}

// Check if the ball is in a pocket
static int check_pocket_collision(ball_t *ball) {
    // These positions are kind of hard coded (since the pockets really only exist in the background image)

    // First check the top set of pockets
    if (ball->y <= INT_TO_FIXED(20 + 8)) {
        // Left
        if (ball->x <= INT_TO_FIXED(20 + 8)) {
            return 1;
        }
        // Middle
        if (ball->x >= INT_TO_FIXED((WIDTH >> 1) - 8) && ball->x <= INT_TO_FIXED((WIDTH >> 1) + 8)) {
            return 1;
        }
        // Right
        if (ball->x >= INT_TO_FIXED(WIDTH - 20 - 8)) {
            return 1;
        }
    } else if (ball->y >= INT_TO_FIXED(HEIGHT - 20 - 8)) { // Bottom set of pockets
        // Left
        if (ball->x <= INT_TO_FIXED(20 + 8)) {
            return 1;
        }
        // Middle
        if (ball->x >= INT_TO_FIXED((WIDTH >> 1) - 8) && ball->x <= INT_TO_FIXED((WIDTH >> 1) + 8)) {
            return 1;
        }
        // Right
        if (ball->x >= INT_TO_FIXED(WIDTH - 20 - 8)) {
            return 1;
        }
    }
    // If the ball wasn't in any of these locations, it's not in a pocket
    return 0;
}

// Calculate the movement for the ball
static void update_ball(ball_t *ball) {
    // The ball moves by the amount of the velocity
    ball->x += ball->vx;
    ball->y += ball->vy;

    // Remember, velocities are fixed point
    // vx, vy decay from friction
    fixed_t oldvx = ball->vx, oldvy = ball->vy;
    ball->vx = FIXED_MULT(ball->vx, FIXED_ONE - FRICTION);
    ball->vy = FIXED_MULT(ball->vy, FIXED_ONE - FRICTION);

    // If friction calculation didn't decrease the value of vx/vy, just stop the balls 
    // This can happen if the vx/vy are too small, because multiplying by e.g. .9 won't
    // change the number
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

// Check if there's any balls still moving (so we know when to let the player shoot again)
static int no_balls_moving(AppState *state) {
    // Check the cue ball
    if (state->cue_ball->vx != 0 || state->cue_ball->vy != 0) {
        return 0;
    }
    // And then the other balls
    for (int i = 0; i < 15; i++) {
        // Doesn't matter if it's moving if the ball is in a pocket
        if (state->balls[i]->alive == ENTITY_DEAD) {
            continue;
        }
        if (state->balls[i]->vx != 0 || state->balls[i]->vy != 0) {
            return 0;
        }
    }
    // Return true if none of the balls were moving
    return 1;
}

// Check if all (non-8 ball) balls are in pockets
static int all_dead(AppState *state) {
    for (int i = 0; i < 15; i++) {
        if (i == 7) // Not including 8 ball
            continue;
        if (state->balls[i]->alive == ENTITY_ALIVE)
            return 0;
    }
    // Return true if none of the balls were alive
    return 1;
}

// Basic collision resolution (get balls unstuck)
static void collide_static(ball_t *a, ball_t *b) {
    // Delta (distance) between balls
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;
    fixed_t d = FIXED_SQRT(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));
    // Overlap = how much we'll need to push the balls apart
    fixed_t overlap = d - a->radius - b->radius;

    // Now shift the balls in opposite directions based on their overlap
    fixed_t amt_to_move = FIXED_MULT(overlap, FIXED_DIV(dx, d)) >> 1;

    a->x -= amt_to_move;
    a->y -= amt_to_move;

    b->x += amt_to_move;
    b->y += amt_to_move;
}

// Dynamic collision resolution (bounce balls)
static void collide_dynamic(ball_t *a, ball_t *b) {
    // Lots of physics stuff I don't fully understand :O

    // Delta (distance) between balls
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;
    fixed_t d = FIXED_SQRT(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));

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

// Set the default AppState stuff for a game startup
void initializeAppState(AppState *appState) {
    appState->gameOver = 0;
    appState->score = 0;
    appState->turns = 0;

    cue_t *cue = malloc(sizeof(cue_t));
    cue->angle = INT_TO_FIXED(0);
    cue->dist_from_ball = INT_TO_FIXED(0);
    cue->alive = ENTITY_ALIVE;
    appState->cue = cue;

    // Set the default position of the cue ball to (WIDTH / 8, HEIGHT / 2 - 5)
    ball_t *cue_ball = malloc(sizeof(ball_t));
    cue_ball->radius = INT_TO_FIXED(4);
    cue_ball->x = INT_TO_FIXED(WIDTH >> 3);
    cue_ball->y = INT_TO_FIXED((HEIGHT >> 1) - 5);
    cue_ball->vx = INT_TO_FIXED(0);
    cue_ball->vy = INT_TO_FIXED(0);
    cue_ball->alive = ENTITY_ALIVE;
    appState->cue_ball = cue_ball;

    for (int i = 0; i < 15; i++) {
        appState->balls[i] = malloc(sizeof(ball_t));
        appState->balls[i]->radius = INT_TO_FIXED(5);
        // Don't initialize x, y yet... we'll do those during the racking stage
        appState->balls[i]->vx = INT_TO_FIXED(0);
        appState->balls[i]->vy = INT_TO_FIXED(0);
        appState->balls[i]->alive = ENTITY_ALIVE;
    }

    /*
    The racking order for pool requires 4 things:
    - One at the front (i.e. position 0)
    - Eight in the center of the 3rd row (i.e. position 4)
    - The rear corners (i.e. positions 10 and 14) to be opposite colors (stripes vs. solids)

    To generate a valid rack, we'll first set the known balls. We'll also use a short's
    last 15 bits to represent which balls have been chosen.

    BTW, can't tell if this is a bug in the random function (causing it to almost never return
    the last number), but it seems to be exclusive rather than inclusive. If I use randint(0, 14)
    then it will never return 14, causing the game to freeze when it runs out of balls, leaving
    the 15 ball out of the new rack.
    */

    ball_t *rack[15];
    short balls_used = 0;

    // Choose the one ball for the front
    rack[0] = appState->balls[0];
    balls_used |= (1 << 0);

    // Choose the eight ball for position 4
    rack[4] = appState->balls[7];
    balls_used |= (1 << 7);

    // Choose the back left corner
    int back_left_ball;
    // Keep re-rolling till we get a ball that works
    do {
        // No need to try the 0 ball
        back_left_ball = randint(1, 15);
    } while (balls_used & (1 << back_left_ball));
    rack[10] = appState->balls[back_left_ball];
    balls_used |= (1 << back_left_ball);
    
    // Based on what color the back left corner was, pick the back right corner
    int back_right_ball;
    // Keep re-rolling till we get a ball that works
    do {
        if (back_left_ball < 7) { // Back left was solids
            back_right_ball = randint(8, 15);
        } else { // Back left was stripes
            // No need to try the 0 ball
            back_right_ball = randint(1, 7);
        }
    } while (balls_used & (1 << back_right_ball));
    rack[14] = appState->balls[back_right_ball];
    balls_used |= (1 << back_right_ball);


    // And finally pick everything else
    for (int i = 0; i < 14; i++) {
        if (i == 0 || i == 4 || i == 10 || i == 14) {
            continue; // Don't overwrite the balls we chose already!
        }

        int next_ball;
        do {
            next_ball = randint(1, 15);
        } while (balls_used & (1 << next_ball));
        rack[i] = appState->balls[next_ball];
        balls_used |= (1 << next_ball);
    }

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

    // Now place the balls according to racking order in the racking pattern
    // defined above, relative to the point (.625 * WIDTH, HEIGHT/2 - 25)
    for (int i = 0; i < 15; i++) {
        rack[i]->x = INT_TO_FIXED((WIDTH >> 1) + (WIDTH >> 3) + xs[i]);
        rack[i]->y = INT_TO_FIXED((HEIGHT >> 1) - 25 + ys[i]);
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
// state of your application. This function is kinda behemoth but I find it easier
// to deal with all the state in the same place here.
void processAppState(AppState *state, u32 keysPressedBefore, u32 keysPressedNow) {
    // If nothing is moving, we're in cue mode
    if (no_balls_moving(state)) {
        // Make sure the cue is getting rendered
        state->cue->alive = ENTITY_ALIVE;
        // When the user releases the A button, we take the shot
        if (KEY_JUST_RELEASED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
            // Calculate the strength (basically % pulled back * max strength)
            fixed_t strength = FIXED_MULT(MAX_CUE_STRENGTH, FIXED_DIV(state->cue->dist_from_ball, MAX_CUE_DISTANCE));
            // And the angle to move the ball (= -angle, but normalized to 360 degrees). Needed because the cue is on
            // the opposite side of the ball vs. the way we want it to move
            fixed_t hit_angle = INT_TO_FIXED(360) - state->cue->angle;

            // Translate the strength * the hit angle into a velocity vector
            fixed_t vx = FIXED_MULT(strength, FIXED_COS(hit_angle));
            fixed_t vy = FIXED_MULT(strength, FIXED_SIN(hit_angle));

            state->cue_ball->vx = vx;
            state->cue_ball->vy = vy;

            // Now reset the cue because we finished the cue mode
            state->cue->dist_from_ball = 0;
            state->cue->alive = ENTITY_DEAD;
            state->turns++; // Increment turns counter
        } else if (KEY_DOWN(BUTTON_A, keysPressedNow)) {
            // If the button is still getting held, just keep increasing until we hit the max distance
            if (state->cue->dist_from_ball < MAX_CUE_DISTANCE) {
                state->cue->dist_from_ball += FIXED_ONE >> 1;
            }
        }

        // Allow the user to keep aiming after drawing back the cue
        if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
            // If the cue angle drops below 0 degrees, add 360 to normalize it
            if (state->cue->angle <= 0) {
                state->cue->angle += INT_TO_FIXED(360);
            }
            state->cue->angle -= FIXED_ONE;
        } else if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
            // If the cue angle goes above 360 degrees, subtract 360 to normalize it
            if (state->cue->angle >= INT_TO_FIXED(360)) {
                state->cue->angle -= INT_TO_FIXED(360);
            }
            state->cue->angle += FIXED_ONE;
        }
    } else { // Not cue mode, because balls are still moving
        // Just calculate motion for all the balls
        update_ball(state->cue_ball);

        for (int i = 0; i < 15; i++) {
            update_ball(state->balls[i]);
        }

        // Check for scratches (cue ball in pocket)
        if (check_pocket_collision(state->cue_ball)) {
            // Reset to the break position & remove velocity to stop motion + decrement score for a penalty
            state->cue_ball->x = INT_TO_FIXED(WIDTH >> 3);
            state->cue_ball->y = INT_TO_FIXED((HEIGHT >> 1) - 5);
            state->cue_ball->vx = 0;
            state->cue_ball->vy = 0;
            state->score--;
        }

        // Check for losses (8 ball in pocket when others are on the board)
        if (check_pocket_collision(state->balls[7])) {
            if (all_dead(state)) { // No more balls left, we're good!
                state->score++;
                state->balls[7]->alive = ENTITY_DEAD;
                state->gameOver = GAME_OVER_WIN;
            } else { // Got the 8 ball in before other balls, so you lose
                state->balls[7]->alive = ENTITY_DEAD;
                state->gameOver = GAME_OVER_LOSS;
            }
            return; // Game over so just skip the rest of this function and yield back to main loop
        }

        // Check collision of all the balls
        // There's probably a way to cut out repeated comparisons
        for (int i = 0; i < 15; i++) {
            // Skip balls that are in pockets
            if (state->balls[i]->alive == ENTITY_DEAD) {
                continue;
            }

            // Check to see if this ball is in a pocket, and "kill" it if so
            if (check_pocket_collision(state->balls[i])) {
                state->balls[i]->alive = ENTITY_DEAD;
                state->score++;
                continue; // Don't check collision
            }

            // Check against this ball's collisions the cue ball
            if (check_collision(state->cue_ball, state->balls[i])) {
                // Note, we first resolve static collisions (so that balls don't get stuck) and then dynamic collisions
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
