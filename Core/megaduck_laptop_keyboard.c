#include "gb.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "megaduck_laptop_periph.h"

// TODO: Needs better key coverage, modifier keys, repeat, etc

// Enqueue keyboard reply data
// [0]: Length of reply (always 4)
// [1]: Keyboard modifier keys + left printscreen
//         KEY REPEAT : |= 0x01 (so far looks like with no key value set in 3rd Byte)
//         CAPS_LOCK: |= 0x02
//         SHIFT: |= 0x04
//         LEFT_PRINTSCREEN: |= 0x08
// [2]: Keyboard scan codes
// [3]: checksum
void MD_keyboard_enqueue_reply(GB_megaduck_laptop_t * periph) {

    #ifdef MEGADUCK_SYS_KEYBOARD_LOG_SEND_NON_NULL
        if (periph->key != MEGADUCK_KBD_CODE_NONE)
             printf("       **** Sending keyboard reply **** (key: 0x%02x, key_mod: 0x%02x)\n",
                periph->key, periph->key_modifiers);
    #endif

    MD_send_buf_enqueue(periph, MEGADUCK_KBD_BUF_REPLY_LEN);
    MD_send_buf_enqueue(periph, periph->key_modifiers);
    MD_send_buf_enqueue(periph, periph->key);

    MD_send_buf_calc_enqueue_checksum(periph);
    MD_send_buf_finalize_and_transmit(periph);
    // Override with longer delay for first reply byte
    // since command that initiated the buffer requires 2+ msec delay
    // for unknown reasons (maybe extra delay for RTC latch on reads?)
    periph->t_states_till_update = MEGADUCK_LAPTOP_TICK_COUNT_TX_BUF_REPLY_START;


    // TODO: FIXME: Is this accurate? // Now that key is queued up, clear keypress?
    // TODO: handle key repeat?
    periph->key = MEGADUCK_KBD_CODE_NONE;
}

