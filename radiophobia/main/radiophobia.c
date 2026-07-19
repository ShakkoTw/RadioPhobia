#include <string.h>
#include "wifiSetup.h"
#include "beaconScanner.h"
#include "deauther.h"
#include "captivePortal.h"
#include "jammer.h"

#define SCAN_SIZE 20

void app_main(void)
{
    wifiInit();
    uint16_t scanSize = SCAN_SIZE;
    wifi_ap_record_t ap_info[scanSize];
    memset(ap_info, 0, sizeof(ap_info));

    //wifiScan(ap_info, scanSize);

    //deauth();

    //wifiSetupAP();
    //captivePortal();

    wifiJamSingle(ap_info[0].primary);
    //wifiJamAll();
    //bluetoothJam();
}
