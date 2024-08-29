#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#include "Core/megaduck_laptop.h"



uint8_t megaduck_laptop_SDLscancode_to_key(SDL_Event event) {

    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_0: return MEGADUCK_KBD_CODE_0;
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_UMLAUT;
        case SDL_SCANCODE_1: return MEGADUCK_KBD_CODE_1;
        case SDL_SCANCODE_2: return MEGADUCK_KBD_CODE_2;
        case SDL_SCANCODE_3: return MEGADUCK_KBD_CODE_3;
        case SDL_SCANCODE_4: return MEGADUCK_KBD_CODE_4;
        case SDL_SCANCODE_5: return MEGADUCK_KBD_CODE_5;
        case SDL_SCANCODE_6: return MEGADUCK_KBD_CODE_6;
        case SDL_SCANCODE_7: return MEGADUCK_KBD_CODE_7;
        case SDL_SCANCODE_8: return MEGADUCK_KBD_CODE_8;
        case SDL_SCANCODE_9: return MEGADUCK_KBD_CODE_9;

        case SDL_SCANCODE_RETURN: return MEGADUCK_KBD_CODE_ENTER;

        // case SDL_SCANCODE_KP_EXCLAM: return MEGADUCK_KBD_CODE_EXCLAMATION_MARK;  // TODO: DUCK: SHIFT
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_DOLLAR;
        // case SDL_SCANCODE_KP_HASH: return MEGADUCK_KBD_CODE_HASH;
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_TILDE;
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_ASTERISK;
        case SDL_SCANCODE_KP_PLUS:       return MEGADUCK_KBD_CODE_PLUS;
        case SDL_SCANCODE_KP_MINUS:      return MEGADUCK_KBD_CODE_MINUS;
        // case SDL_SCANCODE_KP_LEFTPAREN:  return MEGADUCK_KBD_CODE_LEFT_PARENTHESIS;
        // case SDL_SCANCODE_KP_RIGHTPAREN: return MEGADUCK_KBD_CODE_RIGHT_PARENTHESIS;
        // case SDL_SCANCODE_SEMICOLON:     return MEGADUCK_KBD_CODE_SEMICOLON;
        // case SDL_SCANCODE_KP_COLON:      return MEGADUCK_KBD_CODE_COLON;
        // case SDL_SCANCODE_KP_PERCENT:    return MEGADUCK_KBD_CODE_PERCENT;  // TODO: DUCK: SHIFT
        case SDL_SCANCODE_EQUALS:     return MEGADUCK_KBD_CODE_EQUALS;
        case SDL_SCANCODE_COMMA:      return MEGADUCK_KBD_CODE_COMMA;
        case SDL_SCANCODE_KP_LESS:    return MEGADUCK_KBD_CODE_LESS_THAN;
        case SDL_SCANCODE_PERIOD:     return MEGADUCK_KBD_CODE_PERIOD;
        // case SDL_SCANCODE_KP_GREATER: return MEGADUCK_KBD_CODE_GT;  // TODO: DUCK: SHIFT
        // case SDL_SCANCODE_SLASH:      return MEGADUCK_KBD_CODE_SLASH;
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_QUESTION_MARK;
        case SDL_SCANCODE_SPACE: return MEGADUCK_KBD_CODE_SPACE;
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_QUOTE;
        // case SDL_SCANCODE_: return MEGADUCK_KBD_CODE_AT;

        case SDL_SCANCODE_Q: return MEGADUCK_KBD_CODE_Q;
        case SDL_SCANCODE_W: return MEGADUCK_KBD_CODE_W;
        case SDL_SCANCODE_E: return MEGADUCK_KBD_CODE_E;
        case SDL_SCANCODE_R: return MEGADUCK_KBD_CODE_R;
        case SDL_SCANCODE_T: return MEGADUCK_KBD_CODE_T;
        case SDL_SCANCODE_Y: return MEGADUCK_KBD_CODE_Y;
        case SDL_SCANCODE_U: return MEGADUCK_KBD_CODE_U;
        case SDL_SCANCODE_I: return MEGADUCK_KBD_CODE_I;
        case SDL_SCANCODE_O: return MEGADUCK_KBD_CODE_O;
        case SDL_SCANCODE_P: return MEGADUCK_KBD_CODE_P;
        case SDL_SCANCODE_A: return MEGADUCK_KBD_CODE_A;
        case SDL_SCANCODE_S: return MEGADUCK_KBD_CODE_S;
        case SDL_SCANCODE_D: return MEGADUCK_KBD_CODE_D;
        case SDL_SCANCODE_F: return MEGADUCK_KBD_CODE_F;
        case SDL_SCANCODE_G: return MEGADUCK_KBD_CODE_G;
        case SDL_SCANCODE_H: return MEGADUCK_KBD_CODE_H;
        case SDL_SCANCODE_J: return MEGADUCK_KBD_CODE_J;
        case SDL_SCANCODE_K: return MEGADUCK_KBD_CODE_K;
        case SDL_SCANCODE_L: return MEGADUCK_KBD_CODE_L;
        case SDL_SCANCODE_Z: return MEGADUCK_KBD_CODE_Z;
        case SDL_SCANCODE_X: return MEGADUCK_KBD_CODE_X;
        case SDL_SCANCODE_C: return MEGADUCK_KBD_CODE_C;
        case SDL_SCANCODE_V: return MEGADUCK_KBD_CODE_V;
        case SDL_SCANCODE_B: return MEGADUCK_KBD_CODE_B;
        case SDL_SCANCODE_N: return MEGADUCK_KBD_CODE_N;
        case SDL_SCANCODE_M: return MEGADUCK_KBD_CODE_M;


        case SDL_SCANCODE_F1:  return MEGADUCK_KBD_CODE_F1;
        case SDL_SCANCODE_F2:  return MEGADUCK_KBD_CODE_F2;
        case SDL_SCANCODE_F3:  return MEGADUCK_KBD_CODE_F3;
        case SDL_SCANCODE_F4:  return MEGADUCK_KBD_CODE_F4;
        case SDL_SCANCODE_F5:  return MEGADUCK_KBD_CODE_F5;
        case SDL_SCANCODE_F6:  return MEGADUCK_KBD_CODE_F6;
        case SDL_SCANCODE_F7:  return MEGADUCK_KBD_CODE_F7;
        case SDL_SCANCODE_F8:  return MEGADUCK_KBD_CODE_F8;
        case SDL_SCANCODE_F9:  return MEGADUCK_KBD_CODE_F9;
        case SDL_SCANCODE_F10: return MEGADUCK_KBD_CODE_F10;

        case SDL_SCANCODE_DELETE:    return MEGADUCK_KBD_CODE_DELETE;
        case SDL_SCANCODE_BACKSPACE: return MEGADUCK_KBD_CODE_BACKSPACE;
            // TODO: shift handling
        // case SDL_SCANCODE_LSHIFT: return MEGADUCK_KBD_CODE_SHIFT_DOWN;
        // case SDL_SCANCODE_RSHIFT: return MEGADUCK_KBD_CODE_SHIFT_DOWN;
        case SDL_SCANCODE_UP:     return MEGADUCK_KBD_CODE_ARROW_UP;
        case SDL_SCANCODE_DOWN:   return MEGADUCK_KBD_CODE_ARROW_DOWN;
        case SDL_SCANCODE_LEFT:   return MEGADUCK_KBD_CODE_ARROW_LEFT;
        case SDL_SCANCODE_RIGHT:  return MEGADUCK_KBD_CODE_ARROW_RIGHT;
        case SDL_SCANCODE_ESCAPE: return MEGADUCK_KBD_CODE_ESCAPE;
        // case SDL_SCANCODE_DECIMALSEPARATOR: return MEGADUCK_KBD_CODE_DECIMAL_POINT;  // TODO: DUCK: SHIFT
        // case SDL_SCANCODE_ANSI_KEYPADCLEAR: return MEGADUCK_KBD_CODE_M;
        case SDL_SCANCODE_KP_MULTIPLY: return MEGADUCK_KBD_CODE_MULTIPLY;
        case SDL_SCANCODE_KP_DIVIDE: return MEGADUCK_KBD_CODE_DIVIDE;

        default: return MEGADUCK_KBD_CODE_NONE;
    }

    // Default no action
    return MEGADUCK_KBD_CODE_NONE;
};