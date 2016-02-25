#include "espressif/esp_common.h"
#include "esp/uart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ssid_config.h"

// lpd6803 driver
#include "espLPD6803/lpd6803.h"

// built in effects, optional
#include "espLPD6803/effects.h"

#define EFFECT_SWITCH_DELAY_TICKS 5000 / portTICK_RATE_MS

void user_init(void)
{
    // the usual setup
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);
    
    // start LED strip task
    led_strip_init(rainbow_cycle);
    
    /* cycle through effects, this could use an array of function pointers
     * and be much less verbose. */
    for(;;) {
        vTaskDelay(EFFECT_SWITCH_DELAY_TICKS);
        led_strip_set_effect(rainbow_cycle192);
        vTaskDelay(EFFECT_SWITCH_DELAY_TICKS);
        led_strip_set_effect(cylon);
        vTaskDelay(EFFECT_SWITCH_DELAY_TICKS);
        led_strip_set_effect(drip);
        vTaskDelay(EFFECT_SWITCH_DELAY_TICKS);
        led_strip_set_effect(chase);
        vTaskDelay(EFFECT_SWITCH_DELAY_TICKS);
        led_strip_set_effect(rainbow_cycle);
    }
}
