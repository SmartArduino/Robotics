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
    
	//ע�����ֻ���ο�֮�ã�Ҫ�ﵽ�������Ч��������Ҫͬѧ��ϸ�ĵ��ԡ�	
	//ע������ģ����ߣ�����֮�˾����Ӵ��ߣ��ջ�ģ�飬��һ��Ҫ�˶Ժ���ͨ�����飬ģ��Ӵ����ջ������ޣ��������ġ�
	/****************************************************************************
	 Ӳ������
	 ������4���߶��壺
	  VCC
      GND
	  TDX
	  RXD
	 ���������뵥Ƭ��J7�ӿ���	 --ע�ⲻҪ�Ӵ��ߣ������ջ�����ģ�飬�����ޣ���������
	****************************************************************************/

#include<AT89x51.H>

	//HL-1С���������߶���
    #define Left_moto_go      {P1_2=0,P1_3=1;}    //��ߵ����ǰ��
	#define Left_moto_back    {P1_2=1,P1_3=0;}    //��ߵ�����ת
	#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //��ߵ��ͣת                     
	#define Right_moto_go     {P1_6=1,P1_7=0;}	//�ұߵ����ǰ��
	#define Right_moto_back   {P1_6=0,P1_7=1;}	//�ұߵ�������
	#define Right_moto_Stop   {P1_6=0,P1_7=0;}	//�ұߵ��ͣת   

	#define left     'C'
    #define right    'D'
	#define up       'A'
    #define down     'B'
	#define stop     'F'

	char code str[] =  "�յ�ָ���ǰ!\n";
	char code str1[] = "�յ�ָ����!\n";
	char code str2[] = "�յ�ָ�����!\n";
	char code str3[] = "�յ�ָ�����!\n";
	char code str4[] = "�յ�ָ�ֹͣ!\n";

	bit  flag_REC=0; 
	bit  flag    =0;  
	

	unsigned char  i=0;
	unsigned char  dat=0;
    unsigned char  buff[5]=0; //���ջ����ֽ�


   
/************************************************************************/	
//��ʱ����	
   void delay(unsigned int k)
{    
     unsigned int x,y;
	 for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}

/************************************************************************/
//�ַ������ͺ���
	  void send_str( )
                   // �����ִ�
    {
	    unsigned char i = 0;
	    while(str[i] != '\0')
	   {
		SBUF = str[i];
		while(!TI);				// �������ݴ���
		TI = 0;					// ������ݴ��ͱ�־
		i++;					// ��һ���ַ�
	   }	
    }
	
		  void send_str1( )
                   // �����ִ�
    {
	    unsigned char i = 0;
	    while(str1[i] != '\0')
	   {
		SBUF = str1[i];
		while(!TI);				// �������ݴ���
		TI = 0;					// ������ݴ��ͱ�־
		i++;					// ��һ���ַ�
	   }	
    }	

			  void send_str2( )
                   // �����ִ�
    {
	    unsigned char i = 0;
	    while(str2[i] != '\0')
	   {
		SBUF = str2[i];
		while(!TI);				// �������ݴ���
		TI = 0;					// ������ݴ��ͱ�־
		i++;					// ��һ���ַ�
	   }	
    }	
	    	
			  void send_str3()
                   // �����ִ�
    {
	    unsigned char i = 0;
	    while(str3[i] != '\0')
	   {
		SBUF = str3[i];
		while(!TI);				// �������ݴ���
		TI = 0;					// ������ݴ��ͱ�־
		i++;					// ��һ���ַ�
	   }	
    }	

	      void send_str4()
                   // �����ִ�
    {
	    unsigned char i = 0;
	    while(str4[i] != '\0')
	   {
		SBUF = str4[i];
		while(!TI);				// �������ݴ���
		TI = 0;					// ������ݴ��ͱ�־
		i++;					// ��һ���ַ�
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
    
	 Left_moto_back ;   //������ǰ��
	 Right_moto_back ;  //�ҵ����ǰ��
}

//��ת
     void  leftrun(void)
{
    
	 Left_moto_back ;   //������ǰ��
	 Right_moto_go ;  //�ҵ����ǰ��
}

//��ת
     void  rightrun(void)
{
    
	 Left_moto_go ;   //������ǰ��
	 Right_moto_back ;  //�ҵ����ǰ��
}
//STOP
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //������ǰ��
	 Right_moto_Stop ;  //�ҵ����ǰ��
}
/************************************************************************/
void sint() interrupt 4	  //�жϽ���3���ֽ�
{ 
 
    if(RI)	                 //�Ƿ�����ж�
    {
       RI=0;
       dat=SBUF;
       if(dat=='O'&&(i==0)) //�������ݵ�һ֡
         {
            buff[i]=dat;
            flag=1;        //��ʼ��������
         }
       else
      if(flag==1)
     {
      i++;
      buff[i]=dat;
      if(i>=2)
      {i=0;flag=0;flag_REC=1 ;}  // ֹͣ����
     }
	 }
}
/*********************************************************************/		 
/*--������--*/
	void main(void)
{
	TMOD=0x20;  
    TH1=0xFd;  		   //11.0592M����9600������
    TL1=0xFd;
    SCON=0x50;  
    PCON=0x00; 
    TR1=1;
	ES=1;   
    EA=1;   
  	
	while(1)							/*����ѭ��*/
	{ 
	  if(flag_REC==1)				    //
	   {
		flag_REC=0;
		if(buff[0]=='O'&&buff[1]=='N')	//��һ���ֽ�ΪO���ڶ����ֽ�ΪN���������ֽ�Ϊ������
		switch(buff[2])
	     {
		      case up :						    // ǰ��
			  send_str( );
			  run();
			  break;
		      case down:						// ����
			   send_str1( );
			  backrun();
			  break;
		      case left:						// ��ת
			   send_str3( );
			  leftrun();
			  break;
		      case right:						// ��ת
			  send_str2( );
			  rightrun();
			  break;
		      case stop:						// ֹͣ
			   send_str4( );
			  stoprun();
			  break;

	     }
      
					 
	 }
	}
}	