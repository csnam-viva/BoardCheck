#if !defined(AFX_LOGCLASS_H__5A705F5B_4F57_422D_B1F1_86B5EBD66B62__INCLUDED_)
#define AFX_LOGCLASS_H__5A705F5B_4F57_422D_B1F1_86B5EBD66B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogClass window

class AFX_EXT_CLASS CLogClass : public CWnd
{
// Construction
public:
	CLogClass();

// Attributes
private:
	CString m_strLogDir;
	CString m_strSubDir;
	CString m_strLogFileExt;
	// tizone 2004-07-12
	// 180일이 지난 로그파일은 지운다.
	time_t	m_180Days;
	time_t	m_1Day;
	time_t	m_StartUp;
	// insert end
public:

// Operations
private:
	HANDLE OpenFile();
	void CloseFile(HANDLE hLogFile);
	void CheckDirectories();
	// tizone 2004-07-12
	// 180일이 지난 로그파일은 지운다.
	void CheckOldLogs();
	// insert end
	void DoLog(char *szData, BOOL bLF);
public:
	void SetLogDirectory(CString str);
	void SetSubDirectory(CString str);
	void SetLogFileExt(CString str);
	void Log(BOOL bLF, const char *format, ...);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogClass)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogClass)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGCLASS_H__5A705F5B_4F57_422D_B1F1_86B5EBD66B62__INCLUDED_)
