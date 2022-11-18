#if !defined(AFX_NESERIAL_H__50ABA727_D5D4_46F9_AD8F_8F33140B01C0__INCLUDED_)
#define AFX_NESERIAL_H__50ABA727_D5D4_46F9_AD8F_8F33140B01C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NESerial.h : header file
//

#include "serial.h"

/////////////////////////////////////////////////////////////////////////////
// CNESerial window

class CNESerial : public CSerial
{
// Construction
public:
	CNESerial();

// Attributes
public:

// Operations
public:
	BOOL SerialPortGetState(int port, LPDCB pdcb);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNESerial)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNESerial();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NESERIAL_H__50ABA727_D5D4_46F9_AD8F_8F33140B01C0__INCLUDED_)
