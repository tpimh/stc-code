/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 定时器2用作串口3的波特率发生器举例------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/* 如果要在文章中应用此代码,请在文章中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//假定测试芯片的工作频率为18.432MHz

#include "reg51.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 18432000L          //系统频率
#define BAUD 115200             //串口波特率

#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT EVEN_PARITY   //定义校验位

sfr AUXR  = 0x8e;               //辅助寄存器
sfr S3CON = 0xac;               //UART3 控制寄存器
sfr S3BUF = 0xad;               //UART3 数据寄存器
sfr T2H   = 0xd6;               //定时器2高8位
sfr T2L   = 0xd7;               //定时器2低8位
sfr IE2   = 0xaf;               //中断控制寄存器2

#define S3RI  0x01              //S3CON.0
#define S3TI  0x02              //S3CON.1
#define S3RB8 0x04              //S3CON.2
#define S3TB8 0x08              //S3CON.3

sfr P_SW2   = 0xBA;             //外设功能切换寄存器2
#define S3_S0 0x02              //P_SW2.1

bit busy;

void SendData(BYTE dat);
void SendString(char *s);

void main()
{
    P_SW2 &= ~S3_S0;            //S3_S0=0 (P0.0/RxD3, P0.1/TxD3)
//  P_SW2 |= S3_S0;             //S3_S0=1 (P5.0/RxD3_2, P5.1/TxD3_2)

#if (PARITYBIT == NONE_PARITY)
    S3CON = 0x10;               //8位可变波特率
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S3CON = 0x9a;               //9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    S3CON = 0x92;               //9位可变波特率,校验位初始为0
#endif

    T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    IE2 = 0x08;                 //使能串口3中断
    EA = 1;

    SendString("STC15F2K60S2\r\nUart2 Test !\r\n");
    while(1);
}

/*----------------------------
UART3 中断服务程序
-----------------------------*/
void Uart3() interrupt 17 using 1
{
    if (S3CON & S3RI)
    {
        S3CON &= ~S3RI;         //清除S3RI位
        P0 = S3BUF;             //P0显示串口数据
        P2 = (S3CON & S3RB8);   //P2.2显示校验位
    }
    if (S3CON & S3TI)
    {
        S3CON &= ~S3TI;         //清除S3TI位
        busy = 0;               //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
#if (PARITYBIT == ODD_PARITY)
        S3CON &= ~S3TB8;        //设置校验位为0
#elif (PARITYBIT == EVEN_PARITY)
        S3CON |= S3TB8;         //设置校验位为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S3CON |= S3TB8;         //设置校验位为1
#elif (PARITYBIT == EVEN_PARITY)
        S3CON &= ~S3TB8;        //设置校验位为0
#endif
    }
    busy = 1;
    S3BUF = ACC;                //写数据到UART3数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

