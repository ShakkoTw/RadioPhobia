#ifndef _80211_H
#define _80211_H

#include <stdint.h>
#include <sys/types.h>

struct _80211_header {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t  destination[6];
    uint8_t  source[6];
    uint8_t  bssid[6];
    uint16_t seq_control;
    //Frame Body
    uint64_t timestamp;
    uint16_t beacon_interval;
    uint16_t capability_info;
    uint8_t tag_num;
    uint8_t tag_len;
} __attribute__((__packed__));

#endif
