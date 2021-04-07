#include "stdio.h"


typedef struct str_data{
    uint8_t ssid_WF[33] ;
    uint8_t password_WF[65] ;
    int32_t required_size_SSID;
    int32_t required_size_PSSWD;
}str_data;

str_data NVS_data;