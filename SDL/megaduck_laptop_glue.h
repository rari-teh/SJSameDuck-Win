#pragma once

#include <stdint.h>
#include <SDL.h>

void megaduck_laptop_SDLscancode_to_key(SDL_Event event, uint8_t * key, uint8_t * key_modifiers);