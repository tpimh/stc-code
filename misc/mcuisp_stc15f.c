/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- 使用主芯片对从芯片(限STC15系列)进行ISP下载举例 -----------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/* 如果要在文章中应用此代码,请在文章中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//假定测试芯片的工作频率为11.0592MHz

//注意:使用本代码对STC15系列的单片机进行下载时,必须要执行了Download代码之后,
//才能给目标芯片上电,否则目标芯片将无法正确下载

#include "reg51.h"

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

//宏、常量定义
#define FALSE               0
#define TRUE                1
#define LOBYTE(w)           ((BYTE)(WORD)(w))
#define HIBYTE(w)           ((BYTE)((WORD)(w) >> 8))

#define MINBAUD             2400L
#define MAXBAUD             115200L

#define FOSC                11059200L                   //主控芯片工作频率
#define BR(n)               (65536 - FOSC/4/(n))        //主控芯片串口波特率计算公式
#define T1MS                (65536 - FOSC/1000)         //主控芯片1ms定时初值

#define FUSER               24000000L                   //15系列目标芯片工作频率
#define RL(n)               (65536 - FUSER/4/(n))       //15系列目标芯片串口波特率计算公式

//SFR定义
sfr AUXR = 0x8e;

//变量定义
BOOL f1ms;                                              //1ms标志位
BOOL UartBusy;                                          //串口发送忙标志位
BOOL UartReceived;                                      //串口数据接收完成标志位
BYTE UartRecvStep;                                      //串口数据接收控制
BYTE TimeOut;                                           //串口通讯超时计数器
BYTE xdata TxBuffer[256];                               //串口数据发送缓冲区
BYTE xdata RxBuffer[256];                               //串口数据接收缓冲区
char code DEMO[256];                                    //演示代码数据

//函数声明
void Initial(void);
void DelayXms(WORD x);
BYTE UartSend(BYTE dat);
void CommInit(void);
void CommSend(BYTE size);
BOOL Download(BYTE *pdat, long size);

//主函数入口
void main(void)
{
    while (1)
    {
        Initial();
        if (Download(DEMO, 0x0100))
        {
            //下载成功
            P3 = 0xff;
            DelayXms(500);
            P3 = 0x00;
            DelayXms(500);
            P3 = 0xff;
            DelayXms(500);
            P3 = 0x00;
            DelayXms(500);
            P3 = 0xff;
            DelayXms(500);
            P3 = 0x00;
            DelayXms(500);
            P3 = 0xff;
        }
        else
        {
            //下载失败
            P3 = 0xff;
            DelayXms(500);
            P3 = 0xf3;
            DelayXms(500);
            P3 = 0xff;
            DelayXms(500);
            P3 = 0xf3;
            DelayXms(500);
            P3 = 0xff;
            DelayXms(500);
            P3 = 0xf3;
            DelayXms(500);
            P3 = 0xff;
        }
    }
}

//1ms定时器中断服务程序
void tm0(void) interrupt 1 using 1
{
    static BYTE Counter100;
    
    f1ms = TRUE;
    if (Counter100-- == 0)
    {
        Counter100 = 100;
        if (TimeOut) TimeOut--;
    }
}

//串口中断服务程序
void uart(void) interrupt 4 using 1
{
    static WORD RecvSum;
    static BYTE RecvIndex;
    static BYTE RecvCount;
    BYTE dat;

    if (TI)
    {   
        TI = 0;
        UartBusy = FALSE;
    }
    
    if (RI)
    {
        RI = 0;
        dat = SBUF;
        switch (UartRecvStep)
        {
        case 1:
            if (dat != 0xb9) goto L_CheckFirst;
            UartRecvStep++;
            break;
        case 2:
            if (dat != 0x68) goto L_CheckFirst;
            UartRecvStep++;
            break;
        case 3:
            if (dat != 0x00) goto L_CheckFirst;
            UartRecvStep++;
            break;
        case 4:
            RecvSum = 0x68 + dat;
            RecvCount = dat - 6;
            RecvIndex = 0;
            UartRecvStep++;
            break;
        case 5:
            RecvSum += dat;
            RxBuffer[RecvIndex++] = dat;
            if (RecvIndex == RecvCount) UartRecvStep++;
            break;
        case 6:
            if (dat != HIBYTE(RecvSum)) goto L_CheckFirst;
            UartRecvStep++;
            break;
        case 7:
            if (dat != LOBYTE(RecvSum)) goto L_CheckFirst;
            UartRecvStep++;
            break;
        case 8:
            if (dat != 0x16) goto L_CheckFirst;
            UartReceived = TRUE;
            UartRecvStep++;
            break;
L_CheckFirst:
        case 0:
        default:
            CommInit();
            UartRecvStep = (dat == 0x46 ? 1 : 0);
            break;
        }
    }
}

//系统初始化
void Initial(void)
{
    UartBusy = FALSE;

    SCON = 0xd0;                    //串口数据模式必须为8位数据+1位偶检验
    AUXR = 0xc0;
    TMOD = 0x00;
    TH0 = HIBYTE(T1MS);
    TL0 = LOBYTE(T1MS);
    TR0 = 1;
    TH1 = HIBYTE(BR(MINBAUD));
    TL1 = LOBYTE(BR(MINBAUD));
    TR1 = 1;
    ET0 = 1;
    ES = 1;
    EA = 1;
}

//Xms延时程序
void DelayXms(WORD x)
{
    do
    {
        f1ms = FALSE;
        while (!f1ms);
    } while (x--);
}

//串口数据发送程序
BYTE UartSend(BYTE dat)
{
    while (UartBusy);
    
    UartBusy = TRUE;
    ACC = dat;
    TB8 = P;
    SBUF = ACC;
    
    return dat;
}

//串口通讯初始化
void CommInit(void)
{
    UartRecvStep = 0;
    TimeOut = 20;
    UartReceived = FALSE;
}

//发送串口通讯数据包
void CommSend(BYTE size)
{
    WORD sum;
    BYTE i;
    
    UartSend(0x46);
    UartSend(0xb9);
    UartSend(0x6a);
    UartSend(0x00);
    sum = size + 6 + 0x6a;
    UartSend(size + 6);
    for (i=0; i<size; i++)
    {
        sum += UartSend(TxBuffer[i]);
    }
    UartSend(HIBYTE(sum));
    UartSend(LOBYTE(sum));
    UartSend(0x16);
    while (UartBusy);

    CommInit();
}

//对STC15系列的芯片进行数据下载程序
BOOL Download(BYTE *pdat, long size)
{
    BYTE arg;
    BYTE cnt;
    WORD addr;
    
    //握手
    CommInit();
    while (1)
    {
        if (UartRecvStep == 0)
        {
            UartSend(0x7f);
            DelayXms(10);
        }
        if (UartReceived)
        {
            arg = RxBuffer[4];
            if (RxBuffer[0] == 0x50) break;
            return FALSE;
        }
    }

    //设置参数(设置从芯片使用最高的波特率以及擦除等待时间等参数)
    TxBuffer[0] = 0x01;
    TxBuffer[1] = arg;
    TxBuffer[2] = 0x40;
    TxBuffer[3] = HIBYTE(RL(MAXBAUD));
    TxBuffer[4] = LOBYTE(RL(MAXBAUD));
    TxBuffer[5] = 0x00;
    TxBuffer[6] = 0x00;
    TxBuffer[7] = 0xc3;
    CommSend(8);
    while (1)
    {
        if (TimeOut == 0) return FALSE;
        if (UartReceived)
        {
            if (RxBuffer[0] == 0x01) break;
            return FALSE;
        }
    }

    //准备
    TH1 = HIBYTE(BR(MAXBAUD));
    TL1 = LOBYTE(BR(MAXBAUD));
    DelayXms(10);
    TxBuffer[0] = 0x05;
    CommSend(1);
    while (1)
    {
        if (TimeOut == 0) return FALSE;
        if (UartReceived)
        {
            if (RxBuffer[0] == 0x05) break;
            return FALSE;
        }
    }
    
    //擦除
    DelayXms(10);
    TxBuffer[0] = 0x03;
    TxBuffer[1] = 0x00;
    CommSend(2);
    TimeOut = 100;
    while (1)
    {
        if (TimeOut == 0) return FALSE;
        if (UartReceived)
        {
            if (RxBuffer[0] == 0x03) break;
            return FALSE;
        }
    }

    //写用户代码
    DelayXms(10);
    addr = 0;
    TxBuffer[0] = 0x22;
    while (addr < size)
    {
        TxBuffer[1] = HIBYTE(addr);
        TxBuffer[2] = LOBYTE(addr);
        cnt = 0;
        while (addr < size)
        {
            TxBuffer[cnt+3] = pdat[addr];
            addr++;
            cnt++;
            if (cnt >= 128) break;
        }
        CommSend(cnt + 3);
        while (1)
        {
            if (TimeOut == 0) return FALSE;
            if (UartReceived)
            {
                if ((RxBuffer[0] == 0x02) && (RxBuffer[1] == 'T')) break;
                return FALSE;
            }
        }
        TxBuffer[0] = 0x02;
    }

    ////写硬件选项
    ////如果不需要修改硬件选项,此步骤可直接跳过,此时所有的硬件选项
    ////都维持不变,MCU的频率为上一次所调节频率
    ////若写硬件选项,MCU的内部IRC频率将被固定写为24M,
    ////建议:第一次使用STC-ISP下载软件将从芯片的硬件选项设置好
    ////     以后再使用主芯片对从芯片下载程序时不写硬件选项
    //DelayXms(10);
    //for (cnt=0; cnt<128; cnt++)
    //{
    //    TxBuffer[cnt] = 0xff;
    //}
    //TxBuffer[0] = 0x04;
    //TxBuffer[1] = 0x00;
    //TxBuffer[2] = 0x00;
    //TxBuffer[34] = 0xfd;
    //TxBuffer[62] = arg;
    //TxBuffer[63] = 0x7f;
    //TxBuffer[64] = 0xf7;
    //TxBuffer[65] = 0x7b;
    //TxBuffer[66] = 0x1f;
    //CommSend(67);
    //while (1)
    //{
    //    if (TimeOut == 0) return FALSE;
    //    if (UartReceived)
    //    {
    //        if ((RxBuffer[0] == 0x04) && (RxBuffer[1] == 'T')) break;
    //        return FALSE;
    //    }
    //}

    //下载完成
    return TRUE;
}

char code DEMO[256] = 
{
    0x02,0x00,0x5E,0x12,0x00,0x4B,0x75,0xB0,
    0xEF,0x12,0x00,0x2C,0x75,0xB0,0xDF,0x12,
    0x00,0x2C,0x75,0xB0,0xFE,0x12,0x00,0x2C,
    0x75,0xB0,0xFD,0x12,0x00,0x2C,0x75,0xB0,
    0xFB,0x12,0x00,0x2C,0x75,0xB0,0xF7,0x12,
    0x00,0x2C,0x80,0xDA,0xE4,0xFF,0xFE,0xE4,
    0xFD,0xFC,0x0D,0xBD,0x00,0x01,0x0C,0xBC,
    0x01,0xF8,0xBD,0xF4,0xF5,0x0F,0xBF,0x00,
    0x01,0x0E,0xBE,0x03,0xEA,0xBF,0xE8,0xE7,
    0x02,0x00,0x4B,0x75,0x80,0xFF,0x75,0x90,
    0xFF,0x75,0xA0,0xFF,0x75,0xB0,0xFF,0x75,
    0xC0,0xFF,0x75,0xC8,0xFF,0x22,0x78,0x7F,
    0xE4,0xF6,0xD8,0xFD,0x75,0x81,0x07,0x02,
    0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

