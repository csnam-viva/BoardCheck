#if !defined(AFX_DEVLIBIF_H__2433255D_30A0_4B90_BA5D_37DE96A4C01A__INCLUDED_)
#define AFX_DEVLIBIF_H__2433255D_30A0_4B90_BA5D_37DE96A4C01A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DEVLIBIF.h : header file
//

#include "commondef.h"
#include "neapidef.h"
#include "logclass.h"
#include <MMSystem.h>

/////////////////////////////////////////////////////////////////////////////
// CDEVLIBIF window
class AFX_EXT_CLASS CDevLibIF : public CWnd
{
// Construction
public:
	CDevLibIF();

// Attributes
private:
protected:
	static BOOL					m_bLibLoaded;
	static HINSTANCE			m_hDll;

	DWORD						m_dwStatus;

public:
	//CLogClass					m_Log;


// Operations
private:

public:
	virtual void* CreateDevice(int type);
	virtual void* CreateDevice();
	virtual void  DestroyDevice();

	virtual BOOL LoadDll() {return TRUE;};
	virtual BOOL LoadDll(BYTE type) {return TRUE;};
	virtual void UnloadDll() {return;};
	
	virtual void GetQueryInterface(int idx) {return;};
	virtual void ResetQueryInterface(int idx) {return;};
	
	virtual BOOL Connect(int idx, LPSTR ip, int port, LPSTR id, LPSTR pwd, int vs_type=2, int protocol=3) {return TRUE;};
	virtual void Disconnect(int idx) {return;};

	// servera
	virtual BOOL Connect(int idx, int slot_idx, NETWORK_INFO ni, int cam_idx, DWORD data_type) {return TRUE;};
	virtual void Disconnect(int idx, int slot_idx) {return;};

	// for second stream
	virtual BOOL Connect2(int idx, int slot_idx, NETWORK_INFO ni, int cam_idx, DWORD data_type) {return TRUE;};

	virtual BOOL SetUp(int idx) {return TRUE;};
	virtual BOOL SetUp(int idx, VIDEO_DATA_CALLBACK pAddr_Video_callback, AUDIO_DATA_CALLBACK pAddr_Audio_callback) {return TRUE;};
	virtual void CleanUp(int idx) {return;};
	
	virtual void GetEventHandles(int idx, HANDLE handles[NUM_EVENTS]) {return;};
	virtual void ReleaseEventHandles(int idx, HANDLE handles[NUM_EVENTS]) {return;};

	virtual BOOL Run(int idx) {return TRUE;};
	virtual void Stop(int idx) {return;};

	virtual void SetVideoCallback(VIDEO_DATA_CALLBACK pAddr_Display_callback) {return;};
	virtual void SetAudioCallback(AUDIO_DATA_CALLBACK pAddr_Audio_callback) {return;};
	virtual void SetEncodedCallback(VIDEO_DATA_CALLBACK pAddr_Encoded_callback,VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary=NULL) {return;};
	
	virtual BOOL SetVideoMode(int idx, int bd_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetVideoFormat(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetImageFormat(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetImageRes(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetImageResSub(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetAutoFrameRate(int idx, int bd_id) {return TRUE;};
	// 20090907 tizone VS_MNC modified
	virtual BOOL SetUserFrameRate(int idx, int bd_id, UCHAR pFPS[], int length, BOOL DropAtNight=FALSE) {return TRUE;};
	virtual BOOL SetFrameRate(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetFrameRateSub(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetCodecQuality(int idx, int bd_id, int cam_id, int w, int h, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetCodecQualitySub(int idx, int bd_id, int cam_id, int w, int h, ULONG uProperty[4]) {return TRUE;};
	
	virtual BOOL SetBitRate(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetBright(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetContrast(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetSaturation_U(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetSaturation_V(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetSaturation(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetSharpness(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetHue(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetOSD(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL EnableOSD(int idx, int bd_id, int cam_id, BOOL bEnable) {return TRUE;};
	virtual BOOL SetStreamType(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL EnableVideo(int idx, int bd_id, int cam_id, BOOL bEnable) {return TRUE;};
	virtual BOOL EnableAudio(int idx, int bd_id, int aud_id, BOOL bEnable) {return TRUE;};
	virtual BOOL EnableMotionDetection(int idx, int bd_id, int cam_id, BOOL bEnable, int sensitivity, int ratio=0) {return TRUE;};
	virtual BOOL EnableMotionDetection(int idx, int bd_id, int cam_id, BOOL bEnable, MOTIONINFO motion_info) {return TRUE;};
	virtual BOOL EnableMotionDetection(int idx, int bd_id, int cam_id, BOOL bEnable, BOOL bDetectMotionDisp, MOTIONINFO motion_info) {return TRUE;};
	virtual BOOL SetGain(int idx, int bd_id, int aud_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetBPS(int idx, int bd_id, int aud_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL SetFrequency(int idx, int bd_id, int aud_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL StopWatchDog(int idx, int bd_id) {return TRUE;};
	virtual int  SetWatchDog(int idx, int bd_id, int timeout, int beep_timeout) {return 0;};
	virtual void SetSecondarySizeLevel(int idx, int bd_id, int cam_id, int res, BOOL bEnable){return;};
	virtual void SetCaptureSizeLevel(int idx, int bd_id, int cam_id, int level, BOOL bDecrease){return;};

	virtual BOOL ExtractSystemInfo() {return TRUE;};
	virtual int  GetVSType(int idx) {return -1;};
	virtual int  GetBoardNum(int idx) {return 0;};
	virtual int  GetBoardType(int idx, int bd_idx) {return -1;};
	virtual int  GetAudBoardNum(int idx) {return 0;};
	virtual int  GetCamNum(int idx, int bd_idx) {return 0;};
	virtual int  GetAudioNum(int idx, int bd_idx) {return 0;};
	virtual int  GetSensorNum(int idx, int bd_idx) {return 0;};
	virtual int  GetRelayNum(int idx, int bd_idx) {return 0;};
	virtual BOOL GetCameraStatus(int idx, int bd_idx, ULONGLONG* pdata) {return TRUE;};
	virtual BOOL GetSensorStatus(int idx, int bd_idx, ULONG* pdata) {return TRUE;};
	virtual BOOL GetRelayStatus(int idx, int bd_idx, ULONG* pdata) {return TRUE;};
	virtual BOOL GetMotionStatus(int idx, int bd_idx, ULONG* pdata) {return TRUE;};
	virtual BOOL GetVideoFormat(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL GetImageFormat(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual BOOL GetImageRes(int idx, int bd_id, int cam_id, ULONG uProperty[4]) {return TRUE;};
	virtual void SetEncoder(int idx, int bd_id, int cam_id, BYTE codec) {return;};
	virtual int  SetEncodeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate=0) {return 0;};
	virtual int  SetEncodeLevelSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate=0) {return 0;};
	virtual int  EncodeImage(LPVIDEO_SERVER pvs, int bd_id, int cam_id, LPBYTE pimage, LPBYTE dst, BOOL* pKeyOK) {return 0;};
	virtual void ResetEncoder(int idx, int bd_id, int cam_id, BYTE codec) {return;};

	virtual	int  GetNetworkData(int idx, LPDATA_INFO pdata) {return 0;};
	virtual int  GetAudioData(int idx, LPDATA_INFO pdata) {return 0;};
	virtual int  GetCodImageData(int idx, LPDATA_INFO pdata) {return 0;};
	virtual int  GetRawImageData(int idx, LPDATA_INFO pdata) {return 0;};
	virtual int  GetSensorData(int idx, LPDATA_INFO pdata) {return 0;};
	virtual int  GetVStatusData(int idx, LPDATA_INFO pdata) {return 0;};
	virtual void ReleaseNetworkData(int idx, LPDATA_INFO pdata) {return;};
	virtual void ReleaseAudioData(int idx, LPDATA_INFO pdata) {return;};
	virtual void ReleaseCodImageData(int idx, LPDATA_INFO pdata) {return;};
	virtual void ReleaseRawImageData(int idx, LPDATA_INFO pdata) {return;};
	virtual void ReleaseSensorData(int idx, LPDATA_INFO pdata) {return;};
	virtual void ReleaseVStatusData(int idx, LPDATA_INFO pdata) {return;};
	virtual void GetKeyPacket(int idx, LPSTR pKey) {return;}; // mnc 360

	virtual int  RelayControl(int idx, int bd_id, int relay_id, int onoff) {return 0;};
	virtual int  RelayControl(LPSTR strRelay, int onoff) {return 0;}; // 140515 onvif relay control

	virtual BOOL SerialPortGetState(int idx, int port, LPDCB pdcb) {return TRUE;};
	virtual BOOL SerialPortSetState(int idx, int port, DCB dcb) {return TRUE;};
	virtual BOOL SerialPortSend(int idx, int port, LPSTR buf, int size) {return TRUE;};
	virtual BOOL SerialPortRecv(int idx, int port, LPSTR buf, LPINT psize) {return TRUE;};
	virtual BOOL SerialPortClearState(int idx, int port) {return TRUE;};

	virtual BOOL SendPTZCommand(int idx, int cmd, int timeout) {return TRUE;};
	virtual BOOL SetPTZSpeed(int idx, int cmd, DWORD dwSpeed) {return TRUE;};
	virtual BOOL NetCommand(int idx, ULONG cmd, ULONG* pparam1, ULONG* pparam2, ULONG* pparam3, ULONG* pparam4) {return TRUE;};

	virtual void SetWindowForDraw(int idx, int bd_id, int cam_id, HWND drawhwnd) {return;};
	virtual void SetDisplayArea(int idx, int bd_id, int cam_id, CRect rt) {return;};

	virtual void SetOSDColor(int idx, COLORREF osd_color) {return;};
	virtual void SetOSDInfo(int idx, int bd_id, int cam_id, BOOL bShow, BOOL btime, int time_type, BOOL bosd, LPSTR osd, int pos, int translucent) {return;};

	virtual BOOL PresetSubInfo(int idx, int preset_idx) {return TRUE;};
	virtual BOOL PresetSubInfo(int idx, LPSTR param_str) {return TRUE;};

	virtual void ExtractBdCh(ULONG mixedCh,int & bdid,int& bdch) {return;};
	virtual DWORD MakeExMixedCh(ULONG bdid,ULONG bdch) {return 0;};
	virtual int  ConvertBDChToLogicalCh(int channelNo, int* bd_idx, int* cam_idx) {return 0;}; // channelNo는 UDP 보드의 mixedCh임.
	virtual DWORD ConvertLogicalChToBDCh(int channelNo, int* bd_idx, int* cam_idx) {return 0;}; // channelNo는 logical camera index 임.
	virtual int  ConvertBDSensorToLogicalSensor(int channelNo, int* bd_idx, int* sensor_idx) {return 0;}; // channelNo는 UDP 보드의 mixedCh임.

	virtual __int64 GetDTS() {return 0;};

	virtual void OvrSetOverlayWindow(CWnd* pWnd) {return;};
	virtual void OvrSetOverlayWindow(HWND hwnd) {return;};
	virtual BOOL OvrSetVideoFormat(ULONG vf = VF_NTSC) {return TRUE;};
	virtual BOOL OvrSetScreenMode(ULONG sm = 1) {return TRUE;}; // M720
	virtual BOOL OvrSetOverlayMode(int mode = OVR_MODE_COMBINE, int tx_mode = OVR5_TX_INDIRECT_GRAPHICS_EVENT) {return TRUE;};
	virtual void OvrShowOverlay(BOOL bInterlace) {return;};
	virtual void OvrHideOverlay() {return;};
	virtual BOOL OvrCaptureEnable(int bd_id, BOOL bEnable = TRUE) {return TRUE;};
	virtual BOOL OvrEnableVideo(int bd_id, int cam_id, int ch_id, BOOL bEnable = TRUE) {return TRUE;};
	virtual BOOL OvrSetSplit(int split_id, int start_ch, int count) {return TRUE;};
	virtual void OvrArrayClear() {return;};

	virtual void OvrMoveWantedRect(RECT over_rect) {return;};

	virtual int  CheckSecurity(int idx, int bd_idx) {return 0;};

/*	virtual BOOL ActivateExtVout(DWORD bd) {return FALSE;};
	virtual BYTE GetExtVoutCardId() {return 0;};
	virtual void SetExtVout(unsigned long uSel, unsigned long uOut) {return;};
	virtual void SetVoutMask(DWORD bd, DWORD port, DWORD type, DWORD uMask) {return;};*/

	virtual int GetVoutNum(int idx, int bd_id) {return 0;};
	virtual void SetOnBoardVout(int idx, DWORD bd, DWORD port, DWORD ch) {return;};
	virtual void SetOnBoardVout(int idx, DWORD bd, DWORD port, DWORD split, DWORD *cam_info) {return;};
	
	virtual BOOL GetVSTime(int idx, LPSYSTEMTIME psysTime) {return TRUE;};
	virtual BOOL SetVSTime(int idx, LPSYSTEMTIME psysTime) {return TRUE;};

	virtual DWORD GetImageResType(int idx, int bd_id, int cam_id) {return 0;};

	// 20090612 change port
	virtual void SetPortInfo(LPPORTINFO pPorts) {return;};

	// 20090907 tizone VS_MNC 360 camera sdk
	virtual BOOL SetViewMode(int view_mode, BOOL reverse) {return TRUE;};
	virtual BOOL SetViewAngle(int angle) {return TRUE;};
	virtual BOOL SetMotionArea(int area_idx, BOOL bEnable) {return TRUE;};
	virtual BOOL ConfirmMotionArea(UINT uiMotionHoldTime) {return TRUE;};

	// 140515 Audio UpLoad
	virtual BOOL Init2Way(int idx) {return FALSE;};
	virtual BOOL AudioUpload(int idx, LPBYTE pData, int size) {return FALSE;};
	virtual BOOL Close2Way(int idx) {return FALSE;};
	virtual BOOL GetAudioUploadFormat(int idx, WAVEFORMATEX* pWave) {return FALSE;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevLibIF)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDevLibIF();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDevLibIF)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVLIBIF_H__2433255D_30A0_4B90_BA5D_37DE96A4C01A__INCLUDED_)
