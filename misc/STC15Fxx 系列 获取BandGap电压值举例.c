/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15Fxx 系列 获取掉BandGap电压值举例并用软件模拟串口输出显示---*/
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

//-----------------------------------------
//define baudrate const
//BAUD = 256 - FOSC/3/BAUDRATE/M (1T:M=1; 12T:M=12)
//NOTE: (FOSC/3/BAUDRATE) must be greater then 98, (RECOMMEND GREATER THEN 110)

//#define BAUD	0xF400                  // 1200bps @ 11.0592MHz
//#define BAUD	0xFA00                  // 2400bps @ 11.0592MHz
//#define BAUD	0xFD00                  // 4800bps @ 11.0592MHz
//#define BAUD	0xFE80                  // 9600bps @ 11.0592MHz
//#define BAUD	0xFF40                  //19200bps @ 11.0592MHz
//#define BAUD	0xFFA0					//38400bps @ 11.0592MHz

//#define BAUD	0xEC00                  // 1200bps @ 18.432MHz
//#define BAUD	0xF600                  // 2400bps @ 18.432MHz
//#define BAUD	0xFB00                  // 4800bps @ 18.432MHz
//#define BAUD	0xFD80                  // 9600bps @ 18.432MHz
//#define BAUD	0xFEC0                  //19200bps @ 18.432MHz
#define BAUD	0xFF60                  //38400bps @ 18.432MHz

//#define BAUD	0xE800                  // 1200bps @ 22.1184MHz
//#define BAUD	0xF400                  // 2400bps @ 22.1184MHz
//#define BAUD	0xFA00                  // 4800bps @ 22.1184MHz
//#define BAUD	0xFD00                  // 9600bps @ 22.1184MHz
//#define BAUD	0xFE80                  //19200bps @ 22.1184MHz
//#define BAUD	0xFF40                  //38400bps @ 22.1184MHz
//#define BAUD	0xFF80                  //57600bps @ 22.1184MHz

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

sfr AUXR = 0x8E;
sbit RXB = P3^0;                        //define UART TX/RX port
sbit TXB = P3^1;

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;

BYTE TBUF,RBUF;
BYTE TDAT,RDAT;
BYTE TCNT,RCNT;
BYTE TBIT,RBIT;
BOOL TING,RING;
BOOL TEND,REND;

void UART_INIT();
void UART_SEND(BYTE dat);

BYTE t, r;
BYTE buf[16];

void main()
{
    BYTE idata *iptr;
    BYTE code *cptr;
    BYTE i;
    
    TMOD = 0x00;                        //timer0 in 16-bit auto reload mode
    AUXR = 0x80;                        //timer0 working at 1T mode
    TL0 = BAUD;
    TH0 = BAUD>>8;                      //initial timer0 and set reload value
    TR0 = 1;                            //tiemr0 start running
    ET0 = 1;                            //enable timer0 interrupt
    PT0 = 1;                            //improve timer0 interrupt priority
    EA = 1;                             //open global interrupt switch

    UART_INIT();
    iptr = ID_ADDR_RAM;                 //从RAM区读取BandGap电压值(单位:毫伏mV)
    for (i=0; i<2; i++)                 //读2个字节(高字节在前)
    {
        UART_SEND(*iptr++);
    }
    
    cptr = ID_ADDR_ROM;                 //从程序区读取BandGap电压值(单位:毫伏mV)
    for (i=0; i<2; i++)                 //读2个字节(高字节在前)
    {
        UART_SEND(*cptr++);
    }

    while (1);
}

//-----------------------------------------
//Timer interrupt routine for UART

void tm0() interrupt 1 using 1
{
    if (RING)
    {
        if (--RCNT == 0)
        {
            RCNT = 3;                   //reset send baudrate counter
            if (--RBIT == 0)
            {
                RBUF = RDAT;            //save the data to RBUF
                RING = 0;               //stop receive
                REND = 1;               //set receive completed flag
            }
            else
            {
                RDAT >>= 1;
                if (RXB) RDAT |= 0x80;	//shift RX data to RX buffer
            }
        }
    }
    else if (!RXB)
    {
        RING = 1;                       //set start receive flag
        RCNT = 4;                       //initial receive baudrate counter
        RBIT = 9;                       //initial receive bit number (8 data bits + 1 stop bit)
    }

    if (--TCNT == 0)
    {
        TCNT = 3;                       //reset send baudrate counter
        if (TING)                       //judge whether sending
        {
            if (TBIT == 0)
            {
                TXB = 0;                //send start bit
                TDAT = TBUF;            //load data from TBUF to TDAT
                TBIT = 9;               //initial send bit number (8 data bits + 1 stop bit)
            }
            else
            {
                TDAT >>= 1;             //shift data to CY
                if (--TBIT == 0)
                {
                    TXB = 1;
                    TING = 0;           //stop send
                    TEND = 1;           //set send completed flag
                }
                else
                {
                    TXB = CY;           //write CY to TX port
                }
            }
        }
    }
}

//-----------------------------------------
//initial UART module variable

void UART_INIT()
{
    TING = 0;
    RING = 0;
    TEND = 1;
    REND = 0;
    TCNT = 0;
    RCNT = 0;
}

//-----------------------------------------
//Send UART data

void UART_SEND(BYTE dat)
{
    while (!TEND);
    TEND = 0;
    TBUF = dat;
    TING = 1;
}

