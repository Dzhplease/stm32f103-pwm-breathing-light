#include "stm32f10x.h"                  // Device header

void LED_Init()
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init (GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits (GPIOA,GPIO_Pin_0);
	GPIO_SetBits (GPIOA,GPIO_Pin_1);
}

void LED1_ON()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}
void LED1_OFF()
{
	GPIO_SetBits (GPIOA,GPIO_Pin_0);
}
	
	
void LED2_ON()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
void LED2_OFF()
{
	GPIO_SetBits (GPIOA,GPIO_Pin_1);
}
	
void LED_Turn()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		LED1_OFF();
		LED2_ON();
	}
	else 
	{
		LED2_OFF ();
		LED1_ON ();
	}
}
void LED_Stop()
{
	LED2_OFF ();
	LED1_OFF ();
}