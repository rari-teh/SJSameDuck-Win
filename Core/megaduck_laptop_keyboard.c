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

    MD_enqueue_ext_clk_send(periph, MEGADUCK_KBD_BUF_REPLY_LEN);
    MD_enqueue_ext_clk_send(periph, periph->key_modifiers);
    MD_enqueue_ext_clk_send(periph, periph->key);

    uint8_t checksum = MEGADUCK_KBD_BUF_REPLY_LEN + periph->key_modifiers + periph->key;
    checksum = ~checksum + 1;  // 2's complement
    MD_enqueue_ext_clk_send(periph, checksum);

    MD_enqueue_ext_clk_send_finalize(periph);
    // Override with longer delay for first reply byte
    // since command that initiated the buffer requires 2+ msec delay
    // for unknown reasons (maybe extra delay for RTC latch on reads?)
    periph->t_states_till_update = MEGADUCK_LAPTOP_TICK_COUNT_KBD_REPLY_START;


    // TODO: FIXME: Is this accurate? // Now that key is queued up, clear keypress?
    // TODO: handle key repeat?
    periph->key = MEGADUCK_KBD_CODE_NONE;
}


// Process the command completed reply from the Mega Duck
void MD_keyboard_handle_tx_reply(GB_megaduck_laptop_t * periph) {

    switch (periph->byte_being_received) {

        case 0x81:
        case MEGADUCK_SYS_CMD_DONE_OR_OK:
            // If this is the ack stage then success, transfer is finished. Return to normal state
            // Otherwise ignore
            if (periph->state == MEGADUCK_SYS_STATE_READ_KEYS_WAIT_ACK) {
                periph->state = MEGADUCK_SYS_STATE_INIT_OK_READY;
            }
            break;

        // Handle failure scenario, do a soft-reset back to initialized and ready // TODO: maybe this should be a hard reset?
        case MEGADUCK_SYS_CMD_ABORT_OR_FAIL:
            MD_periph_reset(periph, MEGADUCK_SYS_KEEP_INIT_RESET);
            break;

        // Any other unexpected reply value seems like it should be same as explicit abort/fail
        default:
            MD_periph_reset(periph, MEGADUCK_SYS_KEEP_INIT_RESET);
            break;
    }
}