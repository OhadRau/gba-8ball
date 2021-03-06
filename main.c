#include "gba.h"
#include "logic.h"
#include "draw.h"
#include "myLib.h"

#include "sprites/sprites.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AppState enum definition
typedef enum {
    START,
    TITLE_SCREEN,
    APP_INIT,
    APP,
    APP_EXIT,
    APP_EXIT_NODRAW,
} GBAState;

// Initialize a ball's sprite to a non-affine, transparent, 4bit-per-pixel (AKA palette) sprite
#define INIT_BALL(LOCATION, NAME) \
    (LOCATION) = (OBJ_ATTR) { \
        .attr0 = (s16) (ATTR0_Y(0) | ATTR0_REG | ATTR0_BLEND | ATTR0_4BPP | NAME##_BALL_SPRITE_SHAPE), \
        .attr1 = (s16) (ATTR1_X(0) | NAME##_BALL_SPRITE_SIZE), \
        .attr2 = (s16) (ATTR2_ID(NAME##_BALL_ID) | ATTR2_PALBANK(NAME##_BALL_PALETTE_ID)) \
    };

void init_objects(void) {
    OBJ_ATTR *oballs = &obj_buffer[0];

    INIT_BALL(oballs[0], CUE); // AFF0
    INIT_BALL(oballs[1], ONE);
    INIT_BALL(oballs[2], TWO);
    INIT_BALL(oballs[3], THREE);
    INIT_BALL(oballs[4], FOUR); // AFF1
    INIT_BALL(oballs[5], FIVE);
    INIT_BALL(oballs[6], SIX);
    INIT_BALL(oballs[7], SEVEN);
    INIT_BALL(oballs[8], EIGHT); // AFF2
    INIT_BALL(oballs[9], NINE);
    INIT_BALL(oballs[10], TEN);
    INIT_BALL(oballs[11], ELEVEN);
    INIT_BALL(oballs[12], TWELVE); // AFF3
    INIT_BALL(oballs[13], THIRTEEN);
    INIT_BALL(oballs[14], FOURTEEN);
    INIT_BALL(oballs[15], FIFTEEN);

    // Init cue *affine* sprite with similar settings, but enable ATTR0_AFF and pass in the affine index
    OBJ_ATTR *ocue = (OBJ_ATTR *) &obj_buffer[16]; // AFF4
    *ocue = (OBJ_ATTR) {
        .attr0 = (ATTR0_Y(0) | ATTR0_AFF | ATTR0_BLEND | ATTR0_4BPP | CUE_SPRITE_SHAPE),
        .attr1 = (ATTR1_X(0) | ATTR1_AFFIDX(4) | CUE_SPRITE_SIZE),
        .attr2 = (ATTR2_ID(CUE_ID) | ATTR2_PALBANK(CUE_PALETTE_ID))
    };

    // Reference the affine object (i.e. transformation matrix) for the cue
    OBJ_AFFINE *acue = (OBJ_AFFINE *) &obj_aff_buffer[4];
    
    // Set transformation matrix to identity matrix
    acue->pa = FIXED_ONE;
    acue->pb = 0;
    acue->pc = 0;
    acue->pd = FIXED_ONE;
}

void hide_objects(void) {
    for (int i = 0; i < 16; i++) {
        obj_hide(&obj_buffer[i]);
    }
    obj_aff_hide(&obj_buffer[16]);
}

int main(void) {
    // Copy tiles & palettes for sprites into object memory
    memcpy(&tile_mem[5][0], sprites, SPRITES_SIZE);
    memcpy(pal_obj_mem, sprites_palette, SPRITES_PALETTE_SIZE);

    oam_init(obj_buffer, 128);

    REG_DISPCNT = MODE3 | BG2_ENABLE | SPRITES_ENABLE | SPRITES_DIMENSION_TYPE;

    GBAState state = START;

    // We store the current state.
    AppState appState;

    // We store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    while (1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            // Free all used memory (resets game state)
            cleanupAppState(&appState);
            // Hide all objects in the buffer
            hide_objects();
            // Apply the object buffer
            oam_copy(oam_mem, obj_buffer, 16 + 4);
            // Set the game to the start state
            state = START;
        }

        // TA-TODO: Manipulate the state machine below as needed.
        switch(state) {
            case START:
                // Wait for VBlank
                waitForVBlank();

                // TA-TODO: Draw the start state here.
                fullDrawTitleScreen();
                
                state = TITLE_SCREEN;
                break;
            case TITLE_SCREEN:
                // TA-TODO: Check for a button press here to start the app.
                // Start the app by switching the state to APP_INIT.
                if (KEY_JUST_PRESSED(ANY_KEY, currentButtons, previousButtons)) {
                    state = APP_INIT;
                }
                break;
            case APP_INIT:
                // Initialize the sprites
                init_objects();

                // Initialize the app. Switch to the APP state.
                initializeAppState(&appState);
                updateSprites(&appState, obj_buffer);

                // Draw the initial state of the app
                fullDrawAppState(&appState);

                // 16 balls (obj) + 1 cue (affine obj)
                oam_copy(oam_mem, obj_buffer, 16 + 4);

                state = APP;
                break;
            case APP:
                // Process the app for one frame, store the next state
                processAppState(&appState, previousButtons, currentButtons);
                updateSprites(&appState, obj_buffer);

                // Wait for VBlank before we do any drawing.
                waitForVBlank();

                // Undraw the previous state
                undrawAppState(&appState);

                // Draw the current state
                drawAppState(&appState);

                // 16 balls (obj) + 1 cue (affine obj)
                oam_copy(oam_mem, obj_buffer, 16 + 4);

                // Check if the app is exiting. If it is, then go to the exit state.
                if (appState.gameOver) {
                    state = APP_EXIT;
                }

                break;
            case APP_EXIT:
                // Wait for VBlank
                waitForVBlank();

                // TA-TODO: Draw the exit / gameover screen
                fullDrawGameOverScreen(&appState);

                // Hide all objects in the buffer
                hide_objects();
                // Apply the object buffer
                oam_copy(oam_mem, obj_buffer, 16 + 4);

                state = APP_EXIT_NODRAW;
                break;
            case APP_EXIT_NODRAW:
                // TA-TODO: Check for a button press here to go back to the start screen
                // Would be redundant, since we already check for the reset button at the top of this loop :)

                break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
