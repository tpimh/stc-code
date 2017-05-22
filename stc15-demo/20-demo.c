/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 SPI的主模式读写外部串行Flash举例(中断方式)----*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了宏晶科技的资料及程序   */
/* 如果要在文章中应用此代码,请在文章中注明使用了宏晶科技的资料及程序   */
/*---------------------------------------------------------------------*/

//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//假定测试芯片的工作频率为18.432MHz
//本示例所读写的目标Flash为PM25LV040,本代码已使用U7编程器测试通过

#include "reg51.h"

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#define FOSC            18432000L
#define BAUD            (65536 - FOSC / 4 / 115200)

#define NULL            0
#define FALSE           0
#define TRUE            1

sfr  AUXR           =   0x8e;                   //辅助寄存器
sfr P_SW1           =   0xa2;                   //外设功能切换寄存器1
#define SPI_S0          0x04
#define SPI_S1          0x08

sfr SPSTAT          =   0xcd;                   //SPI状态寄存器
#define SPIF            0x80                    //SPSTAT.7
#define WCOL            0x40                    //SPSTAT.6
sfr SPCTL           =   0xce;                   //SPI控制寄存器
#define SSIG            0x80                    //SPCTL.7
#define SPEN            0x40                    //SPCTL.6
#define DORD            0x20                    //SPCTL.5
#define MSTR            0x10                    //SPCTL.4
#define CPOL            0x08                    //SPCTL.3
#define CPHA            0x04                    //SPCTL.2
#define SPDHH           0x00                    //CPU_CLK/4
#define SPDH            0x01                    //CPU_CLK/16
#define SPDL            0x02                    //CPU_CLK/64
#define SPDLL           0x03                    //CPU_CLK/128
sfr SPDAT           =   0xcf;                   //SPI数据寄存器

sbit SS             =   P2^4;                   //SPI的SS脚,连接到Flash的CE

sfr IE2	            =   0xAF;                   //中断控制寄存器2
#define ESPI            0x02	                //IE2.1

#define SFC_WREN        0x06                    //串行Flash命令集
#define SFC_WRDI        0x04
#define SFC_RDSR        0x05
#define SFC_WRSR        0x01
#define SFC_READ        0x03
#define SFC_FASTREAD    0x0B
#define SFC_RDID        0xAB
#define SFC_PAGEPROG    0x02
#define SFC_RDCR        0xA1
#define SFC_WRCR        0xF1
#define SFC_SECTORER    0xD7
#define SFC_BLOCKER     0xD8
#define SFC_CHIPER      0xC7

void InitUart();
void SendUart(BYTE dat);
void InitSpi();
BYTE SpiShift(BYTE dat);
BOOL FlashCheckID();
BOOL IsFlashBusy();
void FlashWriteEnable();
void FlashErase();
void FlashRead(DWORD addr, DWORD size, BYTE *buffer);
void FlashWrite(DWORD addr, DWORD size, BYTE *buffer);

#define BUFFER_SIZE     1024                    //缓冲区大小
#define TEST_ADDR       0                       //Flash测试地址

BYTE xdata g_Buffer[BUFFER_SIZE];               //Flash读写缓冲区
BOOL g_fFlashOK;                                //Flash状态
BOOL g_fSpiBusy;                                //SPI的工作状态

void main()
{
    int i;
    
    //初始化Flash状态
    g_fFlashOK = FALSE;
    g_fSpiBusy = FALSE;
    
    //初始化串口和SPI
    InitUart();
    InitSpi();

    //使能SPI传输中断
    IE2 |= ESPI;
    EA = 1;

    //检测Flash状态
    FlashCheckID();
    
    //擦除Flash
    FlashErase();
    //读取测试地址的数据
    FlashRead(TEST_ADDR, BUFFER_SIZE, g_Buffer);
    //发送到串口
    for (i=0; i<BUFFER_SIZE; i++) SendUart(g_Buffer[i]);
    
    //修改置缓冲区
    for (i=0; i<BUFFER_SIZE; i++) g_Buffer[i] = i>>2;
    //将缓冲区的数据写到Flash中
    FlashWrite(TEST_ADDR, BUFFER_SIZE, g_Buffer);
    
    //读取测试地址的数据
    FlashRead(TEST_ADDR, BUFFER_SIZE, g_Buffer);
    //发送到串口
    for (i=0; i<BUFFER_SIZE; i++) SendUart(g_Buffer[i]);
    
    FlashErase();
    //读取测试地址的数据
    FlashRead(TEST_ADDR, BUFFER_SIZE, g_Buffer);
    //发送到串口
    for (i=0; i<BUFFER_SIZE; i++) SendUart(g_Buffer[i]);
    
    //修改置缓冲区
    for (i=0; i<BUFFER_SIZE; i++) g_Buffer[i]= 255-(i>>2);
    //将缓冲区的数据写到Flash中
    FlashWrite(TEST_ADDR, BUFFER_SIZE, g_Buffer);
    
    //读取测试地址的数据
    FlashRead(TEST_ADDR, BUFFER_SIZE, g_Buffer);
    //发送到串口
    for (i=0; i<BUFFER_SIZE; i++) SendUart(g_Buffer[i]);
    
    while (1);
}

/************************************************
SPI中断服务程序
************************************************/
void spi_isr() interrupt 9 using 1
{
    SPSTAT = SPIF | WCOL;                       //清除SPI状态位
    g_fSpiBusy = FALSE;
}

/************************************************
串口初始化
入口参数: 无
出口参数: 无
************************************************/
void InitUart()
{
    AUXR = 0x40;                                //设置定时器1为1T模式
    TMOD = 0x00;                                //定时器1为16位重载模式
    TH1 = BAUD >> 8;                            //设置波特率
    TL1 = BAUD;
    TR1 = 1;
    SCON = 0x5a;                                //设置串口为8位数据位,波特率可变模式
}

/************************************************
发送数据到串口
入口参数:
    dat : 准备发送的数据
出口参数: 无
************************************************/
void SendUart(BYTE dat)
{
    while (!TI);                                //等待上一个数据发送完成
    TI = 0;                                     //清除发送完成标志
    SBUF = dat;                                 //触发本次的数据发送
}

/************************************************
SPI初始化
入口参数: 无
出口参数: 无
************************************************/
void InitSpi()
{
//  ACC = P_SW1;                                //切换到第一组SPI
//  ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=0 SPI_S1=0
//  P_SW1 = ACC;                                //(P1.2/SS, P1.3/MOSI, P1.4/MISO, P1.5/SCLK)

    ACC = P_SW1;                                //可用于测试U7,U7使用的是第二组SPI控制Flash
    ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=1 SPI_S1=0
    ACC |= SPI_S0;                              //(P2.4/SS_2, P2.3/MOSI_2, P2.2/MISO_2, P2.1/SCLK_2)
    P_SW1 = ACC;  

//  ACC = P_SW1;                                //切换到第三组SPI
//  ACC &= ~(SPI_S0 | SPI_S1);                  //SPI_S0=0 SPI_S1=1
//  ACC |= SPI_S1;                              //(P5.4/SS_3, P4.0/MOSI_3, P4.1/MISO_3, P4.3/SCLK_3)
//  P_SW1 = ACC;  

    SPSTAT = SPIF | WCOL;                       //清除SPI状态
    SS = 1;
    SPCTL = SSIG | SPEN | MSTR;                 //设置SPI为主模式
}

/************************************************
使用SPI方式与Flash进行数据交换
入口参数:
    dat : 准备写入的数据
出口参数:
    从Flash中读出的数据
************************************************/
BYTE SpiShift(BYTE dat)
{
    g_fSpiBusy = TRUE;
    SPDAT = dat;                                //触发SPI发送
    while (g_fSpiBusy);                         //等待SPI数据传输完成
    
    return SPDAT;
}

/************************************************
检测Flash是否准备就绪
入口参数: 无
出口参数:
    0 : 没有检测到正确的Flash
    1 : Flash准备就绪
************************************************/
BOOL FlashCheckID()
{
    BYTE dat1, dat2;
    
    SS = 0;
    SpiShift(SFC_RDID);                         //发送读取ID命令
    SpiShift(0x00);                             //空读3个字节
    SpiShift(0x00);
    SpiShift(0x00);
    dat1 = SpiShift(0x00);                      //读取制造商ID1
    SpiShift(0x00);                             //读取设备ID
    dat2 = SpiShift(0x00);                      //读取制造商ID2
    SS = 1;
                                                //检测是否为PM25LVxx系列的Flash
    g_fFlashOK = ((dat1 == 0x9d) && (dat2 == 0x7f));
    
    return g_fFlashOK;
}

/************************************************
检测Flash的忙状态
入口参数: 无
出口参数:
    0 : Flash处于空闲状态
    1 : Flash处于忙状态
************************************************/
BOOL IsFlashBusy()
{
    BYTE dat;
    
    SS = 0;
    SpiShift(SFC_RDSR);                         //发送读取状态命令
    dat = SpiShift(0);                          //读取状态
    SS = 1;
    
    return (dat & 0x01);                        //状态值的Bit0即为忙标志
}

/************************************************
使能Flash写命令
入口参数: 无
出口参数: 无
************************************************/
void FlashWriteEnable()
{
    while (IsFlashBusy());                      //Flash忙检测
    SS = 0;
    SpiShift(SFC_WREN);                         //发送写使能命令
    SS = 1;
}

/************************************************
擦除整片Flash
入口参数: 无
出口参数: 无
************************************************/
void FlashErase()
{
    if (g_fFlashOK)
    {
        FlashWriteEnable();                     //使能Flash写命令
        SS = 0;
        SpiShift(SFC_CHIPER);                   //发送片擦除命令
        SS = 1;
    }
}

/************************************************
从Flash中读取数据
入口参数:
    addr   : 地址参数
    size   : 数据块大小
    buffer : 缓冲从Flash中读取的数据
出口参数:
    无
************************************************/
void FlashRead(DWORD addr, DWORD size, BYTE *buffer)
{
    if (g_fFlashOK)
    {
        while (IsFlashBusy());                  //Flash忙检测
        SS = 0;
        SpiShift(SFC_FASTREAD);                 //使用快速读取命令
        SpiShift(((BYTE *)&addr)[1]);           //设置起始地址
        SpiShift(((BYTE *)&addr)[2]);
        SpiShift(((BYTE *)&addr)[3]);
        SpiShift(0);                            //需要空读一个字节
        while (size)
        {
            *buffer = SpiShift(0);              //自动连续读取并保存
            addr++;
            buffer++;
            size--;
        }
        SS = 1;
    }
}

/************************************************
写数据到Flash中
入口参数:
    addr   : 地址参数
    size   : 数据块大小
    buffer : 缓冲需要写入Flash的数据
出口参数: 无
************************************************/
void FlashWrite(DWORD addr, DWORD size, BYTE *buffer)
{
    if (g_fFlashOK)
    while (size)
    {
        FlashWriteEnable();                     //使能Flash写命令
        SS = 0;
        SpiShift(SFC_PAGEPROG);                 //发送页编程命令
        SpiShift(((BYTE *)&addr)[1]);           //设置起始地址
        SpiShift(((BYTE *)&addr)[2]);
        SpiShift(((BYTE *)&addr)[3]);
        while (size)
        {
            SpiShift(*buffer);                  //连续页内写
            addr++;
            buffer++;
            size--;
            if ((addr & 0xff) == 0) break;
        }
        SS = 1;
    }
}

