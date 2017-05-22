/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 PCA扩展为外部中断唤醒掉电模式举例-------------*/
/* --- Mobile: (86)13922805190 -------------- -------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/* 如果要在文章中应用此代码,请在文章中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//假定测试芯片的工作频率为18.432MHz

//本测试程序以PCA模块0为例进行说明,PCA的模块1和模块2与模块0的实用方法相同

#include "reg51.h"
#include "intrins.h"

#define FOSC    18432000L

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

sfr P_SW1   = 0xA2;             //外设功能切换寄存器1

#define CCP_S0 0x10             //P_SW1.4
#define CCP_S1 0x20             //P_SW1.5

sfr CCON        =   0xD8;           //PCA控制寄存器
sbit CCF0       =   CCON^0;         //PCA模块0中断标志
sbit CCF1       =   CCON^1;         //PCA模块1中断标志
sbit CR         =   CCON^6;         //PCA定时器运行控制位
sbit CF         =   CCON^7;         //PCA定时器溢出标志
sfr CMOD        =   0xD9;           //PCA模式寄存器
sfr CL          =   0xE9;           //PCA定时器低字节
sfr CH          =   0xF9;           //PCA定时器高字节
sfr CCAPM0      =   0xDA;           //PCA模块0模式寄存器
sfr CCAP0L      =   0xEA;           //PCA模块0捕获寄存器 LOW
sfr CCAP0H      =   0xFA;           //PCA模块0捕获寄存器 HIGH
sfr CCAPM1      =   0xDB;           //PCA模块1模式寄存器
sfr CCAP1L      =   0xEB;           //PCA模块1捕获寄存器 LOW
sfr CCAP1H      =   0xFB;           //PCA模块1捕获寄存器 HIGH
sfr CCAPM2      =   0xDC;           //PCA模块2模式寄存器
sfr CCAP2L      =   0xEC;           //PCA模块2捕获寄存器 LOW
sfr CCAP2H      =   0xFC;           //PCA模块2捕获寄存器 HIGH
sfr PCA_PWM0    =   0xf2;           //PCA模块0的PWM寄存器
sfr PCA_PWM1    =   0xf3;           //PCA模块1的PWM寄存器
sfr PCA_PWM2    =   0xf4;           //PCA模块2的PWM寄存器

sbit P10 = P1^0;

void main()
{
    ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;    
//  
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
    //  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
    //  P_SW1 = ACC;  

    CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CCAP0L = 0;
    CCAP0H = 0;
    CMOD = 0x08;                    //设置PCA时钟源为系统时钟
    CCAPM0 = 0x21;                  //PCA模块0为16位捕获模式(上升沿捕获,可测从高电平开始的整个周期),且产生捕获中断,此时CCP0上的上升沿中断可唤醒掉电模式
//  CCAPM0 = 0x11;                  //PCA模块0为16位捕获模式(下降沿捕获,可测从低电平开始的整个周期),且产生捕获中断,此时CCP0上的下降沿中断可唤醒掉电模式
//  CCAPM0 = 0x31;                  //PCA模块0为16位捕获模式(上升沿/下降沿捕获,可测高电平或者低电平宽度),且产生捕获中断,此时CCP0上的上升沿和下降沿中断可唤醒掉电模式

    CR = 1;                         //PCA定时器开始工作
    EA = 1;


    while (1)
    {
        PCON = 0x02;                //MCU进入掉电模式
        _nop_();                //掉电模式被唤醒后,首先执行此语句,然后再进入中断服务程序
        _nop_();
    }
}

void PCA_isr() interrupt 7 using 1
{
    if (CCF0)                       //判断是否为捕获中断
    {
        CCF0 = 0;
        P10 = !120;          	    //将测试口取反
    }
}

