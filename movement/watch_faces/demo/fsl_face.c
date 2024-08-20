/* vim: et sw=4 ts=4
 * MIT License
 *
 * Copyright (c) 2024 <#author_name#>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include "fsl_face.h"

void fsl_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(fsl_state_t));
        memset(*context_ptr, 0, sizeof(fsl_state_t));
    }
}

char *screens[] = {
    "     FSLur",
    "    EGGBuG",
    NULL
};

void fsl_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    fsl_state_t *state = (fsl_state_t *)context;
    state->screen = 0;
    state->colon = FSL_COLON_ON;
    state->blink = 1;
}

bool fsl_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    fsl_state_t *state = (fsl_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            watch_display_string(screens[state->screen], 0);
            watch_clear_all_indicators();
            break;

        case EVENT_TICK:
            if (state->colon == FSL_COLON_BLINK) {
                if (state->blink) {
                    watch_set_colon();
                } else {
                    watch_clear_colon();
                }
                state->blink = !state->blink;
            }
            break;

        case EVENT_LIGHT_BUTTON_UP:
            state->colon += 1;
            state->colon %= 3;

            if (state->colon == FSL_COLON_ON) {
                watch_set_colon();
            }
            if (state->colon == FSL_COLON_OFF) {
                watch_clear_colon();
            }

            break;

        case EVENT_ALARM_BUTTON_UP:
            state->screen++;
            if (screens[state->screen] == NULL) {
                state->screen = 0;
            }
            watch_display_string(screens[state->screen], 0);
            break;

        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;

        default:
            return movement_default_loop_handler(event, settings);
    }

    return true;
}

void fsl_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
}

