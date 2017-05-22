/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 获取掉BandGap电压值举例-----------------------*/
/* --- Mobile: (86)13922805190 -------------- -------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/* 如果要在文章中应用此代码,请在文章中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//假定测试芯片的工作频率为18.432MHz

#include "reg51.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;


#define     URMD    0           //0:使用定时器2作为波特率发生器
                                //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器

sfr T2H   = 0xd6;               //定时器2高8位
sfr T2L   = 0xd7;               //定时器2低8位

sfr  AUXR       =   0x8e;       //辅助寄存器                              

//关于此参数的更详细的说明请参考2013年5月20日的重要说明

#define ID_ADDR_RAM 0xef        //对于只有256字节RAM的MCU存放地址为0EFH
//#define ID_ADDR_RAM 0x6f        //对于只有128字节RAM的MCU存放地址为06fH

//注意:需要在下载代码时选择"在ID号前添加重要测试参数"选项,才可在程序中获取此参数
//#define ID_ADDR_ROM 0x03f7      //1K程序空间的MCU
//#define ID_ADDR_ROM 0x07f7      //2K程序空间的MCU
//#define ID_ADDR_ROM 0x0bf7      //3K程序空间的MCU
//#define ID_ADDR_ROM 0x0ff7      //4K程序空间的MCU
//#define ID_ADDR_ROM 0x13f7      //5K程序空间的MCU
//#define ID_ADDR_ROM 0x1ff7      //8K程序空间的MCU
//#define ID_ADDR_ROM 0x27f7      //10K程序空间的MCU
//#define ID_ADDR_ROM 0x2ff7      //12K程序空间的MCU
//#define ID_ADDR_ROM 0x3ff7      //16K程序空间的MCU
//#define ID_ADDR_ROM 0x4ff7      //20K程序空间的MCU
//#define ID_ADDR_ROM 0x5ff7      //24K程序空间的MCU
//#define ID_ADDR_ROM 0x6ff7      //28K程序空间的MCU
//#define ID_ADDR_ROM 0x7ff7      //32K程序空间的MCU
//#define ID_ADDR_ROM 0x9ff7      //40K程序空间的MCU
//#define ID_ADDR_ROM 0xbff7      //48K程序空间的MCU
//#define ID_ADDR_ROM 0xcff7      //52K程序空间的MCU
//#define ID_ADDR_ROM 0xdff7      //56K程序空间的MCU
#define ID_ADDR_ROM 0xeff7      //60K程序空间的MCU

//-----------------------------------------

void InitUart();
void SendUart(BYTE dat);

//-----------------------------------------

void main()
{
    BYTE idata *iptr;
    BYTE code *cptr;
    BYTE i;
    
    InitUart();                 //串口初始化
    
    iptr = ID_ADDR_RAM;         //从RAM区读取BandGap电压值(单位:毫伏mV)
    for (i=0; i<2; i++)         //读2个字节(高字节在前)
    {
        SendUart(*iptr++);
    }
    
    cptr = ID_ADDR_ROM;         //从程序区读取BandGap电压值(单位:毫伏mV)
    for (i=0; i<2; i++)         //读2个字节(高字节在前)
    {
        SendUart(*cptr++);
    }

    while (1);                  //程序终止
}

/*----------------------------
初始化串口
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                //设置串口为8位可变波特率
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

/*----------------------------
发送串口数据
----------------------------*/

void SendUart(BYTE dat)
{
    while (!TI);                //等待前面的数据发送完成
    TI = 0;                     //清除发送完成标志
    SBUF = dat;                 //发送串口数据
}

