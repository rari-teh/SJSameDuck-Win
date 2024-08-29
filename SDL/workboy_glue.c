#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

#include "Core/workboy.h"



uint8_t workboy_SDLscancode_to_key(SDL_Event event) {

    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_0: return GB_WORKBOY_0;
        // case SDL_SCANCODE_: return GB_WORKBOY_UMLAUT;
        case SDL_SCANCODE_1: return GB_WORKBOY_1;
        case SDL_SCANCODE_2: return GB_WORKBOY_2;
        case SDL_SCANCODE_3: return GB_WORKBOY_3;
        case SDL_SCANCODE_4: return GB_WORKBOY_4;
        case SDL_SCANCODE_5: return GB_WORKBOY_5;
        case SDL_SCANCODE_6: return GB_WORKBOY_6;
        case SDL_SCANCODE_7: return GB_WORKBOY_7;
        case SDL_SCANCODE_8: return GB_WORKBOY_8;
        case SDL_SCANCODE_9: return GB_WORKBOY_9;

        case SDL_SCANCODE_RETURN: return GB_WORKBOY_ENTER;

        case SDL_SCANCODE_KP_EXCLAM: return GB_WORKBOY_EXCLAMATION_MARK;
        // case SDL_SCANCODE_: return GB_WORKBOY_DOLLAR;
        case SDL_SCANCODE_KP_HASH: return GB_WORKBOY_HASH;
        // case SDL_SCANCODE_: return GB_WORKBOY_TILDE;
        // case SDL_SCANCODE_: return GB_WORKBOY_ASTERISK;
        case SDL_SCANCODE_KP_PLUS:       return GB_WORKBOY_PLUS;
        case SDL_SCANCODE_KP_MINUS:      return GB_WORKBOY_MINUS;
        case SDL_SCANCODE_KP_LEFTPAREN:  return GB_WORKBOY_LEFT_PARENTHESIS;
        case SDL_SCANCODE_KP_RIGHTPAREN: return GB_WORKBOY_RIGHT_PARENTHESIS;
        case SDL_SCANCODE_SEMICOLON:     return GB_WORKBOY_SEMICOLON;
        case SDL_SCANCODE_KP_COLON:      return GB_WORKBOY_COLON;
        case SDL_SCANCODE_KP_PERCENT:    return GB_WORKBOY_PERCENT;
        case SDL_SCANCODE_EQUALS:     return GB_WORKBOY_EQUAL;
        case SDL_SCANCODE_COMMA:      return GB_WORKBOY_COMMA;
        case SDL_SCANCODE_KP_LESS:    return GB_WORKBOY_LT;
        case SDL_SCANCODE_PERIOD:     return GB_WORKBOY_DOT;
        case SDL_SCANCODE_KP_GREATER: return GB_WORKBOY_GT;
        case SDL_SCANCODE_SLASH:      return GB_WORKBOY_SLASH;
        // case SDL_SCANCODE_: return GB_WORKBOY_QUESTION_MARK;
        case SDL_SCANCODE_SPACE: return GB_WORKBOY_SPACE;
        // case SDL_SCANCODE_: return GB_WORKBOY_QUOTE;
        // case SDL_SCANCODE_: return GB_WORKBOY_AT;

        case SDL_SCANCODE_Q: return GB_WORKBOY_Q;
        case SDL_SCANCODE_W: return GB_WORKBOY_W;
        case SDL_SCANCODE_E: return GB_WORKBOY_E;
        case SDL_SCANCODE_R: return GB_WORKBOY_R;
        case SDL_SCANCODE_T: return GB_WORKBOY_T;
        case SDL_SCANCODE_Y: return GB_WORKBOY_Y;
        case SDL_SCANCODE_U: return GB_WORKBOY_U;
        case SDL_SCANCODE_I: return GB_WORKBOY_I;
        case SDL_SCANCODE_O: return GB_WORKBOY_O;
        case SDL_SCANCODE_P: return GB_WORKBOY_P;
        case SDL_SCANCODE_A: return GB_WORKBOY_A;
        case SDL_SCANCODE_S: return GB_WORKBOY_S;
        case SDL_SCANCODE_D: return GB_WORKBOY_D;
        case SDL_SCANCODE_F: return GB_WORKBOY_F;
        case SDL_SCANCODE_G: return GB_WORKBOY_G;
        case SDL_SCANCODE_H: return GB_WORKBOY_H;
        case SDL_SCANCODE_J: return GB_WORKBOY_J;
        case SDL_SCANCODE_K: return GB_WORKBOY_K;
        case SDL_SCANCODE_L: return GB_WORKBOY_L;
        case SDL_SCANCODE_Z: return GB_WORKBOY_Z;
        case SDL_SCANCODE_X: return GB_WORKBOY_X;
        case SDL_SCANCODE_C: return GB_WORKBOY_C;
        case SDL_SCANCODE_V: return GB_WORKBOY_V;
        case SDL_SCANCODE_B: return GB_WORKBOY_B;
        case SDL_SCANCODE_N: return GB_WORKBOY_N;
        case SDL_SCANCODE_M: return GB_WORKBOY_M;


        case SDL_SCANCODE_F1:  return GB_WORKBOY_CLOCK;
        case SDL_SCANCODE_F2:  return GB_WORKBOY_TEMPERATURE;
        case SDL_SCANCODE_F3:  return GB_WORKBOY_MONEY;
        case SDL_SCANCODE_F4:  return GB_WORKBOY_CALCULATOR;
        case SDL_SCANCODE_F5:  return GB_WORKBOY_DATE;
        case SDL_SCANCODE_F6:  return GB_WORKBOY_CONVERSION;
        case SDL_SCANCODE_F7:  return GB_WORKBOY_RECORD;
        case SDL_SCANCODE_F8:  return GB_WORKBOY_WORLD;
        case SDL_SCANCODE_F9:  return GB_WORKBOY_PHONE;
        case SDL_SCANCODE_F10: return GB_WORKBOY_UNKNOWN;

        case SDL_SCANCODE_DELETE:    return GB_WORKBOY_BACKSPACE;
        case SDL_SCANCODE_BACKSPACE: return GB_WORKBOY_BACKSPACE;
        case SDL_SCANCODE_LSHIFT: return GB_WORKBOY_SHIFT_DOWN;
        case SDL_SCANCODE_RSHIFT: return GB_WORKBOY_SHIFT_DOWN;
        case SDL_SCANCODE_UP:     return GB_WORKBOY_UP;
        case SDL_SCANCODE_DOWN:   return GB_WORKBOY_DOWN;
        case SDL_SCANCODE_LEFT:   return GB_WORKBOY_LEFT;
        case SDL_SCANCODE_RIGHT:  return GB_WORKBOY_RIGHT;
        case SDL_SCANCODE_ESCAPE: return GB_WORKBOY_ESCAPE;
        case SDL_SCANCODE_DECIMALSEPARATOR: return GB_WORKBOY_DECIMAL_POINT;
        // case SDL_SCANCODE_ANSI_KEYPADCLEAR: return GB_WORKBOY_M;
        case SDL_SCANCODE_KP_MULTIPLY: return GB_WORKBOY_H;
        case SDL_SCANCODE_KP_DIVIDE: return GB_WORKBOY_J;

        default: return GB_WORKBOY_NONE;
    }

    // Default no action
    return GB_WORKBOY_NONE;
};