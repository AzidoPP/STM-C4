#include "1602a.h"
#include "stm32f10x_rcc.h"

void delay_us(unsigned int us){
	unsigned int  i;
	
	do{
		i = 10;
		while(i--) __nop();
	} while (--us);

}
/***********************************GPIO初始化********************************************/
void GPIO_INIT(void){		//GPIO初始化
	GPIO_InitTypeDef PB;
	GPIO_InitTypeDef PA;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//禁用jtag，不然写入程序和程序执行都会受影响
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );		//打开GPIOA~B
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
	
	PB.GPIO_Pin = EN|RW|RS;
	PB.GPIO_Mode = GPIO_Mode_Out_PP;
	PB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &PB);
	
	PA.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|
				  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|
				  GPIO_Pin_6|GPIO_Pin_7;
	PA.GPIO_Mode = GPIO_Mode_Out_PP;
	PA.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PA);
	
}
/***********************************GPIO初始化********************************************/

/***********************************LCD初始化********************************************/
void LCD_INIT(void){	//初始化
	GPIO_INIT();	//GPIO的初始化，在LCD_INIT被调用时自动调用
	
	GPIO_Write( GPIOA, 0x0000 );		//初始化GPIOA和GOIOB的值为0x0000，可以不用，个人习惯
	GPIO_Write( GPIOB, 0x0000 );
	
	delay_us(500);
	
	LCD_WRITE_CMD( 0x38 );
	LCD_WRITE_CMD( 0x0c );	//开启光标和闪烁0x0d
	LCD_WRITE_CMD( 0x06 );
	LCD_WRITE_CMD( 0x01 );
}
/***********************************LCD初始化********************************************/

/***********************************写入命令函数********************************************/
void LCD_WRITE_CMD( unsigned char CMD ){		//写入命令函数
	ReadBusy();
	GPIO_ResetBits( GPIOB, RS );
	GPIO_ResetBits( GPIOB, RW );
	GPIO_ResetBits( GPIOB, EN );
	GPIO_Write( GPIOA, CMD );		//
	GPIO_SetBits( GPIOB, EN );
	GPIO_ResetBits( GPIOB, EN );
}
/***********************************写入命令函数********************************************/

/***********************************写入单个Byte函数********************************************/
void LCD_WRITE_ByteDATA( unsigned char ByteData ){	//写入单个Byte函数
	ReadBusy();
	GPIO_SetBits( GPIOB, RS );
	GPIO_ResetBits( GPIOB, RW );
	GPIO_ResetBits( GPIOB, EN );
	GPIO_Write( GPIOA, ByteData );
	GPIO_SetBits( GPIOB, EN );
	GPIO_ResetBits( GPIOB, EN );
}

/***********************************写入单个Byte函数********************************************/

/***********************************写入字符串函数********************************************/

void LCD_WRITE_StrDATA( unsigned char *StrData, unsigned char row, unsigned char col ){//写入字符串
	unsigned char baseAddr = 0x00;			//定义8位地址
	if ( row ){
		baseAddr = 0xc0;
	}else{
		baseAddr = 0x80;																				   
	} 	//row为1用户选择第二行
		//row为0用户选择第一行
	baseAddr += col;

	while ( *StrData != '\0' ){

		LCD_WRITE_CMD( baseAddr );
		LCD_WRITE_ByteDATA( *StrData );
	
		baseAddr++;			   //每次循环地址加一，数据指针加一
		StrData++;
	}
}
/***********************************写入字符串函数********************************************/

/***********************************读忙函数********************************************/
void ReadBusy(void){		//读忙函数，读忙之前记得更改引脚的工作方式！！！因为STM32的IO不是准双向IO
	GPIO_Write( GPIOA, 0x00ff );	
	
	GPIO_InitTypeDef p;
	p.GPIO_Pin = GPIO_Pin_7;		//选定GPIOA的第七Pin
	p.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//第七Pin的工作方式为浮空输入模式，用于检测LCD1602的忙状态
	p.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &p );
	
	GPIO_ResetBits( GPIOB, RS );//RS拉低
	GPIO_SetBits( GPIOB, RW );//RW拉高
	
	GPIO_SetBits( GPIOB, EN );	//使能开
	while( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_7 ) );	//读第七Pin状态，如果一直为1则循环等待
	GPIO_ResetBits( GPIOB, EN );//使能关
		
	p.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|
				  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|
				  GPIO_Pin_6|GPIO_Pin_7;		//使GPIOA的状态还原成推挽模式
	p.GPIO_Mode = GPIO_Mode_Out_PP;
	p.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &p  );
}
/***********************************读忙函数********************************************/

/***********************************写入用户自定义图像********************************************/
void WUserImg(unsigned char pos,unsigned char *ImgInfo){ //写入用户自定义图像
	unsigned char cgramAddr;			//CGRAM的用户自定义字符位
	
	if( pos <= 1 ) cgramAddr = 0x40;		// 
	if( pos > 1 && pos <= 3 ) cgramAddr = 0x50;
	if( pos > 3 && pos <= 5 ) cgramAddr = 0x60;
	if( pos > 5 && pos <= 7 ) cgramAddr = 0x70;

	LCD_WRITE_CMD( (cgramAddr + (pos%2) * 8) );	//指定字模写入的地址，一般从0x40开始，0x78结束
	
	while( *ImgInfo != '\0' ){		//循环写入tem数据,即用户取模的数据
		LCD_WRITE_ByteDATA( *ImgInfo );
		ImgInfo++;
	}
}
/***********************************写入用户自定义图像********************************************/


void lcdupdate(void){
	static int dt=0;
	static char lcdtp=0;
	dt++;
	if(dt>=30000){
	switch(lcdtp){
		case 0:
			LCD_WRITE_StrDATA("*            ",0,0);
			lcdtp++;
			break;
		case 1:
			LCD_WRITE_StrDATA(" *           ",0,0);
			lcdtp++;
			break;
		case 2:
			LCD_WRITE_StrDATA("  *          ",0,0);
			lcdtp++;
			break;
		case 3:
			LCD_WRITE_StrDATA("   *         ",0,0);
			lcdtp++;
			break;
		default:
			LCD_WRITE_StrDATA("    *        ",0,0);
			lcdtp=0;
			break;
	}dt=0;}
}
