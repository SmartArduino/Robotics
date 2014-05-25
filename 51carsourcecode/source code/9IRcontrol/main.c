/*****************HL-1�۾���������С������*****************
*  ƽ̨��HJ-4WD/HJ-E/HL-1����С�� + Keil U4 + STC89C52
*  ���ƣ�
*  ��˾���۾����ӿƼ����޹�˾
*  �Ա���http://shop37031453.taobao.com       
*  ��վ��www.hjmcu.com	����С��ר����վ��WWW.HLMCU.COM
*  ��д��
*  ���ڣ�2008-8-08
*  ����:���ܳ�QQ:1526248688  ������QQ:398115088
*  ����:11.0592MHZ
*  ˵������ѿ�Դ�����ṩԴ���������������ֱ�ӵ��۾���̳����
*  ��̳��http://hjdz.haotui.com 
*  ��ѽ�Ե��5�Ǻ�������18�����ϰ���׷�����ۺ��ټ���20G���ϣ�������������
*  ��Ƶ�̳̣���С��������ϸ��Ƶ�̳̣�����Ҫ�뵽�۾���վ�������
******************************************************************/
    // ����������PWMС������ȫ�ٶ����С�
    //���»۾�ר�ú���ҡ����ǰ����������С������λ���뿴����ҡ��������˵��ͼ��
	//���»۾�ר�ú���ҡ����ֹͣ������ֹͣС��	
	//ע�����ֻ���ο�֮�ã�Ҫ�ﵽ�������Ч��������Ҫͬѧ��ϸ�ĵ��ԡ�
	//ע�����һ��Ҫ��P3.2IO���ϵĶŰ���ȡ�¡�
		
#include<AT89x51.H>

    #define Left_moto_go      {P1_2=0,P1_3=1;}    //��ߵ����ǰ��
	#define Left_moto_back    {P1_2=1,P1_3=0;}    //��ߵ�����ת
	#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //��ߵ��ͣת                     
	#define Right_moto_go     {P1_6=1,P1_7=0;}	//�ұߵ����ǰ��
	#define Right_moto_back   {P1_6=0,P1_7=1;}	//�ұߵ�������
	#define Right_moto_Stop   {P1_6=0,P1_7=0;}	//�ұߵ��ͣת  

    #define Imax 14000    //�˴�Ϊ����Ϊ11.0592ʱ��ȡֵ, 
    #define Imin 8000    //��������Ƶ�ʵľ���ʱ,
    #define Inum1 1450    //Ҫ�ı���Ӧ��ȡֵ��
    #define Inum2 700 
    #define Inum3 3000 

	unsigned char f=0;
    unsigned char Im[4]={0x00,0x00,0x00,0x00};
    unsigned char show[2]={0,0};
    unsigned long m,Tc;
    unsigned char IrOK;

   
/************************************************************************/	
//��ʱ����	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}
/************************************************************************/	
//�ⲿ�жϽ������
  void intersvr0(void) interrupt 1 using 1
{
    Tc=TH0*256+TL0;                                               //��ȡ�ж�ʱ����ʱ��
    TH0=0; 
    TL0=0;              //��ʱ�ж���������
 if((Tc>Imin)&&(Tc<Imax))
      { 
        m=0;
        f=1;
        return;
      }       //�ҵ���ʼ��

   if(f==1)
      {
        if(Tc>Inum1&&Tc<Inum3) 
    {
   Im[m/8]=Im[m/8]>>1|0x80; m++; 
       }
      if(Tc>Inum2&&Tc<Inum1) 
        {
         Im[m/8]=Im[m/8]>>1; m++; //ȡ��
  }
  if(m==32) 
   {
         m=0;  
         f=0;
         if(Im[2]==~Im[3]) 
      {
           IrOK=1; 
   }
        else IrOK=0;   //ȡ����ɺ��ж϶����Ƿ���ȷ
     }
               //׼������һ��
   }
 
}
/************************************************************************/
//ǰ��ǰ��
     void  run(void)
{
    
	 Left_moto_go ;   //������ǰ��
	 Right_moto_go ;  //�ҵ����ǰ��
}

//ǰ�ٺ���
     void  backrun(void)
{
    
	 Left_moto_back ;   //��������
	 Right_moto_back ;  //�ҵ������
}

//��ת
     void  rightrun(void)
{
    
	 Left_moto_go ;   //����ǰ��
	 Right_moto_Stop ;  //�ҵ��ֹͣ
}

//��ת
     void  Leftrun(void)
{
    
	 Left_moto_Stop ;   //����ֹͣ
	 Right_moto_go ;  //�ҵ��ǰ��
}
//STOP
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //����ֹͣ
	 Right_moto_Stop ;  //�ҵ��ֹͣ
}


/*********************************************************************/		 
/*--������--*/
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


	while(1)							/*����ѭ��*/
	{ 

	    if(IrOK==1) 
     {
           	switch(Im[2])
   		{
     case 0x18:  run(); 			     //ǰ��
             break;
     case 0x52:  backrun();  			 //����	 
             break;
     case 0x08:  leftrun(); 			 //��ת
             break;
	 case 0x5A:  rightrun(); 			 //��ת
             break;
	 case 0x1C:  stoprun();			     //ֹͣ
             break;
	 default:break;

   		}

           IrOK=0;
     }
	 
					 
	 }
}
		