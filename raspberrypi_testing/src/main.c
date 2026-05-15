#include <stdio.h>
#include <string.h>
#include <pcap.h>

#include <pthread.h> // Per il thread di channel hopping

#include "radiotap.h"
#include "80211.h"
#include "utils.h"

void devsel(pcap_if_t **devp, char *errbuf);
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main(int argc, char *argv[])
{	
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *devp; //interfaccia selezionata
	pcap_t *handle; //capture handle
	int dlt;

	struct bpf_program fp; //compiled filter expression
	char filter_exp[] = "type mgt subtype beacon";

	struct sniffer_context ctx;

	pthread_t hopper_tid;

	devsel(&devp, errbuf);
	
	handle = pcap_open_live(devp->name, BUFSIZ, 0, 1000, errbuf);
	if(handle != NULL)
	{
		dlt = pcap_datalink(handle);
		if(dlt == DLT_IEEE802_11_RADIO)
		{
			printf("DLT: %d (%s)\n", dlt, pcap_datalink_val_to_name(dlt));
		}
		else
		{
			printf("DLT: %d (%s)\n", dlt, pcap_datalink_val_to_name(dlt));
			printf("Interfaccia non fornisce Radiotap header! (probabilmente non e' in moniotr mode)");
			return 1;
		}
	}
	else
	{
		printf("errore: %s", errbuf);
		return 1;
	}

	if(pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN)==PCAP_ERROR)
	{
		printf("Impossibile parsare il filtro %s: %s\n", filter_exp, pcap_geterr(handle));
		return 1;
	}
	
	if(pcap_setfilter(handle, &fp)!=0)
	{
		printf("Impossibile settare il filtro %s: %s\n", filter_exp, pcap_geterr(handle));
		return 1;
	}

	//inizializzazione struct di contesto per lo scan
	ctx.unique_count = 0;
	ctx.handle = handle;
	memset(ctx.seen_bssids, 0, sizeof(ctx.seen_bssids));
	strncpy(ctx.dev_name, devp->name, sizeof(ctx.dev_name)-1);
	ctx.dev_name[sizeof(ctx.dev_name)-1] = '\0';

	if (pthread_create(&hopper_tid, NULL, channel_hopper_thread, (void *)&ctx) != 0) {
    	perror("Incapace di creare il thread hopper");
    	return 1;
	}

	pcap_loop(handle, 0, got_packet, (u_char *)&ctx); //scanning loop

	pthread_cancel(hopper_tid);
	pthread_join(hopper_tid, NULL);

	printf("Seleziona un ");

	return 0;
}

void devsel(pcap_if_t **devp, char *errbuf)
{
	int count=0, sel;
	pcap_if_t *alldevsp;

	if(pcap_findalldevs(&alldevsp, errbuf)!=PCAP_ERROR)
	{
		if(alldevsp!=NULL)
		{
			printf("capture devices:\n");
			for(pcap_if_t *d = alldevsp; d != NULL; d = d->next)
			{	
				count++;
				printf("%d- %s\n", count, d->name);
			}
			printf("Seleziona l'interfaccia da usare:\n> ");
			
			scanf("%d", &sel);

			if(sel > 0 && sel <= count)
			{
				*devp = alldevsp;
				for(int i = sel-1; i>0; i--)
				{
					*devp = (*devp) -> next;
				}
				printf("Interfaccia selezionata: %s\n", (*devp) -> name);
			}
			else
			{
				printf("Interfaccia non valida!");
			}
		}
		else
		{
			printf("nessun capture device disponibile!");
		}
	}
	else
	{
		printf("errore: %s", errbuf);
	}

	return;
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	struct sniffer_context *ctx = (struct sniffer_context *)args;
	const struct radiotap_header *radiotap;
	const struct _80211_header *_80211;

	radiotap = (struct radiotap_header*)(packet);
	_80211 = (struct _80211_header*)(packet + radiotap->it_len);

	for (int i = 0; i < ctx->unique_count; i++)
	{
		if(memcmp(ctx->seen_bssids[i], _80211->bssid, 6) == 0) //mac len is always 6 bytes
		{
			return; //duplicato
		}
	}

	//nuovo BSSID
	memcpy(ctx->seen_bssids[ctx->unique_count], _80211->bssid, 6);
	ctx->unique_count++;

	printf("----------------------------\n");
	printf("Destination: ");
	macprint(_80211->destination);
	printf("Source: ");
	macprint(_80211->source);
	printf("BSSID: ");
	macprint(_80211->bssid);

	if(ctx->unique_count >= MAX_UNIQUE_BEACONS)
	{
		pcap_breakloop(ctx->handle);
	}

	/*if(tasto invio premuto)
	{
		pcap_breakloop(ctx->handle);
	}*/
}