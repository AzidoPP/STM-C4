#include "stm32f10x.h"
#include "matrixkey.h"
#include "1602a.h"
#include "delay.h"
#include "OLED.H"
#include "LED.H"
#include "math.h"

unsigned char tem[] = {0x1C,0x14,0x1F,0x09,0x08,0x08,0x09,0x0F};	//摄氏度符号“℃”的字模

char areArraysEqual(char arr1[], char arr2[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return 0; // 如果发现有不相等的元素，返回false
        }
    }
    return 1; // 所有元素都相等，返回true
}

void rightShiftArray(char arr[], int size) {
    if (size > 0) {
        char last = arr[size - 1]; // 保存最后一个元素的值
        for (int i = size - 1; i > 0; i--) {
            arr[i] = arr[i - 1]; // 将每个元素向右移动
        }
        arr[0] = '*'; // 将最后一个元素的值移动到数组的第一个位置
    }
}


void leftShiftArray(char arr[], int size) {
    if (size > 0) {
        int first = arr[0]; // 保存第一个元素的值
        for (int i = 0; i < size - 1; i++) {
            arr[i] = arr[i + 1]; // 将每个元素向左移动
        }
        arr[7] = 0; // 将最后一个元素设置为0
    }
}

char KeyNum;
char ispressed=0;
char password[8]={'*','*','*','*','*','*','*'};
char password1[8]={'*','*','*','*','*','*','*'};

void keyscan(char pass[]){
	KeyNum = MatrixKey_GetValue();
	//LCD_WRITE_StrDATA(pass,0,3 );
	if((KeyNum!=' ')&&(KeyNum!='*')&&(KeyNum!='#')&&(ispressed==0)){
		leftShiftArray(pass,8);
		pass[6]=KeyNum;
		LCD_WRITE_StrDATA(pass,0,3 );
	}
	else if(KeyNum=='*'&&(ispressed==0)){
		rightShiftArray(pass,7);
		LCD_WRITE_StrDATA(pass,0,3 );
	}
	else if(KeyNum=='#'&&(ispressed==0)){
		for (int i = 0; i < 7; i++) { // 注意：size - 1是为了保留字符串的null终止符
			pass[i] = '*';
		}
		LCD_WRITE_StrDATA(pass,0,3 );
	}
	if(KeyNum!=' '){
		ispressed=1;
	}else{
		ispressed=0;
	}
}
int ar=0;
void keyscan1(char pass[]){
	KeyNum = MatrixKey_GetValue();
	//LCD_WRITE_StrDATA(pass,0,3 );
	if(ispressed==0){
		if((KeyNum!=' ')&&(KeyNum!='*')&&(KeyNum!='#')&&(ispressed==0)){
			pass[ar]=KeyNum;
			ar++;
			if(ar>=7)ar=0;
			LCD_WRITE_StrDATA(pass,0,3 );
		}
		else if(KeyNum=='*'&&(ispressed==0)){
			ar--;
			pass[ar]='*';
			LCD_WRITE_StrDATA(pass,0,3 );
		}
		else if(KeyNum=='#'&&(ispressed==0)){
			for (int i = 0; i < 7; i++) { // 注意：size - 1是为了保留字符串的null终止符
				pass[i] = '*';
			}
			ar=0;
			LCD_WRITE_StrDATA(pass,0,3 );
		}
	}
	if(KeyNum!=' '){
		ispressed=1;
	}else{
		ispressed=0;
		Delay_ms(20);
	}
}



int main(){
	LED_Init();
	Delay_ms(50);
	LED1_Turn();
	Delay_ms(50);
	LED1_Turn();
	Delay_ms(50);
	LED1_Turn();
	Delay_ms(50);
	LED1_Turn();
	LCD_INIT();		//LCD1602初始化
	MatrixKey_Init();
	//OLED_Init();
	LCD_WRITE_StrDATA("   *******     ",0,0 );		//向屏幕第一行第五列输出HELLO
	
	//WUserImg( 0, tem );	//写入用户自定义字符到第一个用户自定义字符位
	//LCD_WRITE_CMD( 0x80 );				//指定屏幕第一行第一列输出
	//LCD_WRITE_ByteDATA( 0x00 );			//输出第一个用户自定义字符
	while(1){
		keyscan(password);
		if(password[0]!='*'){for(long i=0; i<100000; i++){keyscan(password);}
		if(password[0]!='*'){
			float ddt=0.1;
			float dt=17;
			float start=1000;
			LCD_WRITE_StrDATA("________________",0,0);
			LED2_ON();
			for(int j=0; j<100; j++){
				LED1_ON();
				for(int i=0; i<2; i++){
					keyscan1(password1);
				}
				LED1_OFF();
				for(int i=0; i<((int)((pow(((double)0.978),((double)j)))*50)-2); i++){
				//for(int i=0; i<((int)((0.125)*50)-2); i++){
				//for(int i=0; i<((int)((-0.013*((float)j)+1)*50)); i++){
					keyscan1(password1);
				}
				if(password1[6]!='*'){
					if(areArraysEqual(password,password1,8)){
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
						Delay_ms(50);
						LED1_Turn();
						Delay_ms(50);
						LED1_Turn();
						return 0;
					}
					else{
						for (int i = 0; i < 7; i++) { // 注意：size - 1是为了保留字符串的null终止符
							password1[i] = '*';
						}
						ar=0;
						LCD_WRITE_StrDATA(password1,0,3 );
					}
				}
			}
			LED1_ON();
			Delay_ms(1300);
			LED1_OFF();
			
			while(1){
				LED2_OFF();
			}
		}
		
		if(KeyNum!=' '){
			ispressed=1;
		}else{
			ispressed=0;
		}
	}}
}
