#pragma once

#include "main.h"

typedef struct {
    void (*wifi_sta_start_callback)(void *context);
    void (*wifi_connected_callback)(void *context);
    void (*wifi_disconnected_callback)(void *context);
    void (*wifi_got_ip_callback)(void *context);
} WiFiHandler_t;

void wifi_init_sta(WiFiHandler_t *wifi_handler);