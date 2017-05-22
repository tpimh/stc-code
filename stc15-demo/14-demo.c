/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 PCA实现16位定时器+16位外部捕获+6/7/8位PWM-----*/
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

#define FOSC    18432000L
#define T1ms    (FOSC / 1000)

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

sfr P_SW1       = 0xA2;             //外设功能切换寄存器1

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

sfr CCON        =   0xD8;           //PCA控制寄存器
sbit CCF0       =   CCON^0;         //PCA模块0中断标志
sbit CCF1       =   CCON^1;         //PCA模块1中断标志
sbit CCF2       =   CCON^2;         //PCA模块2中断标志
sbit CR         =   CCON^6;         //PCA定时器运行控制位
sbit CF         =   CCON^7;         //PCA定时器溢出标志
sfr CMOD        =   0xD9;           //PCA模式寄存器
sfr CL          =   0xE9;           //PCA定时器低字节
sfr CH          =   0xF9;           //PCA定时器高字节
sfr CCAPM0      =   0xDA;           //PCA模块0模式寄存器
sfr CCAPM1      =   0xDB;           //PCA模块1模式寄存器
sfr CCAPM2      =   0xDC;           //PCA模块2模式寄存器
sfr CCAP0L      =   0xEA;           //PCA模块0捕获寄存器 LOW
sfr CCAP1L      =   0xEB;           //PCA模块1捕获寄存器 LOW
sfr CCAP2L      =   0xEC;           //PCA模块2捕获寄存器 LOW
sfr CCAP0H      =   0xFA;           //PCA模块0捕获寄存器 HIGH
sfr CCAP1H      =   0xFB;           //PCA模块1捕获寄存器 HIGH
sfr CCAP2H      =   0xFC;           //PCA模块2捕获寄存器 HIGH
sfr PCA_PWM0    =   0xF2;           //PCA模块0的PWM寄存器
sfr PCA_PWM1    =   0xF3;           //PCA模块1的PWM寄存器
sfr PCA_PWM2    =   0xF4;           //PCA模块2的PWM寄存器

sbit PCA_LED1   =   P0^0;           //测试LED1
sbit PCA_LED2   =   P0^1;           //测试LED2
sbit PCA_LED3   =   P0^2;           //测试LED3

WORD value;
BYTE cnt;                           //存储PCA计时溢出次数
DWORD count0;                       //记录上一次的捕获值
DWORD count1;                       //记录本次的捕获值
DWORD length;                       //存储捕获到的脉冲宽度(count1 - count0)

void PCA_isr() interrupt 7 using 1
{
    if (CF)                         //判断是否为PCA计时溢出中断
    {
        CF = 0;
        cnt++;                      //PCA计时溢出次数+1
    }
    if (CCF0)                       //判断是否为CCF0中断(16位定时器中断)
    {
        CCF0 = 0;                   //清中断标志
        CCAP0L = value;
    	CCAP0H = value >> 8;        //更新比较值
        value += T1ms;
        PCA_LED1 = !PCA_LED1;
    }
    if (CCF1)                       //判断是否为CCF1中断(捕获中断)
    {
        CCF1 = 0;
        count0 = count1;                //备份上一次的捕获值
        ((BYTE *)&count1)[3] = CCAP1L;  //保存本次的捕获值
        ((BYTE *)&count1)[2] = CCAP1H;
        ((BYTE *)&count1)[1] = cnt;
        ((BYTE *)&count1)[0] = 0;
        length = count1 - count0;       //计算两次捕获的差值,即为捕获到的脉冲宽度
        PCA_LED2 = !PCA_LED2;
    }
    if (CCF2)                       //判断是否为CCF2中断(PWM中断)
    {
        CCF2 = 0;
        PCA_LED3 = !PCA_LED3;
    }
}

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
    CMOD = 0x09;                    //设置PCA时钟源为系统时钟/1
                                    //使能PCA定时器溢出中断
    value = T1ms;
    CCAP0L = value;                 //PCA模块0实现16位定时器
	CCAP0H = value >> 8;            //初始化PCA模块0
    value += T1ms;
    CCAPM0 = 0x49;                  //PCA模块0实现16位定时器(1ms)
//  CCAPM0 = 0x4d;                  //PCA模块0实现16位定时器(1ms),同时输出高速脉冲到CEX0口

    CCAP1L = 0;                     //PCA模块1实现16位捕获
    CCAP1H = 0;                     //初始化PCA模块1
    CCAPM1 = 0x21;                  //PCA模块1为16位捕获模式(上升沿捕获,可测从高电平开始的整个周期),且产生捕获中断
//  CCAPM1 = 0x11;                  //PCA模块1为16位捕获模式(下降沿捕获,可测从低电平开始的整个周期),且产生捕获中断
//  CCAPM1 = 0x31;                  //PCA模块1为16位捕获模式(上升沿/下降沿捕获,可测高电平或者低电平宽度),且产生捕获中断
    cnt = 0;                        //初始化捕获变量
    count0 = 0;
    count1 = 0;

    CCAP2L = 0x20;                  //PCA模块2实现6/7/8位PWM
    CCAP2H = 0x20;                  //初始化PCA模块2
    PCA_PWM2 = 0x00;                //PCA模块2工作于8位PWM,PWM的占空比为87.5% ((100H-20H)/100H)
//  PCA_PWM2 = 0x40;                //PCA模块2工作于7位PWM,PWM的占空比为75% ((80H-20H)/80H)
//  PCA_PWM2 = 0x80;                //PCA模块2工作于6位PWM,PWM的占空比为50% ((40H-20H)/40H)
    CCAPM2 = 0x42;                  //PCA模块2为PWM模式,不产生中断
//  CCAPM2 = 0x53;                  //PCA模块2为PWM模式,且产生下降沿中断
//  CCAPM2 = 0x63;                  //PCA模块2为PWM模式,且产生上升沿中断
//  CCAPM2 = 0x73;                  //PCA模块2为PWM模式,且产生翻转中断

    CR = 1;                         //PCA定时器开始工作
    EA = 1;

    while (1);
}

