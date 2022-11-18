#if !defined(AFX_NETDEVIF_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_)
#define AFX_NETDEVIF_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CNetDevIF.h : header file
//

#include <afxmt.h>

#include "../include/commondef.h"
#include "../include/neapidef.h"
#include "../include/logclass.h"
#include "../include/devlibif.h"
#include "../Include/NetDevDef.h"
#include "../Include/onvifdefs.h"

// 20190626 dido supoprt.
class TiXmlElement;

class CMyRtspClient;


/////////////////////////////////////////////////////////////////////////////
// CNetDevIF window

class AFX_EXT_CLASS CNetDevNvrIF
	: public CDevLibIF
{
// Construction
public:
	CNetDevNvrIF();

// Attributes
public:
	CLogClass		m_Log;
	int				m_nLogCount;
	DWORD			m_dwStatus;

	LPSLOTINFO		m_pslot_info;

	CCriticalSection m_mutex;
	INT64 m_nErrorCode;

	HANDLE			m_hHandles[NUM_EVENTS];
	DWORD			m_dwConnectStatus[2];

	// 140308
	stIFrameHeader	m_ifHeader[2];
	
protected:
	HANDLE			m_hThread;
	CWinThread*		m_pThread;

public: // 140404 protected --> public
	VIDEO_DATA_CALLBACK m_pAddr_Encoded_callback[2];
	BOOL			m_bMotion;
	BOOL			m_bNotify;
	// 140515 audio
	AUDIO_DATA_CALLBACK m_pAddr_Audio_callback;

// Operations
private:
	HMODULE		 m_hRTSP;
	CMyRtspClient*	 m_pcRtspStream[2];

	HRESULT		 _create_rtspstreaming( LPTSTR _lpszurl, LPTSTR _lpszurl2, BOOL bUseTCP , BOOL bUseTCP2 );
	HRESULT		 _destroy_streaming();

	int			m_nFrameNo[2];
	SYSTEMTIME	m_JpegArrivalTime[2];

	VOID		_CreateThreadAndEvent( ULONG _ulContextPtr );
	VOID		_DestroyThreadAndEvent();
	float		_GausianDistribution( float _fomean, float _fosigma, float _fox );

	char*		m_pImageData[2];
	BOOL OnvifPTZ(int idx, int cmd, int time_out); // 140314

	// 140728
	HANDLE		m_hRtspSequence;

protected:
	void GetSystemDTS(__int64* dts);

public:

	void* CreateDevice(int type);
	void  DestroyDevice();

	BOOL LoadDll(BYTE type);
	void GetQueryInterface(int idx);
	void ResetQueryInterface(int idx);

	BOOL Connect(int idx, int slot_idx, NETWORK_INFO ni, int cam_idx, DWORD data_type);
	BOOL Connect2(int idx, int slot_idx, NETWORK_INFO ni, int cam_idx, DWORD data_type);
	void Disconnect(int idx, int slot_idx);
	void Disconnect(int idx);

	BOOL SetUp(int idx);
	void CleanUp(int idx);

	void GetEventHandles(int idx, HANDLE handles[NUM_EVENTS]);
	void ReleaseEventHandles(int idx, HANDLE handles[NUM_EVENTS]);
	BOOL Run(int idx);
	void Stop(int idx);

	int GetNetworkData(int idx, LPDATA_INFO pdata);
	void ReleaseNetworkData(int idx, LPDATA_INFO pdata);
	int GetCodImageData(int idx, LPDATA_INFO pdata);
	void ReleaseCodImageData(int idx, LPDATA_INFO pdata);

	// 20190626
	static VOID CALLBACK CaptureEventDataProc(LPVOID lpData, long nData, LPVOID _pUserData);
	void CaptureEventProc(LPVOID lpData, long nData, LPVOID _pUserData);
	int	GetSensorData(int idx, LPDATA_INFO pdata);
	void ReleaseSensorData(int idx, LPDATA_INFO pdata);
	DWORD	m_dwSensorStatus;

	// 20190626
	void searchXMLData(TiXmlElement* pElement, int nDepth);

	BOOL GetCameraStatus(int idx, int bd_idx, ULONG* pdata);

	void SetEncodedCallback(VIDEO_DATA_CALLBACK pAddr_Encoded_callback, VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary=NULL);
	//140308
	static VOID CALLBACK CaptureEncodedDataProc(LPVOID lpHdr, LPVOID lpData, LPVOID _pUserData, int streamNo);
	void CaptureEncodedProc(LPVOID lpHdr, LPVOID lpData);
	void CaptureEncodedProc_Secondary(LPVOID lpHdr, LPVOID lpData);
	// 140515 audio support
	void SetAudioCallback(AUDIO_DATA_CALLBACK pAddr_Audio_callback);
	static VOID CALLBACK CaptureAudioDataProc(LPVOID lpHdr, LPVOID lpData, LPVOID _pUserData);
	void CaptureAudioProc(LPVOID lpHdr, LPVOID lpData);
	// 140515 set video resolution - rtsp나 rtp header 에 해상도가 제대로 표시되지 않는 경우가 있다.
	BOOL SetImageRes(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	// 140515 onvif Relay Control
	int RelayControl(LPSTR strRelay, int onoff);

	CString SendHTTPRequest(LPSTR ip, int port, LPSTR user, LPSTR pwd, BOOL bAuth, LPSTR method, LPSTR request);
	// 140314
	BOOL SendPTZCommand(int idx, int cmd, int time_out);
	BOOL SetPTZSpeed(int idx, int cmd, DWORD dwSpeed);
	BOOL PresetSubInfo(int idx, int preset_idx);
	BOOL PresetSubInfo(int idx, LPSTR param_str);
	int m_PresetIndex;
	char m_PresetDesc[64];
	int m_PanSpeed;
	int m_TiltSpeed;
	int m_ZoomSpeed;
	int m_FocusSpeed;

	// for second stream
	BOOL IsFirstStreamUsed()
		{ return (m_pslot_info->first_stream.data_type!=0); };
	BOOL IsSecondStreamUsed()
		{ return (m_pslot_info->second_stream.data_type!=0); };
	// 140314 매 접속 마다 URI 를 새로 가져온다
	BOOL GetStreamUris(TCHAR* szrtspurl, TCHAR* szrtspurl2);
	////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetDevNvrIF)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetDevNvrIF();

	// Generated message map functions
protected:
			
	//{{AFX_MSG(CNetDevNvrIF)
	//afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETDEVIF_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_)
