#include "RGB.h"
#include "gpio.h"
#include "EventRecorder.h"
#include "key.h"


uint8_t Macaroon_table[18]={
0x00,0xFF,0xFF,
0xFF,0x00,0xFF,
0xFF,0xFF,0x00,
0xFF,0x00,0x00,
0x00,0xFF,0x00,
0x00,0x00,0xFF,  
};


uint8_t sin_table[72]=
{
    127 ,
138 ,
149 ,
160 ,
170 ,
181 ,
191 ,
200 ,
209 ,
217 ,
224 ,
231 ,
237 ,
242 ,
246 ,
250 ,
252 ,
254 ,
254 ,
254 ,
252 ,
250 ,
246 ,
242 ,
237 ,
231 ,
224 ,
217 ,
209 ,
200 ,
191 ,
181 ,
170 ,
160 ,
149 ,
138 ,
127 ,
116 ,
105 ,
94 ,
84 ,
73 ,
64 ,
54 ,
45 ,
37 ,
30 ,
23 ,
17 ,
12 ,
8 ,
4 ,
2 ,
0 ,
0 ,
0 ,
2 ,
4 ,
8 ,
12 ,
17 ,
23 ,
30 ,
37 ,
45 ,
54 ,
63 ,
73 ,
84 ,
94 ,
105 ,
116 
};

uint8_t sin90_table[72]=
{
    241 ,
235 ,
229 ,
222 ,
214 ,
206 ,
197 ,
187 ,
177 ,
167 ,
156 ,
146 ,
134 ,
123 ,
112 ,
101 ,
91 ,
80 ,
70 ,
60 ,
51 ,
43 ,
35 ,
27 ,
21 ,
15 ,
10 ,
6 ,
3 ,
1 ,
0 ,
0 ,
1 ,
3 ,
5 ,
9 ,
13 ,
19 ,
25 ,
32 ,
40 ,
48 ,
57 ,
67 ,
77 ,
87 ,
98 ,
108 ,
120 ,
131 ,
142 ,
153 ,
163 ,
174 ,
184 ,
194 ,
203 ,
211 ,
219 ,
227 ,
233 ,
239 ,
244 ,
248 ,
251 ,
253 ,
254 ,
254 ,
253 ,
251 ,
249 ,
245 
};

uint8_t sin180_table[72]=
{
    25 ,
19 ,
14 ,
9 ,
5 ,
3 ,
1 ,
0 ,
0 ,
1 ,
3 ,
6 ,
10 ,
15 ,
21 ,
27 ,
34 ,
42 ,
51 ,
60 ,
70 ,
80 ,
90 ,
101 ,
112 ,
123 ,
134 ,
145 ,
156 ,
167 ,
177 ,
187 ,
197 ,
206 ,
214 ,
222 ,
229 ,
235 ,
240 ,
245 ,
249 ,
251 ,
253 ,
254 ,
254 ,
253 ,
251 ,
248 ,
244 ,
239 ,
233 ,
227 ,
220 ,
212 ,
203 ,
194 ,
184 ,
174 ,
164 ,
153 ,
142 ,
131 ,
120 ,
109 ,
98 ,
87 ,
77 ,
67 ,
57 ,
48 ,
40 ,
32 
};


void delay_us(uint32_t ns)
{
    uint32_t i=0,j=0;
    
    for(i=ns;i>0;i--)
        for(j=0;j<1000;j++);
}

void ZERO_CODE(void)
{
    EventStartA(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    delay_us(5);
    EventStopA(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    delay_us(20);
}

void ONE_CODE(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    delay_us(15);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    delay_us(10);
}

void RESET_CODE(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    delay_us(500);
}


void write_data(uint8_t RGB_DAT)
{ 
    for(uint8_t i=0; i<8; i++)
    {
        if( RGB_DAT & 0x80)
        {
            ONE_CODE();
        }
        else
        {
            ZERO_CODE();
        }
        RGB_DAT <<= 1;
    }  
} 

void one_colour(uint8_t R_Value,uint8_t G_Value,uint8_t B_Value)
{
    uint8_t i=0;
    
    for(i=0;i<6;i++)
    {
        write_data(G_Value);
        write_data(R_Value);
        write_data(B_Value);
        RESET_CODE();
    }
}

void RGB_jianbian()
{
    uint8_t R_Value = 200;
    uint8_t G_Value = 200;
    uint8_t B_Value = 200;
    uint8_t i=0,j=0,times=0;
    uint8_t key_value_temp = key_value;
    
    for( times = 0; times < 7 ; times++ )
    {
        if(times == 6)
        {
            R_Value = 200;
            G_Value = 200;
            B_Value = 200;
        }
        
        for(i=0;i<200;i++)
        {
            switch(times)
            {
                case 0:R_Value--;G_Value=0xff;B_Value=0xff;
                    break;
                case 1:R_Value=0xff;G_Value--;B_Value=0xff;
                    break;
                case 2:R_Value=0xff;G_Value=0xff;B_Value--;
                    break;
                case 3:R_Value--;G_Value--;B_Value=0xff;
                    break;
                case 4:R_Value--;G_Value=0xff;B_Value--;
                    break;
                case 5:R_Value=0xff;G_Value--;B_Value--;
                    break;
                case 6:R_Value--;G_Value--;B_Value--;
                    break;
                default:
                    break;
            }  
            one_colour(R_Value,G_Value,B_Value);
            HAL_Delay(5);
            if(  key_value != key_value_temp)
            {
                break;
            }
        }
        for(i=0;i<200;i++)
        {
            switch(times)
            {
                case 0:R_Value++;G_Value=0xff;B_Value=0xff;
                    break;
                case 1:R_Value=0xff;G_Value++;B_Value=0xff;
                    break;
                case 2:R_Value=0xff;G_Value=0xff;B_Value++;
                    break;
                case 3:R_Value++;G_Value++;B_Value=0xff;
                    break;
                case 4:R_Value++;G_Value=0xff;B_Value++;
                    break;
                case 5:R_Value=0xff;G_Value++;B_Value++;
                    break;
                case 6:R_Value++;G_Value++;B_Value++;
                    break;
                default:
                    break;
            } 
            one_colour(R_Value,G_Value,B_Value);
            HAL_Delay(5);
            if(  key_value != key_value_temp)
            {
                break;
            }
        }
        if(  key_value != key_value_temp)
        {
            break;
        }
    }        
}

void Macaroon(void)
{
    uint16_t i=0;
    uint8_t key_value_temp = key_value;
    
        for(i=0;i<72;i++)
        { 
            write_data(sin_table[(i+8)%72]); //G
            write_data(sin90_table[(i+8)%72]); //R
            write_data(sin180_table[(i+8)%72]); //B

            write_data(sin_table[(i+20)%72]); //G
            write_data(sin90_table[(i+20)%72]); //R
            write_data(sin180_table[(i+20)%72]); //B
            
            write_data(sin_table[(i+31)%72]); //G
            write_data(sin90_table[(i+31)%72]); //R
            write_data(sin180_table[(i+31)%72]); //B
            
            write_data(sin_table[(i+43)%72]); //G
            write_data(sin90_table[(i+43)%72]); //R
            write_data(sin180_table[(i+43)%72]); //B
            
            write_data(sin_table[(i+54)%72]); //G
            write_data(sin90_table[(i+54)%72]); //R
            write_data(sin180_table[(i+54)%72]); //B
            
            write_data(sin_table[(i+66)%72]); //G
            write_data(sin90_table[(i+66)%72]); //R
            write_data(sin180_table[(i+66)%72]); //B
             
            RESET_CODE();
            HAL_Delay(50);   
            if(  key_value != key_value_temp)
            {
                return;
            } 
        } 
}


void Macaroon1(void)
{
    uint8_t R_Value = 0xff;
    uint8_t G_Value = 0xff;
    uint8_t B_Value = 0xff;
    uint16_t i=0,j=0,k=0,m=0;
    uint8_t key_value_temp = key_value;
    
    
    for(k=0;k<18;k+=3)
    { 
        for(m=0;m<18;m+=3)
        {//0 3 6 9 12 15 18
            write_data(Macaroon_table[(m+0+k)%18]); //G
            write_data(Macaroon_table[(m+1+k)%18]); //R
            write_data(Macaroon_table[(m+2+k)%18]); //B
            if(  key_value != key_value_temp)
            {
                return;
            }
        } 
        RESET_CODE();
        HAL_Delay(500); 
        if(  key_value != key_value_temp)
        {
            return;
        }
        HAL_Delay(500); 
        if(  key_value != key_value_temp)
        {
            return;
        }
    }
}

void display_rgb(void)
{
    
    switch( key_value )
    {
         case 0:
             RGB_jianbian();
             break;
         case 1:
             one_colour(0xff,0xff,0);
             break;
         case 2:
             one_colour(0xff,0,0xff);
             break;
         case 3:
             one_colour(0,0xff,0xff);
             break;
         case 4:
             one_colour(0,0xff,0);
             break;
         case 5:
             Macaroon();
             break;
         case 6:
             Macaroon1();
             break;
         default:
             break;
    }
    HAL_Delay(20);
}

void rgb_off(void)
{
    one_colour(0xff,0xff,0xff);
    HAL_Delay(1000);
}