#include "jammer.h"
#include "RF24.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"

#define RADIO_CNT 2
#define JAM_BURST 50

static const char *TAG = "jammer";

static TaskHandle_t jammerTask_handle = NULL;

const char msg[] = "doggo";

enum wifiJam {
    CHANNEL_ALL,
    CHANNEL_SINGLE
} wifiJamMethod;

uint8_t wifiChannel; //canali dal 1 al 14

SPIClass *spi = nullptr;
RF24 *radios[RADIO_CNT] = {};
int radio_ces[] = {26, 4};
int radio_csns[] = {10, 5};
bool constCarrier = false;

void wifiJamTask(void *args){
    switch (wifiJamMethod) {
	case CHANNEL_ALL:
	    ESP_LOGI(TAG, "[nRF24] WiFi Jamming avviato - CHANNEL_ALL");
	    while (true) {
		int freq = ((wifiChannel - 1) * 5) + 1; //calcolo della prima frequenza del canale

		for (int i = 0; i < 23; i++, freq++) {
		    for (int j = 0; j < RADIO_CNT; j++) {
    			radios[j]->setChannel(freq);
			radios[j]->writeFast(&msg, sizeof(msg));
		    }
		}
		vTaskDelay(1);
	    }
	    return;
	
	case CHANNEL_SINGLE:
	    ESP_LOGI(TAG, "[nRF24] WiFi Jamming avviato - CHANNEL_SINGLE [%d]", wifiChannel);
	    while (true) {
		int freq = ((wifiChannel - 1) * 5) + 1; //calcolo della prima frequenza del canale

		for (int i = 0; i < 23; i++, freq++) {
		    for (int j = 0; j < RADIO_CNT; j++) {
    			radios[j]->setChannel(freq);
			radios[j]->writeFast(&msg, sizeof(msg));
		    }
		}
		vTaskDelay(1);
	    }
	    return;
    }
}

void bluetoothJamTask(void *args){
    ESP_LOGI(TAG, "[nRF24] Bluetooth Jamming avviato!");
    while (true) {
	for (int i = 0; i < RADIO_CNT; i++) {
	    int random_channel = esp_random() % 80;
	    radios[i]->setChannel(random_channel);
	}
	vTaskDelay(1);
    }
}

void deinitRadios(void){
    for (int i = 0; i < RADIO_CNT; i++) {
	if (radios[i] != nullptr) {
	    if (constCarrier) {
		radios[i]->stopConstCarrier();
	    }
	    radios[i]->powerDown();
	    delete radios[i];
	    radios[i] = nullptr;
	}
    }

    constCarrier = false;

    if (spi != nullptr) {
	spi->end(SPI2_HOST);
	delete spi;
	spi = nullptr;
    }
}

int initRadios(void){
    spi = new SPIClass();
    spi->begin(SPI2_HOST, SPI_MASTER_FREQ_16M);

    for (int i = 0; i < RADIO_CNT; i++) {
	radios[i] = new RF24(radio_ces[i], radio_csns[i]);
	
	ESP_LOGI(TAG, "[nRF24 - %d] Inizializzazione...", i);

	if (!(radios[i]->begin(spi))) {
	    ESP_LOGI(TAG, "[nRF24 - %d] Inizializzazione fallita!", i);
	    deinitRadios();
	    return 0;
	}
	ESP_LOGI(TAG, "[nRF24 - %d] Applicando configurazione...", i);
	radios[i]->setAutoAck(false);
	radios[i]->stopListening();
	radios[i]->setRetries(0, 0);
	radios[i]->setPayloadSize(5);
	radios[i]->setAddressWidth(3);
	radios[i]->setPALevel(RF24_PA_MAX, false);
	radios[i]->setDataRate(RF24_2MBPS);
	radios[i]->setCRCLength(RF24_CRC_DISABLED);
	radios[i]->disableCRC();
	radios[i]->disableAckPayload();
	radios[i]->disableDynamicPayloads();
    }
    return 1;
}

void wifiJamAll(){
    if(!initRadios()){return;}

    wifiJamMethod = CHANNEL_ALL; //selezione modalità di attacco

    xTaskCreate(
	wifiJamTask,
	"wifiJamTask",
	4096,
	NULL,
	5,
	&jammerTask_handle
    );
}

void wifiJamSingle(uint8_t channel){
    if(!initRadios()){return;}

    wifiJamMethod = CHANNEL_SINGLE; //selezione modalità di attacco
    wifiChannel = channel; //selezione canale wifi da attaccare (solo CHANNEL_SINGLE)

    xTaskCreate(
	wifiJamTask,
	"wifiJamTask",
	4096,
	NULL,
	5,
	&jammerTask_handle
    );
}

void bluetoothJam(void){
    if(!initRadios()){return;}

    for (int i = 0; i < RADIO_CNT; i++) {
	radios[i]->startConstCarrier(static_cast<rf24_pa_dbm_e>(RF24_PA_MAX), 45);
    }

    constCarrier = true;

    xTaskCreate(
	bluetoothJamTask,
	"bluetoothJamTask",
	4096,
	NULL,
	5,
	&jammerTask_handle
    );
}
