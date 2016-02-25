#ifndef LPD6803_EFFECTS_H
#define LPD6803_EFFECTS_H

#include <stdint.h>

/* These prototypes can be used with the driver as effect_handler_t type 
 * function pointers.
 */
uint16_t * rainbow_cycle(uint16_t *strip);
uint16_t * rainbow_cycle192(uint16_t *strip);
uint16_t * cylon(uint16_t *strip);
uint16_t * drip(uint16_t *strip);
uint16_t * chase(uint16_t *strip);
uint16_t * edges(uint16_t *edges);

#endif
