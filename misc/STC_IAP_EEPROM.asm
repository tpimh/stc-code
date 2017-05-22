;STC12C5201AD系列单片机EEPROM/IAP功能测试程序演示
;/* ---  STC International Limited -------------------------------------------------- */
;/* ---  宏晶科技 姚永平  设计 2009/1/10   V1.0 --------------------------------------- */
;/* ---  演示 STC12C5201AD 系列 MCU EEPROM/IAP功能----------------------------------- */
;/* ---  Mobile: 13922805190 -------------------------------------------------------- */
;/* ---  Fax: 0755-82944243 --------------------------------------------------------- */
;/* ---  Tel: 0755-82948409 --------------------------------------------------------- */
;/* ---  Web: www.mcu-memory.com ---------------------------------------------------- */
;本演示程序在STC-ISP Ver 3.0A.PCB的下载编程工具上测试通过,EEPROM的数据在P1口上显示
;如果要在程序中使用或在文章中引用该程序,请在程序中或文章中注明使用了宏晶科技的资料及程序 
;------------------------------------------------
;声明与IAP/ISP/EEPROM有关的特殊功能寄存器的地址
IAP_DATA        EQU   0C2H
IAP_ADDRH       EQU   0C3H
IAP_ADDRL       EQU   0C4H
IAP_CMD         EQU   0C5H
IAP_TRIG        EQU   0C6H
IAP_CONTR       EQU   0C7H
;定义ISP/IAP命令
ISP_IAP_BYTE_READ	  EQU	  1H	;字节读
ISP_IAP_BYTE_PROGRAM	  EQU	  2H	;字节编程,可以将1写成0，要将1变成0，必须执行字节编程
ISP_IAP_SECTOR_ERASE  	  EQU      	  3H	;扇区擦除,可以将0擦成1，要将0变成1，必须擦除整个扇区
;定义Flash 操作等待时间及允许IAP/ISP/EEPROM操作的常数
;ENABLE_IAP      EQU   80H     ;系统工作时钟<30MHz时，对IAP_CONTR寄存器设置此值
;ENABLE_IAP      EQU   81H     ;系统工作时钟<24MHz时，对IAP_CONTR寄存器设置此值
 ENABLE_IAP      EQU   82H     ;系统工作时钟<20MHz时，对IAP_CONTR寄存器设置此值
;ENABLE_IAP      EQU   83H     ;系统工作时钟<12MHz时，对IAP_CONTR寄存器设置此值
;ENABLE_IAP      EQU   84H     ;系统工作时钟<6MHz时，对IAP_CONTR寄存器设置此值
;ENABLE_IAP      EQU   85H     ;系统工作时钟<3MHz时，对IAP_CONTR寄存器设置此值
;ENABLE_IAP      EQU   86H     ;系统工作时钟<2MHz时，对IAP_CONTR寄存器设置此值
;ENABLE_IAP      EQU   87H     ;系统工作时钟<1MHz时，对IAP_CONTR寄存器设置此值
DEBUG_DATA       EQU   5AH ;是本测试程序选定的EEPROM单元的数值如正确应等于的数值
;------------------------
;选择 MCU EEPROM测试起始地址
DATA_FLASH_START_ADDRESS EQU 0000H  ;STC12C5201AD系列单片机的EEPROM测试起始地址
;------------------------------------------------
    ORG   0000H
    LJMP  MAIN
;------------------------------------------------
    ORG   0100H
MAIN:
    MOV    P1,#0F0H  	     ;演示程序开始工作,让P1.0/P1.1/P1.2/P1.3控制的灯亮
    LCALL	 Delay		;延时
    MOV    P1,#0FH	     ;演示程序开始工作,让P1.7/P1.6/P1.5/P1.4控制的灯亮    
    LCALL	 Delay		;延时
    MOV	 SP,  #7FH	     ;堆栈指针指向 7FH 单元
;************************************************
;将EEPROM测试起始地址单元的内容读出
MAIN1:
    MOV    DPTR, #DATA_FLASH_START_ADDRESS  ;将EEPROM测试起始地址送DPTR数据指针
    LCALL  Byte_Read
    MOV    40H, A                ;将EEPROM的值送40H 单元保存
    CJNE   A, #DEBUG_DATA, DATA_NOT_EQU_DEBUG_DATA   ;如果数据比较不正确,就跳转

DATA_IS_DEBUG_DATA:
;数据是对的,亮P1.7控制的灯,然后在P1口上将EEPROM的数据显示出来
    MOV	 P1,  #01111111B  ;如 (DATA_FLASH_START_ADDRESS)的值等于#DEBUG_DATA, 亮P1.7
    LCALL  Delay		  ;延时
    MOV	 A, 40H   ;将保存在40H单元中EEPROM的值从40H单元送累加器A
    CPL    A        ;取反的目的是相应的灯亮代表1,不亮代表0
    MOV   P1,A    ;数据是对的，送 P1显示
WAIT1:
    SJMP  WAIT1   ;数据是对的，送 P1显示后，CPU在此无限循环执行此句
 
DATA_NOT_EQU_DEBUG_DATA:
;EEPROM里的数据是错的,亮P1.3控制的灯,然后在P1口上将错误的数据显示出来,
;再将该EEPROM所在的扇区整个擦除,将正确的数据写入后,亮P1.5控制的灯
    MOV    P1,  #11110111B ;如 (DATA_FLASH_START_ADDRESS)的值不等于#DEBUG_DATA, 亮P1.3
    LCALL	Delay		;延时
    MOV	 A,   40H  ;将保存在40H单元中EEPROM的值从40H单元送累加器A
    CPL    A         ;取反的目的是相应的灯亮代表1,不亮代表0
    MOV   P1,  A    ;数据不对，送 P1显示
    LCALL	 Delay	;延时

    MOV    DPTR,#DATA_FLASH_START_ADDRESS  ;将EEPROM测试起始地址送DPTR数据指针
    ACALL  Sector_Erase  ;擦除整个扇区
    MOV    DPTR, #DATA_FLASH_START_ADDRESS  ;将EEPROM测试起始地址送DPTR数据指针
    MOV    A,    #DEBUG_DATA      ;写入 EEPROM 的数据为 #DEBUG_DATA
    ACALL  Byte_Program           ;字节编程
    MOV    P1,   #11011111B       ;将先前亮的P1.3灯关闭 ,再亮 P1.5灯,代表数据已被修改
WAIT2:
    SJMP   WAIT2       ;字节编程后,CPU在此无限循环执行此句
;************************************************

;------------------------------------------------
;读一字节，调用前需打开 IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
Byte_Read:
    MOV     IAP_CONTR,  #ENABLE_IAP     ;打开 IAP 功能, 设置 Flash 操作等待时间
    MOV     IAP_CMD,    #ISP_IAP_BYTE_READ  ;设置为IAP/ISP/EEPROM字节读模式命令
    MOV     IAP_ADDRH,  DPH             ;设置目标单元地址的高8位地址
    MOV     IAP_ADDRL,  DPL             ;设置目标单元地址的低8位地址
    ;CLR     EA
    MOV     IAP_TRIG,   #5AH       ;先送5Ah,再送A5h到ISP/IAP触发寄存器,每次都需如此
    MOV     IAP_TRIG,  #0A5H      ;送完A5h后，ISP/IAP命令立即被触发起动
    NOP
    MOV     A,   IAP_DATA          ;读出的数据在IAP_DATA单元中,送入累加器A
    ;SETB    EA
    ACALL IAP_Disable ;关闭 IAP 功能, 清相关的特殊功能寄存器,使CPU处于安全状态,
                      ;一次连续的IAP操作完成之后建议关闭IAP功能,不需要每次都关
    RET 
;------------------------------------------------
;字节编程，调用前需打开 IAP 功能，入口:DPTR = 字节地址, A=须编程字节的数据
Byte_Program:
    MOV   IAP_CONTR, #ENABLE_IAP    ;打开 IAP 功能, 设置 Flash 操作等待时间
    MOV   IAP_CMD, #ISP_IAP_BYTE_PROGRAM   ;设置为IAP/ISP/EEPROM字节编程模式命令
    MOV   IAP_ADDRH, DPH            ;设置目标单元地址的高8位地址
    MOV   IAP_ADDRL, DPL            ;设置目标单元地址的低8位地址
    MOV   IAP_DATA, A               ;要编程的数据先送进ISP_DATA寄存器
    ;CLR   EA
    MOV   IAP_TRIG, #5AH            ;先送5Ah,再送A5h到ISP/IAP触发寄存器,每次都需如此
    MOV   IAP_TRIG, #0A5H          ;送完A5h后，ISP/IAP命令立即被触发起动
    NOP
    ;SETB  EA
     ACALL IAP_Disable ;关闭 IAP 功能, 清相关的特殊功能寄存器,使CPU处于安全状态,
                       ;一次连续的IAP操作完成之后建议关闭IAP功能,不需要每次都关
    RET
;------------------------------------------------
;擦除扇区, 入口:DPTR = 扇区地址
Sector_Erase:
    MOV   IAP_CONTR, #ENABLE_IAP    ;打开 IAP 功能, 设置 Flash 操作等待时间
    MOV   IAP_CMD, #03H             ;设置为IAP/ISP/EEPROM扇区擦除模式命令
    MOV   IAP_ADDRH, DPH            ;设置目标单元地址的高8位地址
    MOV   IAP_ADDRL, DPL            ;设置目标单元地址的低8位地址
    ;CLR   EA
    MOV   IAP_TRIG, #5AH            ;先送5Ah,再送A5h到ISP/IAP触发寄存器,每次都需如此
    MOV   IAP_TRIG, #0A5H          ;送完A5h后，ISP/IAP命令立即被触发起动
    NOP
    ;SETB  EA
     ACALL IAP_Disable ;关闭 IAP 功能, 清相关的特殊功能寄存器,使CPU处于安全状态,
                       ;一次连续的IAP操作完成之后建议关闭IAP功能,不需要每次都关
    RET
;------------------------------------------------
IAP_Disable:                       
;关闭 IAP 功能, 清相关的特殊功能寄存器,使CPU处于安全状态,
;一次连续的IAP操作完成之后建议关闭IAP功能,不需要每次都关
    MOV   IAP_CONTR, #0             ;关闭 IAP 功能
    MOV   IAP_CMD,  #0              ;清命令寄存器,使命令寄存器无命令,此句可不用
    MOV   IAP_TRIG, #0              ;清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    MOV   IAP_ADDRH,	#0FFH 			;送地址高字节单元为00,指向非EEPROM区
    MOV   IAP_ADDRL,	#0FFH 			;送地址低字节单元为00,防止误操作
    RET
;------------------------------------------------
Delay:
    CLR   A
    MOV   R0, A
    MOV   R1, A
    MOV   R2, #20H
Delay_Loop:
    DJNZ  R0, Delay_Loop
    DJNZ  R1, Delay_Loop
    DJNZ  R2, Delay_Loop
    RET

;------------------------------------------------

    END
;************************************************
