#ifndef flash_H
#define flash_H
#include "stm32g0xx_hal.h"

typedef struct
{
    uint32_t safe_flag;
    uint32_t key_value;
    uint32_t power_on_times;//50
}USER_EEPROM_PAGE;

extern USER_EEPROM_PAGE user_eeprom;

void user_parameter_load(void);
void flash_save_user_parameter(void);

#endif
