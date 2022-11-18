// DevLibIF.cpp : implementation file
//

#include "stdafx.h"
#include "../include/DevLibIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CDevLibIF::m_bLibLoaded;
HINSTANCE CDevLibIF::m_hDll;

#pragma message ("### Making Interfaces for DevLib")

/////////////////////////////////////////////////////////////////////////////
// CDevLibIF

CDevLibIF::CDevLibIF()
{
	CDevLibIF::m_bLibLoaded = FALSE;
	CDevLibIF::m_hDll = NULL;

	m_dwStatus = API_ST_NOTHING;
}

CDevLibIF::~CDevLibIF()
{
}

void* CDevLibIF::CreateDevice(int type)
{
	return (void*)this;
}

void* CDevLibIF::CreateDevice()
{
	return (void*)this;
}

void CDevLibIF::DestroyDevice()
{
	delete this;
}


BEGIN_MESSAGE_MAP(CDevLibIF, CWnd)
	//{{AFX_MSG_MAP(CDevLibIF)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDevLibIF message handlers

