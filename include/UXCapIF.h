#if !defined(AFX_UXCAPIF_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_)
#define AFX_UXCAPIF_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STCapIF.h : header file
//
#include <stdint.h>

#include "commondef.h"
//#include "neapidef.h"
//#include "logclass.h"
//#include "devlibif.h"
//#include "NESerial.h"
//#include "CaptureSound.h"
//#include "mixerControl.h"		// sound card volumn

#include "Ubiqmicro/uq3388/uq3388_lib_define.h"
#include "Ubiqmicro/uq3388/uq3388_lib.h"

//#include "Ubiqmicro/FreeType/ft2build.h"
//#pragma comment(lib,"../library/Ubiqmicro/FreeType/vs2010_x86/freetype.lib")


#include "DevLibIF.h"

//class uq3388_osd_lib;
//class uq3388_motion_lib;


#define	MAX_DIO_BOARD	2

/////////////////////////////////////////////////////////////////////////////
// CUXCapIF window

class AFX_EXT_CLASS CUXCapIF : public CDevLibIF
{
// Construction
public:
	CUXCapIF();

// Attributes
public:

// Operations
public:
	static void video_preview_callback(UQ_VIDEO_PREVIEW_DATA& data);
	static void video_stream_callback(UQ_VIDEO_STREAM_DATA& data);
	static void audio_stream_callback(UQ_AUDIO_STREAM_DATA& data);
//	static void jpeg_stream_callback(JSTR_EVENT_DATA& data);
	static void motion_data_callback(UQ_MOTION_STREAM_DATA& data);
	static void irq_callback(UQ_INTERRUPT_DATA& data);
	static void video_input_callback(UQ_VIDEO_INPUT_STATE& data);

	void* CreateDevice(int type);
	void  DestroyDevice();

	void GetQueryInterface(int idx);
	void ResetQueryInterface(int idx);

	BOOL SetUp(int idx);
	void CleanUp(int idx);

	void GetEventHandles(int idx, HANDLE handles[NUM_EVENTS]);
	void ReleaseEventHandles(int idx, HANDLE handles[NUM_EVENTS]);
	int GetVStatusData(int idx, LPDATA_INFO pdata);
	void ReleaseVStatusData(int idx, LPDATA_INFO pdata);
	int GetSensorData(int idx, LPDATA_INFO pdata);
	void ReleaseSensorData(int idx, LPDATA_INFO pdata);

	BOOL Run(int idx);
	void Stop(int idx);
	
	BOOL ExtractSystemInfo();
	BOOL SetVideoMode(int idx, int bd_id, ULONG uProperty[4]);
	BOOL EnableVideo(int idx, int bd_id, int cam_id, BOOL bEnable);
	BOOL EnableAudio(int idx, int bd_id, int aud_id, BOOL bEnable);

	BOOL SetImageRes(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetImageResSub(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetFrameRate(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetFrameRateSub(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetCodecQuality(int idx, int bd_id, int cam_id, int w, int h, ULONG uProperty[4]);
	BOOL SetCodecQualitySub(int idx, int bd_id, int cam_id, int w, int h, ULONG uProperty[4]);
	void SetCaptureSizeLevel(int idx, int bd_id, int cam_id, int level, BOOL bDecrease);
	void SetSecondarySizeLevel(int idx, int bd_id, int cam_id, int res, BOOL bEnable);
	void SetOSDColor(int idx, COLORREF osd_color);
	void SetOSDInfo(int idx, int bd_id, int cam_id, BOOL bShow, BOOL btime, int time_type, BOOL bosd, LPSTR osd, int pos, int translucent);

	int  SetWatchDog(int idx, int bd_id, int timeout, int beep_timeout);
	BOOL StopWatchDog(int idx, int bd_id);

	int	 RelayControl(int idx, int bd_id, int relay_id, int onoff);
	BOOL EnableMotionDetection(int idx, int bd_id, int cam_id, BOOL bEnable, BOOL bDetectMotionDisp, MOTIONINFO motion_info);

	void SetVideoCallback(VIDEO_DATA_CALLBACK pAddr_Display_callback);
	void SetEncodedCallback(VIDEO_DATA_CALLBACK pAddr_Encoded_callback, VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary=NULL);
	void SetAudioCallback(AUDIO_DATA_CALLBACK pAddr_Display_callback);

	int GetVSType(int idx);
	int GetBoardNum(int idx);
	int GetBoardType(int idx, int bd_idx);
	int GetCamNum(int idx, int bd_idx);
	int GetAudioNum(int idx, int bd_idx);
	int GetSensorNum(int idx, int bd_idx);
	int GetRelayNum(int idx, int bd_idx);

	BOOL GetCameraStatus(int idx, int bd_idx, ULONGLONG* pdata);
	BOOL GetSensorStatus(int idx, int bd_idx, ULONG* pdata);
	BOOL GetRelayStatus(int idx, int bd_idx, ULONG* pdata);

	void VideoStatusProc( UQ_VIDEO_INPUT_STATE& data );
	void DisplayStreamProc( UQ_VIDEO_PREVIEW_DATA& data );
	void EncodeStreamProc( UQ_VIDEO_STREAM_DATA& data );
	void AudioStreamProc( UQ_AUDIO_STREAM_DATA& data );
	void SensorStatusProc( UQ_INTERRUPT_DATA& data );
	void MotionDetectProc(/* sdvr_chan_handle_t handle */);

	BOOL SetBright(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetContrast(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetSaturation(int idx, int bd_id, int cam_id, ULONG uProperty[4]);
	BOOL SetHue(int idx, int bd_id, int cam_id, ULONG uProperty[4]);

	BOOL SetGain(int idx, int bd_id, int aud_id, ULONG uProperty[4]);

	DWORD GetImageResType(int idx, int bd_id, int cam_id);

	int GetVoutNum(int idx, int bd_id);
	void SetOnBoardVout(int idx, DWORD bd, DWORD port, DWORD split, DWORD *cam_info);

	int  CheckSecurity(int idx, int bd_idx);
	void SetTimeSync();

	BOOL SerialPortGetState(int idx, int port, LPDCB pdcb);
	BOOL SerialPortSetState(int idx, int port, DCB dcb);
	BOOL SerialPortSend(int idx, int port, LPSTR buf, int size);
	BOOL SerialPortRecv(int idx, int port, LPSTR buf, LPINT psize);
	BOOL SerialPortClearState(int idx, int port);			// 2008.04.23

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUXCapIF)
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CUXCapIF();

	// Generated message map functions
private:
	void			GetSystemDTS(__int64* dts);

protected:
	// serial port
	DWORD				m_dwStatus;
	//CLogClass			m_Log;

	//CNESerial			m_Serial[MAX_SERIAL_PORT];			// 2008.04.23

	HANDLE				m_hNotifyHandles[NUM_EVENTS];


//	UQ3388					*m_uq3388;

	UINT8					m_nBoardCount;
	DWORD					m_nBoardType;

	UINT8					m_nMaxVideo;
	UINT8					m_nMaxAudio;
	UINT8					m_nMaxSensor;
	UINT8					m_nMaxRelay;

	DWORD					m_VideoFormat[MAX_CAMERA_PER_BOARD];			// NTSC, PAL
	UINT8					m_ImageFormat;									// YUV420

	BOOL					m_bVideoLock[MAX_CAMERA_PER_BOARD];
	BOOL					m_bVideoEnable[MAX_CAMERA_PER_BOARD];

	__int64					m_last_bd_dts;

	// Color
	int						m_brightness[MAX_CAMERA_PER_BOARD];
	int						m_contrast[MAX_CAMERA_PER_BOARD];
	int						m_saturation[MAX_CAMERA_PER_BOARD];
	int						m_hue[MAX_CAMERA_PER_BOARD];

	int						m_sharpness[MAX_CAMERA_PER_BOARD];

//							encode[main,sub][Cam]
	UINT8					m_nEncodeGOPSize[2][MAX_CAMERA_PER_BOARD];
	UINT8					m_nEncodeFPS[2][MAX_CAMERA_PER_BOARD];
	RESOLUTION				m_nEncodeResolution[2][MAX_CAMERA_PER_BOARD];
	UINT8					m_nEncodeLevel[2][MAX_CAMERA_PER_BOARD];		// BitRate
	UINT8					m_nEncodeMode[2][MAX_CAMERA_PER_BOARD];			// VBR, CBR

//							preview
	RESOLUTION				m_nDisplayResolutionMax[MAX_CAMERA_PER_BOARD];
	UINT8					m_nDisplayResolutionLevel[MAX_CAMERA_PER_BOARD];
	BOOL					m_bDisplayStart[MAX_CAMERA_PER_BOARD];

//							audio
	BOOL					m_bAudioEnable[MAX_CAMERA_PER_BOARD];

//							osd
	//uq3388_osd_lib			*m_uq_osd;
	COLORREF				m_osd_color;
	BOOL					m_osd_show[MAX_CAMERA_PER_BOARD];
	UINT8					m_osd_pos[MAX_CAMERA_PER_BOARD];
	UINT8					m_osd_time_type[MAX_CAMERA_PER_BOARD];
	wchar_t					m_osd_text[MAX_CAMERA_PER_BOARD][128];
	UINT8					GetOSDFontIndex( int in_x, int in_y);
	void					SetOSDTime();

//							motion
	//uq3388_motion_lib		*m_uq_motion;
	BOOL					m_motionstart[MAX_CAMERA_PER_BOARD];
	MOTIONINFO				m_motioninfo[MAX_CAMERA_PER_BOARD];
	DWORD					m_tmLastMotionChecked[MAX_CAMERA_PER_BOARD];
	BOOL					EnableMotion(int cam_id, BOOL bEnable);
	void					GetMotionDetectArea(int max_x, int max_y, BOOL bPatial, RECT &in_rect, RECT &out_rect);
//	UQ_MOTION_DETECTION_DATA m_motion_data[MAX_CAMERA_PER_BOARD];
//	BOOL					set_motion_region(UINT8 ch_id, UINT8 region_id, RECT rect, UINT8 spatial_th, UINT8 temporal_th, UINT8 temporal_window);
	void					check_motion_data(UQ_MOTION_STREAM_DATA& data);

//							tvout
//	UINT8					m_nTVoutEnable;
	CRITICAL_SECTION		m_criticalSectionTVout;
	UINT8					m_nTVoutSplit;
	UINT8					m_nTVoutPort[MAX_CAMERA_PER_BOARD];

//							control
	UINT8					m_nEnable3dnr;
	UINT8					m_nEnable2dnr;
	UINT8					m_nEnableSharpness;

//							dio
	UINT8					m_nDIDOBoardIndex[MAX_DIO_BOARD];


	VIDEO_DATA_CALLBACK		m_pAddr_Encoded_callback;
	VIDEO_DATA_CALLBACK		m_pAddr_Encoded_callback_Secondary;
	VIDEO_DATA_CALLBACK		m_pAddr_Display_callback;
	VIDEO_DATA_CALLBACK		m_pAddr_Audio_callback;


//	HANDLE					m_hCloseEvent;
//	UINT					m_hTimeCheckThread;



	BOOL SetUp_Video();
	BOOL SetUp_Audio();
	void CleanUp_Video();
	void CleanUp_Audio();

	BOOL Run_Video();
	BOOL Run_Audio();
	void Stop_Video();
	void Stop_Audio();

	BOOL SetCodecGOPSize(int cam_id, ULONG frame_rate);
	BOOL GetChannelResolution(int cam_id, int stream_idx, int *width, int *height);

	void SetVideoPort( UQ_VIDEO_INPUT_STATE& data );
	void GetDisplayResolution( int in_x, int in_y, UINT8 level, UINT16& out_x, UINT16& out_y );
	UINT8 GetAudioIndex( UINT8 in_virtual_index );

	//{{AFX_MSG(CUXCapIF)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UXCAPIF_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_)
