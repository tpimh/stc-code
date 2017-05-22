
#ifndef __STC8F_H_
#define __STC8F_H_

/////////////////////////////////////////////////

//包含本头文件后,不用另外再包含"REG51.H"
//STC8系列单片机：-1T8051/8K RAM/64K ROM/12位ADC/8组PWM/I2C/SPI/4串口
//详细数据手册等资料请至STC官网:www.STCMCU.com、www.GXWMCU.com下载.
//本STC8系列单片机头文件从STC-ISP V6.85o导出，
//由第三方STCISP.COM根据STC8-160506数据手册整理@2016.07.12;13

//内核特殊功能寄存器
sfr ACC         =   0xe0;                                       //累加器
sfr B           =   0xf0;                                       //B寄存器
sfr PSW         =   0xd0;                                       //程序状态字寄存器
sbit CY         =   PSW^7;
sbit AC         =   PSW^6;
sbit F0         =   PSW^5;
sbit RS1        =   PSW^4;                                      //-工作寄存器选择位1
sbit RS0        =   PSW^3;                                      //-工作寄存器选择位0
sbit OV         =   PSW^2;
sbit P          =   PSW^0;
sfr SP          =   0x81;                                       //堆栈指针
sfr DPL         =   0x82;                                       //数据指针(低字节)
sfr DPH         =   0x83;                                       //数据指针(高字节)
sfr TA          =   0xae;                                       //DPTR时序控制寄存器
sfr DPS         =   0xe3;                                       //DPTR指针选择器
sfr DPL1        =   0xe4;                                       //第二组数据指针(低字节)
sfr DPH1        =   0xe5;                                       //第二组数据指针(高字节)


//I/O 口特殊功能寄存器
sfr P0          =   0x80;                                       //P0端口
sfr P1          =   0x90;                                       //P1端口
sfr P2          =   0xa0;                                       //P2端口
sfr P3          =   0xb0;                                       //P3端口
sfr P4          =   0xc0;                                       //P4端口
sfr P5          =   0xc8;                                       //P5端口
sfr P6          =   0xe8;                                       //P6端口
sfr P7          =   0xf8;                                       //P7端口
sfr P0M0        =   0x94;                                       //P0口配置寄存器0
sfr P0M1        =   0x93;                                       //P0口配置寄存器1
sfr P1M0        =   0x92;                                       //P1口配置寄存器0
sfr P1M1        =   0x91;                                       //P1口配置寄存器1
sfr P2M0        =   0x96;                                       //P2口配置寄存器0
sfr P2M1        =   0x95;                                       //P2口配置寄存器1
sfr P3M0        =   0xb2;                                       //P3口配置寄存器0
sfr P3M1        =   0xb1;                                       //P3口配置寄存器1
sfr P4M0        =   0xb4;                                       //P4口配置寄存器0
sfr P4M1        =   0xb3;                                       //P4口配置寄存器1
sfr P5M0        =   0xca;                                       //P5口配置寄存器0
sfr P5M1        =   0xc9;                                       //P5口配置寄存器1
sfr P6M0        =   0xcc;                                       //P6口配置寄存器0
sfr P6M1        =   0xcb;                                       //P6口配置寄存器1
sfr P7M0        =   0xe2;                                       //P7口配置寄存器0
sfr P7M1        =   0xe1;                                       //P7口配置寄存器1

sbit P00        =   P0^0;
sbit P01        =   P0^1;
sbit P02        =   P0^2;
sbit P03        =   P0^3;
sbit P04        =   P0^4;
sbit P05        =   P0^5;
sbit P06        =   P0^6;
sbit P07        =   P0^7;
sbit P10        =   P1^0;
sbit P11        =   P1^1;
sbit P12        =   P1^2;
sbit P13        =   P1^3;
sbit P14        =   P1^4;
sbit P15        =   P1^5;
sbit P16        =   P1^6;
sbit P17        =   P1^7;
sbit P20        =   P2^0;
sbit P21        =   P2^1;
sbit P22        =   P2^2;
sbit P23        =   P2^3;
sbit P24        =   P2^4;
sbit P25        =   P2^5;
sbit P26        =   P2^6;
sbit P27        =   P2^7;
sbit P30        =   P3^0;
sbit P31        =   P3^1;
sbit P32        =   P3^2;
sbit P33        =   P3^3;
sbit P34        =   P3^4;
sbit P35        =   P3^5;
sbit P36        =   P3^6;
sbit P37        =   P3^7;
sbit P40        =   P4^0;
sbit P41        =   P4^1;
sbit P42        =   P4^2;
sbit P43        =   P4^3;
sbit P44        =   P4^4;
sbit P45        =   P4^5;
sbit P46        =   P4^6;
sbit P47        =   P4^7;
sbit P50        =   P5^0;
sbit P51        =   P5^1;
sbit P52        =   P5^2;
sbit P53        =   P5^3;
sbit P54        =   P5^4;
sbit P55        =   P5^5;
sbit P56        =   P5^6;
sbit P57        =   P5^7;
sbit P60        =   P6^0;
sbit P61        =   P6^1;
sbit P62        =   P6^2;
sbit P63        =   P6^3;
sbit P64        =   P6^4;
sbit P65        =   P6^5;
sbit P66        =   P6^6;
sbit P67        =   P6^7;
sbit P70        =   P7^0;
sbit P71        =   P7^1;
sbit P72        =   P7^2;
sbit P73        =   P7^3;
sbit P74        =   P7^4;
sbit P75        =   P7^5;
sbit P76        =   P7^6;
sbit P77        =   P7^7;

//如下特殊功能寄存器位于扩展RAM区域
//访问这些寄存器,需先将P_SW2的BIT7设置为1,才可正常读写
#define P0PU        (*(unsigned char volatile xdata *)0xfe10)   //P0口上拉电阻控制寄存器
#define P1PU        (*(unsigned char volatile xdata *)0xfe11)   //P1口上拉电阻控制寄存器
#define P2PU        (*(unsigned char volatile xdata *)0xfe12)   //P2口上拉电阻控制寄存器
#define P3PU        (*(unsigned char volatile xdata *)0xfe13)   //P3口上拉电阻控制寄存器
#define P4PU        (*(unsigned char volatile xdata *)0xfe14)   //P4口上拉电阻控制寄存器
#define P5PU        (*(unsigned char volatile xdata *)0xfe15)   //P5口上拉电阻控制寄存器
#define P6PU        (*(unsigned char volatile xdata *)0xfe16)   //P6口上拉电阻控制寄存器
#define P7PU        (*(unsigned char volatile xdata *)0xfe17)   //P7口上拉电阻控制寄存器
#define P0NCS       (*(unsigned char volatile xdata *)0xfe18)   //P0口施密特触发控制寄存器
#define P1NCS       (*(unsigned char volatile xdata *)0xfe19)   //P1口施密特触发控制寄存器
#define P2NCS       (*(unsigned char volatile xdata *)0xfe1a)   //P2口施密特触发控制寄存器
#define P3NCS       (*(unsigned char volatile xdata *)0xfe1b)   //P3口施密特触发控制寄存器
#define P4NCS       (*(unsigned char volatile xdata *)0xfe1c)   //P4口施密特触发控制寄存器
#define P5NCS       (*(unsigned char volatile xdata *)0xfe1d)   //P5口施密特触发控制寄存器
#define P6NCS       (*(unsigned char volatile xdata *)0xfe1e)   //P6口施密特触发控制寄存器
#define P7NCS       (*(unsigned char volatile xdata *)0xfe1f)   //P7口施密特触发控制寄存器

//系统管理特殊功能寄存器
sfr PCON        =   0x87;                                       //电源控制寄存器
#define SMOD        0x80                                        //-串口1波特率控制位
#define SMOD0       0x40                                        //-帧错误检测控制位
#define LVDF        0x20                                        //-低压检测标志位
#define POF         0x10                                        //-上电标志位
#define GF1         0x08
#define GF0         0x04
#define PD          0x02                                        //-掉电模式控制位
#define IDL         0x01                                        //-IDLE（空闲）模式控制位
sfr AUXR        =   0x8e;                                       //辅助寄存器
#define T0x12       0x80                                        //-定时器0速度控制位
#define T1x12       0x40                                        //定时器1速度控制位
#define UART_M0x6   0x20                                        //-串口1模式0的通讯速度控制位
#define T2R         0x10                                        //-定时器2的运行控制位
#define T2_CT       0x08                                        //-定时器0用作定时器或计数器控制位
#define T2x12       0x04                                        //-定时器2速度控制位
#define EXTRAM      0x02                                        //-扩展RAM访问控制位
#define S1ST2       0x01                                        //-串口1波特率发射器选择位
sfr AUXR2       =   0x97;                                       //辅助寄存器2
#define TXLNRX      0x10                                        //-串口1中继广播方式控制位
sfr BUS_SPEED   =   0xa1;                                       //总线速度控制寄存器
sfr P_SW1       =   0xa2;                                       //外设端口切换寄存器1
sfr P_SW2       =   0xba;                                       //外设端口切换寄存器2
#define EAXFR       0x80
sfr VOCTRL      =   0xbb;                                       //电压控制寄存器
sfr RSTCFG      =   0xff;                                       //复位配置寄存器

//如下特殊功能寄存器位于扩展RAM区域
//访问这些寄存器,需先将P_SW2的BIT7设置为1,才可正常读写
#define CKSEL       (*(unsigned char volatile xdata *)0xfe00)   //时钟选择寄存器
#define CLKDIV      (*(unsigned char volatile xdata *)0xfe01)   //时钟分频寄存器
#define IRC24MCR    (*(unsigned char volatile xdata *)0xfe02)   //内部24M振荡器控制寄存器
#define XOSCCR      (*(unsigned char volatile xdata *)0xfe03)   //外部晶振控制寄存器
#define IRC32KCR    (*(unsigned char volatile xdata *)0xfe04)   //内部32K振荡器控制寄存器

//中断特殊功能寄存器
sfr IE          =   0xa8;                                       //中断允许寄存器
sbit EA         =   IE^7;                                       //-总中断允许控制位
sbit ELVD       =   IE^6;                                       //-低压检测中断允许位
sbit EADC       =   IE^5;                                       //-A/D转换中断允许位
sbit ES         =   IE^4;                                       //-串行口1中断允许位
sbit ET1        =   IE^3;                                       //-定时/计数器T1的溢出中断允许位
sbit EX1        =   IE^2;                                       //-外部中断1中断允许位
sbit ET0        =   IE^1;                                       //-定时/计数器T0的溢出中断允许位
sbit EX0        =   IE^0;                                       //-外部中断0中断允许位
sfr IE2         =   0xaf;                                       //中断允许寄存器2
#define ET4         0x40                                        //-定时/计数器T4的溢出中断允许位
#define ET3         0x20                                        //-定时/计数器T3的溢出中断允许位
#define ES4         0x10                                        //-串行口4中断允许位
#define ES3         0x08                                        //-串行口3中断允许位
#define ET2         0x04                                        //定时/计数器T2的溢出中断允许位
#define ESPI        0x02                                        //SPI中断允许位
#define ES2         0x01                                        //串行口2中断允许位
sfr IP          =   0xb8;                                       //中断优先级控制寄存器
sbit PPCA       =   IP^7;                                       //-CCP/PCA中断优先级控制位
sbit PLVD       =   IP^6;                                       //-低压检测中断优先级控制位
sbit PADC       =   IP^5;                                       //-ADC中断优先级控制位
sbit PS         =   IP^4;                                       //-串口1中断优先级控制位
sbit PT1        =   IP^3;                                       //-定时器1中断优先级控制位
sbit PX1        =   IP^2;                                       //-外部中断1中断优先级控制位
sbit PT0        =   IP^1;                                       //-定时器0中断优先级控制位
sbit PX0        =   IP^0;                                       //-外部中断0中断优先级控制位
sfr IP2         =   0xb5;                                       //中断优先级控制寄存器2
#define PI2C        0x40                                        //-I2C中断优先级控制位
#define PCMP        0x20                                        //-比较器中断优先级控制位
#define PX4         0x10                                        //-外部中断4中断优先级控制位
#define PPWMFD      0x08                                        //-强型PWM异常检测中断优先级控制位
#define PPWM        0x04                                        //-增强型PWM中断优先级控制位
#define PSPI        0x02                                        //-SPI中断优先级控制位
#define PS2         0x01                                        //-串口2中断优先级控制位
sfr IPH         =   0xb7;                                       //高中断优先级控制寄存器
#define PPCAH       0x80                                        //-CCP/PCA中断优先级控制位
#define PLVDH       0x40                                        //-低压检测中断优先级控制位
#define PADCH       0x20                                        //-ADC中断优先级控制位
#define PSH         0x10                                        //-串口1中断优先级控制位
#define PT1H        0x08                                        //-定时器1中断优先级控制位
#define PX1H        0x04                                        //-外部中断1中断优先级控制位
#define PT0H        0x02                                        //-定时器0中断优先级控制位
#define PX0H        0x01                                        //-外部中断0中断优先级控制位
sfr IP2H        =   0xb6;                                       //高中断优先级控制寄存器2
#define PI2CH       0x40                                        //-I2C中断优先级控制位
#define PCMPH       0x20                                        //-比较器中断优先级控制位
#define PX4H        0x10                                        //-外部中断4中断优先级控制位
#define PPWMFDH     0x08                                        //-增强型PWM异常检测中断优先级控制位
#define PPWMH       0x04                                        //-增强型PWM中断优先级控制位
#define PSPIH       0x02                                        //-SPI中断优先级控制位
#define PS2H        0x01                                        //-串口2中断优先级控制位
sfr INTCLKO     =   0x8f;                                       //中断与时钟输出控制寄存器
#define EX4         0x40                                        //-外部中断4中断允许位
#define EX3         0x20                                        //-外部中断3中断允许位
#define EX2         0x10                                        //-外部中断2中断允许位
#define T2CLKO      0x04                                        //-定时器2时钟输出控制
#define T1CLKO      0x02                                        //-定时器1时钟输出控制
#define T0CLKO      0x01                                        //-定时器0时钟输出控制
sfr AUXINTIF    =   0xef;                                       //扩展外部中断标志寄存器
#define INT4IF      0x40                                        //-外部中断4中断请求标志
#define INT3IF      0x20                                        //外部中断3中断请求标志
#define INT2IF      0x10                                        //外部中断2中断请求标志
#define T4IF        0x04                                        //-定时器4溢出中断标志
#define T3IF        0x02                                        //-定时器3溢出中断标志
#define T2IF        0x01                                        //-定时器2溢出中断标志

//定时器特殊功能寄存器
sfr TCON        =   0x88;                                       //定时器控制寄存器
sbit TF1        =   TCON^7;                                     //-T1溢出中断标志
sbit TR1        =   TCON^6;                                     //-定时器T1的运行控制位
sbit TF0        =   TCON^5;                                     //-T0溢出中断标志
sbit TR0        =   TCON^4;                                     //-定时器T0的运行控制位
sbit IE1        =   TCON^3;                                     //-外部中断1请求源（INT1/P3.3）标志
sbit IT1        =   TCON^2;                                     //-外部中断源1触发控制位
sbit IE0        =   TCON^1;                                     //-外部中断0请求源（INT0/P3.2）标志
sbit IT0        =   TCON^0;                                     //-外部中断源0触发控制位
sfr TMOD        =   0x89;                                       //定时器模式寄存器
#define T1_GATE     0x80                                        //-控制定时器
#define T1_CT       0x40                                        //-控制定时器1用作定时器或计数器
#define T1_M1       0x20                                        //定时器定时器/计数器1模式选择1
#define T1_M0       0x10                                        //定时器定时器/计数器1模式选择0
#define T0_GATE     0x08                                        //-控制定时器0
#define T0_CT       0x04                                        //-控制定时器0用作定时器或计数器
#define T0_M1       0x02                                        //定时器定时器/计数器0模式选择1
#define T0_M0       0x01                                        //定时器定时器/计数器0模式选择0
sfr TL0         =   0x8a;                                       //定时器0低8位寄存器
sfr TL1         =   0x8b;                                       //定时器1低8位寄存器
sfr TH0         =   0x8c;                                       //定时器0高8位寄存器
sfr TH1         =   0x8d;                                       //定时器1高8位寄存器
sfr T4T3M       =   0xd1;                                       //定时器4/3控制寄存器
#define T4R         0x80                                        //-定时器4的运行控制位
#define T4_CT       0x40                                        //-控制定时器4用作定时器或计数器
#define T4x12       0x20                                        //-定时器4速度控制位
#define T4CLKO      0x10                                        //-定时器4时钟输出控制
#define T3R         0x08                                        //-定时器3的运行控制位
#define T3_CT       0x04                                        //-控制定时器3用作定时器或计数器
#define T3x12       0x02                                        //-定时器3速度控制位
#define T3CLKO      0x01                                        //-定时器3时钟输出控制
sfr T4H         =   0xd2;                                       //定时器4高字节
sfr T4L         =   0xd3;                                       //定时器4低字节
sfr T3H         =   0xd4;                                       //定时器3高字节
sfr T3L         =   0xd5;                                       //定时器3低字节
sfr T2H         =   0xd6;                                       //定时器2高字节
sfr T2L         =   0xd7;                                       //定时器2低字节
sfr TH4         =   0xd2;
sfr TL4         =   0xd3;
sfr TH3         =   0xd4;
sfr TL3         =   0xd5;
sfr TH2         =   0xd6;
sfr TL2         =   0xd7;
sfr WKTCL       =   0xaa;                                       //掉电唤醒定时器低字节
sfr WKTCH       =   0xab;                                       //掉电唤醒定时器高字节
#define WKTEN       0x80                                        //-掉电唤醒定时器的使能控制位
sfr WDT_CONTR   =   0xc1;                                       //看门狗控制寄存器
#define WDT_FLAG    0x80                                        //-看门狗溢出标志
#define EN_WDT      0x20                                        //-看门狗使能位
#define CLR_WDT     0x10                                        //-看门狗定时器清零
#define IDL_WDT     0x08                                        //-IDLE模式时的看门狗控制位

//串行口特殊功能寄存器
sfr SCON        =   0x98;                                       //串口1控制寄存器
sbit SM0        =   SCON^7;                                     //-串口1的通信工作模式控制0
sbit SM1        =   SCON^6;                                     //-串口1的通信工作模式控制1
sbit SM2        =   SCON^5;                                     //-允许模式2或模式3多机通信控制位
sbit REN        =   SCON^4;                                     //-允许/禁止串口接收控制位
sbit TB8        =   SCON^3;                                     //-当串口1使用模式2或模式3时，TB8为要发送的第9位数据
sbit RB8        =   SCON^2;                                     //-当串口1使用模式2或模式3时，RB8为接收到的第9位数据
sbit TI         =   SCON^1;                                     //-串口1发送中断请求标志位
sbit RI         =   SCON^0;                                     //-串口1接收中断请求标志位
sfr SBUF        =   0x99;                                       //串口1数据寄存器
sfr S2CON       =   0x9a;                                       //串口2控制寄存器
#define S2SM0       0x80                                        //-串口2的通信工作模式控制位
#define S2ST4       0x40
#define S2SM2       0x20                                        //-允许串口2在模式1时允许多机通信控制位
#define S2REN       0x10                                        //-串口2允许/禁止串口接收控制位
#define S2TB8       0x08                                        //-当串口2使用模式1时，S2TB8为要发送的第9位数据
#define S2RB8       0x04                                        //-当串口2使用模式1时，S2RB8为接收到的第9位数据
#define S2TI        0x02                                        //-串口2发送中断请求标志位
#define S2RI        0x01                                        //-串口2接收中断请求标志位
sfr S2BUF       =   0x9b;                                       //串口2数据寄存器
sfr S3CON       =   0xac;                                       //串口3控制寄存器
#define S3SM0       0x80                                        //-串口3的通信工作模式控制位
#define S3ST4       0x40
#define S3SM2       0x20                                        //-允许串口3在模式1时允许多机通信控制位
#define S3REN       0x10                                        //-串口3允许/禁止串口接收控制位
#define S3TB8       0x08                                        //-当串口3使用模式1时，S3TB8为要发送的第9位数据
#define S3RB8       0x04                                        //-当串口3使用模式1时，S3RB8为接收到的第9位数据
#define S3TI        0x02                                        //-串口3发送中断请求标志位
#define S3RI        0x01                                        //-串口3接收中断请求标志位
sfr S3BUF       =   0xad;                                       //串口3数据寄存器
sfr S4CON       =   0x84;                                       //串口4控制寄存器
#define S4SM0       0x80                                        //-串口4的通信工作模式控制位
#define S4ST4       0x40                                        //-选择串口4的波特率发生器
#define S4SM2       0x20                                        //-允许串口4在模式1时允许多机通信控制位
#define S4REN       0x10                                        //-串口4允许/禁止串口接收控制位
#define S4TB8       0x08                                        //-当串口4使用模式1时，S3TB8为要发送的第9位数据
#define S4RB8       0x04                                        //-当串口4使用模式1时，S3RB8为接收到的第9位数据
#define S4TI        0x02                                        //-串口4发送中断请求标志位
#define S4RI        0x01                                        //-串口4接收中断请求标志位
sfr S4BUF       =   0x85;                                       //串口4数据寄存器
sfr SADDR       =   0xa9;                                       //串口1从机地址寄存器
sfr SADEN       =   0xb9;                                       //串口1从机地址屏蔽寄存器

//ADC 特殊功能寄存器
sfr ADC_CONTR   =   0xbc;                                       //ADC控制寄存器
#define ADC_POWER   0x80                                        //-ADC电源控制位
#define ADC_START   0x40                                        //-ADC转换启动控制位
#define ADC_FLAG    0x20                                        //-ADC转换完成中断请求标志
sfr ADC_RES     =   0xbd;                                       //ADC转换结果高位寄存器
sfr ADC_RESL    =   0xbe;                                       //ADC转换结果低位寄存器
sfr ADCCFG      =   0xde;                                       //ADC配置寄存器
#define ADC_RESFMT  0x20                                        //-ADC转换结果格式控制位

//SPI 特殊功能寄存器
sfr SPSTAT      =   0xcd;                                       //SPI状态寄存器
#define SPIF        0x80                                        //-SPI中断标志位
#define WCOL        0x40                                        //-SPI写冲突标志位
sfr SPCTL       =   0xce;                                       //SPI控制寄存器
#define SSIG        0x80                                        //-SS引脚功能控制位
#define SPEN        0x40                                        //-SPI使能控制位
#define DORD        0x20                                        //-SPI数据位发送/接收的顺序
#define MSTR        0x10                                        //-器件主/从模式选择位
#define CPOL        0x08                                        //-SPI时钟极性控制
#define CPHA        0x04                                        //-SPI时钟相位控制
sfr SPDAT       =   0xcf;                                       //数据寄存器

//IAP/ISP 特殊功能寄存器
sfr IAP_DATA    =   0xc2;                                       //IAP数据寄存器
sfr IAP_ADDRH   =   0xc3;                                       //IAP高地址寄存器
sfr IAP_ADDRL   =   0xc4;                                       //IAP低地址寄存器
sfr IAP_CMD     =   0xc5;                                       //IAP命令寄存器
#define IAP_IDL     0x00                                        //IAP_空操作
#define IAP_READ    0x01                                        //IAP_读
#define IAP_WRITE   0x02                                        //IAP_写
#define IAP_ERASE   0x03                                        //IAP_擦除
sfr IAP_TRIG    =   0xc6;                                       //IAP触发寄存器
sfr IAP_CONTR   =   0xc7;                                       //IAP控制寄存器
#define IAPEN       0x80                                        //-EEPROM操作使能控制位
#define SWBS        0x40                                        //-软件复位选择控制位
#define SWRST       0x20                                        //-软件复位控制位
#define CMD_FAIL    0x10                                        //-EEPROM操作失败状态位
sfr ISP_DATA    =   0xc2;                                       //ISP数据寄存器
sfr ISP_ADDRH   =   0xc3;                                       //ISP高地址寄存器
sfr ISP_ADDRL   =   0xc4;                                       //ISP低地址寄存器
sfr ISP_CMD     =   0xc5;                                       //ISP命令寄存器
sfr ISP_TRIG    =   0xc6;                                       //ISP触发寄存器
sfr ISP_CONTR   =   0xc7;                                       //ISP控制寄存器

//比较器特殊功能寄存器
sfr CMPCR1      =   0xe6;                                       //比较器控制寄存器1
#define CMPEN       0x80                                        //-比较器模块使能位
#define CMPIF       0x40                                        //-比较器中断标志位
#define PIE         0x20                                        //-比较器上升沿中断使能位
#define NIE         0x10                                        //-比较器下降沿中断使能位
#define PIS         0x08                                        //-比较器的正极选择位
#define NIS         0x04                                        //-比较器的负极选择位
#define CMPOE       0x02                                        //-比较器结果输出控制位
#define CMPRES      0x01                                        //-比较器的比较结果
sfr CMPCR2      =   0xe7;                                       //比较器控制寄存器2
#define INVCMPO     0x80                                        //-比较器结果输出控制
#define DISFLT      0x40                                        //-模拟滤波功能控制

//PCA/PWM 特殊功能寄存器
sfr CCON        =   0xd8;                                       //PCA控制寄存器
sbit CF         =   CCON^7;                                     //-PCA计数器溢出中断标志
sbit CR         =   CCON^6;                                     //-PCA计数器允许控制位
sbit CCF3       =   CCON^3;                                     //-PCA模块3中断请求标志
sbit CCF2       =   CCON^2;                                     //-PCA模块2中断请求标志
sbit CCF1       =   CCON^1;                                     //-PCA模块1中断请求标志
sbit CCF0       =   CCON^0;                                     //-PCA模块0中断请求标志
sfr CMOD        =   0xd9;                                       //PCA模式寄存器
#define CIDL        0x80                                        //-空闲模式下是否停止PCA计数
#define ECF         0x01                                        //PCA计数器溢出中断允许位
sfr CL          =   0xe9;                                       //PCA计数器低字节
sfr CH          =   0xf9;                                       //PCA计数器高字节
sfr CCAPM0      =   0xda;                                       //PCA模块0模式控制寄存器
#define ECOM0       0x40                                        //允许PCA模块0的比较功能
#define CCAPP0      0x20                                        //允许PCA模块0进行上升沿捕获
#define CCAPN0      0x10                                        //允许PCA模块0进行下降沿捕获
#define MAT0        0x08                                        //允许PCA模块0的匹配功能
#define TOG0        0x04                                        //允许PCA模块0的高速脉冲输出功能
#define PWM0        0x02                                        //允许PCA模块0的脉宽调制输出功能
#define ECCF0       0x01                                        //允许PCA模块0的匹配/捕获中断
sfr CCAPM1      =   0xdb;                                       //PCA模块1模式控制寄存器
#define ECOM1       0x40                                        //允许PCA模块1的比较功能
#define CCAPP1      0x20                                        //允许PCA模块1进行上升沿捕获
#define CCAPN1      0x10                                        //允许PCA模块1进行下降沿捕获
#define MAT1        0x08                                        //允许PCA模块1的匹配功能
#define TOG1        0x04                                        //允许PCA模块1的高速脉冲输出功能
#define PWM1        0x02                                        //允许PCA模块1的脉宽调制输出功能
#define ECCF1       0x01                                        //允许PCA模块1的匹配/捕获中断
sfr CCAPM2      =   0xdc;                                       //PCA模块2模式控制寄存器
#define ECOM2       0x40                                        //允许PCA模块2的比较功能
#define CCAPP2      0x20                                        //允许PCA模块2进行上升沿捕获
#define CCAPN2      0x10                                        //允许PCA模块2进行下降沿捕获
#define MAT2        0x08                                        //允许PCA模块2的匹配功能
#define TOG2        0x04                                        //允许PCA模块2的高速脉冲输出功能
#define PWM2        0x02                                        //允许PCA模块2的脉宽调制输出功能
#define ECCF2       0x01                                        //允许PCA模块2的匹配/捕获中断
sfr CCAPM3      =   0xdd;                                       //PCA模块3模式控制寄存器
#define ECOM3       0x40                                        //-允许PCA模块3的比较功能
#define CCAPP3      0x20                                        //-允许PCA模块3进行上升沿捕获
#define CCAPN3      0x10                                        //-允许PCA模块3进行下降沿捕获
#define MAT3        0x08                                        //-允许PCA模块3的匹配功能
#define TOG3        0x04                                        //-允许PCA模块3的高速脉冲输出功能
#define PWM3        0x02                                        //-允许PCA模块3的脉宽调制输出功能
#define ECCF3       0x01                                        //-允许PCA模块3的匹配/捕获中断
sfr CCAP0L      =   0xea;                                       //PCA模块0低字节
sfr CCAP1L      =   0xeb;                                       //PCA模块1低字节
sfr CCAP2L      =   0xec;                                       //PCA模块2低字节
sfr CCAP3L      =   0xed;                                       //PCA模块3低字节
sfr CCAP0H      =   0xfa;                                       //PCA模块0高字节
sfr CCAP1H      =   0xfb;                                       //PCA模块1高字节
sfr CCAP2H      =   0xfc;                                       //PCA模块2高字节
sfr CCAP3H      =   0xfd;                                       //PCA模块3高字节
sfr PCA_PWM0    =   0xf2;                                       //PCA0的PWM模式寄存器
sfr PCA_PWM1    =   0xf3;                                       //PCA1的PWM模式寄存器
sfr PCA_PWM2    =   0xf4;                                       //PCA2的PWM模式寄存器
sfr PCA_PWM3    =   0xf5;                                       //PCA3的PWM模式寄存器

//增强型PWM波形发生器特殊功能寄存器
sfr PWMCFG      =   0xf1;                                       //PWM计数器
#define CBIF        0x80                                        //-增强型PWM计数器中断请求标志
#define ETADC       0x40                                        //-PWM是否与ADC关联
sfr PWMIF       =   0xf6;                                       //增强型PWM中断标志寄存器
#define C7IF        0x80                                        //增强型PWM通道7中断请求标志(需要软件清零)
#define C6IF        0x40                                        //增强型PWM通道6中断请求标志(需要软件清零)
#define C5IF        0x20                                        //增强型PWM通道5中断请求标志(需要软件清零)
#define C4IF        0x10                                        //增强型PWM通道4中断请求标志(需要软件清零)
#define C3IF        0x08                                        //增强型PWM通道3中断请求标志(需要软件清零)
#define C2IF        0x04                                        //增强型PWM通道2中断请求标志(需要软件清零)
#define C1IF        0x02                                        //增强型PWM通道1中断请求标志(需要软件清零)
#define C0IF        0x01                                        //增强型PWM通道0中断请求标志(需要软件清零)
sfr PWMFDCR     =   0xf7;                                       //PWM异常检测控制寄存器
#define INVCMP      0x80                                        //-比较器器结果异常信号处理
#define INVIO       0x40                                        //-外部端口P3.5异常信号处理
#define ENFD        0x20                                        //-PWM外部异常检测控制位
#define FLTFLIO     0x10                                        //-发生PWM外部异常时对PWM输出口控制位
#define EFDI        0x08                                        //-PWM异常检测中断使能位
#define FDCMP       0x04                                        //-比较器输出异常检测使能位
#define FDIO        0x02                                        //-P3.5口电平异常检测使能位
#define FDIF        0x01                                        //-增强型PWM异常检测中断请求标志
sfr PWMCR       =   0xfe;                                       //PWM控制寄存器
#define ENPWM       0x80                                        //-增强型PWM波形发生器使能位
#define ECBI        0x40                                        //-PWM计数器归零中断使能位

//如下特殊功能寄存器位于扩展RAM区域
//访问这些寄存器,需先将P_SW2的BIT7设置为1,才可正常读写
#define PWMC        (*(unsigned int  volatile xdata *)0xfff0)   //PWM计数器
#define PWMCH       (*(unsigned char volatile xdata *)0xfff0)   //PWM计数器高字节
#define PWMCL       (*(unsigned char volatile xdata *)0xfff1)   //PWM计数器低字节
#define PWMCKS      (*(unsigned char volatile xdata *)0xfff2)   //PWM时钟选择
#define TADCP       (*(unsigned char volatile xdata *)0xfff3)   //触发ADC计数值
#define TADCPH      (*(unsigned char volatile xdata *)0xfff3)   //触发ADC计数值高字节
#define TADCPL      (*(unsigned char volatile xdata *)0xfff4)   //触发ADC计数值低字节
#define PWM0T1      (*(unsigned int  volatile xdata *)0xff00)   //PWM0T1计数值
#define PWM0T1H     (*(unsigned char volatile xdata *)0xff00)   //PWM0T1计数值高字节
#define PWM0T1L     (*(unsigned char volatile xdata *)0xff01)   //PWM0T1计数值低字节
#define PWM0T2      (*(unsigned int  volatile xdata *)0xff02)   //PWM0T2数值
#define PWM0T2H     (*(unsigned char volatile xdata *)0xff02)   //PWM0T2数值高字节
#define PWM0T2L     (*(unsigned char volatile xdata *)0xff03)   //PWM0T2数值低字节
#define PWM0CR      (*(unsigned char volatile xdata *)0xff04)   //PWM0控制寄存器
#define PWM0HLD     (*(unsigned char volatile xdata *)0xff05)   //PWM0电平保持控制寄存器
#define PWM1T1      (*(unsigned int  volatile xdata *)0xff10)   //PWM1T1计数值
#define PWM1T1H     (*(unsigned char volatile xdata *)0xff10)   //PWM1T1计数值高字节
#define PWM1T1L     (*(unsigned char volatile xdata *)0xff11)   //PWM1T1计数值低字节
#define PWM1T2      (*(unsigned int  volatile xdata *)0xff12)   //PWM1T2数值
#define PWM1T2H     (*(unsigned char volatile xdata *)0xff12)   //PWM1T2数值高字节
#define PWM1T2L     (*(unsigned char volatile xdata *)0xff13)   //PWM1T2数值低字节
#define PWM1CR      (*(unsigned char volatile xdata *)0xff14)   //PWM1控制寄存器
#define PWM1HLD     (*(unsigned char volatile xdata *)0xff15)   //PWM1电平保持控制寄存器
#define PWM2T1      (*(unsigned int  volatile xdata *)0xff20)   //PWM2T1计数值
#define PWM2T1H     (*(unsigned char volatile xdata *)0xff20)   //PWM2T1计数值高字节
#define PWM2T1L     (*(unsigned char volatile xdata *)0xff21)   //PWM2T1计数值低字节
#define PWM2T2      (*(unsigned int  volatile xdata *)0xff22)   //PWM2T2数值
#define PWM2T2H     (*(unsigned char volatile xdata *)0xff22)   //PWM2T2数值高字节
#define PWM2T2L     (*(unsigned char volatile xdata *)0xff23)   //PWM2T2数值低字节
#define PWM2CR      (*(unsigned char volatile xdata *)0xff24)   //PWM2控制寄存器
#define PWM2HLD     (*(unsigned char volatile xdata *)0xff25)   //PWM2电平保持控制寄存器
#define PWM3T1      (*(unsigned int  volatile xdata *)0xff30)   //PWM3T1计数值
#define PWM3T1H     (*(unsigned char volatile xdata *)0xff30)   //PWM3T1计数值高字节
#define PWM3T1L     (*(unsigned char volatile xdata *)0xff31)   //PWM3T1计数值低字节
#define PWM3T2      (*(unsigned int  volatile xdata *)0xff32)   //PWM3T2数值
#define PWM3T2H     (*(unsigned char volatile xdata *)0xff32)   //PWM3T2数值高字节
#define PWM3T2L     (*(unsigned char volatile xdata *)0xff33)   //PWM3T2数值低字节
#define PWM3CR      (*(unsigned char volatile xdata *)0xff34)   //PWM3控制寄存器
#define PWM3HLD     (*(unsigned char volatile xdata *)0xff35)   //PWM3电平保持控制寄存器
#define PWM4T1      (*(unsigned int  volatile xdata *)0xff40)   //PWM4T1计数值
#define PWM4T1H     (*(unsigned char volatile xdata *)0xff40)   //PWM4T1计数值高字节
#define PWM4T1L     (*(unsigned char volatile xdata *)0xff41)   //PWM4T1计数值低字节
#define PWM4T2      (*(unsigned int  volatile xdata *)0xff42)   //PWM4T2数值
#define PWM4T2H     (*(unsigned char volatile xdata *)0xff42)   //PWM4T2数值高字节
#define PWM4T2L     (*(unsigned char volatile xdata *)0xff43)   //PWM4T2数值低字节
#define PWM4CR      (*(unsigned char volatile xdata *)0xff44)   //PWM4控制寄存器
#define PWM4HLD     (*(unsigned char volatile xdata *)0xff45)   //PWM4电平保持控制寄存器
#define PWM5T1      (*(unsigned int  volatile xdata *)0xff50)   //PWM5T1计数值
#define PWM5T1H     (*(unsigned char volatile xdata *)0xff50)   //PWM5T1计数值高字节
#define PWM5T1L     (*(unsigned char volatile xdata *)0xff51)   //PWM5T1计数值低字节
#define PWM5T2      (*(unsigned int  volatile xdata *)0xff52)   //PWM5T2数值
#define PWM5T2H     (*(unsigned char volatile xdata *)0xff52)   //PWM5T2数值高字节
#define PWM5T2L     (*(unsigned char volatile xdata *)0xff53)   //PWM5T2数值低字节
#define PWM5CR      (*(unsigned char volatile xdata *)0xff54)   //PWM5控制寄存器
#define PWM5HLD     (*(unsigned char volatile xdata *)0xff55)   //PWM5电平保持控制寄存器
#define PWM6T1      (*(unsigned int  volatile xdata *)0xff60)   //PWM6T1计数值
#define PWM6T1H     (*(unsigned char volatile xdata *)0xff60)   //PWM6T1计数值高字节
#define PWM6T1L     (*(unsigned char volatile xdata *)0xff61)   //PWM6T1计数值低字节
#define PWM6T2      (*(unsigned int  volatile xdata *)0xff62)   //PWM6T2数值
#define PWM6T2H     (*(unsigned char volatile xdata *)0xff62)   //PWM6T2数值高字节
#define PWM6T2L     (*(unsigned char volatile xdata *)0xff63)   //PWM6T2数值低字节
#define PWM6CR      (*(unsigned char volatile xdata *)0xff64)   //PWM6控制寄存器
#define PWM6HLD     (*(unsigned char volatile xdata *)0xff65)   //PWM6电平保持控制寄存器
#define PWM7T1      (*(unsigned int  volatile xdata *)0xff70)   //PWM7T1计数值
#define PWM7T1H     (*(unsigned char volatile xdata *)0xff70)   //PWM7T1计数值高字节
#define PWM7T1L     (*(unsigned char volatile xdata *)0xff71)   //PWM7T1计数值低字节
#define PWM7T2      (*(unsigned int  volatile xdata *)0xff72)   //PWM7T2数值
#define PWM7T2H     (*(unsigned char volatile xdata *)0xff72)   //PWM7T2数值高字节
#define PWM7T2L     (*(unsigned char volatile xdata *)0xff73)   //PWM7T2数值低字节
#define PWM7CR      (*(unsigned char volatile xdata *)0xff74)   //PWM7控制寄存器
#define PWM7HLD     (*(unsigned char volatile xdata *)0xff75)   //PWM7电平保持控制寄存器

//I2C特殊功能寄存器
//如下特殊功能寄存器位于扩展RAM区域
//访问这些寄存器,需先将P_SW2的BIT7设置为1,才可正常读写
#define I2CCFG      (*(unsigned char volatile xdata *)0xfe80)   //I2C配置寄存器
#define ENI2C       0x80                                        //-I2C功能使能控制位
#define MSSL        0x40                                        //-I2C工作模式选择位
#define I2CMSCR     (*(unsigned char volatile xdata *)0xfe81)   //I2C主机控制寄存器
#define EMSI        0x80                                        //-I2C主机模式中断使能控制位
#define I2CMSST     (*(unsigned char volatile xdata *)0xfe82)   //I2C主机状态寄存器
#define MSBUSY      0x80                                        //-主机模式时I2C控制器状态位（只读位）
#define MSIF        0x40                                        //-主机模式的中断请求位（中断标志位）
#define MSACKI      0x02                                        //-主机模式时，发送“011”命令到I2CMSCR的MSCMD位后所接收到的ACK数据
#define MSACKO      0x01                                        //-主机模式时，准备将要发送出去的ACK信号
#define I2CSLCR     (*(unsigned char volatile xdata *)0xfe83)   //I2C从机控制寄存器
#define ESTAI       0x40                                        //-I2C从机接收START事件中断允许位
#define ERXI        0x20                                        //-I2C从机接收数据完成事件中断允许位
#define ETXI        0x10                                        //-I2C从机发送数据完成事件中断允许位
#define ESTOI       0x08                                        //-从机模式时接收到STOP信号中断允许位
#define SLRST       0x01                                        //-复位从机模式
#define I2CSLST     (*(unsigned char volatile xdata *)0xfe84)   //I2C从机状态寄存器
#define SLBUSY      0x80                                        //-从机模式时I2C控制器状态位（只读位）
#define STAIF       0x40                                        //-从机模式时接收到START信号后的中断请求
#define RXIF        0x20                                        //-从机模式时接收到1字节的数据后的中断请
#define TXIF        0x10                                        //-从机模式时发送完成1字节的数据后的中断
#define STOIF       0x08                                        //-从机模式时接收到STOP信号后的中断请求
#define TXING       0x04
#define SLACKI      0x02                                        //-从机模式时，接收到的ACK数据
#define SLACKO      0x01                                        //-从机模式时，准备将要发送出去的ACK信号
#define I2CSLADR    (*(unsigned char volatile xdata *)0xfe85)   //I2C从机地址寄存器
#define I2CTXD      (*(unsigned char volatile xdata *)0xfe86)   //I2C数据发送寄存器
#define I2CRXD      (*(unsigned char volatile xdata *)0xfe87)   //I2C数据接收寄存器

/////////////////////////////////////////////////

#endif
