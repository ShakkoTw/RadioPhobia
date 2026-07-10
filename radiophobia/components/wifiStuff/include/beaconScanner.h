#ifndef BEACONSCAN
#define BEACONSCAN

#include "esp_wifi_types_generic.h"

void wifiScan(wifi_ap_record_t *ap_info, uint16_t scanSize);

#endif
