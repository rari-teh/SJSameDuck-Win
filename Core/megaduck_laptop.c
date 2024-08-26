#include "gb.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

#define MEGADUCK_LAPTOP_SERIAL_LOGGING_ENABLED
//#define MEGADUCK_LAPTOP_SERIAL_LOG_ALL_IN_OUT

static inline uint8_t int_to_bcd(uint8_t i)
{
    return (i % 10) + ((i / 10) << 4);
}

static inline uint8_t bcd_to_int(uint8_t i)
{
    return (i & 0xF) + (i >> 4) * 10;
}

static void serial_start(GB_gameboy_t *gb, bool bit_received)
{
    gb->megaduck_laptop.byte_being_received <<= 1;
    gb->megaduck_laptop.byte_being_received |= bit_received;
    gb->megaduck_laptop.bits_received++;
    if (gb->megaduck_laptop.bits_received == 8) {

        //         // gb->megaduck_laptop.byte_to_send = 0;
        // #ifdef MEGADUCK_LAPTOP_SERIAL_LOGGING_ENABLED
        //     #ifdef MEGADUCK_LAPTOP_SERIAL_LOG_ALL_IN_OUT
        //         printf("WorkBoy  from-gb-RX:%02x(%c)  to-gb-TX:%02x(%c)  ( ROMB:%02x  PC:%04x )\n",
        //             gb->megaduck_laptop.byte_being_received, gb->megaduck_laptop.byte_being_received,
        //             gb->megaduck_laptop.byte_to_send, gb->megaduck_laptop.byte_to_send,
        //             gb->mbc_rom_bank, gb->pc);
        //     #endif
        // #endif

        gb->megaduck_laptop.bits_received = 0;
        gb->megaduck_laptop.byte_being_received = 0;
    }  // End: 8 bits received
}

static bool serial_end(GB_gameboy_t *gb)
{
    bool ret = gb->megaduck_laptop.bit_to_send;
    gb->megaduck_laptop.bit_to_send = gb->megaduck_laptop.byte_to_send & 0x80;
    gb->megaduck_laptop.byte_to_send <<= 1;
    return ret;
}

void GB_connect_megaduck_laptop(GB_gameboy_t *gb,
                                GB_megaduck_laptop_set_time_callback set_time_callback,
                                GB_megaduck_laptop_get_time_callback get_time_callback)
{
    memset(&gb->megaduck_laptop, 0, sizeof(gb->megaduck_laptop));
    GB_set_serial_transfer_bit_start_callback(gb, serial_start);
    GB_set_serial_transfer_bit_end_callback(gb, serial_end);
    gb->megaduck_laptop_set_time_callback = set_time_callback;
    gb->megaduck_laptop_get_time_callback = get_time_callback;
    gb->accessory = GB_ACCESSORY_MEGADUCK_LAPTOP;
}

bool GB_megaduck_laptop_is_enabled(GB_gameboy_t *gb)
{
    return gb->accessory == GB_ACCESSORY_MEGADUCK_LAPTOP; // TODO:  && gb->megaduck_laptop.mode;
}

void GB_megaduck_laptop_set_key(GB_gameboy_t *gb, uint8_t key)
{
    if (gb->accessory != GB_ACCESSORY_MEGADUCK_LAPTOP) return;

    gb->megaduck_laptop.key = key;
}
