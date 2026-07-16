#include "captivePortal.h"
#include "dns_server.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_littlefs.h"
#include "esp_http_server.h"

void littlefsInit(void){
    static const char *TAG = "esp_littlefs";

    ESP_LOGI(TAG, "Initializing LittleFS");

    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/data",
        .partition_label = "storage",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find LittleFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get LittleFS partition information (%s)", esp_err_to_name(ret));
        esp_littlefs_format(conf.partition_label);
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    return;
}

static const char *TAG = "CaptivePortal";

static esp_err_t static_file_handler(httpd_req_t *req) {
    char filepath[128];

    //scelta del file da servire in base all'uri della richiesta
    if (strcmp(req->uri, "/") == 0) {
        strcpy(filepath, "/data/captive_portal/index.html");
    } else {
        snprintf(filepath, sizeof(filepath), "/data/captive_portal%s", req->uri);
    }

    //apertura file descriptor del file richiesto
    FILE *f = fopen(filepath, "r");

    //se risorsa non presente, fai redirect a / e mandando lo status corretto
    if (f == NULL) {
        ESP_LOGI(TAG, "Redirecting to root");

         // Set status
        httpd_resp_set_status(req, "303 See Other");
        // Redirect to the "/" root directory
        httpd_resp_set_hdr(req, "Location", "/");
        // iOS requires content in the response to detect a captive portal, simply redirecting is not sufficient.
        return httpd_resp_send(req, "Redirect to the captive portal", HTTPD_RESP_USE_STRLEN);
    }

    //imposto il content type corretto in base al tipo di file richiesto
    if (strstr(filepath, ".html")) httpd_resp_set_type(req, "text/html");
    else if (strstr(filepath, ".css")) httpd_resp_set_type(req, "text/css");
    else if (strstr(filepath, ".js")) httpd_resp_set_type(req, "application/javascript");
    else if (strstr(filepath, ".png")) httpd_resp_set_type(req, "image/png");

    //invia il file a blocchi in quanto la RAM dell'esp è limitata
    char chunk[1024];
    size_t read_bytes;
    while ((read_bytes = fread(chunk, 1, sizeof(chunk), f)) > 0) {
        httpd_resp_send_chunk(req, chunk, read_bytes);
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0); //invia fine trasmissione
    return ESP_OK;
}

static const httpd_uri_t file_uri = {
    .uri       = "/*",
    .method    = HTTP_GET,
    .handler   = static_file_handler,
    .user_ctx  = NULL
};

static httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true; //abilita Least Recently Used purge policy
    config.uri_match_fn = httpd_uri_match_wildcard; //abilita l'utilizzo delle wildcard tipo *

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &file_uri);
    }
    return server;
}

void captivePortal(void){
    //inizializza e monta la partizione littlefs, utilizzata per i file del captive portal
    littlefsInit();

    //DNS server per mandare ogni richiesta dei dispositivi connessi verso l'ip dell'esp
    dns_server_config_t config = DNS_SERVER_CONFIG_SINGLE("*", "WIFI_AP_DEF");
    start_dns_server(&config);

    //avvia il webserver
    start_webserver();
}
