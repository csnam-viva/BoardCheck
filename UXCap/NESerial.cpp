// NESerial.cpp : implementation file
//

#include "stdafx.h"
#include "../include/NESerial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNESerial

CNESerial::CNESerial()
{
}

CNESerial::~CNESerial()
{
}

// ����Ǿ� �־�� ��밡����.
BOOL CNESerial::SerialPortGetState(int port, LPDCB pdcb)
{
	if (m_idComDev==INVALID_HANDLE_VALUE)
		return FALSE;

	return GetCommState(m_idComDev, pdcb);
}
