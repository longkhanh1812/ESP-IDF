#include "str_data.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "smartconfig_main.h"
#include "driver/gpio.h"
#include "gpio_cf_dv.h"


#define TAG "str_data"


void get_set_data(void)
{
        
        err = nvs_get_i32(my_handle, "sizeS", &NVS_data.required_size_SSID);
        printf((err != ESP_OK) ? "Failed!\n" : "Done SSID REQUIRED\n");
        err = nvs_get_i32(my_handle, "sizeP", &NVS_data.required_size_PSSWD);
        printf((err != ESP_OK) ? "Failed!\n" : "Done PSSWD REQUIRED\n");
        size_t rq_SSID = NVS_data.required_size_SSID;
        size_t rq_PSSWD = NVS_data.required_size_PSSWD;
        err = nvs_get_str(my_handle, "SSID", (char *)NVS_data.ssid_WF,&rq_SSID);
        err = nvs_get_str(my_handle, "PASSWORD", (char *)NVS_data.password_WF,&rq_PSSWD);
        err = nvs_get_i32(my_handle, "sttGPIO", &NVS_data.status);
        ESP_LOGI(TAG, "SSID:%s", NVS_data.ssid_WF);
        ESP_LOGI(TAG, "PASSWORD:%s", NVS_data.password_WF);
        ESP_LOGI(TAG, "status LED:%d", NVS_data.status);
        gpio_set_level(GPIO_OUTPUT_1,NVS_data.status);
        ESP_LOGI(TAG, "smartconfig over");
}
