/* ------------------------- 宏晶科技   04/15/2009 -------------------- */ 
/* --------------------Mobile:0755-82948412,13922805190---------------- */ 

//  本程序演示 STC12C5AxxAD 系列 MCU 的 A/D 转换功能。时钟 18.432MHz
//  转换结果以 16 进制形式输出到串行口，可以用串行口调试程序观察输出结果。
//---------------------------------------------------------------------
//#include <REG52.H>
#include <intrins.H>
#include "NEW_8051.H"
//---------------------------------------------------------------------
typedef    unsigned char   INT8U;
typedef    unsigned int    INT16U;
//---------------------------------------------------------------------
//以下选择 ADC 转换速率，只能选择其中一种
                        //            SPEED1 SPEED0  A/D转换所需时间
#define AD_SPEED   0x60 //  0110,0000   1      1   70 个时钟周期转换一次, 
                        //  CPU工作频率21MHz时 A/D转换速度约 300KHz
//#define AD_SPEED   0x40 //0100,0000   1      0     140 个时钟周期转换一次
//#define AD_SPEED   0x20 //0010,0000   0      1     280 个时钟周期转换一次
//#define AD_SPEED   0x00 //0000,0000   0      0     420 个时钟周期转换一次
//---------------------------------------------------------------------
void initiate_RS232 (void);         //串口初始化
void Send_Byte(INT8U one_byte);     //发送一个字节
INT8U get_AD_result(INT8U channel);
void delay(INT8U delay_time);       //延时函数
//---------------------------------------------------------------------
void main()
{
    INT16U ADC_result;

    initiate_RS232();               //波特率 = 115200
    P1ASF = 0x02;                   //0000,0010, 将 P1.1 置成模拟口

//ADRJ = AUXR1^2:
//    0: 10 位A/D 转换结果的高8 位放在ADC_RES 寄存器, 低2 位放在ADC_RESL 寄存器
//    1: 10 位A/D 转换结果的最高2 位放在ADC_RES 寄存器的低2 位, 低8 位放在ADC_RESL 寄存器
    AUXR1 &= ~0x04;                 //0000,0100, 令 ADRJ=0
//    AUXR1 |= 0x04;                  //0000,0100, 令 ADRJ=1

    ADC_CONTR |= 0x80;              //1000,0000 打开 A/D 转换电源
    while(1)
    {
        ADC_result = get_AD_result(1);//P1.1 为 A/D 当前通道, 测量并发送结果
        Send_Byte(0xAA);            //为便于观察, 发送 2 个 0xAA
        Send_Byte(0xAA);
        Send_Byte(ADC_result);
        delay(0x1);
    }
}
//---------------------------------------------------------------------
void delay(INT8U delay_time)        // 延时函数
{
    INT16U n;
    while(delay_time--) 
    { 
        n = 6000;
        while(--n);
    }
}
//---------------------------------------------------------------------
#define Fosc 18432000
#define BAUD 115200                 //波特率
#define RELOAD_115200 (256 - (Fosc/16*10/BAUD+5)/10 )   //1T模式, 波特率加倍
#define BRTx12_enable() AUXR |= 0x04   //BRT 独立波特率发生器的溢出率快 12 倍
#define BRT_start()     AUXR |= 0x10   //启动独立波特率发生器 BRT 计数。

void initiate_RS232 (void)          //串口初始化
{
    ES = 0;                         //禁止串口中断
    SCON = 0x50;                    //可变波特率. 8位无奇偶校验

    AUXR |= 0x01;                   //使用独立波特率发生器
    PCON |= 0x80;                   //波特率加倍
    BRTx12_enable();                //BRT 独立波特率发生器的溢出率快 12 倍
    BRT = RELOAD_115200;            //设置独立波特率发生器 BRT 的自动重装数 
    BRT_start();                    //启动独立波特率发生器 BRT 计数。

    ES = 1;
}
//---------------------------------------------------------------------
void Send_Byte(INT8U one_byte)      //发送一个字节
{
    TI = 0;                         //清零串口发送中断标志
    SBUF = one_byte;
    while (TI == 0);
    TI = 0;                         //清零串口发送中断标志
}
//---------------------------------------------------------------------
INT8U get_AD_result(INT8U channel)
{
    INT8U AD_finished=0;            //存储 A/D 转换标志

    ADC_RES = 0;
    ADC_RESL = 0;

    channel &= 0x07;                //0000,0111 清0高5位
    ADC_CONTR = AD_SPEED;
    _nop_();
    ADC_CONTR |= channel;           //选择 A/D 当前通道
    _nop_();
    ADC_CONTR |= 0x80;              //启动 A/D 电源
    delay(1);                       //使输入电压达到稳定
    ADC_CONTR |= 0x08;              //0000,1000 令 ADCS = 1, 启动A/D转换, 
    AD_finished = 0;
    while (AD_finished ==0 )        //等待A/D转换结束
    {
        AD_finished = (ADC_CONTR & 0x10); //0001,0000 测试A/D转换结束否
    }
    ADC_CONTR &= 0xE7;              //1111,0111 清 ADC_FLAG 位, 关闭A/D转换, 

    return (ADC_RES);               //返回 A/D 高 8 位转换结果
}




