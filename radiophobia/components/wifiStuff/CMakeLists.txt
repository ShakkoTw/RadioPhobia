idf_component_register(SRCS "beaconScanner.c"
                            "deauther.c"
			    "wifiSetup.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES nvs_flash esp_wifi)
