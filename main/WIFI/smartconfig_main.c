#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "smartconfig_main.h"

#include "str_data.h"



/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "smartconfig_example";

static void smartconfig_example_task(void * parm);

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "Scan done");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
        ESP_LOGI(TAG, "Found channel");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        
    

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true) {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(NVS_data.ssid_WF, evt->ssid, sizeof(evt->ssid));
        memcpy(NVS_data.password_WF, evt->password, sizeof(evt->password));
        NVS_data.required_size_SSID= sizeof(evt->ssid);
        NVS_data.required_size_PSSWD = sizeof(evt->password);
        ESP_LOGI(TAG, "SSID:%s", NVS_data.ssid_WF);
        ESP_LOGI(TAG, "PASSWORD:%s", NVS_data.password_WF);
        printf("Update NVS SSID and PASSWORD");
        err = nvs_set_str(my_handle, "SSID", (char *)NVS_data.ssid_WF);
        printf((err != ESP_OK) ? "Failed!\n" : "Done SSID\n");
        err = nvs_set_str(my_handle, "PASSWORD", (char *)NVS_data.password_WF);
        printf((err != ESP_OK) ? "Failed!\n" : "Done PASSWORD\n");
        err = nvs_set_i32(my_handle, "sizeS", NVS_data.required_size_SSID);
        printf((err != ESP_OK) ? "Failed!\n" : "Done SSID REQUIRED\n");
        err = nvs_set_i32(my_handle, "sizeP", NVS_data.required_size_PSSWD);
        printf((err != ESP_OK) ? "Failed!\n" : "Done PSSWD REQUIRED\n");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done COMMIT\n");
        

        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        esp_wifi_connect();
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
    }
}

void init_smartwifi(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void smartconfig_example_task(void * parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    while (1) {
        uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
        if(uxBits & CONNECTED_BIT) {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        if(uxBits & ESPTOUCH_DONE_BIT) {
            int32_t status_next =1;
            
            size_t rq_SSID = required_size_SSID;
            size_t rq_PSSWD = required_size_PSSWD;
            err = nvs_get_str(my_handle, "SSID", (char *)ssid_WF,&rq_SSID);
            err = nvs_get_str(my_handle, "PASSWORD", (char *)password_WF,&rq_PSSWD);
            printf((err != ESP_OK) ? "Failed!\n" : "Done get\n");
            ESP_LOGI(TAG, "SSID:%s", ssid_WF);
            ESP_LOGI(TAG, "PASSWORD:%s", password_WF);
            ESP_LOGI(TAG, "smartconfig over");
            err = nvs_set_i32(my_handle, "STT_WFSCF", status_next);
            printf((err != ESP_OK) ? "Failed!\n" : "Done status\n");
            nvs_close(my_handle);
            esp_smartconfig_stop();
            printf("DONE SMART CONFIG\n");
            vTaskDelete(NULL);
        }
    }
}

void get_set_pro_wifi(void)
{
        err = nvs_get_i32(my_handle, "sizeS", &required_size_SSID);
        printf((err != ESP_OK) ? "Failed!\n" : "Done SSID REQUIRED\n");
        err = nvs_get_i32(my_handle, "sizeP", &required_size_PSSWD);
        printf((err != ESP_OK) ? "Failed!\n" : "Done PSSWD REQUIRED\n");
        size_t rq_SSID = required_size_SSID;
        size_t rq_PSSWD = required_size_PSSWD;
        err = nvs_get_str(my_handle, "SSID", (char *)ssid_WF,&rq_SSID);
        err = nvs_get_str(my_handle, "PASSWORD", (char *)password_WF,&rq_PSSWD);
        ESP_LOGI(TAG, "SSID:%s", ssid_WF);
        ESP_LOGI(TAG, "PASSWORD:%s", password_WF);
        ESP_LOGI(TAG, "smartconfig over");
}

