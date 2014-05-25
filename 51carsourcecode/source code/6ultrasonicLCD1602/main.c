/**********************�۾���������С������************************
*  ƽ̨��HJ-C51 HJ-1G HL-1 HJ-3G HJ-C52  + Keil uVision4 + STC89C52 + AT89S52
*  ���ƣ�HL-1����С����������������
*  ��˾���۾����ӿƼ����޹�˾
*  �Ա���http://shop37031453.taobao.com       
*  ��վ��www.hjmcu.com    www.hlmcu.com
*  ��д�H
*  ���ڣ�2008-8-08
*  �޸����ݣ�������IO�ڷֱ�ӵ�P3.2 P3.3 �ڣ�ÿ��С������ο����ܲ�ͬ�����Լ������ο����Ը��ȶ��ߺ���
*   QQ : 398115088 121350852
*  ����:11.0592MHZ
*  ˵������ѿ�Դ�����ṩԴ���������������ֱ�ӵ��۾���̳����
*  ��̳��
*  Ӳ�����ã�Ҫ���Լ������������������ʵ��
*  ʹ��˵������������IO���Լ��öŰ������Ӹ���ģ�飬�����Լ��޸ĸ���ģ��IO��
*  ��Ҫ����۾���������С�����ģ�������ʵ��--��ֱ�ӵ��۾���վ����
*  ѧϰ���飺�뵽�۾�����ѧϰ��WWW.HJMCU.COM�������52�Ρ����㵽��Ŀ����ѧ51��Ƭ����C���Լ�����Ƶ�̡̳� BT����3.63G
   �ص���ʾ��������ֻ���ο������ṩ����֧�֣����Լ��о����ա�//�ջ�5�Ǻ������������10G�������ͣ��������ء�
   *  ͬʱҪ�Ӻ�LCD1602 ע�����W1�Աȶȣ��õ���USB����ʱ����һ�㣬�õ�ع���ʱ��Сһ��Աȶȡ�

   HL-1��HL-1����С�����̱��Ű��߽��߷�������һ��������������ܲ����������ջ�С����

   J3
   IN1--�ӵ�--ʵ����ϵ�P1.2
   IN2--�ӵ�--ʵ����ϵ�P1.3
   EN1--�ӵ�--ʵ����ϵ�P1.4
   EN2--�ӵ�--ʵ����ϵ�P1.5
   IN3--�ӵ�--ʵ����ϵ�P1.6
   IN4--�ӵ�--ʵ����ϵ�P1.7
   J4 
   IN5--�ӵ�--ʵ����ϵ�P2.1
   IN6--�ӵ�--ʵ����ϵ�P2.0
   J5
   OUT1--�ӵ�--ʵ����ϵ�P3.2
   OUT2--�ӵ�--ʵ����ϵ�P3.3
   OUT3--�ӵ�--ʵ����ϵ�P3.4
   OUT4--�ӵ�--ʵ����ϵ�P3.5

   ��Դ�ӷ�----��ע��˶ԣ�һ��Ҫ100%��ȷ��������ջ�HL-1ʵ���
   J17
   VCC--�ӵ�--ʵ����ϵ� TTL V ����
   GND--�ӵ�--ʵ����ϵ� TTL G ����
******************************************************************/
#include <AT89x51.H>		//���������ļ�
#include <intrins.h>
#define  RX  P2_0
#define  TX  P2_1
#define LCM_RW  P1_1 //����LCD����
#define LCM_RS  P1_0
#define LCM_E   P2_5

#define LCM_Data  P0
#define Busy    0x80 //���ڼ��LCM״̬���е�Busy��ʶ
sbit DU = P2^6;
sbit WE = P2^7;
void cmg88()//�������
{
DU=1;  
P0=0X00;
DU=0;
}

void LCMInit(void);//LCD��ʼ������
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);//LCD��ʾһ���ַ�����
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);//LCD��ʾһ���ַ�������
void Delay5Ms(void);//��ʱ5���뺯��
void Delay400Ms(void);//��ʱ400���뺯��
void Decode(unsigned char ScanCode);
void WriteDataLCM(unsigned char WDLCM);//LCD1602д���ݺ���
void WriteCommandLCM(unsigned char WCLCM,BuysC);//LCDд�����

//unsigned char ReadDataLCM(void);
unsigned char ReadStatusLCM(void);
unsigned char code Range[] ="==Range Finder==";//LCD1602��ʾ��ʽ
unsigned char code ASCII[13] = "0123456789.-M";
unsigned char code table[]="Distance:000.0cm";
unsigned char code table1[]="!!! Out of range";

//static unsigned char DisNum = 0; //��ʾ��ָ��				  
unsigned int  time=0;
unsigned long S=0;
bit  flag =0;
unsigned char disbuff[4]={ 0,0,0,0,};

//д����
void WriteDataLCM(unsigned char WDLCM) 
{
	ReadStatusLCM(); //���æ
	LCM_Data = WDLCM;
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_E = 0; //�������ٶ�̫�߿���������С����ʱ
	LCM_E = 0; //��ʱ
	LCM_E = 1;
}

//дָ��
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysCΪ0ʱ����æ���
{
	if (BuysC) ReadStatusLCM(); //������Ҫ���æ
	LCM_Data = WCLCM;
	LCM_RS = 0;
	LCM_RW = 0;	
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;	
}

//������
/*unsigned char ReadDataLCM(void)
{
	LCM_RS = 1; 
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	return(LCM_Data);
}*/

//��״̬
unsigned char ReadStatusLCM(void)
{
	LCM_Data = 0xFF; 
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 0;
	LCM_E = 0;
	LCM_E = 1;
	while (LCM_Data & Busy); //���æ�ź�
	return(LCM_Data);
}

void LCMInit(void) //LCM��ʼ��
{
	LCM_Data = 0;
	WriteCommandLCM(0x38,0); //������ʾģʽ���ã������æ�ź�
	Delay5Ms(); 
	WriteCommandLCM(0x38,0);
	Delay5Ms(); 
	WriteCommandLCM(0x38,0);
	Delay5Ms(); 

	WriteCommandLCM(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
	WriteCommandLCM(0x08,1); //�ر���ʾ
	WriteCommandLCM(0x01,1); //��ʾ����
	WriteCommandLCM(0x06,1); // ��ʾ����ƶ�����
	WriteCommandLCM(0x0c,1); // ��ʾ�����������
}

//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x1;
	X &= 0xF; //����X���ܴ���15��Y���ܴ���1
	if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
	X |= 0x80; //���ָ����
	WriteCommandLCM(X, 1); //��������
	WriteDataLCM(DData); //������
}

//��ָ��λ����ʾһ���ַ�
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
	unsigned char ListLength;

  ListLength = 0;
	Y &= 0x1;
	X &= 0xF; //����X���ܴ���15��Y���ܴ���1
	while (DData[ListLength]>0x19) //�������ִ�β���˳�
		{
			if (X <= 0xF) //X����ӦС��0xF
				{
					DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
					ListLength++;
					X++;
				}
		}
}

//5ms��ʱ
void Delay5Ms(void)
{
	unsigned int TempCyc = 5552;
	while(TempCyc--);
}

//400ms��ʱ
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
    void Conut(void)			//������������㺯��
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/10+10;     //�������MM
	 if((S>=7000)||flag==1) //����������Χ
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
     void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
  {
    flag=1;			 //�ж������־
	RX=0;
  }
/********************************************************/
     void  StartModule() 		         //����ģ��
  {
	  TX=1;			                     //����һ��ģ��
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
void Timer_Count(void)			   //�������ߵ�ƽ�����ȼ��㺯��
{
		 TR0=1;			    //��������
	     while(RX);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
         Conut();			//����

}
/*********************************************************/
void main(void)
{
    unsigned int valA;
	Delay400Ms(); //�����ȴ�����LCM���빤��״̬
	cmg88();//�������
	LCMInit(); //LCM��ʼ��
	Delay5Ms(); //��ʱƬ��
	DisplayListChar(0, 0, Range);
	DisplayListChar(0, 1, table);
	//ReadDataLCM();//�����þ�������
    TMOD=0x01;//��T0Ϊ��ʽ1��GATE=1��
    EA=1;		    //�������ж�	
    TH0=0;
    TL0=0;          
    ET0=1;             //����T0�ж�
				  		
 	while(1)
	  {
		RX=1;
	    StartModule();		 //����ģ��
        for(valA=7510;valA>0;valA--)
	    {
		   
	       if(RX==1)
		   {
           Timer_Count();		 //�������ߵ�ƽ�����ȼ��㺯��
		   }
    	 }
	  }
}



                