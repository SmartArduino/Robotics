#ifndef __LCD1602display_H_
#define __LCD1602display_H_
#include <intrins.h>//引入一个内部函数文件，里面包含了一些比较常用的函数。此头文件在KEIL/INC目录下

//====================================================================//

//LCD的引脚接线定义，使用并行数据传输模式

//====================================================================//
#define LCM_RW  P1_1 //定义LCD引脚
#define LCM_RS  P1_0
#define LCM_E   P2_5
#define LCM_Data  P0



//====================================================================//
//用于检测LCM状态字中的Busy标识
#define Busy    0x80 


//====================================================================//
// 实现功能： 向LCD写入数据
// 参    数：字符型
// 返 回 值：无
//====================================================================//
void WriteDataLCM(unsigned char WDLCM);


//====================================================================//
// 实现功能： 向LCD写入命令
// 参    数：字符型
// 返 回 值：无
//====================================================================//
void WriteCommandLCM(unsigned char WCLCM,BuysC); //BuysC为0时忽略忙检测


//====================================================================//
// 实现功能： 向LCD读出数据
// 参    数：字符型
// 返 回 值：无
//====================================================================//
//unsigned char ReadDataLCM(void);


//====================================================================//
// 实现功能： 读LCD状态
// 参    数：字符型
// 返 回 值：unsigned char
//====================================================================//
unsigned char ReadStatusLCM(void);


//====================================================================//
// 实现功能LCD初始化函数
// 参    数：无
// 返 回 值：无
//           在main()函数开始时调用，并且调用后紧跟着50ms的延时
//====================================================================//
void LCMInit(void);

//====================================================================//
// 实现功能： 在LCD屏幕上显示1个字符
// 参    数：字符型
// 返 回 值：无
//           X:字符从横向哪一个位置开始显示（取值：0-15）
//           Y:字符显示在哪一行（取值：0-1）
//           DData：要显示的字符（ASCII码）
//====================================================================//
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);

//====================================================================//
// 实现功能： 在LCD屏幕上显示字符串
// 参    数：字符型
// 返 回 值：无
//           X:字符从横向哪一个位置开始显示（取值：0-15）
//           Y:字符显示在哪一行（取值：0-1）
//           DData：要显示的字符串（ASCII码，逐个显示）
//====================================================================//
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);

//====================================================================//
// 实现功能： 5毫秒延时子程序
// 参    数：无
// 返 回 值：无
//====================================================================//
void Delay1ms(unsigned int i); 
void Delay10us(unsigned char i) ;



//****************写数据************************//
void WriteDataLCM(unsigned char WDLCM) 
{
	ReadStatusLCM(); //检测忙
	LCM_Data = WDLCM;
	LCM_RS = 1;
	Delay10us(1); 
	LCM_RW = 0;
	Delay10us(1); 
	LCM_E = 0; //若晶振速度太高可以在这后加小的延时
	Delay10us(1); 
	LCM_E = 0; //延时
	Delay10us(1); 
	LCM_E = 1;
	Delay10us(1); 
}

//****************写指令*************************//
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysC为0时忽略忙检测
{
	if (BuysC) ReadStatusLCM(); //根据需要检测忙
	LCM_Data = WCLCM;
	LCM_RS = 0;
	Delay10us(1); 
	LCM_RW = 0;	
	Delay10us(1); 
	LCM_E = 0;
	Delay10us(1); 
	LCM_E = 0;
	Delay10us(1); 
	LCM_E = 1;
	Delay10us(1); 
}
//****************读数据*************************//
/*unsigned char ReadDataLCM(void)
{
	LCM_RS = 1; 
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	return(LCM_Data);
}*/

//*******************读状态*************************//
unsigned char ReadStatusLCM(void)
{
	LCM_Data = 0xFF; 
	LCM_RS = 0;
	Delay10us(1); 
	LCM_RW = 1;
	Delay10us(1); 
	do{
	LCM_E = 0;
	Delay10us(1); 
	LCM_E = 0;
	Delay10us(1); 
	LCM_E = 1;
	Delay10us(1); 
	}
	while (LCM_Data & Busy); //检测忙信号
	return(LCM_Data);
}

//*******************LCM初始化**********************//
void LCMInit(void) 
{
	LCM_Data = 0;
	WriteCommandLCM(0x38,0); //三次显示模式设置，不检测忙信号
	Delay1ms(5); 
	WriteCommandLCM(0x38,0);
	Delay1ms(5); 
	WriteCommandLCM(0x38,0);
	Delay1ms(5); 

	WriteCommandLCM(0x38,1); //显示模式设置,开始要求每次检测忙信号
	WriteCommandLCM(0x08,1); //关闭显示
	WriteCommandLCM(0x01,1); //显示清屏
	WriteCommandLCM(0x06,1); // 显示光标移动设置
	WriteCommandLCM(0x0c,1); // 显示开及光标设置
}

//*********************按指定位置显示一个字符***********************//
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x1;
	X &= 0xF; //限制X不能大于15，Y不能大于1
	if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
	X |= 0x80; //算出指令码
	WriteCommandLCM(X, 1); //发命令字
	WriteDataLCM(DData); //发数据
}

//**********************按指定位置显示一串字符*************************//
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
	unsigned char ListLength;

  ListLength = 0;
	Y &= 0x1;
	X &= 0xF; //限制X不能大于15，Y不能大于1
	while (DData[ListLength]>0x19) //若到达字串尾则退出
		{
			if (X <= 0xF) //X坐标应小于0xF
				{
					DisplayOneChar(X, Y, DData[ListLength]); //显示单个字符
					ListLength++;
					X++;
				}
		}
}
//********************************************************** 
//函数名称:Delay1ms(unsigned int i) 
//函数功能:延时i*1ms的子程序(对应于22.1184Mhz晶振)   
//形式参数:unsigned int i 
//行参说明:无 
//返回参数:无 
//使用说明:i为要延时的时间长度，单位是MS，最大可以延时65536 ms 
//********************************************************** 
void Delay1ms(unsigned int i) 
{ 
unsigned char j,k; 
do{ 
  j = 10; 
  do{ 
   k = 50; 
   do{ 
    _nop_(); 
   }while(--k);     
  }while(--j); 
}while(--i); 

} 
//********************************************************** 
//函数名称:Delay10us(unsigned char i) 
//函数功能:延时i*10us的子程序(对应于22.1184Mhz晶振)   
//形式参数:无 
//行参说明:无 
//返回参数:无 
//使用说明:i为要延时的时间长度，单位是US，最大可以延时250 ms 
//********************************************************** 
void Delay10us(unsigned char i) 
{ 
   unsigned char j; 
do{ 
  j = 10; 
  do{ 
   _nop_(); 
   }while(--j); 
}while(--i); 
}  

#endif //__LCD_H_

//======================================================================//   
//本程序是LCD1602显示程序，包括在LCD面板任何一个位置现在一个字符，和在
//LCD1602任何一个位置起显示一个字符串的功能。
//MCU类型：C51
//
//操作步骤：                                             
//1）首先将LCD1602display.c和LCD1602display.h两个文件拷贝到工程的根目录下。
//2）在main（）函数开始处调用LCD1602初始化函数LCMInit();
//3）调用Delay5Ms（）,延时50毫秒（本程序中有，不用另外写）。
//4）延时完毕之后便可以调用DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)；
//   在LCD1602任何1个位置显示1个字符（）；
//           X:字符从横向哪一个位置开始显示（取值：0-15）
//           Y:字符显示在哪一行（取值：0-1）
//           DData：要显示的字符（ASCII码）
//   或者调用DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);在任何
//   一行，任何位置上显示一个字符串。
//           X:字符从横向哪一个位置开始显示（取值：0-15）
//           Y:字符显示在哪一行（取值：0-1）
//           DData：要显示的字符串（ASCII码，逐个显示）
//====================================================================//

//======================注意事项==========================================================================//
// 更换不同厂家的1602LCD或者晶振速度太快时，有可能显示不了，或者显示不稳定。可以尝试在控制信号上增加适当延时
// 因为速度太快，LCD没办法反应过来，例如：
//	LCM_E = 0; 
//	Delay10us(2); 
//	LCM_E = 0; //延时
//	Delay10us(2); 
//	LCM_E = 1;
//	Delay10us(2); 
//======================注意事项==============================================================//
