#include "flash.h"

/*
0x0800 F800 - 0x0800 FFFF  Page 31

*/

#define EEPROM_PAGE 31
//Flash �׵�ַ ����**************************************
#define USER_PAGE_ADDR       0x0800F800 



USER_EEPROM_PAGE user_eeprom;

/*******************************************
���ܣ���ȡFlash����,��С1���֡�
����faddr����Ҫ��ȡ�ĵ�ַ
����ֵ����ǰ��ַ��ֵ
********************************************/
uint32_t pl_Flash_ReadWord( uint32_t faddr )
{
    return *(__IO uint32_t*)faddr; 
}

/********************************************
���ܣ�Flashҳ����
����page: ��Ҫ������ҳ�� page30~31  e.g 31 
*********************************************/
void pl_Flash_PageErase(uint32_t page)
{
	uint32_t SectorError;
	FLASH_EraseInitTypeDef EraseInitStruct;
	 
    if( page != EEPROM_PAGE)
    {
        return;
    }

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks =FLASH_BANK_1 ; 
	EraseInitStruct.Page = page ; 
	EraseInitStruct.NbPages = 1;
	HAL_FLASHEx_Erase(&EraseInitStruct,&SectorError);//FLASH_PageErase
     
} 




//�û����ݶ�ȡ****************************
//****************************************
void flash_read_user_parameter(void)
{ 
    uint32_t *p; 
    uint32_t addr = 0;
    uint8_t k=0;
  
    p=(uint32_t *)&user_eeprom;
    addr = USER_PAGE_ADDR;
    k=sizeof(user_eeprom)/4;
  
    for(uint8_t j = 0 ; j < k ; j ++)
    { 
        *p = pl_Flash_ReadWord(addr);
        addr += 4;
        p += 1;
    }
}



/**********************************
д�û����� ��Ϊ����С�ṹ�� д��һҳflash���棬��С��2K
***********************************/
void flash_save_user_parameter(void)
{ 
    uint64_t *p; 
    uint32_t addr=0;
    int8_t k=0,times=0;
   
    HAL_FLASH_Unlock();
    
    pl_Flash_PageErase(EEPROM_PAGE);

    p=(uint64_t *)&user_eeprom;
    addr = USER_PAGE_ADDR;
    k=sizeof(user_eeprom);

    for(times = 0; times < 8 ; times ++ )//�ֽڶ���
    {
        if(k % 8 != 0)
        {
            k++;
        }
        else
        {
            break;
        }
    }
    
    k /= 8;  
    
    for(uint8_t j = 0 ; j < k ; j ++)
    {//ÿһ���ṹ���0x100�Ŀռ�洢��д��flash,����double word˫�� 8�ֽ�д��
        HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD,addr,*p);
        addr+=8;
        p+=1;
    }
    HAL_FLASH_Lock();  
}

void user_setting_reset(void)
{
    user_eeprom.key_value = 0;
    user_eeprom.power_on_times = 100;
}

void user_parameter_load(void)
{
    flash_read_user_parameter();
    if( user_eeprom.safe_flag != 0xAA )
    {
       user_eeprom.safe_flag  = 0xAA;
      
        user_setting_reset();
        flash_save_user_parameter();
    }
}





