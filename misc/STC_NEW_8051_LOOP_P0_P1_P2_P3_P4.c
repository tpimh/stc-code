#include"NEW_8051.h"
#include<intrins.h>

/*
sfr P4   = 0xC0; //8 bitPort4          P4.7  P4.6  P4.5  P4.4  P4.3  P4.2  P4.1  P4.0       1111,1111
sfr P4M0 = 0xB4; //                                                                         0000,0000
sfr P4M1 = 0xB3; //                                                                         0000,0000
//                                      7      6         5         4      3     2     1     0     Reset Value
sfr P4SW = 0xBB; //Port-4 switch	    -   LVD_P4.6  ALE_P4.5  NA_P4.4   -     -     -     -	    x000,xxxx
*/

sbit	P10	=	P1^0;
sbit	P11	=	P1^1;
sbit	P12	=	P1^2;
sbit	P13	=	P1^3;
sbit	P14	=	P1^4;
sbit	P15	=	P1^5;
sbit	P16	=	P1^6;
sbit	P17	=	P1^7;

sbit	P30	=	P3^0;
sbit	P31	=	P3^1;
sbit	P32	=	P3^2;
sbit	P33	=	P3^3;
sbit	P34	=	P3^4;
sbit	P35	=	P3^5;
sbit	P36	=	P3^6;
sbit	P37	=	P3^7;

sbit	P20	=	P2^0;
sbit	P21	=	P2^1;
sbit	P22	=	P2^2;
sbit	P23	=	P2^3;
sbit	P24	=	P2^4;
sbit	P25	=	P2^5;
sbit	P26	=	P2^6;
sbit	P27	=	P2^7;

sbit	P00	=	P0^0;
sbit	P01	=	P0^1;
sbit	P02	=	P0^2;
sbit	P03	=	P0^3;
sbit	P04	=	P0^4;
sbit	P05	=	P0^5;
sbit	P06	=	P0^6;
sbit	P07	=	P0^7;

sbit	P40	=	P4^0;
sbit	P41	=	P4^1;
sbit	P42	=	P4^2;
sbit	P43	=	P4^3;
sbit	P44	=	P4^4;
sbit	P45	=	P4^5;
sbit	P46	=	P4^6;
sbit	P47	=	P4^7;

void delay(void);

void main(void)
{
	P4SW = 0x70;	  //0111,0000

	P10	=	0;
	delay();
	P11	=	0;
	delay();
	P12	=	0;
	delay();
	P13	=	0;
	delay();
	P14	=	0;
	delay();
	P15	=	0;
	delay();
	P16	=	0;
	delay();
	P17	=	0;
	delay();

	P1	=	0xff;

	P30	=	0;
	delay();
	P31	=	0;
	delay();
	P32	=	0;
	delay();
	P33	=	0;
	delay();
	P34	=	0;
	delay();
	P35	=	0;
	delay();
	P36	=	0;
	delay();
	P37	=	0;
	delay();

	P3	=	0xff;

	P20	=	0;
	delay();
	P21	=	0;
	delay();
	P22	=	0;
	delay();
	P23	=	0;
	delay();
	P24	=	0;
	delay();
	P25	=	0;
	delay();
	P26	=	0;
	delay();
	P27	=	0;
	delay();

	P2	=	0xff;

	P07	=	0;
	delay();
	P06	=	0;
	delay();
	P05	=	0;
	delay();
	P04	=	0;
	delay();
	P03	=	0;
	delay();
	P02	=	0;
	delay();
	P01	=	0;
	delay();
	P00	=	0;
	delay();

	P0	=	0xff;

	P40	=	0;
	delay();
	P41	=	0;
	delay();
	P42	=	0;
	delay();
	P43	=	0;
	delay();
	P44	=	0;
	delay();
	P45	=	0;
	delay();
	P46	=	0;
	delay();
	P47	=	0;
	delay();

	P4	=	0xff;

	while(1)
	{
		P1	=	0x00;
		delay();
		P1	=	0xff;

		P3	=	0x00;
		delay();
		P3	=	0xff;

		P2	=	0x00;
		delay();
		P2	=	0xff;

		P0	=	0x00;
		delay();
		P0	=	0xff;

		P4	=	0x00;
		delay();
		P4	=	0xff;
	}
}

void delay(void)
{
	unsigned int i = 0;
    for(i=60000;i>0;i--)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}
