#include "main.h"
#include "wifi.h"
#include "ssd1306/ssd1306_esp32.h"
#include "mic_spectrum.h"

static const char *TAG = "MAIN";
WiFiHandler_t wifi_handler;

static ssd1306_framebuffer_t *oled;

void wifi_sta_start_callback(void *context){
    ssd1306_add_text(oled, 0, 32, "WIFI STA START");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);
}
void wifi_connected_callback(void *context){
    ssd1306_add_text(oled, 0, 32, "WIFI CONNECTED");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);
}
void wifi_disconnected_callback(void *context){
    ssd1306_add_text(oled, 0, 32, "WIFI DISCONNECTED");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);
}
void wifi_got_ip_callback(void *context){
    //ip_event_got_ip_t* event = (ip_event_got_ip_t*) context;
    ESP_LOGI(TAG, "IP GOT");
    ssd1306_add_text(oled, 0, 32, "WIFI GOT IP");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);
}

void app_main(void){
    oled = ssd1306_new();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_handler.wifi_sta_start_callback    = wifi_sta_start_callback;
    wifi_handler.wifi_connected_callback    = wifi_connected_callback;
    wifi_handler.wifi_disconnected_callback = wifi_disconnected_callback;
    wifi_handler.wifi_got_ip_callback       = wifi_got_ip_callback;

    wifi_init_sta(&wifi_handler);

    ssd1306_add_text(oled, 0, 32, "HELLO");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);

    ssd1306_add_text(oled, 0, 32, "FFT");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);

    ssd1306_add_text(oled, 0, 32, "SPECTRUM");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);

    ssd1306_add_text(oled, 0, 32, "by EFESX");
    ssd1306_draw(oled);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    ssd1306_clear(oled);

    mic_spectrum_start(oled);
}