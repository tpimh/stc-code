//读内部R/C时钟及ID号C语言参考程序
/* ---  STC International Limited ----------------------------------------------	*/
/* ---  宏晶科技    姚永平    2009/2/7    V1.0 --------------------------------	*/
/* ---  STC12C5201AD  系列单片机,软件实现自定义下载程序-------------------------	*/
/* ---  Mobile: 13922805190 ----------------------------------------------------	*/
/* ---  Fax: 0755-82944243 -----------------------------------------------------    */
/* ---  Tel: 0755-82948409 -----------------------------------------------------    */
/* ---  Web: www.mcu-memory.com ------------------------------------------------	*/
/* ---  本演示程序在STC-ISP Ver 3.0A.PCB的下载编程工具上测试通过 ---------------	*/
/* ---  如果要在程序中使用该程序,请在程序中注明使用了宏晶科技的资料及程序 -------	*/
/* ---  如果要在文章中引用该程序,请在文章中注明使用了宏晶科技的资料及程序--------   */

#include<reg51.h>
#include<intrins.h>
sfr  IAP_CONTR =  0xC7;

sbit MCU_Start_Led  =  P1^7;
//unsigned char self_command_array[4] =  {0x22,0x33,0x44,0x55};
#define Self_Define_ISP_Download_Command  0x22
#define RELOAD_COUNT  0xfb   //18.432MHz,12T,SMOD=0,9600bps

void serial_port_initial();
void send_UART(unsigned char);
void UART_Interrupt_Receive(void);
void soft_reset_to_ISP_Monitor(void);
void delay(void);
void display_MCU_Start_Led(void);

void main(void)
{
    unsigned  char i = 0;
    unsigned  char j = 0;

    unsigned char idata *idata_point;

    serial_port_initial();      //串口初始化
//    display_MCU_Start_Led();    //点亮发光二极管表示单片机开始工作
//    send_UART(0x34);            //串口发送数据表示单片机串口正常工作
//    send_UART(0xa7);            //串口发送数据表示单片机串口正常工作
	
	idata_point = 0xF1;
	for(j=0;j<15;j++)
	{
		i = *idata_point;
		send_UART(i);
		idata_point++;
	}

    while(1);
}

void serial_port_initial()
{
    SCON    =   0x50;   //0101,0000 8位可变波特率，无奇偶校验位
    TMOD    =   0x21;   //0011,0001 设置顶时器1为8位自动重装计数器
    TH1     =   RELOAD_COUNT;   //设置定时器1自动重装数
    TL1     =   RELOAD_COUNT;
    TR1     =   1;    //开定时器1
    ES      =   1;    //允许串口中断
    EA      =   1;    //开总中断
}

void send_UART(unsigned char i)
{
    ES     =   0;  //关串口中断
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   i;
    while(TI ==0); //等待发送完成
    TI     =   0;  //清零串口发送完成中断请求标志
    ES     =   1;  //允许串口中断
}

void UART_Interrupt_Receive(void) interrupt 4
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
        send_UART(k);
    }
    else
    {
        TI  =  0;
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
    unsigned  char i = 0;
    for(i=0;i<3;i++)
    {
        MCU_Start_Led   =   0;  //顶亮MCU开始工作指示灯
        delay();
        MCU_Start_Led   =   1;  //熄灭MCU开始工作指示灯
        delay();
        MCU_Start_Led   =   0;  //顶亮MCU开始工作指示灯
    }
}

