/**********************慧净电子智能小车例程************************
*  平台：HJ-C51 HJ-1G HL-1 HJ-3G HJ-C52  + Keil uVision4 + STC89C52 + AT89S52
*  名称：HL-1智能小车超声波测距离程序
*  公司：慧净电子科技有限公司
*  淘宝：http://shop37031453.taobao.com       
*  网站：www.hjmcu.com    www.hlmcu.com
*  编写
*  日期：2008-8-08
*  修改内容：传感器IO口分别接到P3.2 P3.3 口，每个小车电机参考可能不同，请自己调整参考，以更稳定走黑线
*   QQ : 398115088 121350852
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析，有问题直接到慧净论坛交流
*  论坛：
*  硬件设置：要有自己动手能力，才能完成实验
*  使用说明：根据下面IO口自己用杜邦线连接各种模块，可以自己修改各种模块IO口
*  需要购买慧净电子智能小车相关模块才能做实验--请直接到慧净网站购买
*  学习建议：请到慧净电子学习网WWW.HJMCU.COM免费下载52课《从零到项目开发学51单片机与C语言技术视频教程》 BT下载3.63G
   重点提示：本程序只做参考，不提供技术支持，请自己研究吸收。//收货5星好评，后面更有10G资料赠送，网上下载。
   *  同时要接好LCD1602 注意调节W1对比度，用电脑USB供电时调大一点，用电池供电时调小一点对比度。

   HL-1与HL-1智能小车底盘本杜邦线接线方法，请一定照做，否则可能不工作，并烧毁小车。

   J3
   IN1--接到--实验板上的P1.2
   IN2--接到--实验板上的P1.3
   EN1--接到--实验板上的P1.4
   EN2--接到--实验板上的P1.5
   IN3--接到--实验板上的P1.6
   IN4--接到--实验板上的P1.7
   J4 
   IN5--接到--实验板上的P2.1
   IN6--接到--实验板上的P2.0
   J5
   OUT1--接到--实验板上的P3.2
   OUT2--接到--实验板上的P3.3
   OUT3--接到--实验板上的P3.4
   OUT4--接到--实验板上的P3.5

   电源接法----请注意核对，一定要100%正确，否则会烧坏HL-1实验板
   J17
   VCC--接到--实验板上的 TTL V 引脚
   GND--接到--实验板上的 TTL G 引脚
******************************************************************/
#include <AT89x51.H>		//器件配置文件
#include <intrins.h>
#define  RX  P2_0
#define  TX  P2_1
#define LCM_RW  P1_1 //定义LCD引脚
#define LCM_RS  P1_0
#define LCM_E   P2_5

#define LCM_Data  P0
#define Busy    0x80 //用于检测LCM状态字中的Busy标识
sbit DU = P2^6;
sbit WE = P2^7;
void cmg88()//关数码管
{
DU=1;  
P0=0X00;
DU=0;
}

void LCMInit(void);//LCD初始化函数
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);//LCD显示一个字符函数
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);//LCD显示一个字符串函数
void Delay5Ms(void);//延时5毫秒函数
void Delay400Ms(void);//延时400毫秒函数
void Decode(unsigned char ScanCode);
void WriteDataLCM(unsigned char WDLCM);//LCD1602写数据函数
void WriteCommandLCM(unsigned char WCLCM,BuysC);//LCD写命令函数

//unsigned char ReadDataLCM(void);
unsigned char ReadStatusLCM(void);
unsigned char code Range[] ="==Range Finder==";//LCD1602显示格式
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]="Distance:000.0cm";
unsigned char code table1[]="!!! Out of range";

//static unsigned char DisNum = 0; //显示用指针				  
unsigned int  time=0;
unsigned long S=0;
bit  flag =0;
unsigned char disbuff[4]={ 0,0,0,0,};

//写数据
void WriteDataLCM(unsigned char WDLCM) 
{
	ReadStatusLCM(); //检测忙
	LCM_Data = WDLCM;
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 0; //若晶振速度太高可以在这后加小的延时
	LCM_E = 0; //延时
	LCM_E = 1;
}

//写指令
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysC为0时忽略忙检测
{
	if (BuysC) ReadStatusLCM(); //根据需要检测忙
	LCM_Data = WCLCM;
	LCM_RS = 0;
	LCM_RW = 0;	
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;	
}

//读数据
/*unsigned char ReadDataLCM(void)
{
	LCM_RS = 1; 
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	return(LCM_Data);
}*/

//读状态
unsigned char ReadStatusLCM(void)
{
	LCM_Data = 0xFF; 
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	while (LCM_Data & Busy); //检测忙信号
	return(LCM_Data);
}

void LCMInit(void) //LCM初始化
{
	LCM_Data = 0;
	WriteCommandLCM(0x38,0); //三次显示模式设置，不检测忙信号
	Delay5Ms(); 
	WriteCommandLCM(0x38,0);
	Delay5Ms(); 
	WriteCommandLCM(0x38,0);
	Delay5Ms(); 

	WriteCommandLCM(0x38,1); //显示模式设置,开始要求每次检测忙信号
	WriteCommandLCM(0x08,1); //关闭显示
	WriteCommandLCM(0x01,1); //显示清屏
	WriteCommandLCM(0x06,1); // 显示光标移动设置
	WriteCommandLCM(0x0c,1); // 显示开及光标设置
}

//按指定位置显示一个字符
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x1;
	X &= 0xF; //限制X不能大于15，Y不能大于1
	if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
	X |= 0x80; //算出指令码
	WriteCommandLCM(X, 1); //发命令字
	WriteDataLCM(DData); //发数据
}

//按指定位置显示一串字符
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

//5ms延时
void Delay5Ms(void)
{
	unsigned int TempCyc = 5552;
	while(TempCyc--);
}

//400ms延时
void Delay400Ms(void)
{
	unsigned char TempCycA = 5;
	unsigned int TempCycB;
	while(TempCycA--)
		{
			TempCycB=7269;
			while(TempCycB--);
		};
}
/********************************************************/
    void Conut(void)			//超声波距离计算函数
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/10+10;     //算出来是MM
	 if((S>=7000)||flag==1) //超出测量范围
	 {	 
	  flag=0;
      DisplayListChar(0, 1, table1);
	 }
	 else
	 {
      disbuff[0]=S%10;
	  disbuff[1]=S/10%10;
	  disbuff[2]=S/100%10;
	  disbuff[3]=S/1000;
	  DisplayListChar(0, 1, table);
	  DisplayOneChar(9, 1, ASCII[disbuff[3]]);
	  DisplayOneChar(10, 1, ASCII[disbuff[2]]);	
	  DisplayOneChar(11, 1, ASCII[disbuff[1]]);
      DisplayOneChar(12, 1, ASCII[10]);
	  DisplayOneChar(13, 1, ASCII[disbuff[0]]);
	 }
	}
/********************************************************/
     void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
  {
    flag=1;			 //中断溢出标志
	RX=0;
  }
/********************************************************/
     void  StartModule() 		         //启动模块
  {
	  TX=1;			                     //启动一次模块
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  TX=0;
  }
/********************************************************/ 
/*void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}*/
void Timer_Count(void)			   //超声波高电平脉冲宽度计算函数
{
		 TR0=1;			    //开启计数
	     while(RX);			//当RX为1计数并等待
	     TR0=0;				//关闭计数
         Conut();			//计算

}
/*********************************************************/
void main(void)
{
    unsigned int valA;
	Delay400Ms(); //启动等待，等LCM讲入工作状态
	cmg88();//关数码管
	LCMInit(); //LCM初始化
	Delay5Ms(); //延时片刻
	DisplayListChar(0, 0, Range);
	DisplayListChar(0, 1, table);
	//ReadDataLCM();//测试用句无意义
    TMOD=0x01;//设T0为方式1，GATE=1；
    EA=1;		    //开启总中断	
    TH0=0;
    TL0=0;          
    ET0=1;             //允许T0中断
				  		
 	while(1)
	  {
		RX=1;
	    StartModule();		 //启动模块
        for(valA=7510;valA>0;valA--)
	    {
		   
	       if(RX==1)
		   {
           Timer_Count();		 //超声波高电平脉冲宽度计算函数
		   }
    	 }
	  }
}



                