#ifndef RGB_H
#define RGB_H
#include <stdint.h>

extern int16_t time_328ns;
void display_rgb(void);
void delay_us(uint32_t ns);
void rgb_off(void);
#endif
