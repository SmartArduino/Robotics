/*****************HL-1慧净蓝牙智能小车例程*****************
*  平台：HJ-4WD/HJ-E/HL-1智能小车 + Keil U4 + STC89C52
*  名称：
*  公司：慧净电子科技有限公司
*  淘宝：http://shop37031453.taobao.com       
*  网站：www.hjmcu.com	智能小车专用网站：WWW.HLMCU.COM
*  编写：
*  日期：2008-8-08
*  交流:智能车QQ:1526248688  开发板QQ:398115088
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析，有问题直接到慧净论坛交流
*  论坛：http://hjdz.haotui.com 
*  免费结缘：5星好评赠送18个资料包，追加评价后再加送20G资料，网上网盘下载
*  视频教程：本小车配套详细视频教程，有需要请到慧净网站免费下载
******************************************************************/
    // 本程序不输入PWM小车就能全速度运行。
    //按下慧净专用红外摇控器前进按键启动小车，（位置请看红外摇控器编码说明图）
	//按下慧净专用红外摇控器停止健可以停止小车	
	//注意程序只做参考之用，要达到最理想的效果，还需要同学们细心调试。
	//注意事项，一定要把P3.2IO口上的杜邦线取下。
		
#include<AT89x51.H>

    #define Left_moto_go      {P1_2=0,P1_3=1;}    //左边电机向前走
	#define Left_moto_back    {P1_2=1,P1_3=0;}    //左边电机向后转
	#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //左边电机停转                     
	#define Right_moto_go     {P1_6=1,P1_7=0;}	//右边电机向前走
	#define Right_moto_back   {P1_6=0,P1_7=1;}	//右边电机向后走
	#define Right_moto_Stop   {P1_6=0,P1_7=0;}	//右边电机停转  

    #define Imax 14000    //此处为晶振为11.0592时的取值, 
    #define Imin 8000    //如用其它频率的晶振时,
    #define Inum1 1450    //要改变相应的取值。
    #define Inum2 700 
    #define Inum3 3000 

	unsigned char f=0;
    unsigned char Im[4]={0x00,0x00,0x00,0x00};
    unsigned char show[2]={0,0};
    unsigned long m,Tc;
    unsigned char IrOK;

   
/************************************************************************/	
//延时函数	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
/************************************************************************/	
//外部中断解码程序
  void intersvr0(void) interrupt 1 using 1
{
    Tc=TH0*256+TL0;                                               //提取中断时间间隔时长
    TH0=0; 
    TL0=0;              //定时中断重新置零
 if((Tc>Imin)&&(Tc<Imax))
      { 
        m=0;
        f=1;
        return;
      }       //找到启始码

   if(f==1)
      {
        if(Tc>Inum1&&Tc<Inum3) 
    {
   Im[m/8]=Im[m/8]>>1|0x80; m++; 
       }
      if(Tc>Inum2&&Tc<Inum1) 
        {
         Im[m/8]=Im[m/8]>>1; m++; //取码
  }
  if(m==32) 
   {
         m=0;  
         f=0;
         if(Im[2]==~Im[3]) 
      {
           IrOK=1; 
   }
        else IrOK=0;   //取码完成后判断读码是否正确
     }
               //准备读下一码
   }
 
}
/************************************************************************/
//前速前进
     void  run(void)
{
    
	 Left_moto_go ;   //左电机往前走
	 Right_moto_go ;  //右电机往前走
}

//前速后退
     void  backrun(void)
{
    
	 Left_moto_back ;   //左电机后退
	 Right_moto_back ;  //右电机后退
}

//右转
     void  rightrun(void)
{
    
	 Left_moto_go ;   //左电机前进
	 Right_moto_Stop ;  //右电机停止
}

//左转
     void  Leftrun(void)
{
    
	 Left_moto_Stop ;   //左电机停止
	 Right_moto_go ;  //右电机前进
}
//STOP
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //左电机停止
	 Right_moto_Stop ;  //右电机停止
}


/*********************************************************************/		 
/*--主函数--*/
	void main(void)
{
	 
    m=0;
    f=0;

    IT0=1;
    EX0=1;
 
    TMOD=0x11;  
    TH0=0;
	TL0=0;
    TR0=1;
	EA=1;



	
	delay(100);	   


	while(1)							/*无限循环*/
	{ 

	    if(IrOK==1) 
     {
           	switch(Im[2])
   		{
     case 0x18:  run(); 			     //前进
             break;
     case 0x52:  backrun();  			 //后退	 
             break;
     case 0x08:  leftrun(); 			 //左转
             break;
	 case 0x5A:  rightrun(); 			 //右转
             break;
	 case 0x1C:  stoprun();			     //停止
             break;
	 default:break;

   		}

           IrOK=0;
     }
	 
					 
	 }
}
		