#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t Key_Num;
void Key_Init()
{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB ,ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 |GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period =100-1;     //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler =720-1;   //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter =0;
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig (TIM4,TIM_IT_Update,ENABLE);   //开启定时器内部中断
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd (TIM4,ENABLE);
}

void TIM4_IRQHandler(void)
{
	static uint8_t Key_State=0;
	if( TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		
		switch (Key_State)			
		{ 
			case 0:
			{
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
				{
					Key_Num=1;
	            	Key_State=1;
				}
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
				{
					Key_Num=2;
	            	Key_State=1;
				}
			}
			case 1:
			{
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1&&GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1)
				{
					Key_State=0;
				}
			}
		}
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t temp=0;
	temp=Key_Num ;
	Key_Num =0;
	return temp;
}
	