/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 串口1地址自动识别举例举例---------------------*/
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

//-----------------------------------------------

#define     SLAVE   1               //定义从机编号,0为从机1, 1为从机2

#if SLAVE == 0
#define     SAMASK  0x33            //从机1地址屏蔽位
#define     SERADR  0x55            //从机1的地址为xx01,xx01
#define     ACKTST  0x78            //从机1应答测试数据
#else
#define     SAMASK  0x3C            //从机2地址屏蔽位
#define     SERADR  0x5A            //从机2的地址为xx01,10xx
#define     ACKTST  0x49            //从机2应答测试数据
#endif

#define     URMD    2               //0:使用定时器2作为波特率发生器
                                    //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                    //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器

sfr T2H   =   0xd6;                 //定时器2高8位
sfr T2L   =   0xd7;                 //定时器2低8位

sfr AUXR  =   0x8e;                 //辅助寄存器                              

sfr SADDR =   0xA9;                 //从机地址寄存器
sfr SADEN =   0xB9;                 //从机地址屏蔽寄存器

void InitUart();

char count;

void main()
{
    InitUart();                     //初始化串口
    ES = 1;
    EA = 1;
    while (1);
}

/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4 using 1
{
    if (TI)
    {
        TI = 0;                     //清除TI位
        if (count != 0)
        {
            count--;
            SBUF = ACKTST;          //继续发送应答数据
        }
        else
        {
            SM2 = 1;                //若发送完成,则重新开始地址检测
        }
    }
    if (RI)
    {
        RI = 0;                     //清除RI位
        SM2 = 0;                    //本机被选中后,进入数据接收状态
        count = 7;
        SBUF = ACKTST;              //并开发送应答数据
    }
}

/*----------------------------
初始化串口
----------------------------*/
void InitUart()
{
    SADDR = SERADR;
    SADEN = SAMASK;
    SCON = 0xf8;                //设置串口为9位可变波特率,使能多机通讯检测,(将TB8设置为1,方便与PC直接通讯测试)
#if URMD == 0
    T2L = 0xd8;                 //设置波特率重装值
    T2H = 0xff;                 //115200 bps(65536-18432000/4/115200)
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
#elif URMD == 1
    AUXR = 0x40;                //定时器1为1T模式
    TMOD = 0x00;                //定时器1为模式0(16位自动重载)
    TL1 = 0xd8;                 //设置波特率重装值
    TH1 = 0xff;                 //115200 bps(65536-18432000/4/115200)
    TR1 = 1;                    //定时器1开始启动
#else
    TMOD = 0x20;                //设置定时器1为8位自动重装载模式
    AUXR = 0x40;                //定时器1为1T模式
    TH1 = TL1 = 0xfb;           //115200 bps(256 - 18432000/32/115200)
    TR1 = 1;
#endif
}
