#include "key.h"
#include "RGB.h"
#include "flash.h" 

uint8_t RGB_OFF_Flag=0;
uint16_t g_time = 0;

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_12)
    { 
        if(key_value != user_eeprom.key_value)
        {
            user_eeprom.key_value = key_value;
            flash_save_user_parameter();
        }
    }
}



void main_loop(void)
{
    user_parameter_load();
     
    if(user_eeprom.power_on_times > 0)
    { 
        user_eeprom.power_on_times --;
        flash_save_user_parameter();
        
    }
    else
    {
        while(1)
        {
            key_value = 0;
            rgb_off();
        }
    }
    
    key_value = user_eeprom.key_value; 
    while(1)
    {
        
        if(RGB_OFF_Flag == 0)
        {
            display_rgb();
        }
        else
        {
            rgb_off();
        }
        
        
        while( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET )
        {
            g_time++;
            HAL_Delay(100);
            if(g_time > 15)
            {
                g_time=0;
                if(RGB_OFF_Flag == 0)
                {
                    RGB_OFF_Flag = 1; 
                }
                else
                {
                    RGB_OFF_Flag = 0; 
                }
                
                 if(RGB_OFF_Flag == 0)
                    {
                        display_rgb();
                        while( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) != GPIO_PIN_RESET);
                    }
                    else
                    {
                        rgb_off();
                        while( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) != GPIO_PIN_RESET);
                    } 
                   
            }
        }
        g_time = 0;
        
                
    }
}
