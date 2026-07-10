#ifndef DEAUTHER
#define DEAUTHER

#include <stdint.h>

struct frame_control {
    uint16_t protocol_version : 2;
    uint16_t type : 2;
    uint16_t subtype : 4;
    uint16_t to_ds : 1;
    uint16_t from_ds : 1;
    uint16_t more_frag : 1;
    uint16_t retry : 1;
    uint16_t power_mgmt : 1;
    uint16_t more_data : 1;
    uint16_t protected_frame :1;
    uint16_t order : 1;
} __attribute__((__packed__));

struct _80211_header {
    struct frame_control frame_control;
    uint16_t duration;
    uint8_t  destination[6]; //MAC del bersaglio
    uint8_t  source[6]; //MAC dell'AP
    uint8_t  bssid[6]; //MAC dell'AP
    uint16_t seq_control;
    uint16_t reason_code;
} __attribute__((__packed__));

void deauth(void);

#endif
