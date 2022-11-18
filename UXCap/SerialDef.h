#ifndef	DEFINE_H
#define DEFINE_H

#define ONEBLOCK	512
#define MAXBLOCK	1024

/////////////////////////////////////////////////////////////////////////
//		Flow control flags define
#define FC_DTRDSR	0x01
#define FC_RTSCTS	0x02
#define FC_XONXOFF	0x04


/////////////////////////////////////////////////////////////////////////
//		ASCII define
#define ASCII_BELL	0x07
#define ASCII_BS	0x08
#define ASCII_LF	0x0A
#define ASCII_CR	0x0D
#define ASCII_XON	0x11
#define ASCII_XOFF	0x13


/////////////////////////////////////////////////////////////////////////
//		Special Charter
#define STX						0x02
#define ETX						0x03
#define EOT						0x04
#define ENQ						0x05
#define ACK						0x06
#define NAK						0x15
#define FS						0x1C


#endif	//define.h
