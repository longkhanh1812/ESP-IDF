#include <stdio.h>
#include "gpio_cf_dv.h"
#include "driver/gpio.h"



void config_gpio(void)
{
	gpio_config_t  io_conf;
	io_conf.intr_type  = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);
}



