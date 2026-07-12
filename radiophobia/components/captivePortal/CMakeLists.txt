idf_component_register(SRCS "captivePortal.c"
                            "dns_server.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_netif joltwallet__littlefs esp_http_server)

littlefs_create_partition_image(storage ../../data FLASH_IN_PROJECT)
