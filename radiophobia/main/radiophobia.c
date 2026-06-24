#include <stdio.h>
#include <string.h>
#include "beaconScanner.h"
#include "nvs_flash.h"
#include "deauther.h"

#define SCAN_SIZE 20

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    uint16_t scanSize = SCAN_SIZE;
    wifi_ap_record_t ap_info[scanSize];
    memset(ap_info, 0, sizeof(ap_info));
    wifiScan(ap_info, scanSize);

    deautherSetup();
}
