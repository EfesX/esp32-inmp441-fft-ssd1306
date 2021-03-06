Отображение аудио спектра на OLED SSD1306.

Настройка подключения к Wi-Fi:
```c
#define APP_WIFI_SSID   "SSID"      // SSID Wi-Fi точки доступа
#define APP_WIFI_PASS   "12345678"  // Пароль Wi-Fi точки доступа
#define APP_MAX_RETRY   5           // Кол-во попыток подключения к точке доступа
```

Настройка частоты дискретизации:
```c
#define SAMPLE_RATE         48000               // Частота дискретизации аудио-сигнала
```
Отображаемые на OLED частоты лежат в диапазоне `0 ... (SAMPLE_RATE / 2) Гц`.

Конфигурация I2S для работы с INMP441:
```c
#define I2S_MIC_NUM I2S_NUM_0               
#define I2S_MIC_SCK GPIO_NUM_32
#define I2S_MIC_WS  GPIO_NUM_25
#define I2S_MIC_SD  GPIO_NUM_33
```

Настройка БПФ:
```c
#define WINDOW              WIN_BLACKMAN_HARRIS     // Выбор оконной функции для FFT
#define K_FILTER            0.3                     // К-нт сглаживания фильтра
#define FFT_OFFSET          (-100)                  // Сдвиг спектра по оси ординат
#define FFT_SCALE_FACTOR    ((float)(0.85))         // К-нт масштабирования спектра
```

С помощью `WINDOW` можно изменить оконную функцию. Допустимые оконные функции (`WIN_BLACKMAN`, `WIN_HANN`, `WIN_BLACKMAN_HARRIS`).

`K_FILTER` определяет степень фильтрации входного сигнала, что влечет за собой плавность отображения спектра на дисплее. 
Диапазон допустимых значений `K_FILTER`: 0 ... 1. 
Меньшие значения `K_FILTER` приводят к большему сглаживанию.
Сглаживание происходит по формуле: `S[i] = K_FILTER * S[i] + (1 - K_FILTER) * S[i - 1]`

С помощью `FFT_OFFSET` можно убрать уровень шумов в спектре за пределы отображения дисплея.


