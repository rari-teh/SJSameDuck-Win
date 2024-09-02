#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "defs.h"

// Multi-byte buffer transfer
void MD_receive_buf_init(GB_megaduck_laptop_t * periph);
void MD_receive_buf_handle_result(GB_megaduck_laptop_t * periph);
void MD_receive_buf(GB_megaduck_laptop_t * periph);

// Init
void MD_init_stage_1_rx_counter(GB_megaduck_laptop_t * periph);
void MD_init_stage_3_wait_tx_count_request(GB_megaduck_laptop_t * periph);
void MD_init_stage_5_wait_tx_count_ack(GB_megaduck_laptop_t * periph);

// RTC
// void MD_rtc_set_to_local_time(GB_megaduck_laptop_t * periph);
void MD_rtc_set_from_buf(GB_megaduck_laptop_t * periph);

// Keyboard
void MD_keyboard_enqueue_reply(GB_megaduck_laptop_t * periph);
void MD_keyboard_handle_tx_reply(GB_megaduck_laptop_t * periph);
