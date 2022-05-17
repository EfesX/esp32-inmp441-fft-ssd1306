#pragma once

#include "main.h"
#include "driver/i2s.h"
#include "ssd1306/ssd1306_esp32.h"

#define SAMPLE_BUFFER_SIZE  256

#define ALPHA               K_FILTER                     
#define BETTA               (1 - ALPHA)


void mic_spectrum_start(ssd1306_framebuffer_t *oledbuf);