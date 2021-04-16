#include "wifi_main.h"
#include "MQTT_task.h"
#include "gatts_demo.h"
#include "freertos/FreeRTOS.h"
#include "MQTT_task.h"
#include "esp_log.h"
#include "gpio_cf_dv.h"
#include "smartconfig_main.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "gatts_demo.h"
#include "http_server.h"


const char *TAG = "ESP32_MAIN_APP" ;



void app_main(void)
{
     
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle...\n");
    
    err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        
        err = nvs_get_i32(my_handle, "STT_WFSCF", &status_SMTCF);
        switch(err)
        {
            // case ESP_OK : 
            //     test_ble();
            //     config_gpio();
                
            //     wifi_init_sta();
            //     start_webserver();
            //     taskmqtt();
            //     break;
            // case ESP_ERR_NVS_NOT_FOUND:
            //     init_smartwifi();
            //     printf("The value is not init\n");
            //     break;
            // default:
            //     printf("Error reading");
            //     break;
        }
    }
    
}