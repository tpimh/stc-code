;/* ---  STC International Limited --------------------------------------------------- */
;/* ---  宏晶科技 姚永平  设计 2009/1/10   V1.0 ---------------------------------------- */
;/* ---  演示 STC12C5201AD 系列 MCU 的 A/D 转换功能------------------------------------ */
;/* ---  Mobile: 13922805190 --------------------------------------------------------- */
;/* ---  Fax: 0755-82944243 ---------------------------------------------------------- */
;/* ---  Tel: 0755-82948409 ---------------------------------------------------------- */
;/* ---  Web: www.mcu-memory.com ----------------------------------------------------- */
;如果要在程序中使用或在文章中引用该程序,请在程序中或文章中注明使用了宏晶科技的资料及程序 
;本程序用宏晶的STC-ISP Ver 3.0A.PCB的下载编程工具测试通过,相关的A/D转换结果在P1口上显示 
;转换结果也以 16 进制形式输出到串行口，可以用串行口调试程序观察输出结果。
;时钟 18.432MHz, 波特率 = 9600。
;转换结果也在 P1 口利用 LED 显示出来, 方便观察。
LED_MCU_START   EQU  P3.7

ADC_CONTR  EQU  0BCH   ;A/D转换寄存器
ADC_RES   EQU  0BDH   ;8位A/D转换结果寄存器
P1ASF      EQU  9DH    ;P1口中的相应位作为模拟功能使用时的控制寄存器,如做A/D用,相应位要置1 

ADC_Power_On_Speed_Channel_0 EQU 11100000B    ;P1.0作为A/D输入
ADC_Power_On_Speed_Channel_1 EQU 11100001B    ;P1.1作为A/D输入
ADC_Power_On_Speed_Channel_2 EQU 11100010B    ;P1.2作为A/D输入
ADC_Power_On_Speed_Channel_3 EQU 11100011B    ;P1.3作为A/D输入
ADC_Power_On_Speed_Channel_4 EQU 11100100B    ;P1.4作为A/D输入
ADC_Power_On_Speed_Channel_5 EQU 11100101B    ;P1.5作为A/D输入
ADC_Power_On_Speed_Channel_6 EQU 11100110B    ;P1.6作为A/D输入
ADC_Power_On_Speed_Channel_7 EQU 11100111B    ;P1.7作为A/D输入
;-------------------------------------------------
;定义变量
ADC_Channel_0_Result     EQU 30H    ;0通道A/D转换结果
ADC_Channel_1_Result     EQU 31H    ;1通道A/D转换结果
ADC_Channel_2_Result     EQU 32H    ;2通道A/D转换结果
ADC_Channel_3_Result     EQU 33H    ;3通道A/D转换结果
ADC_Channel_4_Result     EQU 34H    ;4通道A/D转换结果
ADC_Channel_5_Result     EQU 35H    ;5通道A/D转换结果
ADC_Channel_6_Result     EQU 36H    ;6通道A/D转换结果
ADC_Channel_7_Result     EQU 37H    ;7通道A/D转换结果
;--------------------------------------------------
    ORG   0000H
    LJMP  MAIN

    ORG   0050H
MAIN:
    CLR   LED_MCU_START             ;MCU工作指示灯 LED_MCU_START  EQU P3.7
    MOV   SP, #7FH                  ;设置堆栈
    
    ACALL Initiate_RS232            ;初始化串口 
    
    ACALL ADC_Power_On              ;开ADC电源, 第一次使用时要打开内部模拟电源
                                   ;开ADC电源, 可适当加延时，1mS以内就足够了
    ACALL Set_P12_ASF               ;设置 P1.2 为模拟功能口 
    ACALL Set_ADC_Channel_2         ;设置 P1.2 作为A/D转换通道

    ACALL Get_AD_Result             ;测量电压并且取A/D转换结果
    ACALL Send_AD_Result            ;发送转换结果到 PC 机

    ACALL Set_P12_Normal_IO         ;设置 P1.2 为普通IO
    MOV   A, ADC_Channel_2_Result   ;用 P1 口显示A/D转换结果
    CPL   A
    MOV   P1, A 
 
Wait_Loop:
    SJMP  Wait_Loop                 ;停机

;-------------------------------------------------------------
;-------------------------------------------------------------
;-------------------------------------------------------------
;-------------------------------------------------------------
Initiate_RS232:                     ;串口初始化
    CLR   ES                        ;禁止串口中断
    MOV   TMOD, #20H                ;设置 T1 为波特率发生器
    MOV   SCON, #50H                ;0101,0000 8位数据位, 无奇偶校验
    MOV   TH1, #0FBH                ;18.432MHz 晶振, 波特率 = 9600
    MOV   TL1, #0FBH

    SETB  TR1                       ;启动 T1
    RET
;--------------------------------------------------------------
Send_Byte:
    CLR   TI
    MOV   SBUF, A
Send_Byte_Wait_Finish:
    JNB   TI, Send_Byte_Wait_Finish
    CLR   TI
    RET
;--------------------------------------------------------------
ADC_Power_On:
    PUSH  ACC
    ORL   ADC_CONTR, #80H           ;开A/D转换电源
    MOV   A, #20H
    ACALL Delay                    ;开A/D转换电源后要加延时，1mS以内就足够了
    POP   ACC   
    RET

;-------------------------------------------------------------
;设置P1.2为模拟功能
Set_P12_ASF:
    PUSH  ACC
    MOV   A, #00000100B
    ORL   P1ASF, A
    POP   ACC
    RET

;-------------------------------------------------------------
;设置 P1.2 为普通IO
Set_P12_Normal_IO:
    PUSH  ACC
    MOV   A, #11111011B
    ANL   P1ASF, A
    POP   ACC
    RET

;-------------------------------------------------------------
Set_ADC_Channel_2:
    MOV   ADC_CONTR, #ADC_Power_On_Speed_Channel_2
                    ;选择P1.2作为A/D转换通道
    MOV   A, #05H   ;更换 A/D 转换通道后要适当延时, 使输入电压稳定
                    ;以后如果不更换 A/D 转换通道的话, 不需要加延时
    ACALL Delay     ;切换 A/D 转换通道,加延时20uS～200uS就可以了,与输入电压源的内阻有关
                    ;如果输入电压信号源的内阻在10K以下,可不加延时
    RET

;-------------------------------------------------------------

Send_AD_Result:
    PUSH  ACC
    MOV   A, ADC_Channel_2_Result   ;取 AD 转换结果
    ACALL Send_Byte                 ;发送转换结果到 PC 机
    POP   ACC
    RET

;----------------------------------------------------------
Get_AD_Result:
    PUSH  ACC                       ;入栈保护 
    MOV   ADC_RES, #0   
    ORL   ADC_CONTR, #00001000B     ;启动 AD 转换
    NOP   ;在对ADC_CONTR寄存器进行写操作后,要加4个空操作延时,才能够正确读到ADC_CONTR的值
    NOP   ;在对ADC_CONTR寄存器进行写操作后,要加4个空操作延时,才能够正确读到ADC_CONTR的值
    NOP   ;在对ADC_CONTR寄存器进行写操作后,要加4个空操作延时,才能够正确读到ADC_CONTR的值
    NOP   ;在对ADC_CONTR寄存器进行写操作后,要加4个空操作延时,才能够正确读到ADC_CONTR的值
Wait_AD_Finishe:
    MOV   A, #00010000B             ;判断 AD 转换是否完成
    ANL   A, ADC_CONTR
    JZ    Wait_AD_Finishe           ;AD 转换尚未完成, 继续等待

    ANL   ADC_CONTR, #11100111B     ;清0 ADC_FLAG, ADC_START 位, 停止A/D转换
 
    MOV   A, ADC_RES
    MOV   ADC_Channel_2_Result, A   ;保存 AD 转换结果
    POP   ACC   
    RET
;---------------------------------------------------------------
Delay:
    PUSH  02                        ;将寄存器组0 的 R2 入栈
    PUSH  03                        ;将寄存器组0 的 R3 入栈
    PUSH  04                        ;将寄存器组0 的 R4 入栈
    MOV   R4, A  
Delay_Loop0:
    MOV   R3, #200              ;2 CLOCK  ---------------------+
Delay_Loop1:                    ;                              |
    MOV   R2, #249              ;2 CLOCK  ------+              |
Delay_Loop:                     ;               | 1002 CLOCK   |200406 CLOCK
    DJNZ  R2, Delay_Loop        ;4 CLOCK        |              |
    DJNZ  R3, Delay_Loop1       ;4 CLOCK  ------+              |
    DJNZ  R4, Delay_Loop0       ;4 CLOCK  ---------------------+

    POP   04
    POP   03
    POP   02
    RET
;----------------------------------------------------------------
    END