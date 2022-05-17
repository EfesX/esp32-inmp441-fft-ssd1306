#include "mic_spectrum.h"

#include "../esp-dsp/modules/common/include/esp_dsp.h"
#include <math.h>

TaskHandle_t TaskHandleMicReader;
TaskHandle_t TaskHandleMicFFT;
TaskHandle_t TaskHandleMicOLED;

ssd1306_framebuffer_t *oled;

__attribute__((aligned(16))) float wind[SAMPLE_BUFFER_SIZE];    // Window coefficients
__attribute__((aligned(16))) float y_cf_1[SAMPLE_BUFFER_SIZE*2];  // Working complex array
__attribute__((aligned(16))) float f_samples[SAMPLE_BUFFER_SIZE];  //

static int32_t raw_samples[SAMPLE_BUFFER_SIZE];

static const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate            = SAMPLE_RATE,
    .bits_per_sample        = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format         = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format   = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags       = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count          = 4,
    .dma_buf_len            = 1024,
    .use_apll               = false,
    .tx_desc_auto_clear     = false,
    .fixed_mclk             = 0,
};

static const i2s_pin_config_t i2s_pin_config = {
    .bck_io_num     = I2S_MIC_SCK,
    .ws_io_num      = I2S_MIC_WS,
    .data_out_num   = I2S_PIN_NO_CHANGE,
    .data_in_num    = I2S_MIC_SD
};

void mic_fft_task(void *p){
    int temp = 0;
    uint32_t bytes_read = 0;

    while(1){
        ESP_ERROR_CHECK(i2s_read(I2S_MIC_NUM, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY));

        for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++){
            f_samples[i] = ALPHA * (float)raw_samples[i] + BETTA * (float)(f_samples[i]);
        }

        for (int i=0 ; i< SAMPLE_BUFFER_SIZE ; i++){
            y_cf_1[i*2 + 0] = (float)f_samples[i]*wind[i];
            y_cf_1[i*2 + 1] = 0;
        }

        dsps_fft2r_fc32(y_cf_1, SAMPLE_BUFFER_SIZE);
        dsps_bit_rev_fc32(y_cf_1, SAMPLE_BUFFER_SIZE);
        dsps_cplx2reC_fc32(y_cf_1, SAMPLE_BUFFER_SIZE);

        for (uint8_t i = 0; i < SAMPLE_BUFFER_SIZE / 2; i++) {
            y_cf_1[i] = (10*log10f((y_cf_1[i * 2 + 0] * y_cf_1[i * 2 + 0] + y_cf_1[i * 2 + 1] * y_cf_1[i * 2 + 1])/SAMPLE_BUFFER_SIZE));
            y_cf_1[i] = (y_cf_1[i] - FFT_OFFSET) * FFT_SCALE_FACTOR;
        }

        ssd1306_clear(oled);
        for (uint8_t i = 0; i < SAMPLE_BUFFER_SIZE / 2; ++i){
            temp = (int) (y_cf_1[i]);
            if (temp > 63) temp = 63;
            if (temp <= 1) temp = 1;
            ssd1306_add_vline(oled, ((SAMPLE_BUFFER_SIZE / 2) - 1) - i, (uint8_t) temp);
        }
        ssd1306_draw(oled);
    }
    vTaskDelete(NULL);
}

void mic_spectrum_start(ssd1306_framebuffer_t *oledbuf){

    oled = oledbuf;
    
    ESP_ERROR_CHECK(i2s_driver_install(I2S_MIC_NUM, &i2s_config, 0, NULL));
    ESP_ERROR_CHECK(i2s_set_pin(I2S_MIC_NUM, &i2s_pin_config));

    ESP_ERROR_CHECK(dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE));

    #if (WINDOW == WIN_BLACKMAN_HARRIS)
        dsps_wind_blackman_harris_f32(wind, SAMPLE_BUFFER_SIZE);
    #elif (WINDOW == WIN_BLACKMAN)
        dsps_wind_blackman_f32(wind, SAMPLE_BUFFER_SIZE);
    #elif (WINDOW == WIN_HANN)
        dsps_wind_hann_f32(wind, SAMPLE_BUFFER_SIZE);
    #endif

    xTaskCreatePinnedToCore(mic_fft_task, "mic_fft", 4096, NULL, 0, &TaskHandleMicFFT, 0);
}
