#include <stdio.h>
#include "gpio_cf_dv.h"
#include "esp_log.h"
#include "smartconfig_main.h"


#define TAG "gpio_cf_dv"


void config_gpio(gpio_mode_t mode, gpio_pullup_t p_up,gpio_pulldown_t p_down,gpio_int_type_t irq,int32_t pin_bit_mask)
{
	gpio_config_t  io_conf;
	io_conf.mode = mode;
	io_conf.pin_bit_mask = pin_bit_mask;
	io_conf.pull_down_en = p_down;
	io_conf.pull_up_en = p_up;
	io_conf.intr_type  = irq;
	gpio_config(&io_conf);
}

void set_gpio()
{
	config_gpio(GPIO_MODE_OUTPUT,false,false,GPIO_INTR_DISABLE,GPIO_OUTPUT_PIN_SEL);
	config_gpio(GPIO_MODE_INPUT,true,false,GPIO_INTR_DISABLE,GPIO_INPUT_PIN_SEL);
}

void set_level_gpio(gpio_num_t pin,int32_t mode)
{
	gpio_set_level(pin,mode);
	nvs_set_i32(my_handle,"sttGPIO",mode);
	ESP_LOGI(TAG,"set mode :%d\n",mode);
}



