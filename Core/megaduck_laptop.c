#include "gb.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

#define MEGADUCK_SYS_SERIAL_LOGGING_ENABLED
#define MEGADUCK_SYS_SERIAL_LOG_ALL_IN_OUT

// TODO: maybe reset ext clock send bit counter during new transmits? maybe more accurate not to. periph->ext_clk_send_bit_counter = 0;


// Serial clock speed used is: 8192 Hz  1 KB/s  Bit 1 cleared, Normal speed
//
// 8192 Hz / 8 bits = 1024 Bytes per second / 1000 msec = 1.024 msec per byte

// Power-on init sequence:
//
// # MEGADUCK_SYS_STATE_INIT_1_WAIT_RX_COUNTER,
// 1. Peripheral waits for System ROM to send count up sequence of 0..255 with ~1msec per byte
//    - Reply bytes are ignored
//    - 1msec spacing includes AFTER last byte sent
//
// # MEGADUCK_SYS_STATE_INIT_2_SEND_RX_COUNT_ACK  // External Clock
// 2. Peripheral expected to send a 1 byte response after minimum ~1msec (no timeout in System ROM code):
//    - Success: "0x01" (MEGADUCK_SYS_REPLY_BOOT_OK)
//    - Failure: Any other value reply is interpreted as failure
//
// # MEGADUCK_SYS_STATE_INIT_3_WAIT_TX_COUNTER_REQ
// 3. Peripheral waits for system ROM to send a command 0x00 (MEGADUCK_SYS_CMD_INIT_SEQ_REQUEST)
//    - Reply byte is ignored
//
// # MEGADUCK_SYS_STATE_INIT_4_SEND_TX_COUNTER,      // External Clock
// 4. Peripheral expected to send countdown 255..0 sequence with 1msec per byte
//
// # MEGADUCK_SYS_STATE_INIT_5_WAIT_TX_COUNT_ACK,
// 5. Peripheral waits for System ROM to send ack for sequence
//    - Success: 0x01: MEGADUCK_SYS_CMD_DONE_OR_OK, only happens after last byte sent
//    - Failure: 0x04: MEGADUCK_SYS_CMD_ABORT_OR_FAIL, can happen any time in sequence if byte System ROM receives doesn't match it's parallel counter
//
// # Should be done, now some misc commands will come in, such as unknown 0x09
//   then optionally an RTC reset


static inline uint8_t int_to_bcd(uint8_t i)
{
    return (i % 10) + ((i / 10) << 4);
}

static inline uint8_t bcd_to_int(uint8_t i)
{
    return (i & 0xF) + (i >> 4) * 10;
}


static void power_on_reset(GB_megaduck_laptop_t * periph) {
    periph->state                    = MEGADUCK_SYS_STATE_INIT_1_WAIT_RX_COUNTER;
    periph->init_counter             = MEGADUCK_SYS_INIT_COUNTER_RESET;
    periph->ext_clk_send_bit_counter = 0;
    periph->ext_clk_send_queue_size  = 0;
    periph->ext_clk_send_queue_index = 0;
}


// Add a byte to the external clock send queue (FIFO)
static void enqueue_ext_clk_send(GB_megaduck_laptop_t * periph, uint8_t byte_to_enqueue) {
    printf("  ..  Periph: Enqueue %02x as [%d]\n", byte_to_enqueue, periph->ext_clk_send_queue_size);

    periph->ext_clk_send_queue[ periph->ext_clk_send_queue_size++ ] = byte_to_enqueue;
}


// Called after last bytes added to the external clock send queue
// Resets the buffer index and time until next serial clock / bit send is triggered
static void enqueue_ext_clk_send_finalize(GB_megaduck_laptop_t * periph) {
    // Whenever there is a byte to send, reset and add a longer delay until next serial bit send
    periph->t_states_till_update = MEGADUCK_LAPTOP_TICK_COUNT_TX_DELAY;
    // Reset to start of send queue
    periph->ext_clk_send_queue_index = MEGADUCK_LAPTOP_EXT_CLOCK_SEND_INDEX_RESET;
}


// Receive the 0 - 255 counter sent by the System ROM
// Then send an ACK once it's done
static void init_stage_1_rx_counter(GB_megaduck_laptop_t * periph) {

    // Once final count value is received move to next state
    if (periph->byte_being_received == MEGADUCK_SYS_INIT_COUNTER_DONE) {

        // Counter done, move to next state and use ext clock serial to send reply byte to GB
        if (periph->init_counter == MEGADUCK_SYS_INIT_COUNTER_DONE) {
            // Success: received counter bytes all matched expected increment
            periph->state = MEGADUCK_SYS_STATE_INIT_2_SEND_RX_COUNT_ACK;
            enqueue_ext_clk_send(periph, MEGADUCK_SYS_REPLY_BOOT_OK);
            enqueue_ext_clk_send_finalize(periph);
        } else {
            // Failure: some counter bytes sent by System ROM either dropped or corrupted
            // TODO: What happens here is not really known yet, for now assuming it reverts to power-on Reset
            periph->state = MEGADUCK_SYS_STATE_INIT_1_WAIT_RX_COUNTER;
            enqueue_ext_clk_send(periph, MEGADUCK_SYS_REPLY_BOOT_FAIL);
            enqueue_ext_clk_send_finalize(periph);
        }
    }
    else if (periph->init_counter == periph->byte_being_received) {
        // If the received counter byte matched the expected value then increment it
        periph->init_counter++;
    }

    // TX/Reply byte is ignored by System ROM
    periph->byte_to_send = 0x00; // TODO: Also not yet known what value it typically is
}


// Wait for request to send counter from 0 - 255
// Then send the counter
//
// TODO: Does it need a larger delay between send counter bytes to allow for the System ROM to send 
//       MEGADUCK_SYS_CMD_ABORT_OR_FAIL reply (without serial IO dual internal clock contention)
static void init_stage_3_wait_tx_count_request(GB_megaduck_laptop_t * periph) {

    if (periph->byte_being_received == MEGADUCK_SYS_CMD_INIT_SEQ_REQUEST) {

        // Got expected command, send countdown sequence
        periph->state = MEGADUCK_SYS_STATE_INIT_4_SEND_TX_COUNTER;

        // Load up 0..255 counter to send
        // It will get sent via GB_megaduck_laptop_peripheral_update()
        for (int c = MEGADUCK_SYS_INIT_COUNTER_DONE; c >= MEGADUCK_SYS_INIT_COUNTER_RESET; c--) {
            enqueue_ext_clk_send(periph, c);
        }
        enqueue_ext_clk_send_finalize(periph);
    }

    // TX/Reply byte is ignored by System ROM
    periph->byte_to_send = 0x00; // TODO: Also not yet known what value it typically is
}


// Wait for confirmation of sent counter data
// Then send the final status acknowledgement
static void init_stage_5_wait_tx_count_ack(GB_megaduck_laptop_t * periph) {

    if (periph->byte_being_received == MEGADUCK_SYS_CMD_DONE_OR_OK) {
        periph->state = MEGADUCK_SYS_STATE_INITIAIZED_OK;
    }
    // Failure, reset (System ROM will hang forever in busy loop at 0x0108 instead of retrying)
    else if (periph->byte_being_received == MEGADUCK_SYS_CMD_ABORT_OR_FAIL) {
        // TODO: for now assuming init failure results in a reset of the perihperhal
        power_on_reset(periph);
    }

    // TX/Reply byte is ignored by System ROM
    periph->byte_to_send = 0x00; // TODO: Also not yet known what value it typically is
}


static void idle_handle_commands(GB_megaduck_laptop_t * periph) {

    if (periph->byte_being_received == MEGADUCK_SYS_CMD_INIT_UNKNOWN_0x09) {
        periph->state = MEGADUCK_SYS_STATE_REPLY_CMD_0x09_UNKNOWN;
        enqueue_ext_clk_send(periph, MEGADUCK_SYS_REPLY_CMD_INIT_UNKNOWN_0x09);
        enqueue_ext_clk_send_finalize(periph);
    }
    // TX/Reply byte is ignored by System ROM
    periph->byte_to_send = 0x00; // TODO: Also not yet known what value it typically is
}


static void serial_start(GB_gameboy_t *gb, bool bit_received)
{
    GB_megaduck_laptop_t * periph = &gb->megaduck_laptop;

    periph->byte_being_received <<= 1;
    periph->byte_being_received |= bit_received;
    periph->bits_received++;
    if (periph->bits_received == 8) {

        #ifdef MEGADUCK_SYS_SERIAL_LOGGING_ENABLED
            #ifdef MEGADUCK_SYS_SERIAL_LOG_ALL_IN_OUT
                GB_log(gb, "->> Periph:  from-gb-RX:%02x  ( ROM0/B:%02x %02x  PC:%04x ) [State: %d, RX Counter: %d, EXT-queue: %d]\n",
                    periph->byte_being_received, // periph->byte_being_received,
                    // periph->byte_to_send, periph->byte_to_send,
                     gb->mbc_rom0_bank, gb->mbc_rom_bank, gb->pc,
                     periph->state, periph->init_counter, periph->ext_clk_send_queue_size);
            #endif
        #endif

        switch(periph->state) {
            case MEGADUCK_SYS_STATE_INIT_1_WAIT_RX_COUNTER:
                init_stage_1_rx_counter(periph);
                break;

            case MEGADUCK_SYS_STATE_INIT_2_SEND_RX_COUNT_ACK:
                // Do nothing here, handled by periph_megaduck_laptop_peripheral_update()
                break;

            case MEGADUCK_SYS_STATE_INIT_3_WAIT_TX_COUNTER_REQ:
                init_stage_3_wait_tx_count_request(periph);
                break;

            case MEGADUCK_SYS_STATE_INIT_4_SEND_TX_COUNTER:
                 // Fall through, System ROM might try to interrupt TX counter at any time in sequence if it fails
                 // even though it looks like that might result in both devices trying to drive the clock
            case MEGADUCK_SYS_STATE_INIT_5_WAIT_TX_COUNT_ACK:
                init_stage_5_wait_tx_count_ack(periph);
                break;

            case MEGADUCK_SYS_STATE_INITIAIZED_OK:
                idle_handle_commands(periph);
                break;
        }

        #ifdef MEGADUCK_SYS_SERIAL_LOGGING_ENABLED
            #ifdef MEGADUCK_SYS_SERIAL_LOG_ALL_IN_OUT
                GB_log(gb, "                              (post serial handling)  [State: %d, RX Counter: %d, EXT-queue: %d, Ticks: %d]\n",
                     periph->state, periph->init_counter, periph->ext_clk_send_queue_size,
                                periph->t_states_till_update);
            #endif
        #endif


        periph->bits_received = 0;
        periph->byte_being_received = 0;
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

    power_on_reset(&gb->megaduck_laptop);
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


void GB_megaduck_laptop_reset(GB_gameboy_t *gb) {
    power_on_reset(&gb->megaduck_laptop);
}


// Called from timing.h after 512 T-States have elapsed
// Roughly matches serial clock timing in GB "normal" speed
//
// For sending serial data from peripheral when GB is waiting
// for data to be initiated via external clock
//
// "cycles" param here in SameBoy looks like it means T-States
void GB_megaduck_laptop_peripheral_update(GB_gameboy_t *gb, uint8_t cycles)
{
    GB_megaduck_laptop_t * periph = &gb->megaduck_laptop;

    // GB_log(gb, "  megaduck_laptop [EXT CLK] --------TICK---------\n");
    // Reset counter until next update
    periph->t_states_till_update = MEGADUCK_LAPTOP_TICK_COUNT_RESET;

    if (periph->ext_clk_send_queue_size > 0) {

        // Send data 1 bit at a time
        bool tx_bit = (periph->ext_clk_send_queue[ periph->ext_clk_send_queue_index ] & 0x80) == 0x80;

        // GB_log(gb, "  megaduck_laptop [EXT CLK] ** BIT SEND ** %d = %d (%02x) [queued: %d] {DIV:%04x}\n",
        //     periph->ext_clk_send_bit_counter, tx_bit, periph->ext_clk_send_queue[ periph->ext_clk_send_queue_index ],
        //         periph->ext_clk_send_queue_size,
        //         gb->div_counter);

        periph->ext_clk_send_queue[ periph->ext_clk_send_queue_index ] <<= 1;

        GB_serial_set_data_bit(gb, tx_bit);
        // TODO: Duck laptop peripheral doesn't seem to care about RX data when it's driving the clock
        // GB_serial_get_data_bit(GB_gameboy_t *gb);

        // Once 8 bits sent, move to next byte in queue, or if done change state
        periph->ext_clk_send_bit_counter++;
        if (periph->ext_clk_send_bit_counter == 8) {

            periph->ext_clk_send_bit_counter = 0;

            #ifdef MEGADUCK_SYS_SERIAL_LOGGING_ENABLED
                #ifdef MEGADUCK_SYS_SERIAL_LOG_ALL_IN_OUT
                    GB_log(gb, "    - megaduck_laptop [EXT CLK] send byte done, queue size %d -> %d   ( ROM0/B:%02x %02x  PC:%04x ) [State: %d, RX Counter: %d, EXT-queue: %d]\n",
                        periph->ext_clk_send_queue_size, periph->ext_clk_send_queue_size - 1,
                         gb->mbc_rom0_bank, gb->mbc_rom_bank, gb->pc,
                         periph->state, periph->init_counter, periph->ext_clk_send_queue_size);
                #endif
            #endif

            // Move to next byte
            periph->ext_clk_send_queue_size--;
            if (periph->ext_clk_send_queue_size == 0) {

                // If send queue is done handle changes to next state
                switch(periph->state) {

                    case MEGADUCK_SYS_STATE_INIT_2_SEND_RX_COUNT_ACK:
                        periph->state = MEGADUCK_SYS_STATE_INIT_3_WAIT_TX_COUNTER_REQ;
                        break;

                    case MEGADUCK_SYS_STATE_INIT_4_SEND_TX_COUNTER:
                        periph->state = MEGADUCK_SYS_STATE_INIT_5_WAIT_TX_COUNT_ACK;
                        break;

                    case MEGADUCK_SYS_STATE_REPLY_CMD_0x09_UNKNOWN:
                        // Return to initialized ready waiting state
                        periph->state = MEGADUCK_SYS_STATE_INITIAIZED_OK;
                        break;
                }
            } else {
                // If there is another byte to send, reset and add a longer delay until next serial bit send
                periph->ext_clk_send_queue_index++;  // TODO: could reset this inside ==0 block above once whole send buffer is done
                periph->t_states_till_update = MEGADUCK_LAPTOP_TICK_COUNT_TX_DELAY;
            }

        }
    }

}
