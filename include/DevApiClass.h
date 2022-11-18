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
	void	InitVSIF(LPVIDEO_SERVER pvs);			// query interface 가져오기
	void	UninitVSIF(LPVIDEO_SERVER pvs);			// query interface 해제하기
	int		Connect(LPVIDEO_SERVER pvs);			// 접속
	void	Disconnect(LPVIDEO_SERVER pvs);			// 접속 해제
	int		Ready(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Video_callback = NULL, AUDIO_DATA_CALLBACK pAddr_Audio_callback = NULL);				// vs-vendor supported dll을 사용할 준비
	void	CleanUp(LPVIDEO_SERVER pvs);			// vs-vendor supported dll에 사용 중지를 알림
	BOOL	Run(LPVIDEO_SERVER pvs);				// vs-vendor supported dll을 구동시킴
	void	Stop(LPVIDEO_SERVER pvs);				// vs-vendor supported dll의 작동을 중단함

	// servera
	int		Connect(LPVIDEO_SERVER pvs, int slot_idx, int cam_idx, DWORD data_type);
	void	Disconnect(LPVIDEO_SERVER pvs, int slot_idx);

	void	SetVideoCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Video_callback);
	void	SetAudioCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Audio_callback);
	void	SetEncodedCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Encoded_callback, VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary=NULL);

	int		CheckSecurity(LPVIDEO_SERVER pvs, int bd_idx);// get security byte

	// data event 발생 시 data 가져오기
	int		GetData(int action, LPVIDEO_SERVER pvs, LPDATA_INFO pdata);		// vs-vendor supported dll로부터 각종 데이터 가져오기
	void	ReleaseData(int action, LPVIDEO_SERVER pvs, LPDATA_INFO pdata);	// vs-vendor supported dll로부터 데이터를 가져오기 위해 사용한 resource 해제

	// 릴레이 제어
	int		RelayControl(int action, int act_param, LPVIDEO_SERVER pvs);		// relay control

	// 하드웨어 구성정보 가져오기
	int		GetBoardNum(LPVIDEO_SERVER pvs);					// 보드가 몇개?
	int		GetBoardType(LPVIDEO_SERVER pvs, int bd_idx);		// 보드 종류는?
	int		GetAudBoardNum(LPVIDEO_SERVER pvs);					// 오디오 보드 몇개?
	int		GetCamNum(LPVIDEO_SERVER pvs, int bd_idx = -1);		// 카메라가 몇개?
	int		GetAudioNum(LPVIDEO_SERVER pvs, int bd_idx = -1);	// 오디오 몇개?
	int		GetSensorNum(LPVIDEO_SERVER pvs, int bd_idx = -1);	// 센서 몇개?
	int		GetRelayNum(LPVIDEO_SERVER pvs, int bd_idx = -1);	// 릴레이 몇개?

	// 카메라/센서/릴레이 상태값
	void	GetCameraStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONGLONG* pdata);	// 카메라 상태?
	void	GetSensorStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata);	// 센서 상태?
	void	GetRelayStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata);	// 릴레이 상태?
	void	GetMotionStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata);  // motion 상태?

	// video encode APIs
	void	SetEncoder(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int codec);			// encoder 지정
	void	ResetEncoder(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int codec);		// encoder 해제
	int		SetEncodeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate=0);	// 압축 파라미터 지정하기. level이 높을수록 저화질
	int		SetEncodeLevelSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate=0);	// 압축 파라미터 지정하기. level이 높을수록 저화질
	int		EncodeImage(LPVIDEO_SERVER pvs, int bd_id, int cam_id, LPBYTE pimage, LPBYTE dst, BOOL* KeyOK); // PCI 영상 압축하기

	// serial port control
	BOOL	SerialPortGetState(LPVIDEO_SERVER pvs, int port, LPDCB pdcb);
	BOOL	SerialPortSetState(LPVIDEO_SERVER pvs, int port, DCB dcb);
	BOOL	SerialPortSend(LPVIDEO_SERVER pvs, int port, LPSTR buf, int size);
	BOOL	SerialPortRecv(LPVIDEO_SERVER pvs, int port, LPSTR buf, LPINT psize);
	BOOL	SerialPortClearState(LPVIDEO_SERVER pvs, int port);

	// PTZ control
	//		v2.0.0.3 에서 ptz address 를 따로 할당할 수 있도록 함.
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

	// video 특성값 지정하기
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

	// data enable 시키기
	BOOL	EnableAudio(LPVIDEO_SERVER pvs, int bd_id, int aud_id, BOOL bEnable);
	BOOL	EnableVideo(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable);

	// watchdog
	BOOL	StopWatchDog(LPVIDEO_SERVER pvs, int bd_id);
	int		SetWatchDog(LPVIDEO_SERVER pvs, int bd_id, int timeout, int beep_timeout);

	BOOL	SetAudioGain(LPVIDEO_SERVER pvs, int bd_id, int aud_id, int gain);
	BOOL	SetAudioBPSandFreq(LPVIDEO_SERVER pvs, int bd_id, int aud_id, int BPS, int Freq);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////    V2.0.0.1 ///////////////////////////////////////////////////////////////////////////////////
	// for vivotek / display module을 자체적으로 가지고 있는 업체DLL을 위해
	// display창을 내것으로 바꿔주는 API가 필요함.
	void	SetWindowForDraw(LPVIDEO_SERVER pvs, int bd_id, int cam_id, HWND drawhwnd);
	// vivotek camera는 preset parameter가 string으로 전달되어야 함.
	BOOL	SetParamString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, LPSTR param_str);
	// PTZ Command : 이전 버전에서는 App level에서 PTZ Command string을 받아서 serial port로 전송하였으나
	//					GetPTZCommandString() / SerialPortSend()
	//		이 API에서는 apiclass 내부에서 처리하도록 함.
	//		v2.0.0.3 에서 ptz address 를 따로 할당할 수 있도록 함.
	//BOOL	SendPTZCommand(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int cmd);
	////    V2.0.0.2 ///////////////////////////////////////////////////////////////////////////////////
	// PCI는 frame rate을 camera 별로 조정하도록 수정함. 20070109 tizone
	// 20090907 tizone VS_MNC modified
	BOOL	SetUserFrameRate(LPVIDEO_SERVER pvs, int bd_id, unsigned char pFPS[], int length, BOOL DropAtNight=FALSE);

	////    V2.0.0.3 ///////////////////////////////////////////////////////////////////////////////////
	// PTZ 제어 명령을 가져올 때 PTZ address 를 사용할 수 있도록 함.
	int		GetPTZCommandString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int ptz_addr, int command, LPSTR buf);
	void	SetPTZSpeed(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int ptz_addr, int cmd, DWORD dwSpeed);
	BOOL	SendPTZCommand(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int ptz_addr, int cmd);

	////    V2.0.0.5 ///////////////////////////////////////////////////////////////////////////////////
	// for Axis / display module을 자체적으로 가지고 있으면서
	// window내에서 display할 영역을 지정할 수 있는 API 를 가진 업체DLL을 위해
	void	SetDisplayArea(LPVIDEO_SERVER pvs, int bd_id, int cam_id, CRect rt);
	
	// OSD를 자체적으로 디스플레이할 수 있는 ip camera의 기능을 이용하기 위해.
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
	// 부분모션 추가 : SVE-k 및 SIPC 에만 적용됨.
	BOOL	EnableMotionDetection(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable, MOTIONINFO motion_info);
	BOOL	MotionDetectionTest(LPVIDEO_SERVER pvs, int bd_id, int cam_id, MOTIONINFO motion_info);		// New MPEG

	////    V2.9.2.14 ///////////////////////////////////////////////////////////////////////////////////
	// Video Server Time
	BOOL	GetVSTime(LPVIDEO_SERVER pvs, LPSYSTEMTIME psysTime);
	BOOL	SetVSTime(LPVIDEO_SERVER pvs, LPSYSTEMTIME psysTime);

	//// 20090907 tizone VS_MNC ///////////////////////////////////////////////////////////////////////////////////
	// for 360 camera - 업체 SDK를 이용해 display할 때의 mode
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
