#pragma once

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/* Configuration Wi-Fi */
#define APP_WIFI_SSID   "Netzwerk"
#define APP_WIFI_PASS   "ubuntu56@brahamDC"
#define APP_MAX_RETRY   5

/* Configuration microphone INMP441*/
#define SAMPLE_BUFFER_SIZE  256
#define SAMPLE_RATE         48000

#define I2S_MIC_NUM I2S_NUM_0
#define I2S_MIC_SCK GPIO_NUM_32
#define I2S_MIC_WS  GPIO_NUM_25
#define I2S_MIC_SD  GPIO_NUM_33

/* Configuration FFT */
#define WINDOW              WIN_BLACKMAN_HARRIS
#define ALPHA               0.3
#define BETTA               (1 - ALPHA)
#define FFT_OFFSET          100
#define FFT_SCALE_FACTOR    ((float)(0.85))
