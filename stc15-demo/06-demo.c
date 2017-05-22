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

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;

#define ERROR   0
#define OK      1

//-----------------------------------------------

sfr IAP_DATA    =   0xC2;           //IAP数据寄存器
sfr IAP_ADDRH   =   0xC3;           //IAP地址寄存器高字节
sfr IAP_ADDRL   =   0xC4;           //IAP地址寄存器低字节
sfr IAP_CMD     =   0xC5;           //IAP命令寄存器
sfr IAP_TRIG    =   0xC6;           //IAP命令触发寄存器
sfr IAP_CONTR   =   0xC7;           //IAP控制寄存器

sbit Begin_LED  =   P1^0;
sbit ERROR_LED  =   P1^3;
sbit OK_LED     =   P1^7;

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令

#define     URMD    0               //0:使用定时器2作为波特率发生器
                                    //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                    //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器

sfr T2H   = 0xd6;                   //定时器2高8位
sfr T2L   = 0xd7;                   //定时器2低8位

sfr  AUXR       =   0x8e;           //辅助寄存器                              

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

void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
BYTE sequential_write_flash_in_one_sector(WORD begin_addr, WORD counter, BYTE array[]);
BYTE write_flash_with_protect_in_one_sector(WORD begin_addr, WORD counter, BYTE array[]);
void InitUart();
BYTE SendData(BYTE dat);

#define display_Begin_LED() Begin_LED = 0
#define off_Begin_LED()     Begin_LED = 1
#define display_OK_LED()    OK_LED = 0
#define off_OK_LED()        OK_LED = 1
#define display_ERROR_LED() ERROR_LED = 0
#define off_ERROR_LED()     ERROR_LED = 1

//#define USED_BYTE_QTY_IN_ONE_SECTOR   1
//#define USED_BYTE_QTY_IN_ONE_SECTOR   2
//#define USED_BYTE_QTY_IN_ONE_SECTOR   4
//#define USED_BYTE_QTY_IN_ONE_SECTOR   8
//#define USED_BYTE_QTY_IN_ONE_SECTOR   16
//#define USED_BYTE_QTY_IN_ONE_SECTOR   32
//#define USED_BYTE_QTY_IN_ONE_SECTOR   64
#define USED_BYTE_QTY_IN_ONE_SECTOR     128
//#define USED_BYTE_QTY_IN_ONE_SECTOR   256
//#define USED_BYTE_QTY_IN_ONE_SECTOR   512

#define DEBUG_Data_Memory_Begin_Sector_addr     0x0400

BYTE xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];

/* 测试常量数组 */
BYTE code Test_array_total[512] = 
{
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
    0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
    0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
    0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
    0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
    0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0,
    0xef,0xee,0xed,0xec,0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,
    0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,
    0xcf,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,0xc0,
    0xbf,0xbe,0xbd,0xbc,0xbb,0xba,0xb9,0xb8,0xb7,0xb6,0xb5,0xb4,0xb3,0xb2,0xb1,0xb0,
    0xaf,0xae,0xad,0xac,0xab,0xaa,0xa9,0xa8,0xa7,0xa6,0xa5,0xa4,0xa3,0xa2,0xa1,0xa0,
    0x9f,0x9e,0x9d,0x9c,0x9b,0x9a,0x99,0x98,0x97,0x96,0x95,0x94,0x93,0x92,0x91,0x90,
    0x8f,0x8e,0x8d,0x8c,0x8b,0x8a,0x89,0x88,0x87,0x86,0x85,0x84,0x83,0x82,0x81,0x80,
    0x7f,0x7e,0x7d,0x7c,0x7b,0x7a,0x79,0x78,0x77,0x76,0x75,0x74,0x73,0x72,0x71,0x70,
    0x6f,0x6e,0x6d,0x6c,0x6b,0x6a,0x69,0x68,0x67,0x66,0x65,0x64,0x63,0x62,0x61,0x60,
    0x5f,0x5e,0x5d,0x5c,0x5b,0x5a,0x59,0x58,0x57,0x56,0x55,0x54,0x53,0x52,0x51,0x50,
    0x4f,0x4e,0x4d,0x4c,0x4b,0x4a,0x49,0x48,0x47,0x46,0x45,0x44,0x43,0x42,0x41,0x40,
    0x3f,0x3e,0x3d,0x3c,0x3b,0x3a,0x39,0x38,0x33,0x36,0x35,0x34,0x33,0x32,0x31,0x30,
    0x2f,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,
    0x1f,0x1e,0x1d,0x1c,0x1b,0x1a,0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,
    0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00
};

void main()
{
    InitUart();
  
//  if(sequential_write_flash_in_one_sector(DEBUG_Data_Memory_Begin_Sector_addr, USED_BYTE_QTY_IN_ONE_SECTOR, Test_array_total))
//      display_OK_LED();
//  else
//      display_ERROR_LED();

    if(write_flash_with_protect_in_one_sector(DEBUG_Data_Memory_Begin_Sector_addr+0x3, USED_BYTE_QTY_IN_ONE_SECTOR-0x30, Test_array_total))
        display_OK_LED();
    else
        display_ERROR_LED();

    while(1);
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

/* 写数据进 数据Flash存储器, 只在同一个扇区内写，不保留原有数据 */
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源   */
BYTE sequential_write_flash_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;

    /* 判是否是有效范围,此函数不允许跨扇区操作 */
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return  ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* 擦除 要修改/写入 的扇区 */
    IapEraseSector(begin_addr);

    for(i=0; i<counter; i++)
    {
        /* 写一个字节 */
        IapProgramByte(begin_addr, array[i]);
        /*  比较对错 */
        if (SendData(IapReadByte(begin_addr)) != array[i])
        {
            IapIdle();
            return ERROR;
        }
        begin_addr++;
    }
    IapIdle();
    return  OK;
}

/* 写数据进数据Flash存储器(EEPROM), 只在同一个扇区内写，保留同一扇区中不需修改的数据    */
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源       */
BYTE write_flash_with_protect_in_one_sector(WORD begin_addr, WORD counter, BYTE array[])
{
    WORD i = 0;
    WORD in_sector_begin_addr = 0;
    WORD sector_addr = 0;
    WORD byte_addr = 0;

    /* 判是否是有效范围,此函数不允许跨扇区操作 */
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    /* 假定从扇区的第0个字节开始，到USED_BYTE_QTY_IN_ONE_SECTOR-1个字节结束,后面部分不用,程序易编写 */
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

    /* 将该扇区数据 0 - (USED_BYTE_QTY_IN_ONE_SECTOR-1) 字节数据读入缓冲区保护 */
    sector_addr = (begin_addr & 0xfe00);
    byte_addr = sector_addr;

    for(i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
    {
        protect_buffer[i] = IapReadByte(byte_addr++);
    }

    /* 将要写入的数据写入保护缓冲区的相应区域,其余部分保留 */
    for(i = 0; i < counter; i++)
    {
        protect_buffer[in_sector_begin_addr++] = array[i];
    }

    /* 擦除 要修改/写入 的扇区 */
    IapEraseSector(sector_addr);

    /* 将保护缓冲区的数据写入 Data Flash, EEPROM */
    byte_addr = sector_addr;
    for(i = 0; i< USED_BYTE_QTY_IN_ONE_SECTOR; i++)
    {
        /* 写一个字节 */
        IapProgramByte(byte_addr, protect_buffer[i]);
        /*  比较对错 */
        if (SendData(IapReadByte(begin_addr)) != protect_buffer[i])
        {
            IapIdle();
            return ERROR;
        }
        byte_addr++;
    }
    IapIdle();
    return OK;
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
BYTE SendData(BYTE dat)
{
    while (!TI);                 //等待前一个数据发送完成
    TI = 0;                      //清除发送标志
    SBUF = dat;                  //发送当前数据
    
    return dat;
}

