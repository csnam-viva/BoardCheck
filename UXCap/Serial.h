#if !defined(AFX_SERIAL_H__AD3B23FC_6C88_4498_8EB5_B07BF749692A__INCLUDED_)
#define AFX_SERIAL_H__AD3B23FC_6C88_4498_8EB5_B07BF749692A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Serial.h : header file
//

#include	"SerialDef.h"

/////////////////////////////////////////////////////////////////////////////
// CSerial window
class CCommState;

class AFX_EXT_CLASS CSerial : public CObject
{
// Attributes
private:
	HANDLE		m_hThread;

	BOOL		CreateCommThread();
	void		DeleteCommThread();

	DECLARE_DYNCREATE(CSerial)
public:
	HANDLE		m_idComDev;					//컴포트 디바이스 연결 핸들
	BOOL		m_fConnected;				//컴포트가 연결되면 1로 설정
	BYTE		m_InputQueue[MAXBLOCK + 1];	//컴포트에서 들어오는 데이터
    int			m_RecvLength;		

	BYTE		m_bPort;
	BYTE		m_bByteSize, m_bParity, m_bStopBits;
	DWORD		m_dwBaudRate;

	void		SetComPort(int port, long rate, short bytesize, short stop, short parity);
	int			ReadCommBlock(BYTE*, int);
	BOOL		WriteCommBlock(BYTE*, DWORD);
	BOOL		OpenComPort(void);
	BOOL		SetupConnection(void);
	BOOL		CloseConnection(void);
	BOOL		SetReadData(BYTE* data, int data_len);


private:
	BOOL			m_fCommOpened;
	CCommState*		m_pCommState;

// Operations
public:
	int				Open(int port = 0, int rate = 0, int bytesize = 0, int stop = 0, int parity = 0);
	BOOL			Close(void);
	BOOL			Send(BYTE *Buffer, DWORD Length);
    virtual BOOL	OnReceive(BYTE *lpBuffer, DWORD Length);


// Construction
public:
	CSerial();
	virtual ~CSerial();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SERIAL_H__AD3B23FC_6C88_4498_8EB5_B07BF749692A__INCLUDED_)
