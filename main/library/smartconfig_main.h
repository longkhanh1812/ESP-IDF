#include "nvs.h"

void init_smartwifi(void);
void get_set_pro_wifi(void);
int status_SMTCF;
nvs_handle_t my_handle;
esp_err_t err;
uint8_t ssid_WF[33] ;
uint8_t password_WF[65] ;

int32_t required_size_SSID;
int32_t required_size_PSSWD;


