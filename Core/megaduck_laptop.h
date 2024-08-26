#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "defs.h"


typedef struct {
    // Serial IO state and values
    uint8_t byte_to_send;
    bool bit_to_send;
    uint8_t byte_being_received;
    uint8_t bits_received;

    // Peripheral state
    // uint8_t mode;
    uint8_t key;
    // bool shift_down;
    // bool user_shift_down;

    // uint8_t buffer[0x15];
    // uint8_t buffer_index; // In nibbles during read, in bytes during write
} GB_megaduck_laptop_t;

typedef void (*GB_megaduck_laptop_set_time_callback)(GB_gameboy_t *gb, time_t time);
typedef time_t (*GB_megaduck_laptop_get_time_callback)(GB_gameboy_t *gb);

enum {
    MEGADUCK_LAPTOP_KEY_NONE = 0xFF, // TODO
};


void GB_connect_megaduck_laptop(GB_gameboy_t *gb,
                        GB_megaduck_laptop_set_time_callback set_time_callback,
                        GB_megaduck_laptop_get_time_callback get_time_callback);
bool GB_megaduck_laptop_is_enabled(GB_gameboy_t *gb);
void GB_megaduck_laptop_set_key(GB_gameboy_t *gb, uint8_t key);
