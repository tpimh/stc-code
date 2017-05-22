/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 内部EEPROM举例--------------------------------*/
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

sfr IAP_DATA    =   0xC2;           //IAP数据寄存器
sfr IAP_ADDRH   =   0xC3;           //IAP地址寄存器高字节
sfr IAP_ADDRL   =   0xC4;           //IAP地址寄存器低字节
sfr IAP_CMD     =   0xC5;           //IAP命令寄存器
sfr IAP_TRIG    =   0xC6;           //IAP命令触发寄存器
sfr IAP_CONTR   =   0xC7;           //IAP控制寄存器

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令

//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz

//测试地址
#define IAP_ADDRESS 0x0400

void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);

void main()
{
    WORD i;

    P1 = 0xfe;                      //1111,1110 系统OK
    Delay(10);                      //延时
    IapEraseSector(IAP_ADDRESS);    //扇区擦除
    for (i=0; i<512; i++)           //检测是否擦除成功(全FF检测)
    {
        if (IapReadByte(IAP_ADDRESS+i) != 0xff)
            goto Error;             //如果出错,则退出
    }
    P1 = 0xfc;                      //1111,1100 擦除成功
    Delay(10);                      //延时
    for (i=0; i<512; i++)           //编程512字节
    {
        IapProgramByte(IAP_ADDRESS+i, (BYTE)i);
    }
    P1 = 0xf8;                      //1111,1000 编程完成
    Delay(10);                      //延时
    for (i=0; i<512; i++)           //校验512字节
    {
        if (IapReadByte(IAP_ADDRESS+i) != (BYTE)i)
            goto Error;             //如果校验错误,则退出
    }
    P1 = 0xf0;                      //1111,0000 测试完成
    while (1);
Error:
    P1 &= 0x7f;                     //0xxx,xxxx IAP操作失败
    while (1);
}

/*----------------------------
软件延时
----------------------------*/
void Delay(BYTE n)
{
    WORD x;

    while (n--)
    {
        x = 0;
        while (++x);
    }
}

/*----------------------------
关闭IAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //关闭IAP功能
    IAP_CMD = 0;                    //清除命令寄存器
    IAP_TRIG = 0;                   //清除触发寄存器
    IAP_ADDRH = 0x80;               //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

/*----------------------------
从ISP/IAP/EEPROM区域读取一字节
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //数据缓冲区

    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
    IapIdle();                      //关闭IAP功能

    return dat;                     //返回
}

/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

/*----------------------------
扇区擦除
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    IapIdle();
}

