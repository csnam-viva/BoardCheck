#if !defined(AFX_DEVAPICLASS_H__D6BE98DE_4147_4F3F_A70D_16C09A4ED297__INCLUDED_)
#define AFX_DEVAPICLASS_H__D6BE98DE_4147_4F3F_A70D_16C09A4ED297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoSvrClass.h : header file
//

#include <mmsystem.h>

#include "../include/commondef.h"
#include "../include/devlibif.h"

/////////////////////////////////////////////////////////////////////////////
// CAPIClass window
class AFX_EXT_CLASS CDevApiClass : public CWnd
{
// Construction
public:
	CDevApiClass();
protected:
	DECLARE_DYNAMIC(CDevApiClass);

// Attributes
private:
	// newapi
	CDevLibIF*		m_pDevice;

	HANDLE			m_hPTZ[MAX_VIDEOSERVER][MAX_BOARD_PER_VS][MAX_CAMERA_PER_BOARD];

public:

// Operations
private:
	BOOL	IsPCI(int type);

public:
	// newapi
	BOOL	CreateDevice(int type);
	void	DestroyDevice();
	int		GetVSType();
	
	int		LoadDll(int type);						// load vs-vendor supplied dll
	void	UnloadDll(int type);					// unload dll

	// vs-vendor supported dll wrapper APIs
	void	InitVSIF(LPVIDEO_SERVER pvs);			// query interface ��������
	void	UninitVSIF(LPVIDEO_SERVER pvs);			// query interface �����ϱ�
	int		Connect(LPVIDEO_SERVER pvs);			// ����
	void	Disconnect(LPVIDEO_SERVER pvs);			// ���� ����
	int		Ready(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Video_callback = NULL, AUDIO_DATA_CALLBACK pAddr_Audio_callback = NULL);				// vs-vendor supported dll�� ����� �غ�
	void	CleanUp(LPVIDEO_SERVER pvs);			// vs-vendor supported dll�� ��� ������ �˸�
	BOOL	Run(LPVIDEO_SERVER pvs);				// vs-vendor supported dll�� ������Ŵ
	void	Stop(LPVIDEO_SERVER pvs);				// vs-vendor supported dll�� �۵��� �ߴ���

	// servera
	int		Connect(LPVIDEO_SERVER pvs, int slot_idx, int cam_idx, DWORD data_type);
	void	Disconnect(LPVIDEO_SERVER pvs, int slot_idx);

	void	SetVideoCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Video_callback);
	void	SetAudioCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Audio_callback);
	void	SetEncodedCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Encoded_callback, VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary=NULL);

	int		CheckSecurity(LPVIDEO_SERVER pvs, int bd_idx);// get security byte

	// data event �߻� �� data ��������
	int		GetData(int action, LPVIDEO_SERVER pvs, LPDATA_INFO pdata);		// vs-vendor supported dll�κ��� ���� ������ ��������
	void	ReleaseData(int action, LPVIDEO_SERVER pvs, LPDATA_INFO pdata);	// vs-vendor supported dll�κ��� �����͸� �������� ���� ����� resource ����

	// ������ ����
	int		RelayControl(int action, int act_param, LPVIDEO_SERVER pvs);		// relay control

	// �ϵ���� �������� ��������
	int		GetBoardNum(LPVIDEO_SERVER pvs);					// ���尡 �?
	int		GetBoardType(LPVIDEO_SERVER pvs, int bd_idx);		// ���� ������?
	int		GetAudBoardNum(LPVIDEO_SERVER pvs);					// ����� ���� �?
	int		GetCamNum(LPVIDEO_SERVER pvs, int bd_idx = -1);		// ī�޶� �?
	int		GetAudioNum(LPVIDEO_SERVER pvs, int bd_idx = -1);	// ����� �?
	int		GetSensorNum(LPVIDEO_SERVER pvs, int bd_idx = -1);	// ���� �?
	int		GetRelayNum(LPVIDEO_SERVER pvs, int bd_idx = -1);	// ������ �?

	// ī�޶�/����/������ ���°�
	void	GetCameraStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONGLONG* pdata);	// ī�޶� ����?
	void	GetSensorStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata);	// ���� ����?
	void	GetRelayStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata);	// ������ ����?
	void	GetMotionStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata);  // motion ����?

	// video encode APIs
	void	SetEncoder(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int codec);			// encoder ����
	void	ResetEncoder(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int codec);		// encoder ����
	int		SetEncodeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate=0);	// ���� �Ķ���� �����ϱ�. level�� �������� ��ȭ��
	int		SetEncodeLevelSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate=0);	// ���� �Ķ���� �����ϱ�. level�� �������� ��ȭ��
	int		EncodeImage(LPVIDEO_SERVER pvs, int bd_id, int cam_id, LPBYTE pimage, LPBYTE dst, BOOL* KeyOK); // PCI ���� �����ϱ�

	// serial port control
	BOOL	SerialPortGetState(LPVIDEO_SERVER pvs, int port, LPDCB pdcb);
	BOOL	SerialPortSetState(LPVIDEO_SERVER pvs, int port, DCB dcb);
	BOOL	SerialPortSend(LPVIDEO_SERVER pvs, int port, LPSTR buf, int size);
	BOOL	SerialPortRecv(LPVIDEO_SERVER pvs, int port, LPSTR buf, LPINT psize);
	BOOL	SerialPortClearState(LPVIDEO_SERVER pvs, int port);

	// PTZ control
	//		v2.0.0.3 ���� ptz address �� ���� �Ҵ��� �� �ֵ��� ��.
//	int		GetPTZCommandString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int command, LPSTR buf);
	BOOL	IsPresetable(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx);
	BOOL	SetParamIdx(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int param_idx);
	BOOL	IsAutoScanUsed(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx);
	BOOL	SetAutoScanParamIdx(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int param_idx);
	BOOL	IsSpeedControlUsed(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx);

	// realtime APIs (overlay)
	void	OvrSetOverlayWindow(CWnd* pWnd);
	void	OvrSetOverlayWindow(HWND hwnd);
	BOOL	OvrSetVideoFormat(ULONG vf = VF_NTSC);
	BOOL	OvrSetScreenMode(ULONG sm = 1);	// M720
	BOOL	OvrSetOverlayMode(int mode = OVR_MODE_COMBINE, int tx_mode = OVR5_TX_INDIRECT_GRAPHICS_EVENT);
	void	OvrShowOverlay(BOOL bInterlace);
	void	OvrHideOverlay();
	BOOL	OvrCaptureEnable(int bd_id, BOOL bEnable);
	BOOL	OvrEnableVideo(int bd_id, int cam_id, int ch_id, BOOL bEnable);
	BOOL	OvrSetSplit(int split_id, int start_ch, int count);
	void	OvrArrayClear();

	void	OvrMoveWantedRect(RECT over_rect);

	// video Ư���� �����ϱ�
	BOOL	SetVideoMode(LPVIDEO_SERVER pvs, int bd_id, ULONG uProperty[4]);	// set video mode	
	BOOL	SetBright(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetContrast(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetSaturation(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetHue(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetOSD(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	EnableOSD(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable);

	BOOL	SetImageRes(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetImageResSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetFrameRate(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetFrameRateSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL	SetCodecQuality(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int w, int h, ULONG uProperty[4]);
	BOOL	SetCodecQualitySub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int w, int h, ULONG uProperty[4]);

	// data enable ��Ű��
	BOOL	EnableAudio(LPVIDEO_SERVER pvs, int bd_id, int aud_id, BOOL bEnable);
	BOOL	EnableVideo(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable);

	// watchdog
	BOOL	StopWatchDog(LPVIDEO_SERVER pvs, int bd_id);
	int		SetWatchDog(LPVIDEO_SERVER pvs, int bd_id, int timeout, int beep_timeout);

	BOOL	SetAudioGain(LPVIDEO_SERVER pvs, int bd_id, int aud_id, int gain);
	BOOL	SetAudioBPSandFreq(LPVIDEO_SERVER pvs, int bd_id, int aud_id, int BPS, int Freq);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////    V2.0.0.1 ///////////////////////////////////////////////////////////////////////////////////
	// for vivotek / display module�� ��ü������ ������ �ִ� ��üDLL�� ����
	// displayâ�� �������� �ٲ��ִ� API�� �ʿ���.
	void	SetWindowForDraw(LPVIDEO_SERVER pvs, int bd_id, int cam_id, HWND drawhwnd);
	// vivotek camera�� preset parameter�� string���� ���޵Ǿ�� ��.
	BOOL	SetParamString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, LPSTR param_str);
	// PTZ Command : ���� ���������� App level���� PTZ Command string�� �޾Ƽ� serial port�� �����Ͽ�����
	//					GetPTZCommandString() / SerialPortSend()
	//		�� API������ apiclass ���ο��� ó���ϵ��� ��.
	//		v2.0.0.3 ���� ptz address �� ���� �Ҵ��� �� �ֵ��� ��.
	//BOOL	SendPTZCommand(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int cmd);
	////    V2.0.0.2 ///////////////////////////////////////////////////////////////////////////////////
	// PCI�� frame rate�� camera ���� �����ϵ��� ������. 20070109 tizone
	// 20090907 tizone VS_MNC modified
	BOOL	SetUserFrameRate(LPVIDEO_SERVER pvs, int bd_id, unsigned char pFPS[], int length, BOOL DropAtNight=FALSE);

	////    V2.0.0.3 ///////////////////////////////////////////////////////////////////////////////////
	// PTZ ���� ����� ������ �� PTZ address �� ����� �� �ֵ��� ��.
	int		GetPTZCommandString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int ptz_addr, int command, LPSTR buf);
	void	SetPTZSpeed(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int ptz_addr, int cmd, DWORD dwSpeed);
	BOOL	SendPTZCommand(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int ptz_addr, int cmd);

	////    V2.0.0.5 ///////////////////////////////////////////////////////////////////////////////////
	// for Axis / display module�� ��ü������ ������ �����鼭
	// window������ display�� ������ ������ �� �ִ� API �� ���� ��üDLL�� ����
	void	SetDisplayArea(LPVIDEO_SERVER pvs, int bd_id, int cam_id, CRect rt);
	
	// OSD�� ��ü������ ���÷����� �� �ִ� ip camera�� ����� �̿��ϱ� ����.
	void	SetOSDColor(LPVIDEO_SERVER pvs, COLORREF osd_color);
	void	SetOSDInfo(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bShow, BOOL btime, int time_type, BOOL bosd, LPSTR osd, int pos, int translucent);

	////    V2.0.0.6 ///////////////////////////////////////////////////////////////////////////////////
	// TV out
	/*BOOL	ActivateExtVout(LPVIDEO_SERVER pvs);
	BYTE	GetExtVoutCardId(LPVIDEO_SERVER pvs);
	void	SetExtVout(LPVIDEO_SERVER pvs, unsigned long uSel, unsigned long uOut);
	void	SetVoutMask(LPVIDEO_SERVER pvs, DWORD bd, DWORD port, DWORD type, DWORD uMask);*/

	BOOL	GetVoutNum(LPVIDEO_SERVER pvs, int bd_id);
	void	SetOnBoardVout(LPVIDEO_SERVER pvs, DWORD bd, DWORD port, DWORD split, DWORD *cam_info);

	////    V2.0.0.7 ///////////////////////////////////////////////////////////////////////////////////
	// �κи�� �߰� : SVE-k �� SIPC ���� �����.
	BOOL	EnableMotionDetection(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable, MOTIONINFO motion_info);
	BOOL	MotionDetectionTest(LPVIDEO_SERVER pvs, int bd_id, int cam_id, MOTIONINFO motion_info);		// New MPEG

	////    V2.9.2.14 ///////////////////////////////////////////////////////////////////////////////////
	// Video Server Time
	BOOL	GetVSTime(LPVIDEO_SERVER pvs, LPSYSTEMTIME psysTime);
	BOOL	SetVSTime(LPVIDEO_SERVER pvs, LPSYSTEMTIME psysTime);

	//// 20090907 tizone VS_MNC ///////////////////////////////////////////////////////////////////////////////////
	// for 360 camera - ��ü SDK�� �̿��� display�� ���� mode
	BOOL	SetViewMode(int view_mode, BOOL reverse);
	BOOL	SetViewAngle(int angle);
	BOOL	SetMotionArea(int area_idx, BOOL bEnable);
	BOOL	ConfirmMotionArea(UINT uiMotionHoldTime=0);
	void	GetKeyPacket(int idx, LPSTR pKey);
	///////////////////////////////////////////////////////////////////////////////////

	void	SetSecondarySizeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int res, BOOL bEnable = FALSE);
	void	SetCaptureSizeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int level, BOOL bDecrease = FALSE);
	DWORD	GetImageResType(LPVIDEO_SERVER pvs, int bd_id, int cam_id);

	// 140515 Audio UpLoad
	BOOL	Init2Way(LPVIDEO_SERVER pvs);
	BOOL	AudioUpload(LPVIDEO_SERVER pvs, LPBYTE pData, int size);
	BOOL	Close2Way(LPVIDEO_SERVER pvs);
	BOOL	GetAudioUploadFormat(LPVIDEO_SERVER pvs, WAVEFORMATEX* pWave);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevApiClass)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDevApiClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDevApiClass)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVAPICLASS_H__D6BE98DE_4147_4F3F_A70D_16C09A4ED297__INCLUDED_)
