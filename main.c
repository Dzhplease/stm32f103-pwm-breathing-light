#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"

typedef enum
{
	MODE_ALL_OFF         =0,  //全灭
	MODE_ALL_ON          =1,  //全亮
	MODE_ALTERNATE_BLINK =2,  //交替闪烁
	MODE_SYNC_BREATH     =3,  //同相呼吸灯
	MODE_ASYNC_BREATH    =4,  //反相呼吸灯
}WorkMode;

static WorkMode g_Current_Mode=MODE_ALL_OFF; // 当前工作模式
static uint16_t g_Duty=0;                    // 呼吸灯当前占空比 (0 ~ 100)
static uint8_t g_Breath_Dir=1;                       // 呼吸方向: 1-变亮, 0-变暗

static void Breath_Update()
{
	if(g_Breath_Dir==1)
	{
		g_Duty++;
		if(g_Duty>=100)
		{
			g_Breath_Dir=0;
		}
	}
	else 
	{
		if(g_Duty>0)
		{
			g_Duty--;
		}
		if(g_Duty==0)
		{
			g_Breath_Dir=1;
		}
	}
}

static void State_Reset()
{
	g_Duty=0;
	g_Breath_Dir=1;
}

int main()
{
	uint8_t KeyNum=0;
	
	OLED_Init();
	Key_Init();
	PWM_Init();
	
	
	while(1)
	{
        /*  按键扫描与模式切换 */
	    KeyNum=Key_GetNum ();
		 
		if(KeyNum==1)
		{
			g_Current_Mode = (WorkMode)((g_Current_Mode % 4) + 1);
           State_Reset();
		}
		
		if(KeyNum ==2)
		{
			g_Current_Mode = MODE_ALL_OFF;
            State_Reset();
		}
		
		/*   模式执行   */
		switch(g_Current_Mode)
		{
			case MODE_ALL_OFF:
                PWM_SetCompare1(0);
                PWM_SetCompare2(0);
                break;

            case MODE_ALL_ON:
                PWM_SetCompare1(99);
                PWM_SetCompare2(99);
                break;

            case MODE_ALTERNATE_BLINK:
                PWM_SetCompare1(0);
                PWM_SetCompare2(99);
                Delay_ms(100);
                PWM_SetCompare1(99);
                PWM_SetCompare2(0);
                Delay_ms(100);
                break;

            case MODE_SYNC_BREATH:
                Breath_Update();
                PWM_SetCompare1(g_Duty);
                PWM_SetCompare2(g_Duty);
                Delay_ms(10);
                break;

            case MODE_ASYNC_BREATH:
                Breath_Update();
                PWM_SetCompare1(g_Duty);
                PWM_SetCompare2(100 - g_Duty);
                Delay_ms(10);
                break;

            default:
                g_Current_Mode = MODE_ALL_OFF;
                break;
		}
	}
}

