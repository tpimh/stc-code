/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 AD转换中断方式举例----------------------------*/
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
#define BAUD    9600

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define     URMD    0           //0:使用定时器2作为波特率发生器
                                //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器

sfr T2H   = 0xd6;               //定时器2高8位
sfr T2L   = 0xd7;               //定时器2低8位

sfr  AUXR       =   0x8e;       //辅助寄存器                              

sfr ADC_CONTR   =   0xBC;           //ADC控制寄存器
sfr ADC_RES     =   0xBD;           //ADC高8位结果
sfr ADC_LOW2    =   0xBE;           //ADC低2位结果
sfr P1ASF       =   0x9D;           //P1口第2功能控制寄存器

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

void InitUart();
void SendData(BYTE dat);
void Delay(WORD n);
void InitADC();

BYTE ch = 0;                        //ADC通道号

void main()
{
    InitUart();                     //初始化串口
    InitADC();                      //初始化ADC
    IE = 0xa0;                      //使能ADC中断
                                    //开始AD转换
    while (1);
}

/*----------------------------
ADC中断服务程序
----------------------------*/
void adc_isr() interrupt 5 using 1
{
    ADC_CONTR &= !ADC_FLAG;         //清除ADC中断标志

    SendData(ch);                   //显示通道号
    SendData(ADC_RES);              //读取高8位结果并发送到串口

//    SendData(ADC_LOW2);           //显示低2位结果
    
    if (++ch > 7) ch = 0;           //切换到下一个通道
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
}

/*----------------------------
初始化ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
    Delay(2);                       //ADC上电并延时
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
void SendData(BYTE dat)
{
    while (!TI);                    //等待前一个数据发送完成
    TI = 0;                         //清除发送标志
    SBUF = dat;                     //发送当前数据
}

/*----------------------------
软件延时
----------------------------*/
void Delay(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

