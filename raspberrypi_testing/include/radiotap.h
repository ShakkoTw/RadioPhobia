#ifndef RADIOTAP_H
#define RADIOTAP_H

#include <stdint.h>

struct radiotap_header {
    uint8_t  it_version;     /* set to 0 */
    uint8_t  it_pad;
    uint16_t it_len;         /* entire length */
    uint32_t it_present;     /* fields present */
    
} __attribute__((__packed__));

#endif