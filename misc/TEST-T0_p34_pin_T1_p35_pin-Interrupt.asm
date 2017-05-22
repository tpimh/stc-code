;本程序演示宏晶科技的新单片机P34/P35/P30或P16也支持下降沿中断
;在宏晶科技的ISP烧录工具上焊一根线到地，
;然后接触INT_T0_P34 或 INT_T0_P35 就可以看到P34/P35也支持下降沿中断
;然后接触INT_RxD_P30 或 INT_RxD_P16 就可以看到P30或/P16也支持下降沿中断
	WAKE_CLKO 	EQU 	8FH
	AUXR1	 	EQU 	0A2H

	ORG	0000H
	LJMP	MAIN

	ORG	0003H
	CLR	P1.2		;INT0_P32中断
	SJMP	$


	ORG	000BH
	CLR	P1.4		;INT_T0_P34下降沿中断
	SJMP	$

	ORG	0013H
	CLR	P1.3		;INT1_P33中断
	SJMP	$

	ORG	001BH		;INT_T0_P35下降沿中断
	CLR	P1.5
	SJMP	$

	ORG	0023H		;INT_RxD_P30下降沿中断
	CLR	P1.0
	SJMP	$

MAIN:
;IP, 00001010


	MOV	IP,	#00001010B 
	;INT0_P32中断优先级为0,INT1_P33中断优先级为0
	;INT_T0_P34下降沿中断优先级为1,INT_T0_P35下降沿中断优先级为1
	
	MOV	A,	IP
	CPL	A
	MOV 	P0,	A; 显示中断优先级


	MOV 	WAKE_CLKO,	#01110000B
	;设置INT_RxD_P30或INT_RxD_P16下降沿可以中断
	;设置INT_T0_P34下降沿可以中断,设置INT_T0_P35下降沿可以中断
;	MOV  AUXR1, #10000000B ;设置INT_RxD从P30切换到P16
	
	SETB 	EX0
	SETB 	ET0
	SETB 	EX1
	SETB 	ET1
	SETB 	ES	

	SETB 	EA
	SJMP 	$
 ;在宏晶科技的ISP烧录工具上焊一根线到地，
 ;然后接触INT_T0_P34 或 INT_T0_P35 就可以看到P34/P35也支持下降沿中断
	END
