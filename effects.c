#include "effects.h"
#include "lpd6803.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

uint16_t * rainbow_cycle(uint16_t *strip) {
    static int t;
        
    for(int i = 0; i < NUM_LEDS; ++i) {
        strip[i] = wheel(((i * 96 / NUM_LEDS) + t) % 96);
    }
    
    ++t;
    return strip;
}

uint16_t * rainbow_cycle192(uint16_t *strip) {
    static int t;
    
    for(int i = 0; i < NUM_LEDS; ++i) {
        strip[i] = wheel192(((i * 192 / NUM_LEDS) + t) % 192);
    }
    
    ++t;
    return strip;
}

uint16_t * cylon(uint16_t *strip) {
   static int x = NUM_LEDS - 1;
    
    strip[ABS(x)] = LED_OFF;
    
    --x;
    
    strip[ABS(x)] = LED_RED;
    
    if(ABS(x) == NUM_LEDS - 1)
        x = NUM_LEDS - 1;
        
    vTaskDelay(25 / portTICK_RATE_MS);
    
    return strip;
}

uint16_t * drip(uint16_t *strip) {
    static int x = 0, t = 0;
    
    strip[x] = LED_OFF;
    
    /* Good old 1/2at^2 from HS physics */
    x = (NUM_LEDS - 1) - ((int)(0.5 * GRAV_ACCEL * t * t));
    
    strip[x] = LED_BLUE;
    
    ++t;
    if(x <= 0)
        t = 0;
    return strip;
}

uint16_t * chase(uint16_t *strip) {
    static unsigned int t;
    
    for(int i = 0; i < NUM_LEDS; ++i) {
        strip[i] = (i + t) & 3 ? LED_OFF : wheel(((i * 96 / NUM_LEDS) - t) % 96);
    }
    
    vTaskDelay(50 / portTICK_RATE_MS);
    
    ++t;
    return strip;
}
/*
 * Divide the strip into 4 segments designated by the first LED in that segment,
 * 0 indexed.
*/
#define LEFT_OFFSET 0
#define TOP_OFFSET 11
#define RIGHT_OFFSET 25
#define BOTTOM_OFFSET 36

#define LEFT(x) (x) >= LEFT_OFFSET && (x) < TOP_OFFSET
#define TOP(x) (x) >= TOP_OFFSET && (x) < RIGHT_OFFSET
#define RIGHT(x) (x) >= RIGHT_OFFSET && (x) < BOTTOM_OFFSET
#define BOTTOM(x) (x) >= BOTTOM_OFFSET && (x) < NUM_LEDS

uint16_t * edges(uint16_t *strip) {
    static unsigned int t;
    unsigned int i = 0;
    
    for(; LEFT(i); ++i) {
        strip[i] = LED_RED;
    }
    for(; TOP(i); ++i) {
        strip[i] = LED_GREEN;
    }
    for(; RIGHT(i); ++i) {
        strip[i] = LED_BLUE;
    }
    for(; BOTTOM(i); ++i) {
        strip[i] = LED_YELLOW;
    }
    
    ++t;
    
    return strip;
}
