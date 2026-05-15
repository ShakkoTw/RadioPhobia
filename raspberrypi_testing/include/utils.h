#ifndef UTILS_H
#define UTILS_H

#define MAX_UNIQUE_BEACONS 20

#include <stdio.h>
#include <stdint.h>

#include <pcap.h>

#include <libnl3/netlink/netlink.h>
#include <libnl3/netlink/genl/genl.h>
#include <libnl3/netlink/genl/ctrl.h>
#include <net/if.h>

struct sniffer_context {
    u_char seen_bssids[MAX_UNIQUE_BEACONS][6];
    int unique_count;
    pcap_t *handle;
    char dev_name[IFNAMSIZ];
};

/*
#define MACPRINT(mac)\
do{\
    printf("%X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);\
}while(0);
*/

static inline void macprint(const uint8_t* const mac)
{
    printf("%X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return;
}

void *channel_hopper_thread(void *arg);
int set_interface_frequency(struct nl_sock *socket, int nl_id, int if_index, uint32_t freq);

#endif