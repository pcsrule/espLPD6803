#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "spi/spi.h"

#include "lpd6803.h"

/* message format for telling the task what to do */
struct queueMessage {
    enum {
        SUSPEND,
        CHANGE_EFFECT,
    } message_type;
    effect_handler_t new_effect;
};

/* static prototypes */
static void lpd6803_task(void *pvParameters);

/* static variables */
static xTaskHandle lpd6803TaskHandle;   // primary task for SPI actions
static uint16_t strip[NUM_LEDS];        // buffer to hold state of strip
static effect_handler_t current_effect; // current effect on display
static xQueueHandle taskMessageQueue;   // queue to tell the task what to do

/* Begin API functions */
void led_strip_init(effect_handler_t effect) {
    current_effect = effect;
    
    /* turn off all LEDs */
    for(int i = 0; i < NUM_LEDS; ++i)
        strip[i] = LED_OFF;
    
    xTaskCreate(&lpd6803_task, (signed char *)"lpd6803", 256, NULL, 3, &lpd6803TaskHandle);
}

/* pause the current effect and blank the LED strip */
void led_strip_suspend() {
    static const struct queueMessage suspendMessage = {SUSPEND, NULL};
    xQueueSendToBack(taskMessageQueue, &suspendMessage, 0);
}

/* resume the currently selected effect */
void led_strip_resume() {
    vTaskResume(lpd6803TaskHandle);
}

/* change the LED strip effect, blanking the LEDs inbetween */
void led_strip_set_effect(effect_handler_t newEffect) {
    struct queueMessage changeEffectMessage = {CHANGE_EFFECT, newEffect};
    xQueueSendToBack(taskMessageQueue, &changeEffectMessage, 0);
}

/* get the current strip effect function pointer */
effect_handler_t led_strip_get_effect() {
    return current_effect;
}
/* End API functions */

static void lpd6803_task(void *pvParameters) {
    const portTickType xPeriod = UPDATE_PERIOD_MS / portTICK_RATE_MS;
    portTickType xLastWakeTime;
    struct queueMessage receivedMessage;
    
    taskMessageQueue = xQueueCreate(1, sizeof(struct queueMessage));
    
    if(taskMessageQueue == 0) {
        /* Couldn't create queue. Give up. */
        return;
    }
    
    xLastWakeTime = xTaskGetTickCount();
    
    /* Init SPI and start first frame */
    spi_init(HSPI);
    spi_tx32(HSPI, 0);
    
    for(;;) {
        uint16_t *output = current_effect(strip);
        
        vPortEnterCritical();
        
        for(int i = NUM_LEDS - 1; i >= 0; --i) {
            spi_tx16(HSPI, output[i]);
        }
        // end frame
        spi_tx32(HSPI, 0);
        
        vPortExitCritical();
        
        if(xQueueReceive(taskMessageQueue, &receivedMessage, 0) == pdTRUE) {
        
            switch(receivedMessage.message_type) {
            case SUSPEND:
                /* Someone said stop, turn off the strip and suspend task */
                vPortEnterCritical();
                for(int i = 0; i < NUM_LEDS; ++i) {
                    spi_tx16(HSPI, LED_OFF);
                }
                /* end frame */
                spi_tx32(HSPI, 0);
                vPortExitCritical();
                vTaskSuspend(lpd6803TaskHandle);
                break;
            case CHANGE_EFFECT:
                vPortEnterCritical();
                
                /* Clear out buffer for new effect */
                for(int i = 0; i < NUM_LEDS; ++i)
                    strip[i] = LED_OFF;
                    
                current_effect = receivedMessage.new_effect;
                
                vPortExitCritical();
                break;
            }
            
            /* Skip updates we missed while suspended or busy */
            xLastWakeTime = xTaskGetTickCount();
        } else {
            /* wait for next update */
            vTaskDelayUntil(&xLastWakeTime, xPeriod);
        }
        
        
    }
}

inline uint16_t wheel(uint8_t wheelpos) {
  switch(wheelpos >> 5) {
    case 0:
        return LED_OFF | (wheelpos & 31) << 10 | (31 - (wheelpos & 31)) << 5;
    case 1:
        return LED_OFF | (31 - (wheelpos & 31)) << 10 | (wheelpos & 31);
    case 2:
        return LED_OFF | (wheelpos & 31) << 5 | (31 - (wheelpos & 31));
    default: // Off if over 95
        return LED_OFF;
  }
}

inline uint16_t wheel192(uint8_t wheelpos) {
    switch(wheelpos >> 5) {
    case 0:
        return LED_RED | (31 - (wheelpos & 31));
    case 1:
        return LED_RED | (wheelpos & 31) << 10;
    case 2:
        return LED_GREEN | (31 - (wheelpos & 31)) << 5;
    case 3:
        return LED_GREEN | (wheelpos & 31);
    case 4:
        return LED_BLUE | (31 - (wheelpos & 31)) << 10;
    case 5:
        return LED_BLUE | (wheelpos & 31) << 5;
    default: // Off if over 191
        return LED_OFF;
    }
}
