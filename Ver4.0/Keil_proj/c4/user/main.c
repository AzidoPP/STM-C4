#include "stm32f10x.h"
#include "matrixkey.h"
#include "1602a.h"
#include "delay.h"
#include "LED.H"
#include "math.h"
#include "TIMER.h"
#include "Serial.h"
#include "mp3.h"


unsigned char tem[] = {0x1C,0x14,0x1F,0x09,0x08,0x08,0x09,0x0F};	//摄氏度符号“℃”的字模

char areArraysEqual(unsigned char arr1[], unsigned char arr2[], int size) 
{
    for (int i = 0; i < size; i++)
	{
        if (arr1[i] != arr2[i])
				{
            return 0; // 如果发现有不相等的元素，返回false
        }
    }
    return 1; // 所有元素都相等，返回true
}

void rightShiftArray(unsigned char arr[], int size)
{
    if (size > 0)
		{
//        char last = arr[size - 1]; // 保存最后一个元素的值
        for (int i = size - 1; i > 0; i--)
			  {
            arr[i] = arr[i - 1]; // 将每个元素向右移动
        }
        arr[0] = '*'; // 将第一个元素设置为*
    }
}


void leftShiftArray(unsigned char arr[], int size)
{
    if (size > 0) 
		{
//        int first = arr[0]; // 保存第一个元素的值
        for (int i = 0; i < size - 1; i++) 
			  {
            arr[i] = arr[i + 1]; // 将每个元素向左移动
        }
        arr[7] = 0; // 将最后一个元素设置为0
    }
}

char KeyNum,sign;
char ispressed=0;
uint16_t Num = 0,Num_sign = 0;			//定义在定时器中断里自增的变量
unsigned char password[8]={'*','*','*','*','*','*','*'};	//锁定密码
unsigned char password1[8]={'*','*','*','*','*','*','*'};	//解锁密码


void keyscan(unsigned char pass[])//扫描键盘，更新锁定密码数组并更新显示
{
	KeyNum = MatrixKey_GetValue();//默认被置为' '                                         
	if((KeyNum!=' ')&&(KeyNum!='*')&&(KeyNum!='#')&&(ispressed==0))//若数字0~9被按下   //所有的ispressed==0应该是为了只检测上升沿（按下动作 
	{                                                                                                      //而不是检测上升结果（按下状态 
		Delay_ms(50);
		LED1_Turn();
		Delay_ms(50);
		LED1_Turn();
		leftShiftArray(pass,8);//形参数组左移一位
		pass[6]=KeyNum;//"最后一位"为按下的键
		LCD_WRITE_StrDATA("                ",0);
		LCD_WRITE_StrDATA(pass,3 );//同下，从第3格写字符串
	}
	else if(KeyNum=='*'&&(ispressed==0))//按*则退格
	{
		Delay_ms(50);
		LED1_Turn();
		Delay_ms(50);
		LED1_Turn();
		rightShiftArray(pass,7);
		LCD_WRITE_StrDATA("                ",0);
		LCD_WRITE_StrDATA(pass,3 );
	}
	else if(KeyNum=='#'&&(ispressed==0))//按#则全删
	{
		Delay_ms(50);
		LED1_Turn();
		Delay_ms(50);
		LED1_Turn();
		for (int i = 0; i < 7; i++) 
	  { // 注意：size - 1是为了保留字符串的null终止符
			pass[i] = '*';
		}
		LCD_WRITE_StrDATA("                ",0);
		LCD_WRITE_StrDATA(pass,3 );
	}
	if(KeyNum!=' ')//键盘一旦按下，ispressed同时置1，一旦松开，同时置0
	{
		ispressed=1;//按下
	}
	else
	{
		ispressed=0;//未按
	}
}






int ar=0;
void keyscan1(unsigned char pass[])//扫描键盘，更新解锁密码数组并更新显示
{
	KeyNum = MatrixKey_GetValue();
	if(ispressed==0)
	{ 	
		if((KeyNum!=' ')&&(KeyNum!='*')&&(KeyNum!='#')&&(ispressed==0))//若数字0~9被按下
		{
//			Delay_ms(50);
//			LED1_Turn();
//			Delay_ms(50);
//			LED1_Turn();
			pass[ar]=KeyNum;//第一位为按下的键
			ar++;
			if(ar>=7)ar=0;
			LCD_WRITE_StrDATA("                ",0);
			LCD_WRITE_StrDATA(pass,3 );
		}
		else if(KeyNum=='*'&&(ispressed==0))//退格
		{
//			Delay_ms(50);
//			LED1_Turn();
//			Delay_ms(50);
//			LED1_Turn();
			ar--;
			pass[ar]='*';
			LCD_WRITE_StrDATA("                ",0);
			LCD_WRITE_StrDATA(pass,3 );
		}
		else if(KeyNum=='#'&&(ispressed==0))//全删
		{
//			Delay_ms(50);
//			LED1_Turn();
//			Delay_ms(50);
//			LED1_Turn();
			for (int i = 0; i < 7; i++) 
			{ // 注意：size - 1是为了保留字符串的null终止符
				pass[i] = '*';
			}
			ar=0;
			LCD_WRITE_StrDATA("                ",0);
			LCD_WRITE_StrDATA(pass,3 );
		}
	}
	if(KeyNum!=' ')
	{
		ispressed=1;//按下
		Delay_ms(20);
	}
	else
	{
		ispressed=0;
		Delay_ms(20);
	}
}



unsigned char xing[3]={'*','*','*'};
void LCD_xing(void)
{
	LCD_WRITE_StrDATA("                ",0);
	LCD_WRITE_StrDATA(xing,Num);		
	Delay_ms(20);
}







int main()
{
		
	
	Timer_Init();//定时器初始化
	LED_Init();//led 蜂鸣器 继电器io口初始化
	LCD_INIT();		//LCD1601初始化
	MatrixKey_Init();//矩阵键盘初始化

	LCD_WRITE_StrDATA("   *******      ",0 );	
	Delay_ms(50);	
	mp3_Init();//mp3初始化
	Delay_ms(50);	
	MP3CMD(0x06,25); //设置音量	  建议音量：8欧1瓦；25 若音质下降请调小音量！！

	
	//	mp3_start();//由于市面上mini mp3模块质量参差不齐，导致上电后无法立刻检测到指令，需等段时间才能检测到
					//如果你想启用上电音效，可以尝试取消此注释，尝试是否有上电音效
	while(1)
	{
		keyscan(password);//扫描键盘，更新锁定密码数组并更新显示
		if(password[0]!='*')//若输完密码，                   
		{

			for(long i=0; i<50000; i++)//防止设置密码后立即被检测到，其实就是延时的意思啦
			{
				keyscan(password);
			}
			if(password[0]!='*')//若成功设置密码
			{
				LED2_ON();//继电器
				mp3_over();
		
				for(int j=0; j<100; j++)//倒计时提示
				{
					LED1_ON();//蜂鸣器和led
					for(int i=0; i<2; i++)//蜂鸣器和亮起时间
					{
						  KeyNum = MatrixKey_GetValue();
							if(KeyNum!=' '){sign=1;}
							if(sign==0)
							{
								Delay_ms(20);					
							}
							else
							{
								keyscan1(password1);							
							}
					}
					LED1_OFF();
					
					
					for(int i=0; i<((int)((pow(((double)0.978),((double)j)))*50)-2); i++)//j增大，此for循环随时间增大而执行次数变少，所占用时间变短，即LED1_ON()被执行间隔的越来越短			
					{
						  KeyNum = MatrixKey_GetValue();
							if(KeyNum!=' '){sign=1;}
							if(sign==0)
							{
									LCD_xing();							
							}
							else
							{
									keyscan1(password1);							
							}
					}
					
					
					if(password1[6]!='*')//若输完解锁密码，则开始检测是否正确
					{
						if(areArraysEqual(password,password1,8))//若正确
						{
							
							LCD_WRITE_StrDATA("                ",0);
							Delay_ms(50);//拆弹成功提示音
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							LCD_WRITE_StrDATA(password1,3 );
							Delay_ms(50);//拆弹成功提示音
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							Delay_ms(50);
							LED1_Turn();
							LCD_WRITE_StrDATA("                ",0);
							Delay_ms(400);
							LCD_WRITE_StrDATA(password1,3 );
							Delay_ms(400);
							LCD_WRITE_StrDATA("                ",0);
							Delay_ms(400);
							LCD_WRITE_StrDATA(password1,3 );

							

							return 0;
						}
						else//若不正确，密码不一致，全删
						{
							for (int i = 0; i < 7; i++) 
							{
								password1[i] = '*';
							}
							ar=0;
							LCD_WRITE_StrDATA(password1,3 );
							sign=0;
						}
					}
				}
				
				
				LED1_ON();//40s倒计时结束，蜂鸣器和led常亮1.3s
				LCD_WRITE_StrDATA("                ",0);
				mp3_boom_music();
				Delay_ms(1300);
				LED1_OFF();
				
				
				while(1)//所有程序执行完毕，自动进入while死循环
				{
					LED2_OFF();
				}
			}
		
		
			if(KeyNum!=' ')
			{
				ispressed=1;
			}
			else
			{
				ispressed=0;
			}
		}
  }


}

void TIM2_IRQHandler(void)//中断函数，每秒执行20次
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		if(Num_sign == 0)
		{
			Num ++;											
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
			if(Num>=13){Num_sign=1;}
		}
		else
		{
			Num --;											
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			if(Num<=0){Num_sign=0;}
		}
	}
}




