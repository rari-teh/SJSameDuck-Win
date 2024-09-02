#include "gb.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "megaduck_laptop_periph.h"

static inline uint8_t int_to_bcd(uint8_t i)
{
    return (i % 10) + ((i / 10) << 4);
}

static inline uint8_t bcd_to_int(uint8_t i)
{
    return (i & 0xF) + (i >> 4) * 10;
}

// void MD_rtc_set_to_local_time(GB_megaduck_laptop_t * periph) {

//     time_t rawtime;
//     time(&rawtime);
//     localtime_r(&rawtime, & periph->tm_rtc_time);
// }


void MD_rtc_set_from_buf(GB_megaduck_laptop_t * periph) {

    if (periph->rx_buffer_size == MEGADUCK_BUF_SZ_RTC) {
        periph->tm_rtc_time.tm_sec  = bcd_to_int(periph->rx_buffer[MEGADUCK_RTC_IDX_SEC]);
        periph->tm_rtc_time.tm_min  = bcd_to_int(periph->rx_buffer[MEGADUCK_RTC_IDX_MIN]);
        periph->tm_rtc_time.tm_hour = bcd_to_int(periph->rx_buffer[MEGADUCK_RTC_IDX_HOUR]) + (periph->rx_buffer[MEGADUCK_RTC_IDX_AMPM] * 12);
        // periph->tm_rtc_time.tm_mday = bcd_to_int(periph->rx_buffer[MEGADUCK_RTC_IDX_...]);
        periph->tm_rtc_time.tm_mon  = bcd_to_int(periph->rx_buffer[MEGADUCK_RTC_IDX_MON]) - 1;
        periph->tm_rtc_time.tm_year =  (uint8_t)(periph->rx_buffer[MEGADUCK_RTC_IDX_YEAR]);
    }
}
