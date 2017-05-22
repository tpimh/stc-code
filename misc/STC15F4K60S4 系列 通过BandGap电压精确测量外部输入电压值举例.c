/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 通过BandGap电压精确测量外部输入电压值举例-----*/
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

//说明:
//    ADC的第9通道是用来测试内部BandGap参考电压的,由于内部BandGap参考电
//压很稳定,不会随芯片的工作电压的改变而变化,所以可以通过两次测量和一次计算
//便可得到外部的精确电压.公式如下:
//ADCbg / Vbg = 1023 / VCC
//ADCx / Vx   = 1023 / VCC
//由于两次测量的时间间隔很短,VCC的电压在此期间的波动可忽略不计
//从而可推出 ADCbg / Vbg = ADCx / Vx
//进一步得出 Vx = Vbg * ADCx / ADCbg
//其中:ADCbg为Bandgap电压的ADC测量值
//     Vbg为实际Bandgap的电压值,在单片机进行CP测试时记录的参数,单位为毫伏(mV)
//     ADCx为外部输入电压的ADC测量值
//     Vx外部输入电压的实际电压值,单位为毫伏(mV)
//
//具体的测试方法:首先将P1ASF初始化为0,即关闭所有P1口的模拟功能
//然后通过正常的ADC转换的方法读取第0通道的值,即可通过ADC的第9通道读取当前
//内部BandGap参考电压值ADCbg,然后测量有外部电压输入的ADC通道,测量出
//外部输入电压的ADC测量值ADCx,接下来从RAM区或者ROM区读取实际Bandgap的电压值Vbg,
//最后通过公式Vx = Vbg * ADCx / ADCbg,即可计算出外部输入电压的实际电压值Vx

//-----------------------------------------

WORD idata Vbg_RAM _at_ 0xef;               //对于只有256字节RAM的MCU存放地址为0EFH
//WORD idata Vbg_RAM _at_ 0x6f;             //对于只有128字节RAM的MCU存放地址为06FH

//注意:需要在下载代码时选择"在ID号前添加重要测试参数"选项,才可在程序中获取此参数
//WORD code Vbg_ROM _at_ 0x03f7;            //1K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x07f7;            //2K程序空间的MCU 
//WORD code Vbg_ROM _at_ 0x0bf7;            //3K程序空间的MCU 
//WORD code Vbg_ROM _at_ 0x0ff7;            //4K程序空间的MCU 
//WORD code Vbg_ROM _at_ 0x13f7;            //5K程序空间的MCU 
//WORD code Vbg_ROM _at_ 0x1ff7;            //8K程序空间的MCU 
//WORD code Vbg_ROM _at_ 0x27f7;            //10K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x2ff7;            //12K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x3ff7;            //16K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x4ff7;            //20K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x5ff7;            //24K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x6ff7;            //28K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x7ff7;            //32K程序空间的MCU
//WORD code Vbg_ROM _at_ 0x9ff7;            //40K程序空间的MCU
//WORD code Vbg_ROM _at_ 0xbff7;            //48K程序空间的MCU
//WORD code Vbg_ROM _at_ 0xcff7;            //52K程序空间的MCU
//WORD code Vbg_ROM _at_ 0xdff7;            //56K程序空间的MCU
WORD code Vbg_ROM _at_ 0xeff7;              //60K程序空间的MCU

//-----------------------------------------
                                            
sfr ADC_CONTR   =   0xBC;                   //ADC控制寄存器
sfr ADC_RES     =   0xBD;                   //ADC高8位结果
sfr ADC_LOW2    =   0xBE;                   //ADC低2位结果
sfr P1ASF       =   0x9D;                   //P1口第2功能控制寄存器
                                            
#define ADC_POWER   0x80                    //ADC电源控制位
#define ADC_FLAG    0x10                    //ADC完成标志
#define ADC_START   0x08                    //ADC起始控制位
#define ADC_SPEEDLL 0x00                    //540个时钟
#define ADC_SPEEDL  0x20                    //360个时钟
#define ADC_SPEEDH  0x40                    //180个时钟
#define ADC_SPEEDHH 0x60                    //90个时钟

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

void main()
{
    BYTE ADCbg;
    BYTE ADCx;
    WORD Vx;
    
//第一步:通过ADC的第9通道测试Bandgap电压的ADC测量值
    ADC_RES = 0;                            //清除结果寄存器
    P1ASF = 0x00;                           //不设置P1ASF,即可从ADC的第9通道读取内部Bandgap电压的ADC测量值
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                               //ADC上电并延时
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 0 | ADC_START;
    _nop_();                                //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));        //等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;                 //清除ADC标志
    ADCbg = ADC_RES;
    
//第二步:通过ADC的第2通道测试外部输入电压的ADC测量值
    ADC_RES = 0;                            //清除结果寄存器
    P1ASF = 0x02;                           //设置P1.1口为模拟通道
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                               //ADC上电并延时
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 1 | ADC_START;
    _nop_();                                //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));        //等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;                 //清除ADC标志
    ADCx = ADC_RES;

//第三步:通过公式计算外部输入的实际电压值
    Vx = Vbg_RAM * ADCx / ADCbg;            //使用RAM中的Bandgap电压参数进行计算
    //Vx = Vbg_ROM * ADCx / ADCbg;          //使用ROM中的Bandgap电压参数进行计算
    
    while (1);
}

