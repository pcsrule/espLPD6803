#ifndef LPD6803_H
#define LPD6803_H
#include <stdint.h>

/* change these for your strip */
#define NUM_LEDS 50
#define STRIP_LENGTH_METERS 5
#define UPDATE_PERIOD_MS 10

/* effect handler type You can choose to return your own buffer for the strip
 * but it better be NUM_LEDS * 2 bytes long and be formatted for the LPD6803.
 * (MSB = 1, followed by green, red, blue 5-bit values)*/
typedef uint16_t * (* effect_handler_t)(uint16_t *strip);

/* for easily creating an array of named effects */
typedef struct {
    effect_handler_t handler;
    const char *name;
} effect_t;

/* API */
void led_strip_init(effect_handler_t effect);
void led_strip_suspend();
void led_strip_resume();
void led_strip_set_effect(effect_handler_t newEffect);
effect_handler_t led_strip_get_effect();

/* useful macros/functions for creating effects */

/* packs three r,g,b values (0-31) into a 16-bit frame for the strip */
#define SET_RGB_15(result, r, g, b) *result = 0x8000 | ((g) << 10) | ((r) << 5) | (b)

/* get any color along a 96-color wheel as a 16-bit frame for the strip */
uint16_t wheel(uint8_t wheelpos);
uint16_t wheel192(uint8_t wheelpos);

/* Precomputed colors */
#define LED_OFF 0x8000
#define LED_RED 0x83E0
#define LED_GREEN 0xFC00
#define LED_BLUE 0x801F
#define LED_CYAN (LED_BLUE | LED_GREEN)
#define LED_YELLOW (LED_GREEN | LED_RED)
#define LED_PURPLE (LED_RED | LED_BLUE)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Gravitational acceleration in m/s^2.
 * Change for other planets/celestial bodies */
#define GRAVITY_ACCEL_MSS 9.80665

/* convert acceleration to LED segments per update period squared */
#define GRAV_ACCEL (GRAVITY_ACCEL_MSS * STRIP_LENGTH_METERS / NUM_LEDS / (UPDATE_PERIOD_MS * UPDATE_PERIOD_MS))

#endif
