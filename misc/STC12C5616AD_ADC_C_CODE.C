/* ------------------------- 宏晶科技   05/02/2009 -------------------- */ 
/* ----------------------0755-82948412-------------------------------- */ 

//  ADC DEMO_56xx 程序演示 STC1256xxAD 系列 MCU 的 A/D 转换功能。时钟 18.432MHz
//  转换结果以 16 进制形式输出到串行口，可以用串行口调试程序观察输出结果。
//---------------------------------------------------------------------
#include "STC12C5620AD.H"
//---------------------------------------------------------------------
typedef    unsigned char   INT8U;
typedef    unsigned int    INT16U;
//---------------------------------------------------------------------
//以下选择 ADC 转换速率，只能选择其中一种
                        //          SPEED1 SPEED0  A/D转换所需时间
#define AD_SPEED   0x60 //0110,0000   1      1     270个时钟周期转换一次，
                        //                         CPU工作频率27MHz时，
                        //                         A/D转换速度约100KHz
//#define AD_SPEED   0x40 //0100,0000   1      0     540个时钟周期转换一次
//#define AD_SPEED   0x20 //0010,0000   0      1     810个时钟周期转换一次
//#define AD_SPEED   0x00 //0000,0000   0      0     1080个时钟周期转换一次
//---------------------------------------------------------------------
void initiate_RS232 (void);         // 串口初始化
void Send_Byte(INT8U one_byte);     // 发送一个字节
INT16U get_AD_result(INT8U channel);
void delay(INT8U delay_time);       // 延时函数
//---------------------------------------------------------------------
void main()
{
    INT16U ADC_10bit;

    initiate_RS232();               //波特率 = 9600

    ADC_CONTR |= 0x80;              //1000,0000 打开 A/D 转换电源
    while(1)
    {
        Send_Byte(0xAA);
        Send_Byte(0xAA);

        ADC_10bit = get_AD_result(2);//P1.2 为 A/D 当前通道
        Send_Byte(ADC_10bit >> 8);  //发送高 8 位测量结果
        delay(0x1);

        ADC_10bit = get_AD_result(3);//P1.3 为 A/D 当前通道, 测量并发送结果
        Send_Byte(ADC_10bit >> 8);  //发送高 8 位测量结果
        
        delay(0x30);                //延时
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
void initiate_RS232 (void)          // 串口初始化
{
    ES = 0;                         // 禁止串口中断
    TMOD = 0x20;                    // 设置 T1 为波特率发生器
    SCON = 0x50;                    // 0101,0000 8位数据位, 无奇偶校验
    TH1 = 0xFB;                     // 18.432MHz 晶振, 波特率 = 9600
    TL1 = 0xFB;
    RI = 0;
    TR1 = 1;
    ES = 1;                         // 允许串口中断
}
//---------------------------------------------------------------------
void Send_Byte(INT8U one_byte)      // 发送一个字节
{
    TI = 0;                         // 清零串口发送中断标志
    SBUF = one_byte;
    while (!TI);
    TI = 0;                         // 清零串口发送中断标志
}
//---------------------------------------------------------------------
INT16U get_AD_result(INT8U channel)
{
    ADC_DATA   = 0;

    channel &= 0x07;                //0000,0111 清0高5位
    ADC_CONTR = AD_SPEED;
    ADC_CONTR = 0xE0;               //1110,0000 清 ADC_FLAG, ADC_START 位和低 3 位
    ADC_CONTR |= channel;           //选择 A/D 当前通道
    delay(1);                       //使输入电压达到稳定
    ADC_CONTR |= 0x08;              //0000,1000 令 ADCS = 1, 启动A/D转换, 
    while (1)                       //等待A/D转换结束
    {
        if (ADC_CONTR & 0x10)       //0001,0000 测试A/D转换结束否
        { break; }
    }
    ADC_CONTR &= 0xE7;              //1111,0111 清 ADC_FLAG 位, 关闭A/D转换, 

    return (ADC_DATA<<8 | ADC_LOW2); //返回 A/D 10 位转换结果
}




