


/**************************Timer0*********************************************/
unsigned char IntTH0 = 0; IntTL0 = 0;
unsigned int  T0Counter = 0 ;
bit PWM = 0 ;
//halfperiod : 中断间隔，单位：uS 
void Timer0Setup(float halfperiod)
{    
	TMOD &= 0xF0;			      /* Timer 0 mode 1 with software gate */
	TMOD |= 0x01;              /* GATE0=0; C/T0#=0; M10=0; M00=1; */


	TH0 = (unsigned char)((65535-(1000000.0*12*halfperiod/11059200.0))/256) ;  /* init values */
	IntTH0 = TH0 ;		         
	TL0 = (unsigned char)(65535-(1000000.0*12*halfperiod/11059200.0)) ;	/*11059200*/
	IntTL0 = TL0 ;
	ET0=1;                     /* enable timer0 interrupt */
	EA=1;
	TR0=1;				            /* enable interrupts */
}

void it_timer0(void) interrupt 1 /* interrupt address is 0x000b */
{
	PWM = !PWM ;
	TH0 = IntTH0;			         /* init values */
	TL0 = IntTL0;	
	TF0 = 0;							/* reset  interrupt flag (already done by hardware)*/
	T0Counter ++ ;
//	P1_0 = !P1_0 ;
}


/*******************************************************************************/



/**************************Timer1*********************************************/

unsigned char IntTH1 ,IntTL1 ;
//halfperiod : 中断间隔，单位：uS 
void Timer1Setup(float halfperiod)
{

	TMOD &= 0x0F;			      /* Timer 1 mode 1 with software gate */
	TMOD |= 0x10;              /* GATE0=0; C/T0#=0; M10=0; M00=1; */
	
	TH1 = (unsigned char)((65535-(1000000.0*12*halfperiod/11059200.0))/256) ;  /* init values */
	IntTH1 = TH1 ;		         
	TL1 = (unsigned char)(65535-(1000000.0*12*halfperiod/11059200.0)) ;	/*11059200*/
	IntTL1 = TL1 ;
	ET1=1;                     /* enable timer1 interrupt */
	EA=1;				            /* enable interrupts */
	TR1=1;			     	      /* timer1 run */
}

void it_timer1(void) interrupt 3 /* interrupt address is 0x001b */
{
	RandomFactor = !RandomFactor ;
	TH1 = IntTH1;			         /* init values */
	TL1 = IntTL1;					 	/* reset  interrupt flag (already done by hardware)*/
	TF1 = 0;
						
}

void delay (unsigned int LastTime)
{
	int StatTime = 0 ;

	StatTime = T0Counter ;

	while(T0Counter - StatTime < LastTime )	 ;

}


