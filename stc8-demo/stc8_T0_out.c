/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC8A/STC8F  系列 定时器0可编程时钟分频输出举例-----------------*/
/* --- Mobile: (86)13922805190 -------------- -------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/* 如果要在文章中应用此代码,请在文章中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//若无特别说明,工作频率一般为11.0592MHz


#include "reg51.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L

//-----------------------------------------------

sfr P_SW2       =   0xba;

#define CKSEL       (*(unsigned char volatile xdata *)0xfe00)
#define CLKDIV      (*(unsigned char volatile xdata *)0xfe01)
#define IRC24MCR    (*(unsigned char volatile xdata *)0xfe02)
#define XOSCCR      (*(unsigned char volatile xdata *)0xfe03)
#define IRC32KCR    (*(unsigned char volatile xdata *)0xfe04)

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sfr AUXR      = 0x8e;               //辅助特殊功能寄存器
sfr INT_CLKO  = 0x8f;               //唤醒和时钟输出功能寄存器

sbit T0CLKO   = P3^5;               //定时器0的时钟输出脚

#define F38_4KHz (65536-FOSC/2/38400)    //1T模式
//#define F38_4KHz (65536-FOSC/2/12/38400) //12T模式

//-----------------------------------------------

void main()
{

    //选择内部IRC
    P_SW2 = 0x80;
    CKSEL = 0x00;
    P_SW2 = 0x00;
/*
    //选择外部晶振
    P_SW2 = 0x80;
    XOSCCR = 0xc0;          //启动外部晶振
    while (!(XOSCCR & 1));  //等待时钟稳定
    CLKDIV = 0x00;           //时钟不分频
    CKSEL = 0x01;
    P_SW2 = 0x00;
*/

/*
    //选择内部32K
    P_SW2 = 0x80;
    IRC32KCR = 0x80;        //启动内部32K IRC
    while (!(IRC32KCR & 1));//等待时钟稳定
    CLKDIV = 0x00;           //时钟不分频
    CKSEL = 0x03;
    P_SW2 = 0x00;
*/

    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

    AUXR |= 0x80;                   //定时器0为1T模式
//  AUXR &= ~0x80;                  //定时器0为12T模式

    TMOD = 0x00;                    //设置定时器为模式0(16位自动重装载)
    
    TMOD &= ~0x04;                  //C/T0=0, 对内部时钟进行时钟输出
//  TMOD |= 0x04;                   //C/T0=1, 对T0引脚的外部时钟进行时钟输出

    TL0 = F38_4KHz;                 //初始化计时值
    TH0 = F38_4KHz >> 8;
    TR0 = 1;
    INT_CLKO = 0x01;                //使能定时器0的时钟输出功能

    while (1);                      //程序终止
}

