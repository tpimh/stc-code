/*
  --- STC International Limited ---------------- 
一个完整的EEPROM 测试程序，用宏晶的下载板可以直接测试

STC12C5AxxAD 系列单片机 EEPROM/IAP 功能测试程序演示
STC12C52xxAD 系列单片机 EEPROM/IAP 功能测试程序演示
STC11xx 系列单片机 EEPROM/IAP 功能测试程序演示
STC10xx 系列单片机 EEPROM/IAP 功能测试程序演示
 --- STC International Limited ------------------
 --- 宏晶科技  设计 2009/1/12 V1.0 --------------
 --- Mobile: 13922805190 ------------------------
 --- Fax: 0755-82944243 -------------------------
 --- Tel: 0755-82948412 -------------------------
 --- Web: www.MCU-Memory.com --------------------
本演示程序在STC-ISP Ver 3.0A.PCB 的下载编程工具上测试通过,EEPROM 的数据
在P1 口上显示, 如果要在程序中使用或在文章中引用该程序,请在程序中或文章中
注明使用了宏晶科技的资料及程序
*/

#include <reg51.H>
#include <intrins.H>

typedef unsigned char  INT8U;
typedef unsigned int   INT16U;

sfr IAP_DATA    = 0xC2;
sfr IAP_ADDRH   = 0xC3;
sfr IAP_ADDRL   = 0xC4;
sfr IAP_CMD     = 0xC5;
sfr IAP_TRIG    = 0xC6;
sfr IAP_CONTR   = 0xC7;

//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

#define DEBUG_DATA               0x5A  //本测试程序最终存储在 EEPROM 单元的数值
#define DATA_FLASH_START_ADDRESS 0x00  //STC5Axx 系列 EEPROM 测试起始地址

union union_temp16
{
    INT16U un_temp16;
    INT8U  un_temp8[2];
}my_unTemp16;

INT8U Byte_Read(INT16U add);              //读一字节，调用前需打开IAP 功能
void Byte_Program(INT16U add, INT8U ch);  //字节编程，调用前需打开IAP 功能
void Sector_Erase(INT16U add);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能
void Delay();

void main (void)
{
    INT16U eeprom_address;
    INT8U  read_eeprom;

    P1 = 0xF0;                            //演示程序开始，让 P1[3:0] 控制的灯亮
    Delay();                              //延时
    P1 = 0x0F;                            //演示程序开始，让 P1[7:4] 控制的灯亮
    Delay()    ;                          //延时

    //将EEPROM 测试起始地址单元的内容读出
    eeprom_address = DATA_FLASH_START_ADDRESS;  //将测试起始地址送eeprom_address
    read_eeprom = Byte_Read(eeprom_address);    //读EEPROM的值,存到read_eeprom

    if (DEBUG_DATA == read_eeprom)
    {   //数据是对的，亮  P1.7 控制的灯，然后在 P1 口上将 EEPROM 的数据显示出来
        P1 = ~0x80;
        Delay()    ;                            //延时
        P1 = ~read_eeprom;
    }
    else
    {   //数据是错的，亮 P1.3 控制的灯，然后在 P1 口上将 EEPROM 的数据显示出来
        //再将该EEPROM所在的扇区整个擦除，将正确的数据写入后，亮 P1.5 控制的灯
        P1 = ~0x08;
        Delay()    ;                            //延时
        P1 = ~read_eeprom;
        Delay()    ;                            //延时

        Sector_Erase(eeprom_address);           //擦除整个扇区
        Byte_Program(eeprom_address, DEBUG_DATA);//将 DEBUG_DATA 写入 EEPROM

        P1 = ~0x20;                 //熄灭 P1.3 控制的灯，亮 P1.5 控制的灯
    }

    while (1);                      //CPU 在此无限循环执行此句
}

//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
INT8U Byte_Read(INT16U add)
{
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (IAP_DATA);
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(INT16U add, INT8U ch)
{
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    IAP_DATA = ch;                  //要编程的数据先送进IAP_DATA 寄存器
    //EA = 0;
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(INT16U add)
{
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}

void Delay()
{
    INT8U i;
    INT16U d=5000;
    while (d--)
    {
        i=255;
        while (i--);
    }
}





