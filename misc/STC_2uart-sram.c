//注意,如您使用的MCU没有那个功能,就不要操作相应的寄存器
//注意,如您使用的MCU没有那那么大的扩展SRAM,就不要操作超过范围的SRAM

#include<reg51.h>
#include<intrins.h>
sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,SRB8,S2TI,S2RI

sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

sfr  S2BUF    = 0x9B;

sfr  AUXR    = 0x8e;
sfr	 BRT	=	0x9c;
sfr  IAP_CONTR =  0xC7;
sfr  CCON    = 0xD8;
sfr  CMOD    = 0xD9;
sfr  CL      = 0xE9;
sfr  CH      = 0xF9;
sfr  CCAP0L  = 0xEA;
sfr  CCAP0H  = 0xFA;
sfr  CCAPM0  = 0xDA;
sfr  CCAPM1  = 0xDB;
sbit CR      = 0xDE;
sbit MCU_Start_Led  =  P1^7;
sbit S2_Interrupt_Receive_Led  =  P1^4;
//unsigned char self_command_array[4] =  {0x22,0x33,0x44,0x55};
#define Self_Define_ISP_Download_Command  0x22
#define RELOAD_COUNT  0xfb   //18.432MHz,12T,SMOD=0,9600bps

void serial_port_one_initial();
void send_UART_one(unsigned char);
void UART_one_Interrupt_Receive(void);

void serial_port_two_initial();
void send_UART_two(unsigned char);
void UART_two_Interrupt_Receive(void);

void soft_reset_to_ISP_Monitor(void);
void delay(void);
void display_MCU_Start_Led(void);
void send_PWM(void);

void main(void)
{
    unsigned int array_point = 0;
	unsigned char xdata Test_array_one[512]        =
    {
        0x00,     0x01,     0x02,     0x03,     0x04,     0x05,     0x06,     0x07,
        0x08,     0x09,     0x0a,     0x0b,     0x0c,     0x0d,     0x0e,     0x0f,
        0x10,     0x11,     0x12,     0x13,     0x14,     0x15,     0x16,     0x17,
        0x18,     0x19,     0x1a,     0x1b,     0x1c,     0x1d,     0x1e,     0x1f,
        0x20,     0x21,     0x22,     0x23,     0x24,     0x25,     0x26,     0x27, 
        0x28,     0x29,     0x2a,     0x2b,     0x2c,     0x2d,     0x2e,     0x2f,
        0x30,     0x31,     0x32,     0x33,     0x34,     0x35,     0x36,     0x37,
        0x38,     0x39,     0x3a,     0x3b,     0x3c,     0x3d,     0x3e,     0x3f,
        0x40,     0x41,     0x42,     0x43,     0x44,     0x45,     0x46,     0x47,
        0x48,     0x49,     0x4a,     0x4b,     0x4c,     0x4d,     0x4e,     0x4f,
        0x50,     0x51,     0x52,     0x53,     0x54,     0x55,     0x56,     0x57,
        0x58,     0x59,     0x5a,     0x5b,     0x5c,     0x5d,     0x5e,     0x5f,
        0x60,     0x61,     0x62,     0x63,     0x64,     0x65,     0x66,     0x67,
        0x68,     0x69,     0x6a,     0x6b,     0x6c,     0x6d,     0x6e,     0x6f,
        0x70,     0x71,     0x72,     0x73,     0x74,     0x75,     0x76,     0x77,
        0x78,     0x79,     0x7a,     0x7b,     0x7c,     0x7d,     0x7e,     0x7f,
        0x80,     0x81,     0x82,     0x83,     0x84,     0x85,     0x86,     0x87,
        0x88,     0x89,     0x8a,     0x8b,     0x8c,     0x8d,     0x8e,     0x8f,
        0x90,     0x91,     0x92,     0x93,     0x94,     0x95,     0x96,     0x97,
        0x98,     0x99,     0x9a,     0x9b,     0x9c,     0x9d,     0x9e,     0x9f,
        0xa0,     0xa1,     0xa2,     0xa3,     0xa4,     0xa5,     0xa6,     0xa7,
        0xa8,     0xa9,     0xaa,     0xab,     0xac,     0xad,     0xae,     0xaf,
        0xb0,     0xb1,     0xb2,     0xb3,     0xb4,     0xb5,     0xb6,     0xb7,
        0xb8,     0xb9,     0xba,     0xbb,     0xbc,     0xbd,     0xbe,     0xbf,
        0xc0,     0xc1,     0xc2,     0xc3,     0xc4,     0xc5,     0xc6,     0xc7,
        0xc8,     0xc9,     0xca,     0xcb,     0xcc,     0xcd,     0xce,     0xcf,
        0xd0,     0xd1,     0xd2,     0xd3,     0xd4,     0xd5,     0xd6,     0xd7,
        0xd8,     0xd9,     0xda,     0xdb,     0xdc,     0xdd,     0xde,     0xdf,
        0xe0,     0xe1,     0xe2,     0xe3,     0xe4,     0xe5,     0xe6,     0xe7,
        0xe8,     0xe9,     0xea,     0xeb,     0xec,     0xed,     0xee,     0xef,
        0xf0,     0xf1,     0xf2,     0xf3,     0xf4,     0xf5,     0xf6,     0xf7,
        0xf8,     0xf9,     0xfa,     0xfb,     0xfc,     0xfd,     0xfe,     0xff,
        0xff,     0xfe,     0xfd,     0xfc,     0xfb,     0xfa,     0xf9,     0xf8,
        0xf7,     0xf6,     0xf5,     0xf4,     0xf3,     0xf2,     0xf1,     0xf0,
        0xef,     0xee,     0xed,     0xec,     0xeb,     0xea,     0xe9,     0xe8,
        0xe7,     0xe6,     0xe5,     0xe4,     0xe3,     0xe2,     0xe1,     0xe0,
        0xdf,     0xde,     0xdd,     0xdc,     0xdb,     0xda,     0xd9,     0xd8,
        0xd7,     0xd6,     0xd5,     0xd4,     0xd3,     0xd2,     0xd1,     0xd0,
        0xcf,     0xce,     0xcd,     0xcc,     0xcb,     0xca,     0xc9,     0xc8,
        0xc7,     0xc6,     0xc5,     0xc4,     0xc3,     0xc2,     0xc1,     0xc0,
        0xbf,     0xbe,     0xbd,     0xbc,     0xbb,     0xba,     0xb9,     0xb8, 
        0xb7,     0xb6,     0xb5,     0xb4,     0xb3,     0xb2,     0xb1,     0xb0,
        0xaf,     0xae,     0xad,     0xac,     0xab,     0xaa,     0xa9,     0xa8,
        0xa7,     0xa6,     0xa5,     0xa4,     0xa3,     0xa2,     0xa1,     0xa0,
        0x9f,     0x9e,     0x9d,     0x9c,     0x9b,     0x9a,     0x99,     0x98,
        0x97,     0x96,     0x95,     0x94,     0x93,     0x92,     0x91,     0x90,
        0x8f,     0x8e,     0x8d,     0x8c,     0x8b,     0x8a,     0x89,     0x88,
        0x87,     0x86,     0x85,     0x84,     0x83,     0x82,     0x81,     0x80,
        0x7f,     0x7e,     0x7d,     0x7c,     0x7b,     0x7a,     0x79,     0x78,
        0x77,     0x76,     0x75,     0x74,     0x73,     0x72,     0x71,     0x70,
        0x6f,     0x6e,     0x6d,     0x6c,     0x6b,     0x6a,     0x69,     0x68,
        0x67,     0x66,     0x65,     0x64,     0x63,     0x62,     0x61,     0x60,
        0x5f,     0x5e,     0x5d,     0x5c,     0x5b,     0x5a,     0x59,     0x58, 
        0x57,     0x56,     0x55,     0x54,     0x53,     0x52,     0x51,     0x50,
        0x4f,     0x4e,     0x4d,     0x4c,     0x4b,     0x4a,     0x49,     0x48,
        0x47,     0x46,     0x45,     0x44,     0x43,     0x42,     0x41,     0x40,
        0x3f,     0x3e,     0x3d,     0x3c,     0x3b,     0x3a,     0x39,     0x38,
        0x37,     0x36,     0x35,     0x34,     0x33,     0x32,     0x31,     0x30,
        0x2f,     0x2e,     0x2d,     0x2c,     0x2b,     0x2a,     0x29,     0x28,
        0x27,     0x26,     0x25,     0x24,     0x23,     0x22,     0x21,     0x20,
        0x1f,     0x1e,     0x1d,     0x1c,     0x1b,     0x1a,     0x19,     0x18,
        0x17,     0x16,     0x15,     0x14,     0x13,     0x12,     0x11,     0x10,
        0x0f,     0x0e,     0x0d,     0x0c,     0x0b,     0x0a,     0x09,     0x08,
        0x07,     0x06,     0x05,     0x04,     0x03,     0x02,     0x01,     0x00
    };
    unsigned  char i = 0;

    serial_port_one_initial();      //串口1初始化
//    serial_port_two_initial();      //串口2初始化
    display_MCU_Start_Led();    //点亮发光二极管表示单片机开始工作

 //   send_UART_two(0x55);            //串口2发送数据表示单片机串口正常工作
  //  send_UART_two(0xaa);            //串口2发送数据表示单片机串口正常工作
/*
	for(array_point=0; array_point<512; array_point++)
    {
		send_UART_two(Test_array_one[array_point]);
    }
*/
    send_UART_one(0x34);            //串口1发送数据表示单片机串口正常工作
    send_UART_one(0xa7);            //串口1发送数据表示单片机串口正常工作



    for(array_point=0; array_point<512; array_point++)
    {
		send_UART_one(Test_array_one[array_point]);
    }

//    send_PWM();                 //6kHz PWM,  50% duty
    while(1);
}
void serial_port_one_initial()
{
    SCON    =   0x50;   //0101,0000 8位可变波特率，无奇偶校验位
//   TMOD    =   0x21;   //0011,0001 设置顶时器1为8位自动重装计数器
//    TH1     =   RELOAD_COUNT;   //设置定时器1自动重装数
//    TL1     =   RELOAD_COUNT;
//    TR1     =   1;    //开定时器1
	BRT	=	RELOAD_COUNT;
//  BRTR = 1, S1BRS = 1, EXTRAM = 1 ENABLE EXTRAM
	AUXR	=	0x11; // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS	
    ES      =   1;    //允许串口中断
    EA      =   1;    //开总中断
}

void serial_port_two_initial()
{
//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	S2CON    =   0x50;   //0101,0000 8位可变波特率，无奇偶校验位,允许接收
 
	BRT	=	RELOAD_COUNT;
//  BRTR = 1, S1BRS = 1, EXTRAM = 0 ENABLE EXTRAM
	AUXR	=	0x11; // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS	
//    ES      =   1;    //允许串口1中断
//	ES2	=	1
	IE2	=	0x01;	//允许串口2中断,ES2=1
    EA      =   1;    //开总中断
}

void send_UART_one(unsigned char i)
{
    ES     =   0;  //关串口中断
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   i;
    while(TI ==0); //等待发送完成
    TI     =   0;  //清零串口发送完成中断请求标志
    ES     =   1;  //允许串口中断
}

void send_UART_two(unsigned char i)
{
//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	unsigned char temp = 0;

//    ES     =   0;  //关串口1中断
	IE2	=	0x00;	//关串口2中断,es2=0
//    TI     =   0;  //清零串口1发送完成中断请求标志
    S2CON	=	S2CON & 0xFD; //B'11111101,清零串口2发送完成中断请求标志
//    SBUF   =   i;
    S2BUF   =   i;
//    while(TI ==0); //等待发送完成
    do
	{
		temp = S2CON;
		temp = temp & 0x02;
	}while(temp==0);

	
//	TI     =   0;  //清零串口发送完成中断请求标志
    S2CON	=	S2CON & 0xFD; //B'11111101,清零串口2发送完成中断请求标志
//    ES     =   1;  //允许串口1中断
//	ES2	=	1
	IE2	=	0x01;	//允许串口2中断,ES2=1
}

void UART_one_Interrupt_Receive(void) interrupt 4
{
    unsigned char   k   =   0;
    if(RI==1)
    {
        RI  =   0;
        k   =   SBUF;
        if(k==Self_Define_ISP_Download_Command)   //是自定义下载命令
        {
            delay();    //延时1秒就足够了
            delay();    //延时1秒就足够了
            soft_reset_to_ISP_Monitor();    //软复位到系统ISP监控区
        }
        send_UART_one(k+1);
    }
    else
    {
        TI  =  0;
    }
}

void UART_two_Interrupt_Receive(void) interrupt 8
{
//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	unsigned char   k   =   0;
	k = S2CON ;
	k	= k & 0x01;
	//if(S2RI==1)
    if(k==1)
    {
        //RI  =   0;
		S2CON = S2CON & 0xFE; //1111,1110
		S2_Interrupt_Receive_Led = 0;

        k   =   S2BUF;
        if(k==Self_Define_ISP_Download_Command)   //是自定义下载命令
        {

			delay();    //延时1秒就足够了
            delay();    //延时1秒就足够了

            soft_reset_to_ISP_Monitor();    //软复位到系统ISP监控区
        }
        send_UART_two(k+1);
    }
    else
    {
        //TI  =  0;
		S2CON = S2CON & 0xFD; //1111,1101
    }
}
void soft_reset_to_ISP_Monitor(void)
{
    IAP_CONTR   =    0x60;   //0110,0000   软复位到系统ISP监控区
}
void delay(void)
{
    unsigned int j  =   0;
    unsigned int g  =   0;
    for(j=0;j<5;j++)
    {
        for(g=0;g<60000;g++)
        {
            _nop_();
            _nop_();
            _nop_();
            _nop_();
            _nop_();
        }
    }
}
 
void display_MCU_Start_Led(void)   
{
//sbit MCU_Start_Led  =  P1^7;
    unsigned  char i = 0;
    for(i=0;i<1;i++)
    {
        MCU_Start_Led   =   0;  //顶亮MCU开始工作指示灯
        delay();
        MCU_Start_Led   =   1;  //熄灭MCU开始工作指示灯
        delay();
        MCU_Start_Led   =   0;  //顶亮MCU开始工作指示灯
    }
}

void send_PWM(void)
{
    CMOD    =   0x00;   // CIDL - - - - CPS1 CPS0 ECF  Setup PCA Timer
                        // CPS1 CPS0 = 00, Fosc/12 is PCA/PWM clock
                        // 18432000/12/256 = 6000
    CL      =   0x00;
    CH      =   0x00;
    CCAP0L  =   0x80;   //Set the initial value same as CCAP0H
    CCAP0H  =   0x80;   //50% Duty Cycle
    CCAPM0  =   0x42;   //0100,0010 Setup PCA module 0 in 8BIT PWM, P3.7
    CR      =   1;      //启动 PCA/PWM 定时器
}
