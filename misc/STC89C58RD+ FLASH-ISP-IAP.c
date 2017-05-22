/* ---  STC International Limited ---------------- */
/* ---  宏晶科技 姚永平  设计 2004/9/11   V1.0 --- */
/* ---  宏晶科技 姚永平  设计 2004/12/4   V2.0 --- */
/* ---  RD+/RC系列   Flash IAP/ISP Operation ----- */
/* ---  STC89C54RD+, STC89C58RD+,              --- */
/* ---  STC89LE54RD+,STC89LE58RD+,              -- */
/* ---  STC89C51RC,  STC89C52RC,             ----- */
/* ---  STC89LE51RC, STC89LE52RC,             ---- */
/* ---  Mobile: 13922805190 ---------------------- */
/* ---  Fax: 0755-82944243 ----------------------- */
/* ---  Tel: 0755-82908285 ----------------------- */
/* ---  Web  : www.mcu-memory.com ---------------- */
/* ---  感谢网友2004/12/3的指正,送样品以示感谢 --- */
#include <reg52.h>
#include <intrins.h>        /* use _nop_() function */
//sfr16 DPTR = 0x82;

/*        新增特殊功能寄存器定义        */
sfr	 ISP_DATA	=   0xe2;
sfr     ISP_ADDRH	=   0xe3;  
sfr	 ISP_ADDRL	=   0xe4;
sfr	 ISP_CMD	=   0xe5; 
sfr	 ISP_TRIG	=   0xe6;   
sfr	 ISP_CONTR    =   0xe7; 


/* 定义命令 */
#define READ_AP_and_Data_Memory_Command				0x01        /*  字节读数据存储区   */
#define PROGRAM_AP_and_Data_Memory_Command          0x02        /*  字节编程数据存储区 */
#define SECTOR_ERASE_AP_and_Data_Memory_Command     0x03        /*  扇区擦除数据存储区 */

typedef     unsigned char	INT8U;		/* 8 bit 无符号整型  */
typedef     unsigned int    INT16U;     /* 16 bit 无符号整型 */
#define		DELAY_CONST         60000

/* 定义常量 */
#define ERROR   0
#define OK      1

sbit        Begin_LED       =		P1^0;
sbit        ERROR_LED       =       P1^3;
sbit        OK_LED			=       P1^7;

/* 定义Flash 操作等待时间 */
#define        MCU_CLOCK_40MHz
//#define         MCU_CLOCK_20MHz
//#define        MCU_CLOCK_10MHz
//#define        MCU_CLOCK_5MHz
#ifdef MCU_CLOCK_40MHz
        #define WAIT_TIME        0x00
#endif
#ifdef MCU_CLOCK_20MHz
        #define WAIT_TIME        0x01
#endif
#ifdef MCU_CLOCK_10MHz
        #define WAIT_TIME        0x02
#endif
#ifdef MCU_CLOCK_5MHz
        #define WAIT_TIME        0x03
#endif

/* 调试控制项 */
//#define         DEBUG_STC89C_LE58RD+

#define         DEBUG_STC89C_LE52RC

//#define USED_BYTE_QTY_IN_ONE_SECTOR                1
//#define USED_BYTE_QTY_IN_ONE_SECTOR                2
//#define USED_BYTE_QTY_IN_ONE_SECTOR                4
//#define USED_BYTE_QTY_IN_ONE_SECTOR                8
//#define USED_BYTE_QTY_IN_ONE_SECTOR                16
//#define USED_BYTE_QTY_IN_ONE_SECTOR                32
//#define USED_BYTE_QTY_IN_ONE_SECTOR                64
#define USED_BYTE_QTY_IN_ONE_SECTOR                128
//#define USED_BYTE_QTY_IN_ONE_SECTOR                256
//#define USED_BYTE_QTY_IN_ONE_SECTOR                512

INT8U xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];

#ifdef DEBUG_STC89C_LE58RD+                        //STC89C58RD+,  89LE58RD+
        #define DEBUG_AP_Memory_Begin_Sector_addr		0x0000
        #define DEBUG_AP_Memory_End_Sector_addr         0x7e00
        #define DEBUG_AP_Memory_End_Byte_addr           0x7fff

        #define DEBUG_Data_Memory_Begin_Sector_addr     0x8000
#endif
#ifdef DEBUG_STC89C_LE52RC                        //STC89C52RC,        89LE52RC
        #define DEBUG_AP_Memory_Begin_Sector_addr		0x0000
        #define DEBUG_AP_Memory_End_Sector_addr			0x1e00
        #define DEBUG_AP_Memory_End_Byte_addr			0x1fff

        #define DEBUG_Data_Memory_Begin_Sector_addr		0x2000
#endif


/* 打开 ISP,IAP 功能 */
void ISP_IAP_enable(void)
{
	EA	=	0;	/* 关中断 */
	ISP_CONTR	=	ISP_CONTR & 0x18;       /* 0001,1000 */
	ISP_CONTR	=	ISP_CONTR | WAIT_TIME;
	ISP_CONTR	=	ISP_CONTR | 0x80;       /* 1000,0000 */
}

/* 关闭 ISP,IAP 功能 */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;	/* 0111,1111 */
	ISP_TRIG	=	0x00;
	EA			=   1;                	/* 开中断 */
}

/* 字节读 */
INT8U byte_read(INT16U byte_addr)
{
	ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
	ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	READ_AP_and_Data_Memory_Command;        /* 0000,0001 */

	ISP_IAP_enable();

	ISP_TRIG	=	0x46;
	ISP_TRIG	=	0xb9;
	_nop_();

	ISP_IAP_disable();
	return (ISP_DATA);
}

/* 扇区擦除 */
INT8U sector_erase(INT16U sector_addr)
{
	INT16U get_sector_addr	=	0;
	get_sector_addr			=	(sector_addr & 0xfe00); /* 1111,1110,0000,0000; 取扇区地址 */
	ISP_ADDRH        		=	(INT8U)(get_sector_addr >> 8);
	ISP_ADDRL		        =	0x00;

	ISP_CMD	=	ISP_CMD	&	0xf8;	/* 1111,1000 */
	ISP_CMD	=	ISP_CMD	|	SECTOR_ERASE_AP_and_Data_Memory_Command;	/* 0000,0011 */

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	 _nop_();

	ISP_IAP_disable();
	return OK;
}

/* 字节编程 */
INT8U byte_program(INT16U byte_addr, INT8U original_data)
{
	ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
	ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	PROGRAM_AP_and_Data_Memory_Command;		/* 0000,0010 */
	ISP_DATA	=	original_data;

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	_nop_();

	ISP_IAP_disable();
	return	OK;
}

/* 字节编程并校验 */
INT8U byte_program_and_verify(INT16U byte_addr, INT8U original_data)
{
	ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
	ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	PROGRAM_AP_and_Data_Memory_Command;	/* 0000,0010 */
	ISP_DATA	=	original_data;

	ISP_IAP_enable();

	ISP_TRIG	=	0x46;
	ISP_TRIG	=	0xb9;
	_nop_();

	ISP_DATA	=	0x00;

	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	READ_AP_and_Data_Memory_Command;        /* 0000,0001 */

	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	_nop_();

	ISP_IAP_disable();

	if(ISP_DATA	==	original_data)
		return	OK;
	else
		return	ERROR;
}

/* 写数据进 数据Flash存储器, 只在同一个扇区内写，不保留原有数据	*/
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源	*/
INT8U sequential_write_flash_in_one_sector(INT16U begin_addr, INT16U counter, INT8U array[])
{
	INT16U	i	=	0;
	INT16U	in_sector_begin_addr	=	0;
	INT16U	sector_addr	=	0;

	/* 判是否是有效范围,此函数不允许跨扇区操作 */
	if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
		return	ERROR;
	in_sector_begin_addr =        begin_addr & 0x01ff;         /* 0000,0001,1111,1111 */
	if( (in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR )
		return ERROR;

    /* 擦除 要修改/写入 的扇区 */
	sector_addr	=	(begin_addr & 0xfe00);	/* 1111,1110,0000,0000; 取扇区地址 */
	ISP_ADDRH	=	(INT8U)(sector_addr >> 8);
	ISP_ADDRL	=	0x00;
	ISP_CMD		=	ISP_CMD	&	0xf8;		/* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	SECTOR_ERASE_AP_and_Data_Memory_Command;	/* 0000,0011 */

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	_nop_();

	for(i = 0; i< counter; i++)
	{
		/* 写一个字节 */
		ISP_ADDRH	=	(INT8U)(begin_addr >> 8);
		ISP_ADDRL	=	(INT8U)(begin_addr & 0x00ff);
		ISP_DATA	=	array[i];
		ISP_CMD		=	ISP_CMD	&	0xf8;	/* 1111,1000 */
		ISP_CMD		=	ISP_CMD	|	PROGRAM_AP_and_Data_Memory_Command;		/* 0000,0010 */

		ISP_TRIG	=	0x46;	/* 触发ISP_IAP命令 */
		ISP_TRIG	=	0xb9;	/* 触发ISP_IAP命令 */
		_nop_();

		/* 读回来 */
		ISP_DATA	=	0x00;

		ISP_CMD		=	ISP_CMD	&	0xf8;	/* 1111,1000 */
		ISP_CMD     =	ISP_CMD	|	READ_AP_and_Data_Memory_Command;	/* 0000,0001 */

		ISP_TRIG	=	0x46;	/* 触发ISP_IAP命令 */
		ISP_TRIG	=	0xb9;	/* 触发ISP_IAP命令 */
		_nop_();

		/*  比较对错 */
		if(ISP_DATA != array[i])
		{
			ISP_IAP_disable();
			return ERROR;
		}
        begin_addr++;
	}
	ISP_IAP_disable();
	return	OK;
}

/* 写数据进数据Flash存储器(EEPROM), 只在同一个扇区内写，保留同一扇区中不需修改的数据	*/
/* begin_addr,被写数据Flash开始地址；counter,连续写多少个字节； array[]，数据来源		*/
INT8U write_flash_with_protect_in_one_sector(INT16U begin_addr, INT16U counter, INT8U array[])
{
	INT16U	i	=	0;
	INT16U	in_sector_begin_addr	=	0;
	INT16U	sector_addr	=	0;
	INT16U	byte_addr	=	0;

	/* 判是否是有效范围,此函数不允许跨扇区操作 */
	if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
		return ERROR;
	in_sector_begin_addr =        begin_addr & 0x01ff;         /* 0000,0001,1111,1111 */
	/* 假定从扇区的第0个字节开始，到USED_BYTE_QTY_IN_ONE_SECTOR-1个字节结束,后面部分不用,程序易编写	*/
	if( (in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR )
		return ERROR;

	/* 将该扇区数据 0 - (USED_BYTE_QTY_IN_ONE_SECTOR-1) 字节数据读入缓冲区保护 */
	sector_addr		=	(begin_addr & 0xfe00); 	/* 1111,1110,0000,0000; 取扇区地址		*/
	byte_addr		=   sector_addr;			/* 扇区地址为扇区首字节地址			 	*/

	ISP_IAP_enable();
	for(i = 0; i < USED_BYTE_QTY_IN_ONE_SECTOR; i++)
	{
		ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
		ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

		ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
		ISP_CMD		=	ISP_CMD	|	READ_AP_and_Data_Memory_Command;	/* 0000,0001 */

		ISP_TRIG	=	0x46;
		ISP_TRIG	=	0xb9;
		_nop_();

		protect_buffer[i]	=	ISP_DATA;
		byte_addr++;
	}

	/* 将要写入的数据写入保护缓冲区的相应区域,其余部分保留 */
	for(i = 0; i < counter; i++)
	{
		protect_buffer[in_sector_begin_addr] = array[i];
		in_sector_begin_addr++;
	}

	/* 擦除 要修改/写入 的扇区 */
	ISP_ADDRH	=	(INT8U)(sector_addr >> 8);
	ISP_ADDRL	=	0x00;
	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	SECTOR_ERASE_AP_and_Data_Memory_Command;	/* 0000,0011 */

	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	_nop_();

	/* 将保护缓冲区的数据写入 Data Flash, EEPROM */
	byte_addr	=   sector_addr;			/* 扇区地址为扇区首字节地址	*/
	for(i = 0; i< USED_BYTE_QTY_IN_ONE_SECTOR; i++)
	{
		/* 写一个字节 */
		ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
		ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);
		ISP_DATA	=	protect_buffer[i];
		ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
		ISP_CMD		=	ISP_CMD	|	PROGRAM_AP_and_Data_Memory_Command;		/* 0000,0010 */

		ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
		ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
		_nop_();

		/* 读回来 */
		ISP_DATA	=	0x00;

		ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
		ISP_CMD		=	ISP_CMD	|	READ_AP_and_Data_Memory_Command;	/* 0000,0001 */

		ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
		ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
		_nop_();

		/*  比较对错 */
		if(ISP_DATA != protect_buffer[i])
		{
			ISP_IAP_disable();
			return ERROR;
        }
        byte_addr++;
	}
	ISP_IAP_disable();
	return OK;
}

/* 测试常量数组 */
INT8U code Test_array_total[512]        =
{
        0x00,        0x01,        0x02,        0x03,        0x04,        0x05,        0x06,        0x07,
        0x08,        0x09,        0x0a,        0x0b,        0x0c,        0x0d,        0x0e,        0x0f,
        0x10,        0x11,        0x12,        0x13,        0x14,        0x15,        0x16,        0x17,
        0x18,        0x19,        0x1a,        0x1b,        0x1c,        0x1d,        0x1e,        0x1f,
        0x20,        0x21,        0x22,        0x23,        0x24,        0x25,        0x26,        0x27,
        0x28,        0x29,        0x2a,        0x2b,        0x2c,        0x2d,        0x2e,        0x2f,
        0x30,        0x31,        0x32,        0x33,        0x34,        0x35,        0x36,        0x37,
        0x38,        0x39,        0x3a,        0x3b,        0x3c,        0x3d,        0x3e,        0x3f,
        0x40,        0x41,        0x42,        0x43,        0x44,        0x45,        0x46,        0x47,
        0x48,        0x49,        0x4a,        0x4b,        0x4c,        0x4d,        0x4e,        0x4f,
        0x50,        0x51,        0x52,        0x53,        0x54,        0x55,        0x56,        0x57,
        0x58,        0x59,        0x5a,        0x5b,        0x5c,        0x5d,        0x5e,        0x5f,
        0x60,        0x61,        0x62,        0x63,        0x64,        0x65,        0x66,        0x67,
        0x68,        0x69,        0x6a,        0x6b,        0x6c,        0x6d,        0x6e,        0x6f,
        0x70,        0x71,        0x72,        0x73,        0x74,        0x75,        0x76,        0x77,
        0x78,        0x79,        0x7a,        0x7b,        0x7c,        0x7d,        0x7e,        0x7f,
        0x80,        0x81,        0x82,        0x83,        0x84,        0x85,        0x86,        0x87,
        0x88,        0x89,        0x8a,        0x8b,        0x8c,        0x8d,        0x8e,        0x8f,
        0x90,        0x91,        0x92,        0x93,        0x94,        0x95,        0x96,        0x97,
        0x98,        0x99,        0x9a,        0x9b,        0x9c,        0x9d,        0x9e,        0x9f,
        0xa0,        0xa1,        0xa2,        0xa3,        0xa4,        0xa5,        0xa6,        0xa7,
        0xa8,        0xa9,        0xaa,        0xab,        0xac,        0xad,        0xae,        0xaf,
        0xb0,        0xb1,        0xb2,        0xb3,        0xb4,        0xb5,        0xb6,        0xb7,
        0xb8,        0xb9,        0xba,        0xbb,        0xbc,        0xbd,        0xbe,        0xbf,
        0xc0,        0xc1,        0xc2,        0xc3,        0xc4,        0xc5,        0xc6,        0xc7,
        0xc8,        0xc9,        0xca,        0xcb,        0xcc,        0xcd,        0xce,        0xcf,
        0xd0,        0xd1,        0xd2,        0xd3,        0xd4,        0xd5,        0xd6,        0xd7,
        0xd8,        0xd9,        0xda,        0xdb,        0xdc,        0xdd,        0xde,        0xdf,
        0xe0,        0xe1,        0xe2,        0xe3,        0xe4,        0xe5,        0xe6,        0xe7,
        0xe8,        0xe9,        0xea,        0xeb,        0xec,        0xed,        0xee,        0xef,
        0xf0,        0xf1,        0xf2,        0xf3,        0xf4,        0xf5,        0xf6,        0xf7,
        0xf8,        0xf9,        0xfa,        0xfb,        0xfc,        0xfd,        0xfe,        0xff,
        0xff,        0xfe,        0xfd,        0xfc,        0xfb,        0xfa,        0xf9,        0xf8,
        0xf7,        0xf6,        0xf5,        0xf4,        0xf3,        0xf2,        0xf1,        0xf0,
        0xef,        0xee,        0xed,        0xec,        0xeb,        0xea,        0xe9,        0xe8,
        0xe7,        0xe6,        0xe5,        0xe4,        0xe3,        0xe2,        0xe1,        0xe0,
        0xdf,        0xde,        0xdd,        0xdc,        0xdb,        0xda,        0xd9,        0xd8,
        0xd7,        0xd6,        0xd5,        0xd4,        0xd3,        0xd2,        0xd1,        0xd0,
        0xcf,        0xce,        0xcd,        0xcc,        0xcb,        0xca,        0xc9,        0xc8,
        0xc7,        0xc6,        0xc5,        0xc4,        0xc3,        0xc2,        0xc1,        0xc0,
        0xbf,        0xbe,        0xbd,        0xbc,        0xbb,        0xba,        0xb9,        0xb8,
        0xb7,        0xb6,        0xb5,        0xb4,        0xb3,        0xb2,        0xb1,        0xb0,
        0xaf,        0xae,        0xad,        0xac,        0xab,        0xaa,        0xa9,        0xa8,
        0xa7,        0xa6,        0xa5,        0xa4,        0xa3,        0xa2,        0xa1,        0xa0,
        0x9f,        0x9e,        0x9d,        0x9c,        0x9b,        0x9a,        0x99,        0x98,
        0x97,        0x96,        0x95,        0x94,        0x93,        0x92,        0x91,        0x90,
        0x8f,        0x8e,        0x8d,        0x8c,        0x8b,        0x8a,        0x89,        0x88,
        0x87,        0x86,        0x85,        0x84,        0x83,        0x82,        0x81,        0x80,
        0x7f,        0x7e,        0x7d,        0x7c,        0x7b,        0x7a,        0x79,        0x78,
        0x77,        0x76,        0x75,        0x74,        0x73,        0x72,        0x71,        0x70,
        0x6f,        0x6e,        0x6d,        0x6c,        0x6b,        0x6a,        0x69,        0x68,
        0x67,        0x66,        0x65,        0x64,        0x63,        0x62,        0x61,        0x60,
        0x5f,        0x5e,        0x5d,        0x5c,        0x5b,        0x5a,        0x59,        0x58,
        0x57,        0x56,        0x55,        0x54,        0x53,        0x52,        0x51,        0x50,
        0x4f,        0x4e,        0x4d,        0x4c,        0x4b,        0x4a,        0x49,        0x48,
        0x47,        0x46,        0x45,        0x44,        0x43,        0x42,        0x41,        0x40,
        0x3f,        0x3e,        0x3d,        0x3c,        0x3b,        0x3a,        0x39,        0x38,
        0x33,        0x36,        0x35,        0x34,        0x33,        0x32,        0x31,        0x30,
        0x2f,        0x2e,        0x2d,        0x2c,        0x2b,        0x2a,        0x29,        0x28,
        0x27,        0x26,        0x25,        0x24,        0x23,        0x22,        0x21,        0x20,
        0x1f,        0x1e,        0x1d,        0x1c,        0x1b,        0x1a,        0x19,        0x18,
        0x17,        0x16,        0x15,        0x14,        0x13,        0x12,        0x11,        0x10,
        0x0f,        0x0e,        0x0d,        0x0c,        0x0b,        0x0a,        0x09,        0x08,
        0x07,        0x06,        0x05,        0x04,        0x03,        0x02,        0x01,        0x00
};

void display_Begin_LED(void)
{
	Begin_LED	=	0;  /* Begin_LED on */
}
void off_Begin_LED(void)
{
	Begin_LED	=	1;  /* Begin_LED off */
}
void display_OK_LED(void)
{
	OK_LED		=	0;  /* OK_LED on */
}
void off_OK_LED(void)
{
	OK_LED		=	1;  /* OK_LED off */
}

void display_ERROR_LED(void)
{
	ERROR_LED	=	0;  /* ERROR_LED on */
}
void off_ERROR_LED(void)
{
	ERROR_LED	=	1;  /* ERROR_LED off */
}

void delay(INT16U counter)
{
	INT16U	temp	=	0;
	for(temp = counter; temp>0; temp--)
	{
		_nop_();
		_nop_();
		_nop_();
	}
}

void main()
{
//sbit        Begin_LED	=	P1^0;
//sbit        ERROR_LED	=	P1^3;
//sbit        OK_LED	=	P1^7;

	display_Begin_LED();
	delay(DELAY_CONST);

	sector_erase(DEBUG_Data_Memory_Begin_Sector_addr);
	if(byte_program_and_verify(DEBUG_Data_Memory_Begin_Sector_addr,0x55) == OK)
		display_OK_LED();
	else
		display_ERROR_LED();

/*
	if(sequential_write_flash_in_one_sector(DEBUG_Data_Memory_Begin_Sector_addr, USED_BYTE_QTY_IN_ONE_SECTOR, Test_array_total))
		display_OK_LED();
	else
		display_ERROR_LED();
*/
/*
	if(write_flash_with_protect_in_one_sector(DEBUG_Data_Memory_Begin_Sector_addr+0x3, USED_BYTE_QTY_IN_ONE_SECTOR-0x30, Test_array_total))
		display_OK_LED();
	else
		display_ERROR_LED();
*/
	while(1);
}