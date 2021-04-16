#include "driver/gpio.h"

#define GPIO_OUTPUT_1    18
#define GPIO_OUTPUT_PIN_SEL  (1ULL << GPIO_OUTPUT_1)
#define ESP_INTR_FLAG_DEFAULT 0

#define GPIO_INPUT_1 19
#define GPIO_INPUT_PIN_SEL (1UL << GPIO_INPUT_1)
#define ESP_INTR_FLAG_DEFAULT 0
#define true 1
#define false 0



void config_gpio(gpio_mode_t mode, gpio_pullup_t p_up,gpio_pulldown_t p_down,gpio_int_type_t irq,int32_t pin_bit_mask);
void set_gpio();
void set_level_gpio(int32_t pin_bit_mask,int32_t mode);
