/**********************慧净电子智能小车例程************************
*  平台：HJ-C51 HJ-1G HL-1 HJ-3G HJ-C52  + Keil uVision4 + STC89C52 + AT89S52
*  名称：HL-1智能小车超声波程序
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
//说明：如果没有LCD1602液晶显示屏的同学，请用杜邦把P0.7引脚接到GND，效果一样。
#include <at89x51.h>	
#include <intrins.h>
#include "LCD1602display.h"
//#include "STC12C5A60S2_PWM.h"
#define  TX  P2_1
#define  RX  P2_0
sbit DU = P2^6;
sbit WE = P2^7;
#define Forward_L_DATA  180//当前进不能走直线的时候，请调节这两个参数，理想的时候是100,100，最大256，最小0。0的时候最慢，256的时候最快
#define Forward_R_DATA  180	//例如小车前进的时候有点向左拐，说明右边马达转速过快，那可以取一个值大一点，另外一个值小一点，例如 200  190
                            //直流电机因为制造上的误差，同一个脉宽下也不一定速度一致的，需要自己手动调节

//sbit P4_0=0xc0;	//P4口地址

/*****按照原图接线定义******/
sbit L293D_IN1=P1^2; 
sbit L293D_IN2=P1^3;
sbit L293D_IN3=P1^6;
sbit L293D_IN4=P1^7;
sbit L293D_EN1=P1^4;
sbit L293D_EN2=P1^5;

sbit BUZZ=P2^3;

void cmg88()//关数码管，点阵函数
{
DU=1;  
P0=0X00;
DU=0;
}

void Delay400Ms(void);//延时400毫秒函数

unsigned char code Range[] ="==Range Finder==";//LCD1602显示格式
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]="Distance:000.0cm";
unsigned char code table1[]="!!! Out of range";

unsigned char disbuff[4]={0,0,0,0};//用于分别存放距离的值0.1mm、mm、cm和m的值

void Count(void);//距离计算函数
			  
unsigned int  time=0;//用于存放定时器时间值
unsigned long S=0;//用于存放距离的值
bit  flag =0; //量程溢出标志位
bit  turn_right_flag;

//=========================================================================================================================
void Forward(unsigned char Speed_Right,unsigned char Speed_Left)//	   前进
{

	 L293D_IN1=0; 
	 L293D_IN2=1;
	 L293D_IN3=1;
	 L293D_IN4=0;
//     PWM_Set(255-Speed_Right,255-Speed_Left);
}
void Stop(void)	//刹车
{

     L293D_IN1=0; 
	 L293D_IN2=0;
	 L293D_IN3=0;
	 L293D_IN4=0;
//	 PWM_Set(0,0);
}
void Turn_Right(unsigned char Speed_Right,unsigned char Speed_Left)	 //后
{
    L293D_IN1=1; 
	L293D_IN2=0;
	L293D_IN3=0;
	L293D_IN4=0;
//	PWM_Set(255-Speed_Right,255-Speed_Left);
}
//=========================================================================================================================
/********距离计算程序***************/
    void Conut(void)
	{
	 time=TH1*256+TL1;
	 TH1=0;
	 TL1=0;
	
	 //此时time的时间单位决定于晶振的速度，外接晶振为11.0592MHZ时，
	            //time的值为0.54us*time，单位为微秒
				//那么1us声波能走多远的距离呢？1s=1000ms=1000000us 
				// 340/1000000=0.00034米
				//0.00034米/1000=0.34毫米  也就是1us能走0.34毫米
				//但是，我们现在计算的是从超声波发射到反射接收的双路程，
				//所以我们将计算的结果除以2才是实际的路程

	S=time*2;//先算出一共的时间是多少微秒。
   	S=S*0.17;//此时计算到的结果为毫米，并且是精确到毫米的后两位了，有两个小数点 
	 if(S<=300)	 //
	 {	
	    if(turn_right_flag!=1)
		{
		    Stop();
		    Delay1ms(5);//发现小车自动复位的时候，可以稍微延长一点这个延时，减少电机反向电压对电路板的冲击。
		}
		turn_right_flag=1;
		P2_3=0;
		Delay1ms(50);
		P2_3=1;
	    Turn_Right(120,120); 				//小于设定距离时电机后退转弯
	 }
	 else
	 {
	    turn_right_flag=0;
	    Forward(Forward_R_DATA,Forward_L_DATA);			  //前进（大于20-30CM前进）
	 }
	 //=======================================
	 if((S>=5000)||flag==1) //超出测量范围
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
     void zd0() interrupt 3 		 //T0中断用来计数器溢出,超过测距范围
  {
    flag=1;			 //中断溢出标志
	RX=0;
  }

/********超声波高电平脉冲宽度计算程序***************/
void Timer_Count(void)
{
		 TR1=1;			    //开启计数
	     while(RX);			//当RX为1计数并等待
	     TR1=0;				//关闭计数
         Conut();			//计算

}
/********************************************************/
   void  StartModule() 		         //启动模块
  {
	  TX=1;			                     //启动一次模块
      Delay10us(2);
	  TX=0;
  }
/********************************************************/ 

/*************主程序********************/
void main(void)
{
    unsigned char i;
	unsigned int a;
	cmg88();//关数码管
	Delay1ms(400); //启动等待，等LCM讲入工作状态
	LCMInit(); //LCM初始化
	Delay1ms(5);//延时片刻

	DisplayListChar(0, 0, Range);
	DisplayListChar(0, 1, table);
    TMOD=TMOD|0x10;//设T0为方式1，GATE=1；
    EA=1;					   //开启总中断
    TH1=0;
    TL1=0;          
    ET1=1;             //允许T0中断

	//===============================
	//PWM_ini();
	//===============================
	turn_right_flag=0;
	//=================================
B:		for(i=0;i<50;i++) //判断K3是否按下
		{
		   Delay1ms(1);	//1ms内判断50次，如果其中有一次被判断到K3没按下，便重新检测
		   if(P3_6!=0 )//当K3按下时，启动小车
		   goto B; //跳转到标号B，重新检测
		}
	//蜂鸣器响一声
	BUZZ=0;	//50次检测K3确认是按下之后，蜂鸣器发出“滴”声响，然后启动小车。
	Delay1ms(50);
	BUZZ=1;//响50ms后关闭蜂鸣器
	//=======================================================================================================================			
 	while(1)
	  {
		RX=1;
	    StartModule();				 //启动模块
        for(a=951;a>0;a--)
	    {
		   
	       if(RX==1)
		   {
           Timer_Count();		 //超声波高电平脉冲宽度计算函数
		   }
    	 }
   	}
} 



                