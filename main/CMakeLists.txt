idf_component_register(SRCS  "BLE/gatts_demo.c" 
                              "WIFI/wifi_main.c" 
                              "MQTT/MQTT_task.c"  
                              "ESP32_main.c" 
                              "WIFI/smartconfig_main.c"
                              "HTTP/http_server.c"
                    INCLUDE_DIRS "BLE" "WIFI" "MQTT" "HTTP"
                   )
