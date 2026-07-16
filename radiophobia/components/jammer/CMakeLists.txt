idf_component_register(SRCS "jammer.cpp"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES RF24 esp_timer esp_driver_gpio esp_driver_spi)
