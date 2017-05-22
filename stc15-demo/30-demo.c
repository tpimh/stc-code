/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 获取全球唯一身份证号码(ID号)举例--------------*/
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

#define ID_ADDR_RAM 0xf1        //ID号的存放在RAM区的地址为0F1H

                                //ID号的存放在程序区的地址为程序空间的最后7字节
//#define ID_ADDR_ROM 0x03f9      //1K程序空间的MCU(如                                             STC15F201EA, STC15F101EA)
//#define ID_ADDR_ROM 0x07f9      //2K程序空间的MCU(如                              STC15F402EACS, STC15F202EA, STC15F102EA)
//#define ID_ADDR_ROM 0x0bf9      //3K程序空间的MCU(如                                             STC15F203EA, STC15F103EA)
//#define ID_ADDR_ROM 0x0ff9      //4K程序空间的MCU(如               STC15F804EACS, STC15F404EACS, STC15F204EA, STC15F104EA)
//#define ID_ADDR_ROM 0x13f9      //5K程序空间的MCU(如                                             STC15F205EA, STC15F105EA)
//#define ID_ADDR_ROM 0x1ff9      //8K程序空间的MCU(如STC15F2K08S2,  STC15F808EACS, STC15F408EACS)
//#define ID_ADDR_ROM 0x27f9      //10K程序空间的MCU(如                             STC15F410EACS)
//#define ID_ADDR_ROM 0x2ff9      //12K程序空间的MCU(如              STC15F812EACS, STC15F412EACS)
//#define ID_ADDR_ROM 0x3ff9      //16K程序空间的MCU(如STC15F2K16S2, STC15F816EACS)
//#define ID_ADDR_ROM 0x4ff9      //20K程序空间的MCU(如STC15F2K20S2, STC15F820EACS)
//#define ID_ADDR_ROM 0x5ff9      //24K程序空间的MCU(如              STC15F824EACS)
//#define ID_ADDR_ROM 0x6ff9      //28K程序空间的MCU(如              STC15F828EACS)
//#define ID_ADDR_ROM 0x7ff9      //32K程序空间的MCU(如STC15F2K32S2)
//#define ID_ADDR_ROM 0x9ff9      //40K程序空间的MCU(如STC15F2K40S2)
//#define ID_ADDR_ROM 0xbff9      //48K程序空间的MCU(如STC15F2K48S2)
//#define ID_ADDR_ROM 0xcff9      //52K程序空间的MCU(如STC15F2K52S2)
//#define ID_ADDR_ROM 0xdff9      //56K程序空间的MCU(如STC15F2K56S2)
#define ID_ADDR_ROM 0xeff9      //60K程序空间的MCU(如STC15F2K60S2)

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
    
    iptr = ID_ADDR_RAM;         //从RAM区读取ID号
    for (i=0; i<7; i++)         //读7个字节
    {
        SendUart(*iptr++);      //发送ID到串口
    }
    
    cptr = ID_ADDR_ROM;         //从程序区读取ID号
    for (i=0; i<7; i++)         //读7个字节
    {
        SendUart(*cptr++);      //发送ID到串口
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

