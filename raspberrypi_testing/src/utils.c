#include <stdio.h>
#include <stdint.h>

#include <pthread.h> // Per il thread di channel hopping
#include <unistd.h>  // Per usleep

#include <libnl3/netlink/netlink.h>
#include <libnl3/netlink/genl/genl.h>
#include <libnl3/netlink/genl/ctrl.h>
#include <linux/nl80211.h> //uapi definitions for nl80211
#include <net/if.h>

#include "utils.h"

int set_interface_frequency(struct nl_sock *socket, int nl_id, int if_index, uint32_t freq) {
    struct nl_msg *msg = nlmsg_alloc(); //Alloca un nuovo Netlink message object 
    if (!msg) return -1;

    // Costruiamo il messaggio NL80211
    genlmsg_put(msg, NL_AUTO_PORT, NL_AUTO_SEQ, nl_id, 0, 0, NL80211_CMD_SET_CHANNEL, 0);
    
    // Specifichiamo l'interfaccia e la frequenza (in MHz)
    nla_put_u32(msg, NL80211_ATTR_IFINDEX, if_index);
    nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, freq);

    int ret = nl_send_sync(socket, msg);
    
    return ret;
}

void *channel_hopper_thread(void *arg) {
    struct sniffer_context *ctx = (struct sniffer_context *)arg;

    //Netlink Setup
    struct nl_sock *socket = nl_socket_alloc();
    if (genl_connect(socket) < 0) { //create the local socket file descriptor and binds the socket to the NETLINK_GENERIC Netlink protocol
        fprintf(stderr, "Errore connessione Netlink\n");
        return NULL;
    }
	int nl_id = genl_ctrl_resolve(socket, "nl80211"); //Resolves the Generic Netlink family name to the corresponding numeric family identifier.
    
	//Interface idx 
	int if_index = if_nametoindex(ctx->dev_name);
    if (if_index == 0) {
        fprintf(stderr, "Interfaccia %s non trovata\n", ctx->dev_name);
        nl_socket_free(socket);
        return NULL;
    }

    //Definizione frequenze (Canali 1, 6, 11 per copertura rapida, poi gli altri)
    uint32_t freqs[] = {
        // Inizio con i canali "pilastro" della 2.4GHz e i principali della 5GHz
        2412, 5180, 2437, 5260, 2462, 5500, 
        
        // Proseguo incrociando le bande
        2417, 5200, 2442, 5280, 2412, 5520, // Ritorno sul 2412 (più comune)
        2422, 5220, 2447, 5300, 2437, 5540, // Ritorno sul 2437
        2427, 5240, 2452, 5320, 2462, 5560, // Ritorno sul 2462
        
        // Blocchi alta frequenza (UNII-2 Extended e UNII-3)
        2432, 5580, 2457, 5600, 5745,
        5620, 5640, 5660, 5680, 5700, 5720
    };
    int n_freqs = sizeof(freqs) / sizeof(freqs[0]);
    int i = 0;

    printf("[Channel Hopper] Thread avviato su %s (%d)\n", ctx->dev_name, if_index);

    // 3. Loop di salto
    while (1) {
        set_interface_frequency(socket, nl_id, if_index, freqs[i]);

        // 200ms per dare tempo alla scheda di stabilizzarsi sul nuovo canale e catturare eventuali beacon
        usleep(200000); 
        i = (i + 1) % n_freqs;
        
        // Permette al thread di essere cancellato correttamente da pthread_cancel
        pthread_testcancel();
    }

    nl_socket_free(socket);
    return NULL;
}