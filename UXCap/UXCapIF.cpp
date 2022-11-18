// STCapIF.cpp : implementation file
//

#include "stdafx.h"
#include "../include/UXCapIF.h"
#include <process.h>

#include "spot_layout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVENT_AUTH_ERROR	0
#define EVENT_ENCODE_IMAGE	1
#define EVENT_RAW_IMAGE		2
#define EVENT_AUDIO			3
#define EVENT_SENSOR		4
#define EVENT_VIDEO_STATUS	5

#define MAX_MULTI_BOARD		4
#define LICENSE_LENGTH		40


#pragma comment (lib, "../Library/DevLib2.lib")
#pragma message ("linking 'DevLib2.lib'")

//#pragma comment (lib, "../Library/RDVRLog.lib")
//#pragma message ("linking 'RDVRLog.lib'")

#include "uq3388_osd_lib.h"
#include "uq3388_motion_lib.h"
// move to stdafx.h
// #pragma comment(lib, "../Library/Ubiqmicro/uq3388/uq3388.lib")
// #pragma message("linking 'uq3388.lib'")

//#pragma comment(lib, "../Library/Stretch/sdvr_sdk_dll.lib")
//#pragma message("linking 'sdvr_sdk_dll.lib'")

//#pragma comment (lib, "../Library/wincomm.lib")
//#pragma message ("linking 'wincomm.lib'")

//#pragma comment (lib, "dsound.lib")
//#pragma message ("linking 'dsound.lib'")

//#pragma comment (lib, "dxguid.lib")
//#pragma message ("linking 'dxguid.lib'")

//#pragma comment (lib, "winmm.lib")
//#pragma message ("linking 'winmm.lib'")

static CUXCapIF	*m_pObject = NULL;

//#define _FILE_WRITE


#define API_ST_NOTHING			1
#define API_ST_LOADED			2
#define API_ST_READY			3
#define API_ST_RUN				4
UINT __stdcall TimeCheckProc(LPVOID lpParameter)
{
/*	DWORD dwWaitRet;
	CUXCapIF *pClass = (CUXCapIF *)lpParameter;
	while(1)
	{
		dwWaitRet = WaitForSingleObject(pClass->m_hCloseEvent, 10000);
		if(dwWaitRet == WAIT_TIMEOUT)
			pClass->SetTimeSync();
		else if(dwWaitRet == STATUS_WAIT_0)
			break;
	}
*/
	return 0;
}


void CUXCapIF::video_input_callback(UQ_VIDEO_INPUT_STATE& data)
{
	m_pObject->VideoStatusProc(data);
}

void CUXCapIF::video_preview_callback(UQ_VIDEO_PREVIEW_DATA& data)
{
	m_pObject->DisplayStreamProc( data );
}

void CUXCapIF::video_stream_callback(UQ_VIDEO_STREAM_DATA& data)
{
	if (data.enc)
	{
		m_pObject->EncodeStreamProc( data );
	}
}

void CUXCapIF::audio_stream_callback(UQ_AUDIO_STREAM_DATA& data)
{
	if (data.enc)
	{
		m_pObject->AudioStreamProc( data );
	}
}

void CUXCapIF::motion_data_callback(UQ_MOTION_STREAM_DATA& data)
{
	m_pObject->check_motion_data(data);
}

void CUXCapIF::irq_callback(UQ_INTERRUPT_DATA& data)
{
	if (data.irq_id == UQ_INT_GPIO)
	{
		m_pObject->SensorStatusProc(data);
	}
	else if (data.irq_id == UQ_INT_RTC)
	{
		m_pObject->SetOSDTime();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CUXCapIF

CUXCapIF::CUXCapIF()
{
	int		i;
	char	lpDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpDir);
	char	lpPath[MAX_PATH];
	wsprintf(lpPath, "%s\\Log", lpDir);
	//m_Log.SetLogDirectory(lpPath);
	//m_Log.SetSubDirectory("DVR\\UXCap");
	////m_Log.Log(TRUE, "----------------------------------------------------------------");

	m_nBoardCount = 0;
	m_nBoardType = VS_UNKNOWN;

	m_nMaxVideo = 0;
	m_nMaxAudio = 0;
	m_nMaxSensor = 0;
	m_nMaxRelay = 0;

	m_ImageFormat = UQ_CF_420_PL;

	ZeroMemory(&m_bVideoLock, sizeof(m_bVideoLock));
	ZeroMemory(&m_bVideoEnable, sizeof(m_bVideoEnable));

	ZeroMemory(m_nEncodeGOPSize, sizeof(m_nEncodeGOPSize));
	ZeroMemory(m_nEncodeFPS, sizeof(m_nEncodeFPS));
	ZeroMemory(m_nEncodeResolution, sizeof(m_nEncodeResolution));
	ZeroMemory(m_nEncodeLevel, sizeof(m_nEncodeLevel));

	ZeroMemory(&m_nDisplayResolutionMax, sizeof(m_nDisplayResolutionMax));
	memset(&m_nDisplayResolutionLevel, 0x01, sizeof(m_nDisplayResolutionLevel));
	ZeroMemory(&m_bDisplayStart, sizeof(m_bDisplayStart));

	m_osd_color = RGB(0,0,0);
	ZeroMemory(m_osd_show, sizeof(m_osd_show));
	ZeroMemory(m_osd_pos, sizeof(m_osd_pos));
	ZeroMemory(m_osd_text, sizeof(m_osd_text));

	ZeroMemory(m_brightness, sizeof(m_brightness));
	ZeroMemory(m_contrast, sizeof(m_contrast));
	ZeroMemory(m_saturation, sizeof(m_saturation));
	ZeroMemory(m_hue, sizeof(m_hue));
	ZeroMemory(m_sharpness, sizeof(m_sharpness));

	ZeroMemory(&m_bAudioEnable, sizeof(m_bAudioEnable));

	for (i=0;i<MAX_CAMERA_PER_BOARD;i++)
	{
		m_VideoFormat[i] = VF_NTSC;

		m_nEncodeMode[0][i] = UQ_RC_VBR;
		m_nEncodeMode[1][i] = UQ_RC_VBR;
	}

	ZeroMemory(m_motionstart, sizeof(m_motionstart));
	ZeroMemory(m_motioninfo, sizeof(m_motioninfo));
//	ZeroMemory(m_motion_data, sizeof(m_motion_data));
	ZeroMemory(m_tmLastMotionChecked, sizeof(m_tmLastMotionChecked));

	InitializeCriticalSection(&m_criticalSectionTVout);

	m_nTVoutSplit = 0;
	memset(m_nTVoutPort, 0xff, sizeof(m_nTVoutPort));

	CString	fn = _T("");
	fn.Format(_T("%s%s"), lpDir, _T("\\config\\etc.ini"));
//	m_nTVoutEnable = GetPrivateProfileInt("UQ3388", "tvout", 1, fn);
	m_nEnable3dnr = GetPrivateProfileInt("UQ3388", "3dnr", 0, fn);
	m_nEnable2dnr = GetPrivateProfileInt("UQ3388", "2dnr", 1, fn);
	m_nEnableSharpness = GetPrivateProfileInt("UQ3388", "sharpness", 1, fn);

//	if (m_nTVoutEnable != 0 && m_nTVoutEnable != 1)			m_nTVoutEnable = 0x01;
	if (m_nEnable3dnr != 0 && m_nEnable3dnr != 1)			m_nEnable3dnr = 0x00;
	if (m_nEnable2dnr != 0 && m_nEnable2dnr != 1)			m_nEnable2dnr = 0x01;
	if (m_nEnableSharpness != 0 && m_nEnableSharpness != 1)	m_nEnableSharpness = 0x01;

	for (i=0;i<MAX_DIO_BOARD;i++)
	{
		m_nDIDOBoardIndex[i] = 255;
	}

	m_pAddr_Encoded_callback = NULL;
	m_pAddr_Encoded_callback_Secondary = NULL;
	m_pAddr_Display_callback = NULL;
	m_pAddr_Audio_callback = NULL;


	m_last_bd_dts = NULL;

//	m_hCloseEvent = NULL;
//	m_hTimeCheckThread = NULL;

	m_uq_osd = new uq3388_osd_lib;
	m_uq_motion = new uq3388_motion_lib;
}

CUXCapIF::~CUXCapIF()
{
	DeleteCriticalSection(&m_criticalSectionTVout);

	if (uq_get_device_state() == UQ_STATE_RUN) {
		////m_Log.Log(TRUE, "ux::stop = %d", uq_stop());
	}
	if (uq_get_device_state() == UQ_STATE_SETUP) {
		////m_Log.Log(TRUE, "ux::unsetup = %d", uq_unsetup());
	}
	if (uq_get_device_state() == UQ_STATE_INIT) {
		////m_Log.Log(TRUE, "ux::uninit = %d", uq_uninit());
	}
	uq_destroy_instance();

	if (m_uq_osd) delete m_uq_osd;
	if (m_uq_motion) delete m_uq_motion;
}

void CUXCapIF::GetSystemDTS(__int64* dts)
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	SystemTimeToFileTime(&curtime, (FILETIME*)dts);
}


BEGIN_MESSAGE_MAP(CUXCapIF, CWnd)
	//{{AFX_MSG_MAP(CUXCapIF)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUXCapIF message handlers
void* CUXCapIF::CreateDevice(int type)
{
	BOOL	bret = TRUE;

	uq_get_instance();

	m_pObject = this;

	if (bret)
		return (void*)this;
	else
		return (void*)0;
}

void CUXCapIF::DestroyDevice()
{
	if (uq_get_device_state() == UQ_STATE_RUN) {
		////m_Log.Log(TRUE, "ux::stop = %d", uq_stop());
	}
	if (uq_get_device_state() == UQ_STATE_SETUP) {
		////m_Log.Log(TRUE, "ux::unsetup = %d", uq_unsetup());
	}
	if (uq_get_device_state() == UQ_STATE_INIT) {
		////m_Log.Log(TRUE, "ux::uninit = %d", uq_uninit());
	}
	uq_destroy_instance();

	delete this;
}

void CUXCapIF::GetQueryInterface(int idx)
{
	////m_Log.Log(TRUE, "ux::init() = %d", uq_init()); 

	m_dwStatus = API_ST_LOADED;
}

void CUXCapIF::ResetQueryInterface(int idx)
{
	if (m_dwStatus == API_ST_LOADED)
	{
		////m_Log.Log(TRUE, "ux::uninit() = %d", uq_uninit());

		m_dwStatus = API_ST_NOTHING;
	}
}

void CUXCapIF::SetVideoCallback(VIDEO_DATA_CALLBACK pAddr_Display_callback)
{
	m_pAddr_Display_callback = pAddr_Display_callback;
}

void CUXCapIF::SetEncodedCallback(VIDEO_DATA_CALLBACK pAddr_Encoded_callback, VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary)
{
	m_pAddr_Encoded_callback = pAddr_Encoded_callback;
	m_pAddr_Encoded_callback_Secondary = pAddr_Encoded_callback_Secondary;
}

void CUXCapIF::SetAudioCallback(AUDIO_DATA_CALLBACK pAddr_Display_callback)
{
	m_pAddr_Audio_callback = pAddr_Display_callback;
}

BOOL CUXCapIF::ExtractSystemInfo()
{
	if (m_dwStatus == API_ST_READY)
		return TRUE;
	else if (m_dwStatus != API_ST_LOADED)
	{
		////m_Log.Log(TRUE, "Not correct status(API_ST_LOADED) - ExtractSystemInfo");		
		return FALSE;
	}
	
	UINT8 nBoardCount = uq_get_board_count();

	if (nBoardCount == 0)
	{
		////m_Log.Log(TRUE, "It has not board. - ExtractSystemInfo");
		return FALSE;
	}
	if (nBoardCount > MAX_MULTI_BOARD) {
		nBoardCount = MAX_MULTI_BOARD;
	}
	//return TRUE;
	//if (nBoardCount > 0) {  //csnam 2022.11.18
	//	return TRUE;
	//}
	UINT32	venderID = 0;
	UINT8	codeR[LICENSE_LENGTH]	 = {0,};
	UINT8	code1204[LICENSE_LENGTH] = {'3','3','8','8',0x00,   0x00,0x00,0x00,0x00,0x00,  '1','2','0','4',0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  'S','T','D',0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00};
	UINT8	code2408[LICENSE_LENGTH] = {'3','3','8','8',0x00,   0x00,0x00,0x00,0x00,0x00,  '2','4','0','8',0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  'S','T','D',0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00};
	UINT8	code2416[LICENSE_LENGTH] = {'3','3','8','8',0x00,   0x00,0x00,0x00,0x00,0x00,  '2','4','1','6',0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  'S','T','D',0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00};
	UINT8	code4816[LICENSE_LENGTH] = {'3','3','8','8',0x00,   0x00,0x00,0x00,0x00,0x00,  '4','8','1','6',0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  'S','T','D',0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,  0x00};
	UINT8	i;
	UINT8	indexDIO = 0;

	m_nBoardCount = 0;
	m_nBoardType = VS_UNKNOWN;

	m_nMaxVideo = 0;
	m_nMaxAudio = 0;
	m_nMaxSensor = 0;
	m_nMaxRelay = 0;



	for (i=0;i<nBoardCount;i++)
	{
#if 1
		if (uq_get_vender_id(i, venderID) == FALSE || venderID != 0x00000001)		// 0x00000001 = vivako
		{
			////m_Log.Log(TRUE, "invalid get_vender_id = %x", venderID);
			return FALSE;
		}
#endif
		UINT8	j;
		for (j=0;j<LICENSE_LENGTH;j++)
		{
			if (uq_read_rom(i, j, codeR[j]) == FALSE)
			{
				////m_Log.Log(TRUE, "ux::read_rom = false");
				return FALSE;
			}
		}

		if (memcmp(codeR, code1204, LICENSE_LENGTH) == 0)
		{
			if (m_nBoardType == VS_UNKNOWN || m_nBoardType == BD_UQ3388_1204)
			{
				m_nBoardType = BD_UQ3388_1204;

				m_nBoardCount++;

				m_nMaxVideo += 4;
				m_nMaxAudio += 4;
			}
			else
			{
				////m_Log.Log(TRUE, "Not support board (code=3)");
				return FALSE;
			}
		}
		else if (memcmp(codeR, code2408, LICENSE_LENGTH) == 0)
		{
			if (m_nBoardType == VS_UNKNOWN || m_nBoardType == BD_UQ3388_2408)
			{
				m_nBoardType = BD_UQ3388_2408;

				m_nBoardCount++;

				m_nMaxVideo += 8;
				m_nMaxAudio += 8;
			}
			else
			{
				////m_Log.Log(TRUE, "Not support board (code=3)");
				return FALSE;
			}
		}
		else if (memcmp(codeR, code2416, LICENSE_LENGTH) == 0)
		{
			if (m_nBoardType == VS_UNKNOWN || m_nBoardType == BD_UQ3388_2416)
			{
				m_nBoardType = BD_UQ3388_2416;

				m_nBoardCount++;

				m_nMaxVideo += 16;
				m_nMaxAudio += 16;
			}
			else
			{
				////m_Log.Log(TRUE, "Not support board (code=3)");
				return FALSE;
			}
		}
		else if (memcmp(codeR, code4816, LICENSE_LENGTH) == 0)
		{
			if (m_nBoardType == VS_UNKNOWN || m_nBoardType == BD_UQ3388_4816)
			{
				m_nBoardType = BD_UQ3388_4816;

				m_nBoardCount++;

				m_nMaxVideo += 16;
				m_nMaxAudio += 16;
			}
			else
			{
				////m_Log.Log(TRUE, "Not support board (code=3)");
				return FALSE;
			}
		}
		else
		{
			char	szText[MAX_PATH];
			char	*p = szText;
			memset(szText, 0x00, sizeof(szText));

			for (int j=0;j<LICENSE_LENGTH;j++)
			{
				sprintf(p, " %02X", codeR[j]);
				p += 3;
			}
			////m_Log.Log(TRUE, "code(%d) = %s", i, szText);

			////m_Log.Log(TRUE, "Not support board (code=1)");
			return FALSE;
		}


		UINT8	dido_det;
		if (uq_detect_dido_board(i, dido_det) == TRUE && dido_det == 1)
		{
			if (indexDIO < MAX_DIO_BOARD) {
				m_nDIDOBoardIndex[indexDIO++] = i;

				m_nMaxSensor	+= 8;
				m_nMaxRelay		+= 8;
			}
		}
	}

	if (m_nMaxVideo == 0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CUXCapIF::SetUp(int idx)
{
	if (m_dwStatus == API_ST_READY)
		return TRUE;
	else if (m_dwStatus != API_ST_LOADED)
		return FALSE;

	if (ExtractSystemInfo() == FALSE)
	{
		////m_Log.Log(TRUE, "[Error] ExtractSystemInfo - false");
		return FALSE;
	}

	if (uq_setup() == TRUE)
	{
		////m_Log.Log(TRUE, "ux::setup() = 1");
	}
	else
	{
		////m_Log.Log(TRUE, "ux::setup() = false");
		return FALSE;
	}

	uq_set_video_preview_callback(video_preview_callback);
	uq_set_video_stream_callback(video_stream_callback);
	uq_set_audio_stream_callback(audio_stream_callback);
	uq_set_motion_data_callback(motion_data_callback);
	uq_set_irq_callback(irq_callback);
	uq_set_video_input_callback(video_input_callback);

	for (int i=0;i<MAX_DIO_BOARD;i++)
	{
		if (m_nDIDOBoardIndex[i] < 255) {
			////m_Log.Log(TRUE, "ux::init_dido (%d) = %d",	m_nDIDOBoardIndex[i], uq_init_dido(m_nDIDOBoardIndex[i]));
		}
	}


	if (SetUp_Video())
	{
		SetUp_Audio();

		m_dwStatus = API_ST_READY;
		return TRUE;
	}

	CleanUp_Video();
	return FALSE;
}

BOOL CUXCapIF::SetUp_Video()
{
	if (uq_set_color_format(m_ImageFormat) == FALSE)
	{
//		//m_Log.Log(TRUE, "color_format(%d) = false");
		return FALSE;
	}


//	char str[256] = "Language\\NVR.ttf";			// set_osd_palette
//	//m_Log.Log(TRUE, "osd_font(%s, 0, 24, 0, 0.5, 0.25) = %d", str, uq_set_osd_font(str, 0, 24, 0, 0.5, 0.25));
//	//m_Log.Log(TRUE, "osd_font(%s, 1, 36, 0, 0.5, 0.25) = %d", str, uq_set_osd_font(str, 1, 36, 0, 0.5, 0.25));
//	//m_Log.Log(TRUE, "osd_font(%s, 2, 48, 0, 0.5, 0.25) = %d", str, uq_set_osd_font(str, 2, 48, 0, 0.5, 0.25));
//	//m_Log.Log(TRUE, "osd_font(%s, 3, 72, 0, 0.5, 0.25) = %d", str, uq_set_osd_font(str, 3, 72, 0, 0.5, 0.25));

	return TRUE;
}

BOOL CUXCapIF::SetUp_Audio()
{
	return TRUE;
}

void CUXCapIF::CleanUp(int idx)
{
	CleanUp_Video();
	CleanUp_Audio();

	//m_Log.Log(TRUE, "ux::unsetup() = %d", uq_unsetup());

	m_dwStatus = API_ST_LOADED;
}

void CUXCapIF::CleanUp_Video()
{
}

void CUXCapIF::CleanUp_Audio()
{
}

void CUXCapIF::GetEventHandles(int idx, HANDLE handles[NUM_EVENTS])
{
	handles[0] = CreateEvent(0,0,0,0); // network - unused => auth error
	handles[1] = CreateEvent(0,0,0,0); // encoded image
	handles[2] = CreateEvent(0,0,0,0); // Video
	handles[3] = CreateEvent(0,0,0,0); // Audio
	handles[4] = CreateEvent(0,0,0,0); // Sensor
	handles[5] = CreateEvent(0,0,0,0); // VStatus

	memcpy(m_hNotifyHandles, handles, sizeof(HANDLE)*NUM_EVENTS);
}

void CUXCapIF::ReleaseEventHandles(int idx, HANDLE handles[NUM_EVENTS])
{
	for (int i=0; i<NUM_EVENTS; i++)
		CloseHandle(handles[i]);
}

BOOL CUXCapIF::Run(int idx)
{
	if (m_dwStatus == API_ST_RUN)
		return TRUE;


	if (Run_Audio() && Run_Video())
	{
		if (uq_run() == TRUE)
		{
			////m_Log.Log(TRUE, "ux::run() = 1");
		}
		else
		{
			////m_Log.Log(TRUE, "ux::run() = false");
			return FALSE;
		}
	}

/*	if(bUseOSD)
	{
		SetTimeSync();
		UINT dwThreadID;
		m_hCloseEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hTimeCheckThread = _beginthreadex(NULL, 0, TimeCheckProc, this, 0, &dwThreadID);
	}
*/

	m_dwStatus = API_ST_RUN;

	return TRUE;
}

BOOL CUXCapIF::Run_Video()
{
	UINT8						i;
	UQ_VIDEO_INPUT_STATE		s;

	for (i=0;i<m_nMaxVideo;i++)
	{
//		memset(&s, 0x00, sizeof(s));

//		uq_get_video_input_state(i, s);

//		//m_Log.Log(TRUE, "--------------------------------------------------");
//		//m_Log.Log(TRUE, "ux::get_video_input_state(%d) = %d", i, s.lock);

//		if (s.lock == 0) {
			m_bVideoLock[i] = FALSE;
//		}
//		else
//		{
//			SetVideoPort(s);

//			m_bVideoLock[i] = TRUE;
//		}
	}
	for (i=0;i<m_nMaxVideo;i++)
	{
		if (m_bVideoLock[i] == FALSE)
		{
			SetEvent(m_hNotifyHandles[EVENT_VIDEO_STATUS]);
		}
	}

	////m_Log.Log(TRUE, " ");

	uq_set_rtc_interrupt(3, TRUE); // 1 sec interrupt

	return TRUE;
}

BOOL CUXCapIF::Run_Audio()
{
	UINT32	sample_num = 8000;
	UINT32	sample_rate = 16000;
	UINT8	mode_8bit = 0;

//	//m_Log.Log(TRUE, "audio_encode_sample_rate(%d) = %d",sample_rate, uq_set_audio_encode_sample_rate(sample_rate));
//	//m_Log.Log(TRUE, "audio_encode_sample_num(%d) = %d",sample_num,   uq_set_audio_encode_sample_num(sample_num));
	uq_set_audio_encode_sample_rate(sample_rate);
	uq_set_audio_encode_sample_num(sample_num);
//	uq_set_audio_detection(0x00, 0x05, 3);

	for (UINT8 i=0;i<m_nMaxAudio;i++)
	{
		if (m_bAudioEnable[i])
		{
			UINT8 real_audio_id = GetAudioIndex(i);
//			uq_set_audio_interrupt(real_audio_id, AUDIO_ENCODE, TRUE);
			uq_set_audio_mute(real_audio_id, UQ_AUDIO_ENCODE, 0x00);
			uq_set_audio_codec(real_audio_id, UQ_AUDIO_ENCODE, UQ_AUDIO_CODEC_PCM, mode_8bit);
			uq_set_audio_enable(real_audio_id, UQ_AUDIO_ENCODE, m_bAudioEnable[i]);
		}
	}

	return TRUE;
}

void CUXCapIF::SetTimeSync()
{
	if (m_dwStatus != API_ST_RUN)
		return;

/*	time_t timer;
	struct tm *struct_time;
	for(DWORD i=0; i<m_nBoardCount; i++)
	{
		time(&timer);
		struct_time = gmtime(&timer);
		time_t time_update = timer - mktime(struct_time); 
		sdvr_err_e err = sdvr_set_date_time(i, timer+time_update);
		if(err != SDVR_ERR_NONE)
		{
			//m_Log.Log(TRUE, "[Error] SetTimeSync - sdvr_set_date_time - %d", err);
			
			Sleep(10);
			time(&timer);
			struct_time = gmtime(&timer);
			time_update = timer - mktime(struct_time); 
			err = sdvr_set_date_time(i, timer+time_update);
			//m_Log.Log(TRUE, "[Error] SetTimeSync - sdvr_set_date_time - %d", err);
		}
	}
*/
}

void CUXCapIF::Stop(int idx)
{
	m_dwStatus = API_ST_READY;

	Stop_Video();
	Stop_Audio();

	////m_Log.Log(TRUE, "ux::stop() = %d", uq_stop());

/*	if (m_hTimeCheckThread)
	{
		DWORD  dwExitCodeV;
		
		SetEvent(m_hCloseEvent);		// Thread Close Event
		while (::GetExitCodeThread((HANDLE)m_hTimeCheckThread, &dwExitCodeV))
		{
			if (dwExitCodeV != STILL_ACTIVE)
				break;
		}
		
		CloseHandle((HANDLE)m_hTimeCheckThread);
		m_hTimeCheckThread = NULL;
		CloseHandle(m_hCloseEvent);
		m_hCloseEvent = NULL;
	}
*/
}

void CUXCapIF::Stop_Video()
{
	int		i;
	UINT8	enable;

	EnterCriticalSection(&m_criticalSectionTVout);
	for(i = 0; i < m_nMaxVideo; i++)
	{
		uq_get_video_out_ch_enable(i, enable);
		if (enable)
		{
			uq_set_video_out_ch_enable_ext(i, m_nTVoutPort[i], 0x00);
			////m_Log.Log(TRUE, "set_video_out_ch_enable_ext(%d, %d, 0x00)", i, m_nTVoutPort[i]);
		}
	}
	//all board port disable
	for(i = 0; i < m_nBoardCount; i++){
		uq_set_video_out_hdmi_enable(i, 0x00);
		uq_set_video_out_cvbs_enable(i, 0x00);
	}
	m_nTVoutSplit = 0;
	memset(m_nTVoutPort, 0xff, sizeof(m_nTVoutPort));
	LeaveCriticalSection(&m_criticalSectionTVout);

	for (i=0;i<m_nMaxVideo;i++)
	{
		m_uq_osd->set_osd_box_enable(i, 0x00);
		m_uq_motion->set_channel_enable(i, 0x00);

		uq_set_video_enable(i, UQ_VIDEO_MAIN_PREVIEW, 0x00);
//		uq_set_video_enable(i, UQ_VIDEO_SUB_PREVIEW, 0x00);
		uq_set_video_enable(i, UQ_VIDEO_MAIN_STREAM, 0x00);
		uq_set_video_enable(i, UQ_VIDEO_SUB_STREAM, 0x00);
	}
}

void CUXCapIF::Stop_Audio()
{
	for (UINT8 i=0;i<m_nMaxAudio;i++)
	{
		UINT8 real_audio_id = GetAudioIndex(i);
		uq_set_audio_enable(real_audio_id, UQ_AUDIO_ENCODE, 0x00);
	}
//	//m_Log.Log(TRUE, "audio_enable(all, AUDIO_ENCODE, 0x00)");
}

int CUXCapIF::GetVSType(int idx)				{return VS_UBIQ_3388;}
int CUXCapIF::GetBoardType(int idx, int bd_idx)	{if (bd_idx >= GetBoardNum(idx)) {return 0;}	return m_nBoardType;}
int CUXCapIF::GetBoardNum(int idx)				{return 1;}
int CUXCapIF::GetCamNum(int idx, int bd_idx)	{return (bd_idx == 0 ? min(m_nMaxVideo, MAX_CAMERA_PER_BOARD) : 0);}
int CUXCapIF::GetAudioNum(int idx, int bd_idx)	{return (bd_idx == 0 ? min(m_nMaxAudio, MAX_AUDIO_PER_BOARD) : 0);}
int CUXCapIF::GetSensorNum(int idx, int bd_idx)	{return (bd_idx == 0 ? m_nMaxSensor : 0);}
int CUXCapIF::GetRelayNum(int idx, int bd_idx)	{return (bd_idx == 0 ? m_nMaxRelay : 0);}

BOOL CUXCapIF::GetCameraStatus(int idx, int bd_idx, ULONGLONG* pdata)
{
	ULONGLONG status = 0;
	int limit = GetCamNum(idx, bd_idx);
	for (int i=0; i<limit; i++) {
		status |= (((ULONGLONG) m_bVideoLock[i])<<i);
	}
	memcpy(pdata, &status, sizeof(ULONGLONG));
	////m_Log.Log(TRUE, "GetCameraStatus = %08x", status);
	return TRUE;
}

BOOL CUXCapIF::GetSensorStatus(int idx, int bd_idx, ULONG* pdata)
{
	UINT32	sensor = 0;
	UINT8	rdata;
	BOOL	bret;

	for (int i=0;i<MAX_DIO_BOARD;i++)
	{
		if (m_nDIDOBoardIndex[i] < 255) {
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 0, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+0));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 1, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+1));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 2, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+2));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 3, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+3));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 4, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+4));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 5, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+5));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 6, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+6));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 7, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+7));
		}
	}
	*pdata = sensor;
	return TRUE;
}

int CUXCapIF::GetSensorData(int idx, LPDATA_INFO pdata)
{
	pdata->vs_idx = idx;
	pdata->type = ACT_SALARM;
	pdata->bd_idx = 0;
	pdata->cam_idx = 0;
	pdata->data_size = sizeof(DWORD);		// sizeof(DWORD) * GetBoardNum(idx)
	pdata->pdata = new char[pdata->data_size];
#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	if (pdata->pdata == NULL)
		return 0;

	LPBYTE	p = (LPBYTE) pdata->pdata;
	UINT32	sensor = 0;
	UINT8	rdata;
	BOOL	bret;

	for (int i=0;i<MAX_DIO_BOARD;i++)
	{
		if (m_nDIDOBoardIndex[i] < 255) {
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 0, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+0));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 1, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+1));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 2, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+2));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 3, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+3));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 4, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+4));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 5, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+5));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 6, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+6));
			bret = uq_get_dido_input(m_nDIDOBoardIndex[i], 7, rdata);
			if (bret == TRUE && rdata == 1)
				sensor |= (rdata<<(i*8+7));
		}
	}
	TRACE("GetSensorData=%x \n", sensor);

	memcpy(p, &sensor, sizeof(DWORD));

	return TRUE;
}
void CUXCapIF::ReleaseSensorData(int idx, LPDATA_INFO pdata)	{delete[] pdata->pdata;}

int CUXCapIF::GetVStatusData(int idx, LPDATA_INFO pdata)
{
	ULONGLONG	status = 0;
	DWORD		i = 0;
	DWORD		limit = GetCamNum(idx, 0);

	for (i=0; i<limit; i++) {
		status |= ((ULONGLONG) m_bVideoLock[i])<<i;
	}
	
	pdata->vs_idx		= idx;
	pdata->type			= ACT_VALARM;
	pdata->bd_idx		= 0;
	pdata->cam_idx		= 0;
	pdata->data_size	= sizeof(ULONGLONG);
	pdata->pdata		= new char[pdata->data_size];

	if (pdata->pdata == NULL)
		return 0;

	memcpy(pdata->pdata, &status, sizeof(ULONGLONG));

	return 1;
}

void CUXCapIF::ReleaseVStatusData(int idx, LPDATA_INFO pdata)	{delete[] pdata->pdata;}

DWORD CUXCapIF::GetImageResType(int idx, int bd_id, int cam_id)
{
	DWORD	res_type = 0;

//	if (m_nEncodeResolution[0][cam_id].h >= 1080) {
//		res_type = 3;
//	} else if (m_nEncodeResolution[0][cam_id].h >= 720) {
//		res_type = 2;
//	} else
//	if (m_nEncodeResolution[0][cam_id].w >= 480) {
		res_type = 1;
//	} else {
//		res_type = 0;
//	}

	return res_type;
}

BOOL CUXCapIF::GetChannelResolution(int cam_id, int stream_idx, int *width, int *height)
{
	*width = m_nEncodeResolution[stream_idx][cam_id].w;
	*height = m_nEncodeResolution[stream_idx][cam_id].h;
	return TRUE;
}

void CUXCapIF::SetVideoPort( UQ_VIDEO_INPUT_STATE& data )
{
	UINT8	ch_id		= data.ch_id;

	uq_set_video_enable(ch_id, UQ_VIDEO_MAIN_PREVIEW, 0x00);
//	uq_set_video_enable(ch_id, UQ_VIDEO_SUB_PREVIEW, 0x00);
	uq_set_video_enable(ch_id, UQ_VIDEO_MAIN_STREAM, 0x00);
	uq_set_video_enable(ch_id, UQ_VIDEO_SUB_STREAM, 0x00);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// - encode resolution, fps
	// - display fps
	// all deinterlace
	UINT8	fps;
	UINT8	deinterlace;

	char	szType[16] = {0,};
	if (data.std == UQ_HD_SDI)				sprintf_s(szType, "HD_SDI");
	else if (data.std == UQ_EX_SDI)			sprintf_s(szType, "EX_SDI");
	else if (data.std == UQ_AHD)			sprintf_s(szType, "AHD");
	else if (data.std == UQ_AHDX)			sprintf_s(szType, "AHDX");
	else if (data.std == UQ_CVI)			sprintf_s(szType, "CVI");
	else if (data.std == UQ_CVIX)			sprintf_s(szType, "CVIX");
	else if (data.std == UQ_TVI)			sprintf_s(szType, "TVI");
	else if (data.std == UQ_NTSC)			sprintf_s(szType, "NTSC");
	else if (data.std == UQ_PAL)			sprintf_s(szType, "PAL");
	else									sprintf_s(szType, "%d", data.std);

	char	szMode[16] = {0,};
	if (data.mode == UQ_720P30)				sprintf_s(szMode, "720P30");
	else if (data.mode == UQ_1080P30)		sprintf_s(szMode, "1080P30");
	else if (data.mode == UQ_480i60)		sprintf_s(szMode, "480i60");
	else if (data.mode == UQ_576i60)		sprintf_s(szMode, "576i60");
	else if (data.mode == UQ_720P60)		sprintf_s(szMode, "720P60");
	else if (data.mode == UQ_720P25)		sprintf_s(szMode, "720P25");
	else if (data.mode == UQ_1080P25)		sprintf_s(szMode, "1080P25");
	else if (data.mode == UQ_1080P24)		sprintf_s(szMode, "1080P24");
	else if (data.mode == UQ_720P24)		sprintf_s(szMode, "720P24");
	else if (data.mode == UQ_720P50)		sprintf_s(szMode, "720P50");
	else									sprintf_s(szMode, "%d", data.mode);

	////m_Log.Log(TRUE, "video_info(%d) : std=%s, mode=%s, res(%d,%d)", ch_id, szType, szMode, data.sizex, data.sizey);
	////m_Log.Log(TRUE, " ");

	switch (data.std)
	{
	case UQ_NTSC:	m_VideoFormat[ch_id] = VF_NTSC;
					fps = min(data.fps, 30);
					deinterlace = data.interlace;
					m_nEncodeResolution[0][ch_id].w = min(data.sizex, 720);		// min(m_nEncodeResolution[0][ch_id].w, min(data.sizex, 720));
					m_nEncodeResolution[0][ch_id].h = min(data.sizey, 480);		// min(m_nEncodeResolution[0][ch_id].h, min(data.sizey, 480));
					m_nEncodeFPS[0][ch_id] = min(m_nEncodeFPS[0][ch_id], 30);
					m_nEncodeResolution[1][ch_id].w = min(m_nEncodeResolution[1][ch_id].w, min(data.sizex, 640));
					m_nEncodeResolution[1][ch_id].h = min(m_nEncodeResolution[1][ch_id].h, min(data.sizey, 480));
					m_nEncodeFPS[1][ch_id] = min(m_nEncodeFPS[1][ch_id], 30);
					break;
	case UQ_PAL:	m_VideoFormat[ch_id] = VF_PAL;
					fps = min(data.fps, 25);
					deinterlace = data.interlace;
					m_nEncodeResolution[0][ch_id].w = min(data.sizex, 720);		// min(m_nEncodeResolution[0][ch_id].w, min(data.sizex, 720));
					m_nEncodeResolution[0][ch_id].h = min(data.sizey, 576);		// min(m_nEncodeResolution[0][ch_id].h, min(data.sizey, 576));
					m_nEncodeFPS[0][ch_id] = min(m_nEncodeFPS[0][ch_id], 25);
					m_nEncodeResolution[1][ch_id].w = min(m_nEncodeResolution[1][ch_id].w, min(data.sizex, 640));
					m_nEncodeResolution[1][ch_id].h = min(m_nEncodeResolution[1][ch_id].h, min(data.sizey, 576));
					m_nEncodeFPS[1][ch_id] = min(m_nEncodeFPS[1][ch_id], 25);

					if (m_nEncodeResolution[0][ch_id].h == 480)			m_nEncodeResolution[0][ch_id].h = 576;
					else if (m_nEncodeResolution[0][ch_id].h == 240)	m_nEncodeResolution[0][ch_id].h = 288;

					if (m_nEncodeResolution[1][ch_id].h == 480)			m_nEncodeResolution[1][ch_id].h = 576;
					else if (m_nEncodeResolution[1][ch_id].h == 240)	m_nEncodeResolution[1][ch_id].h = 288;
					break;
	default :		m_VideoFormat[ch_id] = VF_NTSC;
					fps = data.fps;
					if (fps > 40) {
						fps = fps / 2;
					}
					deinterlace = data.interlace;
					m_nEncodeResolution[0][ch_id].w = data.sizex;				// min(m_nEncodeResolution[0][ch_id].w, data.sizex);
					m_nEncodeResolution[0][ch_id].h = data.sizey;				// min(m_nEncodeResolution[0][ch_id].h, data.sizey);
					m_nEncodeFPS[0][ch_id] = min(m_nEncodeFPS[0][ch_id], fps);
					m_nEncodeResolution[1][ch_id].w = min(m_nEncodeResolution[1][ch_id].w, min(data.sizex, 640));
					m_nEncodeResolution[1][ch_id].h = min(m_nEncodeResolution[1][ch_id].h, min(data.sizey, 480));
					m_nEncodeFPS[1][ch_id] = min(m_nEncodeFPS[1][ch_id], fps);
					break;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// - display resolution
	m_nDisplayResolutionMax[ch_id].w = data.sizex;
	m_nDisplayResolutionMax[ch_id].h = data.sizey;

	UINT16	displayX, displayY;
	GetDisplayResolution(m_nDisplayResolutionMax[ch_id].w, m_nDisplayResolutionMax[ch_id].h, m_nDisplayResolutionLevel[ch_id], displayX, displayY);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// - encode bitrate
	UINT32			dwBPS_table[3][5]	= { 1000000,   500000,   350000,   250000,  200000,		// cif
											2500000,  2000000,  1500000,  1000000,  800000,		// d1
											5000000,  4000000,  3000000,  2000000, 1000000};	// hd, full hd
	UINT32			bpsMainMax, bpsMainMin;
	UINT32			bpsSubMax, bpsSubMin;
	int				inc;

	if (m_nEncodeResolution[0][ch_id].h >= 720) {
		inc = 2;
	} else if (m_nEncodeResolution[0][ch_id].w >= 480) {
		inc = 1;
	} else {
		inc = 0;
	}
	if (m_nEncodeMode[0][ch_id] == UQ_RC_CBR) {
		bpsMainMax = dwBPS_table[inc][m_nEncodeLevel[0][ch_id]];
		bpsMainMin = bpsMainMax;
	} else {
		bpsMainMax = dwBPS_table[inc][m_nEncodeLevel[0][ch_id]] + dwBPS_table[inc][m_nEncodeLevel[0][ch_id]]/10;
		bpsMainMin = dwBPS_table[inc][m_nEncodeLevel[0][ch_id]] - dwBPS_table[inc][m_nEncodeLevel[0][ch_id]]/10;
	}


	if (m_nEncodeResolution[1][ch_id].h >= 640) {
		inc = 2;
	} else if (m_nEncodeResolution[1][ch_id].w >= 480) {
		inc = 1;
	} else {
		inc = 0;
	}
	if (m_nEncodeMode[0][ch_id] == UQ_RC_CBR) {
		bpsSubMax = dwBPS_table[inc][m_nEncodeLevel[1][ch_id]];
		bpsSubMin = bpsSubMax;
	} else {
		bpsSubMax = dwBPS_table[inc][m_nEncodeLevel[1][ch_id]] + dwBPS_table[inc][m_nEncodeLevel[1][ch_id]]/10;
		bpsSubMin = dwBPS_table[inc][m_nEncodeLevel[1][ch_id]] - dwBPS_table[inc][m_nEncodeLevel[1][ch_id]]/10;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	//m_Log.Log(TRUE, "video_port_mode(%d, %d, %d) = %d",		board_id, port, data.std,	uq_set_video_port_mode(board_id, port, data.std));
//	//m_Log.Log(TRUE, "video_port_enable(%d, %d, %d) = %d",	board_id, port, TRUE,		uq_set_video_port_enable(board_id, port, TRUE));


	uq_set_video_adjust(ch_id, m_brightness[ch_id], m_contrast[ch_id], m_saturation[ch_id], m_hue[ch_id]);
	uq_set_video_flip(ch_id, 0x00);
	uq_set_video_mirror(ch_id, 0x00);
	uq_set_video_deinterlace(ch_id, deinterlace);		// //m_Log.Log(TRUE, "video_deinterlace(%d, %d) = %d",	ch_id, deinterlace,			uq_set_video_deinterlace(ch_id, deinterlace));
	uq_set_video_2dnr(ch_id, m_nEnable2dnr);			// //m_Log.Log(TRUE, "video_2dnr(%d, %d) = %d",			ch_id, m_nEnable2dnr,		uq_set_video_2dnr(ch_id, m_nEnable2dnr));			// 0:bypass, 1:enable
	uq_set_video_2dnr_threshold(ch_id, 128, 0x01);
	uq_set_video_3dnr(ch_id, m_nEnable3dnr);			// //m_Log.Log(TRUE, "video_3dnr(%d, %d) = %d",			ch_id, m_nEnable3dnr,		uq_set_video_3dnr(ch_id, m_nEnable3dnr));
	uq_set_video_sharpness(ch_id, m_nEnableSharpness);	// //m_Log.Log(TRUE, "video_sharpness(%d, %d) = %d",		ch_id, m_nEnableSharpness,	uq_set_video_sharpness(ch_id, m_nEnableSharpness));	// 0:bypass, 1:enable
	uq_set_video_sharpness_alpha(ch_id, 15);				// 0-10:strength


	UINT8	ch_type = UQ_VIDEO_MAIN_PREVIEW;
	////m_Log.Log(TRUE, "video_resolution(%d, MAIN_PREVIEW, %d, %d) = %d",ch_id, displayX, displayY,	uq_set_video_resolution(ch_id, ch_type, displayX, displayY));
	////m_Log.Log(TRUE, "video_framerate(%d, MAIN_PREVIEW, %d) = %d",		ch_id, fps,					uq_set_video_framerate(ch_id, ch_type, fps));

	////m_Log.Log(TRUE, " ");

	ch_type = UQ_VIDEO_MAIN_STREAM;
	////m_Log.Log(TRUE, "video_resolution(%d, MAIN_STREAM, %d, %d) = %d",	ch_id, m_nEncodeResolution[0][ch_id].w, m_nEncodeResolution[0][ch_id].h,	uq_set_video_resolution(ch_id, ch_type, m_nEncodeResolution[0][ch_id].w, m_nEncodeResolution[0][ch_id].h));
	////m_Log.Log(TRUE, "video_framerate(%d, MAIN_STREAM, %d) = %d",		ch_id, m_nEncodeFPS[0][ch_id],		uq_set_video_framerate(ch_id, ch_type, m_nEncodeFPS[0][ch_id]));
	////m_Log.Log(TRUE, "video_gop(%d, MAIN_STREAM, %d) = %d",			ch_id, m_nEncodeFPS[0][ch_id],		uq_set_video_gop				(ch_id, ch_type, m_nEncodeFPS[0][ch_id]));		// m_nEncodeGOPSize[0][ch_id]
//	//m_Log.Log(TRUE, "video_skip(%d, MAIN_STREAM, %d) = %d",			ch_id, 0,							uq_set_video_skip			(ch_id, ch_type, 0));
//	//m_Log.Log(TRUE, "video_bit_rate_mode(%d, MAIN_STREAM, %d) = %d",	ch_id, m_nEncodeMode[0][ch_id],		uq_set_video_bit_rate_mode	(ch_id, ch_type, m_nEncodeMode[0][ch_id]));
//	//m_Log.Log(TRUE, "video_bit_rate_qp(%d, MAIN_STREAM, %d, %d) = %d",ch_id, 26, 28,						uq_set_video_bit_rate_qp		(ch_id, ch_type, 26, 28));
//	uq_set_video_gop				(ch_id, ch_type, m_nEncodeFPS[0][ch_id]);
	uq_set_video_skip			(ch_id, ch_type, 0);
	uq_set_video_bit_rate_mode	(ch_id, ch_type, m_nEncodeMode[0][ch_id]);
	uq_set_video_bit_rate_qp		(ch_id, ch_type, 26, 28);
	////m_Log.Log(TRUE, "video_bit_rate_bps(%d, MAIN_STREAM, %d, %d) = %d",ch_id, bpsMainMax, bpsMainMin,		uq_set_video_bit_rate_bps	(ch_id, ch_type, bpsMainMax, bpsMainMin));
//	uq_set_video_user_data		(ch_id, ch_type, 0x12345678, 0xAABBCCDD);

	////m_Log.Log(TRUE, " ");

	ch_type = UQ_VIDEO_SUB_STREAM;
	////m_Log.Log(TRUE, "video_resolution(%d, SUB_STREAM, %d, %d) = %d",	ch_id, m_nEncodeResolution[1][ch_id].w, m_nEncodeResolution[1][ch_id].h,	uq_set_video_resolution(ch_id, ch_type, m_nEncodeResolution[1][ch_id].w, m_nEncodeResolution[1][ch_id].h));
	////m_Log.Log(TRUE, "video_framerate(%d, SUB_STREAM, %d) = %d",		ch_id, m_nEncodeFPS[1][ch_id],		uq_set_video_framerate(ch_id, ch_type, m_nEncodeFPS[1][ch_id]));
	////m_Log.Log(TRUE, "video_gop(%d, SUB_STREAM, %d) = %d",				ch_id, m_nEncodeFPS[1][ch_id],		uq_set_video_gop				(ch_id, ch_type, m_nEncodeFPS[1][ch_id]));		// m_nEncodeGOPSize[1][ch_id]
//	//m_Log.Log(TRUE, "video_skip(%d, SUB_STREAM, %d) = %d",			ch_id, 0,							uq_set_video_skip			(ch_id, ch_type, 0));
//	//m_Log.Log(TRUE, "video_bit_rate_mode(%d, SUB_STREAM, %d) = %d",	ch_id, m_nEncodeMode[1][ch_id],		uq_set_video_bit_rate_mode	(ch_id, ch_type, m_nEncodeMode[1][ch_id]));
//	//m_Log.Log(TRUE, "video_bit_rate_qp(%d, SUB_STREAM, %d, %d) = %d",	ch_id, 26, 28,						uq_set_video_bit_rate_qp		(ch_id, ch_type, 26, 28));
//	uq_set_video_gop				(ch_id, ch_type, m_nEncodeFPS[1][ch_id]);
	uq_set_video_skip			(ch_id, ch_type, 0);
	uq_set_video_bit_rate_mode	(ch_id, ch_type, m_nEncodeMode[1][ch_id]);
	uq_set_video_bit_rate_qp		(ch_id, ch_type, 26, 28);
	////m_Log.Log(TRUE, "video_bit_rate_bps(%d, SUB_STREAM, %d, %d) = %d",ch_id, bpsSubMax, bpsSubMin,		uq_set_video_bit_rate_bps	(ch_id, ch_type, bpsSubMax, bpsSubMin));
//	uq_set_video_user_data		(ch_id, ch_type, 0x12345678, 0xAABBCCDD);

	////m_Log.Log(TRUE, " ");

	////m_Log.Log(TRUE, "video_enable(%d, MAIN_PREVIEW, %d) = %d",	ch_id, m_bVideoEnable[ch_id],	uq_set_video_enable(ch_id, UQ_VIDEO_MAIN_PREVIEW, m_bVideoEnable[ch_id]));
//	////m_Log.Log(TRUE, "video_enable(%d, SUB_PREVIEW, %d) = %d",	ch_id, 0x00,					uq_set_video_enable(ch_id, UQ_VIDEO_SUB_PREVIEW, 0x00));
	////m_Log.Log(TRUE, "video_enable(%d, MAIN_STREAM, %d) = %d",	ch_id, m_bVideoEnable[ch_id],	uq_set_video_enable(ch_id, UQ_VIDEO_MAIN_STREAM, m_bVideoEnable[ch_id]));
	////m_Log.Log(TRUE, "video_enable(%d, SUB_STREAM, %d) = %d",	ch_id, m_bVideoEnable[ch_id],	uq_set_video_enable(ch_id, UQ_VIDEO_SUB_STREAM, m_bVideoEnable[ch_id]));



	//UQ_MOTION_STREAM_DATA
	if (m_bVideoEnable[ch_id] && m_motionstart[ch_id])
	{
		EnableMotion(ch_id, TRUE);
	}


	////m_Log.Log(TRUE, " ");


	EnterCriticalSection(&m_criticalSectionTVout);
	if (m_nTVoutPort[ch_id] >= 0 && m_nTVoutPort[ch_id] < MAX_CAMERA_PER_BOARD)
	{
		uq_set_video_out_ch_enable_ext(ch_id, m_nTVoutPort[ch_id], 0x01);
		////m_Log.Log(TRUE, "set_video_out_ch_enable_ext(%d, %d, 0x01) in SetVideoPort", ch_id, m_nTVoutPort[ch_id]);
		////m_Log.Log(TRUE, " ");
	}
	LeaveCriticalSection(&m_criticalSectionTVout);


	ch_type = UQ_VIDEO_MAIN_PREVIEW;
//	UINT8	ch_type2 = UQ_VIDEO_MAIN_STREAM;
//	UINT8	ch_type3 = UQ_VIDEO_SUB_STREAM;
	if (m_bVideoEnable[ch_id] && m_osd_show[ch_id] == TRUE)
	{
#if 0
		UINT16	w;
		UINT16	h;
		GetDisplayResolution(m_nDisplayResolutionMax[ch_id].w, m_nDisplayResolutionMax[ch_id].h, 0, w, h);
#else
		UINT16	w = m_nDisplayResolutionMax[ch_id].w;
		UINT16	h = m_nDisplayResolutionMax[ch_id].h;
#endif
		UINT8	align_w;
//		UINT8	align_h;
		UINT8	box_id = 0;
		UINT8	font_id = GetOSDFontIndex(m_nDisplayResolutionMax[ch_id].w, m_nDisplayResolutionMax[ch_id].h);
//		int		font_h = (font_id == 3 ? (72+50) : ((font_id == 2 ? (48+30) : ((font_id == 1 ? (36+22) : (24+16))) ) ) );
		int		font_h = (font_id == 3 ? (60) : ((font_id == 2 ? (48) : ((font_id == 1 ? (36) : (24))) ) ) );
		RECT	rect;

		wchar_t		osd_str[128] = { 0 };
		CStringW	wstr(m_osd_text[ch_id]);

		int		lengthDesc = wstr.GetLength();
		int		lengthTime = 0;
		int		margin = 20;

		if (m_osd_time_type[ch_id] > 0)
		{
			lengthTime = 20;

			if ((margin*2)+(lengthTime*(font_h*2/3))+(lengthDesc*font_h) > w)
			{
				lengthDesc = (w - ((margin*2)+(lengthTime*(font_h*2/3)))) / font_h;
				wstr = wstr.Left(min(lengthDesc, wstr.GetLength()));
			}

			SYSTEMTIME	tm;
			GetLocalTime(&tm);
			switch (m_osd_time_type[ch_id]) {
			case 1: // mm-dd-yy
					swprintf(osd_str, L"%02d-%02d-%04d %02d:%02d:%02d %s", tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, wstr);
					break;
			case 2: // dd-mm-yy
					swprintf(osd_str, L"%02d-%02d-%04d %02d:%02d:%02d %s", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, wstr);
					break;
			case 3:	// yy-mm-dd
			default:swprintf(osd_str, L"%04d-%02d-%02d %02d:%02d:%02d %s", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, wstr);
					break;
			}
		}
		else
		{
			if ((margin*2)+(lengthDesc*font_h) > w)
			{
				lengthDesc = (w - (margin*2)) / font_h;
				wstr = wstr.Left(min(lengthDesc, wstr.GetLength()));
			}

			swprintf(osd_str, L"%s", wstr);
		}


		switch (m_osd_pos[ch_id]) {
		case 3 :	SetRect(&rect, w-(margin+(lengthTime*(font_h*2/3))+(lengthDesc*font_h)),  margin, w-margin, margin + font_h);
					align_w = UQ_OSD_ALIGN_RIGHT;
//					align_h = UQ_OSD_ALIGN_TOP;
					break;
		case 4 :	SetRect(&rect, margin,  h-margin-font_h, margin+(lengthTime*(font_h*2/3))+(lengthDesc*font_h), h-margin);
					align_w = UQ_OSD_ALIGN_LEFT;
//					align_h = UQ_OSD_ALIGN_BOTTOM;
					break;
		case 6 :	SetRect(&rect, w-(margin+(lengthTime*(font_h))+(lengthDesc*font_h)),  h-margin-font_h, w-margin, h-margin);
					align_w = UQ_OSD_ALIGN_RIGHT;
//					align_h = UQ_OSD_ALIGN_BOTTOM;
					break;
		case 1 :
		default :	SetRect(&rect, margin, margin, margin+(lengthTime*(font_h*2/3))+(lengthDesc*font_h), margin+font_h);
					align_w = UQ_OSD_ALIGN_LEFT;
//					align_h = UQ_OSD_ALIGN_TOP;
					break;
		}


//		m_uq_osd->set_osd_font_size(font_h);

		////m_Log.Log(TRUE, "set_osd_box(%d, (%d, %d, %d, %d), aln=%d, l=%d)",	ch_id, rect.left, rect.top, rect.right-rect.left, rect.bottom, align_w, lengthDesc);

		m_uq_osd->set_osd_box(ch_id, rect.left, rect.top, rect.right-rect.left, rect.bottom);
		m_uq_osd->set_osd_string_color(ch_id, m_osd_color);
		m_uq_osd->set_osd_box_enable(ch_id, 0x01);
		if (m_osd_time_type[ch_id] == 0)
		{
			m_uq_osd->set_osd_string(ch_id, osd_str, align_w, font_h);
		}
		////m_Log.Log(TRUE, "set_osd_box_enable(%d, 0x01) = 1",	ch_id);
	}
	else
	{
		m_uq_osd->set_osd_box_enable(ch_id, 0x00);
		////m_Log.Log(TRUE, "set_osd_box_enable(%d, 0x00) = 1",	ch_id);
	}
}

UINT8 CUXCapIF::GetAudioIndex( UINT8 in_virtual_index )
{
	if (m_nBoardType == BD_UQ3388_4816 || m_nBoardType == BD_UQ3388_2416)
	{
		if ( 4 <= in_virtual_index && in_virtual_index <=  7)			in_virtual_index += 4;
		else if ( 8 <= in_virtual_index && in_virtual_index <= 11)		in_virtual_index -= 4;
		else if (20 <= in_virtual_index && in_virtual_index <= 23)		in_virtual_index += 4;
		else if (24 <= in_virtual_index && in_virtual_index <= 27)		in_virtual_index -= 4;
		else if (36 <= in_virtual_index && in_virtual_index <= 39)		in_virtual_index += 4;
		else if (40 <= in_virtual_index && in_virtual_index <= 43)		in_virtual_index -= 4;
		else if (52 <= in_virtual_index && in_virtual_index <= 55)		in_virtual_index += 4;
		else if (56 <= in_virtual_index && in_virtual_index <= 59)		in_virtual_index -= 4;
	}
	return in_virtual_index;
}

void CUXCapIF::GetDisplayResolution( int in_x, int in_y, UINT8 level, UINT16& out_x, UINT16& out_y )
{
	if (m_nBoardType == BD_UQ3388_1204)
	{
		if (in_y >= 1080) {
			if (level == 0)			{	out_x = min(1920,in_x);	out_y = 1080;	}
			else if (level == 1)	{	out_x = min( 960,in_x);	out_y = 540;	}
			else					{	out_x = min( 480,in_x);	out_y = 270;	}
		}
		else if (in_y >= 720) {
			if (level == 0)			{	out_x = min(1280,in_x);	out_y = 720;	}
			else if (level == 1)	{	out_x = min( 640,in_x);	out_y = 360;	}
			else					{	out_x = min( 320,in_x);	out_y = 240;	}
		}
		else if (in_y == 576) {
			if (level == 0)			{	out_x = min( 720,in_x);	out_y = 576;	}
			else if (level == 1)	{	out_x = min( 720,in_x);	out_y = 576;	}
			else					{	out_x = min( 360,in_x);	out_y = 288;	}
		}
		else if (in_y >= 480) {
			if (level == 0)			{	out_x = min( 720,in_x);	out_y = 480;	}
			else if (level == 1)	{	out_x = min( 720,in_x);	out_y = 480;	}
			else					{	out_x = min( 360,in_x);	out_y = 240;	}
		}
		else if (in_y == 288) {
			if (level == 0)			{	out_x = min( 360,in_x);	out_y = 288;	}
			else if (level == 1)	{	out_x = min( 360,in_x);	out_y = 288;	}
			else					{	out_x = min( 360,in_x);	out_y = 288;	}
		}
		else {
			if (level == 0)			{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}
			else if (level == 1)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}
			else					{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}
		}
	}
	else if (m_nBoardType == BD_UQ3388_4816)
	{
		if (in_y >= 1080) {
			if (level == 0)			{	out_x = min(1920,in_x);	out_y = 1080;	}
			else if (level == 1)	{	out_x = min( 640,in_x);	out_y = 270;	}				//  9 split - 1/12
			else if (level == 2)	{	out_x = min( 480,in_x);	out_y = 270;	}				// 25 split - 1/16
			else if (level == 3)	{	out_x = min( 320,in_x);	out_y = 180;	}				// 36 split - 1/36
			else					{	out_x = min( 320,in_x);	out_y = 180;	}				//			- 1/36
		}
		else if (in_y >= 720) {
			if (level == 0)			{	out_x = min(1280,in_x);	out_y = 720;	}
			else if (level == 1)	{	out_x = min( 640,in_x);	out_y = 240;	}				//  9 split - 1/6
			else if (level == 2)	{	out_x = min( 320,in_x);	out_y = 240;	}				// 25 split - 1/12
			else if (level == 3)	{	out_x = min( 320,in_x);	out_y = 240;	}				// 36 split - 1/12
			else					{	out_x = min( 320,in_x);	out_y = 240;	}				//			- 1/12
		}
		else if (in_y == 576) {
			if (level == 0)			{	out_x = min( 720,in_x);	out_y = 576;	}
			else if (level == 1)	{	out_x = min( 540,in_x);	out_y = 288;	}				//  9 split - 3/8
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 25 split - 1/4
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 36 split - 1/4
			else					{	out_x = min( 360,in_x);	out_y = 288;	}				//			- 1/4
		}
		else if (in_y >= 480) {
			if (level == 0)			{	out_x = min( 720,in_x);	out_y = 480;	}
			else if (level == 1)	{	out_x = min( 540,in_x);	out_y = 360;	}				//  9 split - 9/16
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = 240;	}				// 25 split - 1/4
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = 240;	}				// 36 split - 1/4
			else					{	out_x = min( 360,in_x);	out_y = 240;	}				//			- 1/4
		}
		else if (in_y == 288) {
			if (level == 0)			{	out_x = min( 360,in_x);	out_y = 288;	}
			else if (level == 1)	{	out_x = min( 360,in_x);	out_y = 288;	}				//  9 split - 1
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 25 split - 1
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 36 split - 1
			else					{	out_x = min( 360,in_x);	out_y = 288;	}				//			- 1
		}
		else {
			if (level == 0)			{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}
			else if (level == 1)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		//  9 split - 1
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		// 25 split - 1
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		// 36 split - 1
			else					{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		//			- 1
		}
	}
	else
	{
		if (in_y >= 1080) {
			if (level == 0)			{	out_x = min(1920,in_x);	out_y = 1080;	}
			else if (level == 1)	{	out_x = min( 800,in_x);	out_y = 450;	}				//  9 split - 1/(2.4*2.4)
			else if (level == 2)	{	out_x = min( 480,in_x);	out_y = 270;	}				// 25 split - 1/16
			else if (level == 3)	{	out_x = min( 320,in_x);	out_y = 270;	}				// 36 split - 1/24
			else					{	out_x = min( 320,in_x);	out_y = 270;	}				//			- 1/24
		}
		else if (in_y >= 720) {
			if (level == 0)			{	out_x = min(1280,in_x);	out_y = 720;	}
			else if (level == 1)	{	out_x = min( 640,in_x);	out_y = 360;	}				//  9 split - 1/4
			else if (level == 2)	{	out_x = min( 320,in_x);	out_y = 240;	}				// 25 split - 1/12
			else if (level == 3)	{	out_x = min( 320,in_x);	out_y = 240;	}				// 36 split - 1/12
			else					{	out_x = min( 320,in_x);	out_y = 240;	}				//			- 1/12
		}
		else if (in_y == 576) {
			if (level == 0)			{	out_x = min( 720,in_x);	out_y = 576;	}
			else if (level == 1)	{	out_x = min( 540,in_x);	out_y = 432;	}				//  9 split - 9/16
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 25 split - 1/4
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 36 split - 1/4
			else					{	out_x = min( 360,in_x);	out_y = 288;	}				//			- 1/4
		}
		else if (in_y >= 480) {
			if (level == 0)			{	out_x = min( 720,in_x);	out_y = 480;	}
			else if (level == 1)	{	out_x = min( 540,in_x);	out_y = 360;	}				//  9 split - 9/16
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = 240;	}				// 25 split - 1/4
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = 240;	}				// 36 split - 1/4
			else					{	out_x = min( 360,in_x);	out_y = 240;	}				//			- 1/4
		}
		else if (in_y == 288) {
			if (level == 0)			{	out_x = min( 360,in_x);	out_y = 288;	}
			else if (level == 1)	{	out_x = min( 360,in_x);	out_y = 288;	}				//  9 split - 1
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 25 split - 1
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = 288;	}				// 36 split - 1
			else					{	out_x = min( 360,in_x);	out_y = 288;	}				//			- 1
		}
		else {
			if (level == 0)			{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}
			else if (level == 1)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		//  9 split - 1
			else if (level == 2)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		// 25 split - 1
			else if (level == 3)	{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		// 36 split - 1
			else					{	out_x = min( 360,in_x);	out_y = min(240,in_y);	}		//			- 1
		}
	}
}

UINT8 CUXCapIF::GetOSDFontIndex( int in_x, int in_y)
{
	if (in_y >= 1080) {
		return 3;
	}
	else if (in_y >= 720) {
		return 2;
	}
	else if (in_y >= 480) {
		return 1;
	}
	return 0;
}

void CUXCapIF::SetOSDTime()
{
//	UINT8		box_id  = 0;
//	UINT8		font_id;
	UINT8		align_w;
//	UINT8		align_h;
	wchar_t		osd_str[128] = { 0 };
	BOOL		mv_enable;
//	tm			time;
//	uq_get_rtc_time(&time);

	SYSTEMTIME	tm;
	GetLocalTime(&tm);

	for (UINT8 i=0;i<m_nMaxVideo;i++)
	{
		if (m_osd_show[i] && m_osd_time_type[i] > 0)
		{
			mv_enable = FALSE;

			uq_get_video_enable(i, UQ_VIDEO_MAIN_PREVIEW, mv_enable);

			if (mv_enable)
			{
#if 0
				UINT16	w;
				UINT16	h;
				GetDisplayResolution(m_nDisplayResolutionMax[ch_id].w, m_nDisplayResolutionMax[ch_id].h, 0, w, h);
#else
				UINT16	w = m_nDisplayResolutionMax[i].w;
				UINT16	h = m_nDisplayResolutionMax[i].h;
#endif

				UINT8	font_id = GetOSDFontIndex(m_nDisplayResolutionMax[i].w, m_nDisplayResolutionMax[i].h);
				int		font_h = (font_id == 3 ? (60) : ((font_id == 2 ? (48) : ((font_id == 1 ? (36) : (24))) ) ) );

				CStringW	wstr(m_osd_text[i]);

				int		lengthDesc = wstr.GetLength();
				int		lengthTime = 0;
				int		margin = 20;
				if (m_osd_time_type[i] > 0)
				{
					lengthTime = 20;

					if ((margin*2)+(lengthTime*(font_h*2/3))+(lengthDesc*font_h) > w)
					{
						lengthDesc = (w - ((margin*2)+(lengthTime*(font_h*2/3)))) / font_h;
						wstr = wstr.Left(min(lengthDesc, wstr.GetLength()));
					}

					SYSTEMTIME	tm;
					GetLocalTime(&tm);

					switch (m_osd_time_type[i]) {
					case 1: // mm-dd-yy
							swprintf(osd_str, L"%02d-%02d-%04d %02d:%02d:%02d %s", tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, wstr);
							break;
					case 2: // dd-mm-yy
							swprintf(osd_str, L"%02d-%02d-%04d %02d:%02d:%02d %s", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, wstr);
							break;
					case 3:	// yy-mm-dd
					default:swprintf(osd_str, L"%04d-%02d-%02d %02d:%02d:%02d %s", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, wstr);
							break;
					}
				}
				else
				{
					if ((margin*2)+(lengthDesc*font_h) > w)
					{
						lengthDesc = (w - (margin*2)) / font_h;
						wstr = wstr.Left(min(lengthDesc, wstr.GetLength()));
					}

					swprintf(osd_str, L"%s", wstr);
				}


				switch (m_osd_pos[i]) {
				case 3 :	align_w = UQ_OSD_ALIGN_RIGHT;
//							align_h = UQ_OSD_ALIGN_TOP;
							break;
				case 4 :	align_w = UQ_OSD_ALIGN_LEFT;
//							align_h = UQ_OSD_ALIGN_BOTTOM;
							break;
				case 6 :	align_w = UQ_OSD_ALIGN_RIGHT;
//							align_h = UQ_OSD_ALIGN_BOTTOM;
							break;
				case 1 :
				default :	align_w = UQ_OSD_ALIGN_LEFT;
//							align_h = UQ_OSD_ALIGN_TOP;
							break;
				}


				m_uq_osd->set_osd_string(i, osd_str, align_w, font_h);
			}
		}
	}
}

void CUXCapIF::VideoStatusProc( UQ_VIDEO_INPUT_STATE& data )
{
	UINT8	ch_id		= data.ch_id;

	////m_Log.Log(TRUE, "--------------------------------------------------");
	////m_Log.Log(TRUE, "VieoStatus  ch_id=%d, lock=%d", ch_id, data.lock);

	if (data.lock)
	{
		SetVideoPort(data);
	}
	else
	{
//		UINT8	box_id = 0;
//		uq_set_osdbox_enable(ch_id, UQ_VIDEO_MAIN_PREVIEW, box_id, 0x00);
//		uq_set_osd_enable(ch_id, UQ_VIDEO_MAIN_PREVIEW, 0x00);

		UINT8	enable;
		uq_get_video_out_ch_enable(ch_id, enable);
		if (enable)
		{
			EnterCriticalSection(&m_criticalSectionTVout);
			uq_set_video_out_ch_enable_ext(ch_id, m_nTVoutPort[ch_id], 0x00);
			////m_Log.Log(TRUE, "set_video_out_ch_enable_ext(%d, %d, 0x00)", ch_id, m_nTVoutPort[ch_id]);
			LeaveCriticalSection(&m_criticalSectionTVout);
		}

		uq_set_video_enable(ch_id, UQ_VIDEO_MAIN_PREVIEW, 0x00);
//		uq_set_video_enable(ch_id, UQ_VIDEO_SUB_PREVIEW, 0x00);
		uq_set_video_enable(ch_id, UQ_VIDEO_MAIN_STREAM, 0x00);
		uq_set_video_enable(ch_id, UQ_VIDEO_SUB_STREAM, 0x00);
	}

	if (m_bVideoLock[ch_id] != data.lock)
	{
		m_bVideoLock[ch_id] = data.lock;
		SetEvent(m_hNotifyHandles[EVENT_VIDEO_STATUS]);
	}
}

void CUXCapIF::DisplayStreamProc( UQ_VIDEO_PREVIEW_DATA& data )
{
//	TRACE("VieoStatusProc  cam_idx=%d, x=%d, y=%d \n", data.ch_id, data.size_x, data.size_y);

	if (m_dwStatus != API_ST_RUN)
		return;

	if (m_bVideoLock[data.ch_id] == FALSE)
	{	// video loss 상태에서도 데이터가 올라와서 ..
		return;
	}

	if (data.ch_type == UQ_VIDEO_MAIN_PREVIEW)
	{
		if (m_dwStatus == API_ST_RUN && m_pAddr_Display_callback)
		{
			DATA_INFO data_info_raw;
			ZeroMemory(&data_info_raw, sizeof(data_info_raw));

			data_info_raw.vs_idx		= 0;
			data_info_raw.bd_idx		= 0;
			data_info_raw.cam_idx		= data.ch_id;
			data_info_raw.type			= ACT_RAWIMAGE;
			data_info_raw.frame_type	= FT_VIDEO;
			data_info_raw.res.w			= data.size_x;
			data_info_raw.res.h			= data.size_y;
			data_info_raw.pdata			= data.video_y;
			data_info_raw.data_size		= data.size_x * data.size_y * 3 / 2;

//			LPBYTE p420Y, p420U, p420V;
//			data_info_raw.pdata = (LPVOID) new BYTE[data_info_raw.res.w*data_info_raw.res.h*3/2];
//			p420Y = (unsigned char *) data_info_raw.pdata;
//			p420U = p420Y + (data_info_raw.res.w*data_info_raw.res.h);
//			p420V = p420U + (data_info_raw.res.w*data_info_raw.res.h)/4;
//			memcpy(p420Y, data.video_y, (data_info_raw.res.w*data_info_raw.res.h));
//			memcpy(p420U, data.video_u, (data_info_raw.res.w*data_info_raw.res.h)/4);
//			memcpy(p420V, data.video_v, (data_info_raw.res.w*data_info_raw.res.h)/4);

			m_pAddr_Display_callback(&data_info_raw);

//			delete [] data_info_raw.pdata;
		}
	}
}

void CUXCapIF::EncodeStreamProc(UQ_VIDEO_STREAM_DATA& data)
{
	if (m_bVideoLock[data.ch_id] == FALSE)
	{	// video loss 상태에서도 압축데이터가 올라와서 ..
		return;
	}
	// 150421 tizone IP 카메라와의 형평성을 위해 시스템 시간을 가져와 dts 로 사용한다.
	GetSystemDTS(&m_last_bd_dts);

	DATA_INFO	data_info;

	if (data.ch_type == UQ_VIDEO_MAIN_STREAM)
	{
		if (m_dwStatus == API_ST_RUN && m_pAddr_Encoded_callback)
		{
			UINT8	cam_id = data.ch_id;
#if 0
			if (m_motionstart[cam_id])
			{
				RECT	rect;
				BOOL	bfDetected = FALSE;
				if (m_motioninfo[cam_id].bPatialArea == FALSE) {
					if (data.region.detected[0] == TRUE) {
						bfDetected = TRUE;
						rect = data.region.rect[0];
					}
				} else {
					int max_rect = min(m_motioninfo[cam_id].window_count, MAX_MOTION_REGION);

					for (int i=0;i<max_rect;i++)
					{
						if (data.region.detected[i] == TRUE) {
							bfDetected = TRUE;
							rect = data.region.rect[i];
							break;
						}
					}
				}

				if (bfDetected)
				{
					TRACE("motion data : ch-%d, detect-%x  (%d,%d,%d,%d) \n", cam_id, data.region.detected[0], rect.left, rect.top, rect.right, rect.bottom);
#if 0
					DWORD	mot;
					UINT32	*p = data.motion;

					for (int i=0;i<16;i++)
					{
						TRACE("              ");
						memcpy(&mot, p+i, sizeof(DWORD));
						for (int j=0;j<16;j++)
						{
							if ((mot >> (15-j)) & 0x00000001)
								TRACE("X");
							else
								TRACE("o");
						}
						TRACE("\n");
					}
#endif
					// motion data
					ZeroMemory(&data_info, sizeof(data_info));
					data_info.vs_idx		= 0;
					data_info.bd_idx		= 0;
					data_info.cam_idx		= cam_id;
					data_info.type			= ACT_CODIMAGE;
					data_info.data_size		= 1;
					data_info.pdata			= NULL;
					data_info.frame_type	= FT_MD_FULL;
					GetChannelResolution(data_info.cam_idx, 0, &data_info.res.w, &data_info.res.h);

					m_pAddr_Encoded_callback(&data_info);
				}
			}
#endif

			// stream data
			ZeroMemory(&data_info, sizeof(data_info));
			data_info.vs_idx		= 0;
			data_info.bd_idx		= 0;
			data_info.cam_idx		= cam_id;
			data_info.type			= ACT_CODIMAGE;
			data_info.data_size		= data.size;
			data_info.pdata			= data.vstream;
			data_info.frame_type	= (data.key_frame? FT_H264_I:FT_H264_P);
			data_info.dts			= m_last_bd_dts;
			GetChannelResolution(data_info.cam_idx, 0, &data_info.res.w, &data_info.res.h);

			m_pAddr_Encoded_callback(&data_info);
		}
	}
	else if (data.ch_type == UQ_VIDEO_SUB_STREAM)
	{
		if (m_dwStatus == API_ST_RUN && m_pAddr_Encoded_callback_Secondary)
		{
			ZeroMemory(&data_info, sizeof(data_info));
			data_info.vs_idx		= 0;
			data_info.bd_idx		= 0;
			data_info.cam_idx		= data.ch_id;
			data_info.type			= ACT_CODIMAGE;
			data_info.data_size		= data.size;
			data_info.pdata			= data.vstream;
			data_info.frame_type	= (data.key_frame? FT_H264_I:FT_H264_P);
			data_info.dts			= m_last_bd_dts;
			GetChannelResolution(data_info.cam_idx, 1, &data_info.res.w, &data_info.res.h);

			m_pAddr_Encoded_callback_Secondary(&data_info);
		}
	}
}

void CUXCapIF::AudioStreamProc( UQ_AUDIO_STREAM_DATA& data )
{
	if (m_dwStatus != API_ST_RUN)
		return;

	if (data.ch_type == UQ_AUDIO_ENCODE)
	{
		if (m_dwStatus == API_ST_RUN && m_pAddr_Audio_callback)
		{
			DATA_INFO	data_info_audio;
			ZeroMemory(&data_info_audio, sizeof(data_info_audio));

			data_info_audio.vs_idx		= 0;
			data_info_audio.bd_idx		= 0;
			data_info_audio.cam_idx		= GetAudioIndex(data.ch_id);
			data_info_audio.type		= ACT_AUDIO;
			data_info_audio.frame_type	= FT_AUDIO;
			data_info_audio.data_size	= data.size - 32;
			data_info_audio.pdata		= data.astream + 32;
			// 150421 tizone IP 카메라와의 형평성을 위해 시스템 시간을 가져와 dts 로 사용한다.
			//sx_uint64 longTimeStamp = (sx_uint64)raw_buf->timestamp | ((sx_uint64)raw_buf->timestamp_high << 32);
			//data_info_audio.dts = longTimeStamp*100;
			GetSystemDTS(&data_info_audio.dts);

			m_pAddr_Audio_callback(&data_info_audio);
		}
	}
}

void CUXCapIF::SensorStatusProc(UQ_INTERRUPT_DATA& data)
{
	if (m_dwStatus != API_ST_RUN)
		return;

	UINT32	board_id = data.board_id;
	UINT8	rdata;

	for (int i=0;i<MAX_DIO_BOARD;i++)
	{
		if (board_id == m_nDIDOBoardIndex[i])
		{
			if ((data.irq_data[6] >> 4) & 0x01) {
				uq_get_dido_input(board_id, 0, rdata);
				TRACE("BOARD %d DIDO input 0 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 5) & 0x01) {
				uq_get_dido_input(board_id, 1, rdata);
				TRACE("BOARD %d DIDO input 1 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 6) & 0x01) {
				uq_get_dido_input(board_id, 2, rdata);
				TRACE("BOARD %d DIDO input 2 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 7) & 0x01) {
				uq_get_dido_input(board_id, 3, rdata);
				TRACE("BOARD %d DIDO input 3 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 8) & 0x01) {
				uq_get_dido_input(board_id, 4, rdata);
				TRACE("BOARD %d DIDO input 4 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 9) & 0x01) {
				uq_get_dido_input(board_id, 5, rdata);
				TRACE("BOARD %d DIDO input 5 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 10) & 0x01) {
				uq_get_dido_input(board_id, 6, rdata);
				TRACE("BOARD %d DIDO input 6 DET %d\n", board_id, rdata);
			}
			if ((data.irq_data[6] >> 11) & 0x01) {
				uq_get_dido_input(board_id, 7, rdata);
				TRACE("BOARD %d DIDO input 7 DET %d\n", board_id, rdata);
			}

			SetEvent(m_hNotifyHandles[EVENT_SENSOR]);
		}
	}
}

BOOL CUXCapIF::EnableAudio(int idx, int bd_id, int aud_id, BOOL bEnable)
{
	BOOL	bBackup = m_bAudioEnable[aud_id];
	m_bAudioEnable[aud_id] = bEnable;

	if (m_dwStatus == API_ST_RUN && bBackup != bEnable)
	{
		UINT8 real_audio_id = GetAudioIndex(aud_id);
		////m_Log.Log(TRUE, "audio_enable(%d, AUDIO_ENCODE, %d) = %d", aud_id, bEnable,		uq_set_audio_enable(real_audio_id, UQ_AUDIO_ENCODE, bEnable));
	}

	return TRUE;
}

BOOL CUXCapIF::EnableVideo(int idx, int bd_id, int cam_id, BOOL bEnable)
{
//	//m_Log.Log(TRUE, "CUXCapIF::EnableVideo()    : cam_id=%d, enable=%d", cam_id, bEnable);

	BOOL	bBackup = m_bVideoEnable[cam_id];
	m_bVideoEnable[cam_id] = bEnable;

	if (m_dwStatus == API_ST_RUN && bBackup != bEnable)
	{
		////m_Log.Log(TRUE, "video_enable(%d, VIDEO_MAIN_PREVIEW, %d) = %d",cam_id, bEnable,	uq_set_video_enable(cam_id, UQ_VIDEO_MAIN_PREVIEW, bEnable));
//		////m_Log.Log(TRUE, "video_enable(%d, VIDEO_SUB_PREVIEW, %d) = %d",	cam_id, 0x00,		uq_set_video_enable(cam_id, UQ_VIDEO_SUB_PREVIEW, 0x00));
		////m_Log.Log(TRUE, "video_enable(%d, VIDEO_MAIN_STREAM, %d) = %d",	cam_id, bEnable,	uq_set_video_enable(cam_id, UQ_VIDEO_MAIN_STREAM, bEnable));
		////m_Log.Log(TRUE, "video_enable(%d, VIDEO_SUB_STREAM, %d) = %d",	cam_id, bEnable,	uq_set_video_enable(cam_id, UQ_VIDEO_SUB_STREAM, bEnable));
	}
	return TRUE;
}

BOOL CUXCapIF::EnableMotionDetection(int idx, int bd_id, int cam_id, BOOL bEnable, BOOL bDetectMotionDisp, MOTIONINFO motion_info)
{
//	TRACE("\nCUXCapIF::EnableMotionDetection() : cam_id=%d, bEnable=%d \n", cam_id, bEnable);

//	//m_Log.Log(TRUE, "CUXCapIF::EnableMotionDetection() : cam_id=%d, bEnable=%d", cam_id, bEnable);

	if (bEnable) {
		TRACE("          motion info : partial = %d \n", motion_info.bPatialArea);
		TRACE("                        rect count = %d \n", motion_info.window_count);
		if (motion_info.bPatialArea == 0)
		{
			TRACE("                        sensortivity = %d \n", motion_info.motion_window[0].motion.sensitivity);
		}
		else {
			for (int i=0;i<motion_info.window_count;i++)
			{
				TRACE("                        sensortivity = %d \n", motion_info.motion_window[i].motion.sensitivity);
				TRACE("                        rect %2d = (%d,%d,%d,%d) \n", i, motion_info.motion_window[i].rect.left,motion_info.motion_window[i].rect.top, motion_info.motion_window[i].rect.right, motion_info.motion_window[i].rect.bottom);
			}
		}
	}

	m_motioninfo[cam_id] = motion_info;
	m_motionstart[cam_id] = bEnable;

	EnableMotion(cam_id, bEnable);

	return TRUE;
}

BOOL CUXCapIF::EnableMotion(int cam_id, BOOL bEnable)
{
//	if (m_dwStatus != API_ST_RUN)
//		return TRUE;

	if (bEnable)
	{
		UINT8	ch_id = cam_id;
		UINT16	width=0, height=0;
		BOOL	enabled = FALSE;

		uq_get_video_enable(ch_id, UQ_VIDEO_SUB_STREAM, enabled);
		if (enabled)
		{
			uq_get_video_resolution(ch_id, UQ_VIDEO_SUB_STREAM, width, height);
			if (width>0 && height>0)
			{
				UINT16	sensortivity = 0;
				RECT	rect[MOTION_MAX_REGION] = {0,};
				UINT8	max_rect = 1;
				UINT8	i;

				if (m_motioninfo[ch_id].bPatialArea)
				{
					max_rect = min(m_motioninfo[ch_id].window_count, MOTION_MAX_REGION);

					for (i=0;i<max_rect;i++)
					{
						GetMotionDetectArea(width, height, TRUE, m_motioninfo[ch_id].motion_window[i].rect, rect[i]);
					}
				}
				else
				{
					rect[0].left	= 0;
					rect[0].top		= 0;
					rect[0].right	= width-1;
					rect[0].bottom	= height-1;
				}
				sensortivity = m_motioninfo[ch_id].motion_window[0].motion.sensitivity;
				UINT threshold = 960;			// (width*height > 200000? 960:960);

				////m_Log.Log(TRUE, " ");


				////m_Log.Log(TRUE, "video_motion_enable(%d, UQ_VIDEO_SUB_STREAM, %d) = %d",		ch_id, TRUE,			uq_set_video_motion_enable(ch_id, UQ_VIDEO_SUB_STREAM, TRUE));
				////m_Log.Log(TRUE, "video_motion_mode(%d, UQ_VIDEO_SUB_STREAM, %d) = %d",			ch_id, TRUE,			uq_set_video_motion_mode(ch_id,UQ_VIDEO_SUB_STREAM, TRUE));
				////m_Log.Log(TRUE, "video_motion_threshold(%d, UQ_VIDEO_SUB_STREAM, %d) = %d",		ch_id, threshold,		uq_set_video_motion_threshold(ch_id,UQ_VIDEO_SUB_STREAM, threshold));


				UINT8	spatial_th			= sensortivity;			// sensortivity	// 0(min) ~ 100(max)
				UINT8	temporal_window		= 8;					//(8 x 33msec)
				UINT8	temporal_th			= 2;					//

				m_uq_motion->set_channel(ch_id, width, height);

				for (UINT8 i = 0; i < max_rect; ++i)
				{
					m_uq_motion->set_region(ch_id, i, rect[i], spatial_th, temporal_th, temporal_window);
					TRACE("set_region(ch=%d, rect(%d,%d,%d,%d)) \n", ch_id, rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);
					////m_Log.Log(TRUE, "set_region(ch=%d, rect(%d,%d,%d,%d))", ch_id, rect[i].left, rect[i].top, rect[i].right, rect[i].bottom);

					m_uq_motion->set_region_enable(ch_id, i, 0x01);
				}

				m_uq_motion->set_channel_enable(ch_id, 0x01);
			}
		}
	}
	else
	{
//		////m_Log.Log(TRUE, "set_motion_channel_enable(%d, %d) = %d",		cam_id, FALSE,			m_uq_motion->set_channel_enable(cam_id, FALSE));
//		m_uq_motion->set_channel_enable(cam_id, 0x00);

//		////m_Log.Log(TRUE, "video_motion_enable(%d, VIDEO_SUB_STREAM, %d) = %d",		cam_id, FALSE,			uq_set_video_motion_enable(cam_id, UQ_VIDEO_SUB_STREAM, FALSE));
	}

	return TRUE;
}

void CUXCapIF::GetMotionDetectArea(int max_x, int max_y, BOOL bPatial, RECT &in_rect, RECT &out_rect)
{
	if (!bPatial)
	{
		out_rect.left	= 0;
		out_rect.top	= 0;
		out_rect.right	= max_x;
		out_rect.bottom	= max_y;
	}
	else
	{
		int point_x1 = (in_rect.left*max_x) / 640 ; 
		int point_x2 = (in_rect.right*max_x) / 640 ; 

		int point_y1 = (in_rect.top*max_y) / 480;
		int point_y2 = (in_rect.bottom*max_y) / 480;

//		for (int yy = (point_y1/16) ; yy < (point_y2/16); yy++)
//		{
//			for (int xx = (point_x1/16) ; xx < (point_x2/16); xx++)
//			{
//				int index = yy*(max_x/16) + xx;
//				m_pRegions_map[bd_id][cam_id][index] = 0x01;
//			}
//		}

		out_rect.left	= point_x1;
		out_rect.top	= point_y1;
		out_rect.right	= point_x2;
		out_rect.bottom	= point_y2;
	}
}
/*
BOOL CUXCapIF::set_motion_region(UINT8 ch_id, UINT8 region_id, RECT rect, UINT8 spatial_th, UINT8 temporal_th, UINT8 temporal_window)
{
	// mb-wise quantization
	m_motion_data[ch_id].region[region_id].rect.left	= rect.left & 0xFFF0;
	m_motion_data[ch_id].region[region_id].rect.top		= rect.top & 0xFFF0;
	m_motion_data[ch_id].region[region_id].rect.right	= (rect.right - 15) & 0xFFF0;
	m_motion_data[ch_id].region[region_id].rect.bottom	= (rect.bottom - 15) & 0xFFF0;

	// mb rect
	m_motion_data[ch_id].region[region_id].mb_rect.left		= m_motion_data[ch_id].region[region_id].rect.left	 >> 4;
	m_motion_data[ch_id].region[region_id].mb_rect.right	= m_motion_data[ch_id].region[region_id].rect.right	 >> 4;
	m_motion_data[ch_id].region[region_id].mb_rect.top		= m_motion_data[ch_id].region[region_id].rect.top	 >> 4;
	m_motion_data[ch_id].region[region_id].mb_rect.bottom	= m_motion_data[ch_id].region[region_id].rect.bottom >> 4;

	// region size
	UINT16 region_hsize = static_cast<UINT16>((m_motion_data[ch_id].region[region_id].mb_rect.right - m_motion_data[ch_id].region[region_id].mb_rect.left + 1));
	UINT16 region_vsize = static_cast<UINT16>((m_motion_data[ch_id].region[region_id].mb_rect.bottom - m_motion_data[ch_id].region[region_id].mb_rect.top + 1));
	UINT16 region_size = region_hsize * region_vsize;

	if (region_vsize > 32)
	{
		m_motion_data[ch_id].region[region_id].b_valid =  FALSE;
		return FALSE;
	}

	// mask generation
	memset(m_motion_data[ch_id].region[region_id].mask_cell, 0, 32*sizeof(UINT64)); 

	UINT64  u64temp = 0;
	for (UINT16 n = static_cast<UINT16>(m_motion_data[ch_id].region[region_id].mb_rect.left); n <= m_motion_data[ch_id].region[region_id].mb_rect.right; n++)
	u64temp |= (1ULL << n);

	for (UINT16 k = static_cast<UINT16>(m_motion_data[ch_id].region[region_id].mb_rect.top); k <= m_motion_data[ch_id].region[region_id].mb_rect.bottom; k++)
	m_motion_data[ch_id].region[region_id].mask_cell[k] = u64temp;

	// spatial threshold setting
	UINT16 u16temp = (UINT16)((float)region_size * (float)spatial_th/100.0); 
	u16temp = (u16temp == 0)? 1 : (u16temp > region_size)? region_size : u16temp;

	m_motion_data[ch_id].region[region_id].region_size = region_size;
	m_motion_data[ch_id].region[region_id].spatial_th = u16temp;

	m_motion_data[ch_id].region[region_id].b_valid = TRUE;

	// temporal threshold setting
	UINT8 u8temp = (temporal_window == 0)? 1 : (temporal_window > UQ_MAX_MOTION_TEMPORAL_WINDOW)? UQ_MAX_MOTION_TEMPORAL_WINDOW : temporal_window; 
	m_motion_data[ch_id].region[region_id].temporal_window =  u8temp;
	m_motion_data[ch_id].region[region_id].temporal_th = (temporal_th > u8temp)? u8temp : (temporal_th == 0)? 1 : temporal_th; 
	m_motion_data[ch_id].region[region_id].temporal_sbox = 0;
	m_motion_data[ch_id].region[region_id].temporal_sum = 0;
	m_motion_data[ch_id].region[region_id].temporal_mask = ((UINT64)0xFFFFFFFFFFFFFFFF) >> (64 - m_motion_data[ch_id].region[region_id].temporal_window);

	return TRUE;
}
*/
void CUXCapIF::check_motion_data(UQ_MOTION_STREAM_DATA& data)
{
#if 1
	if (m_uq_motion->check_motion(data) == TRUE)
	{
//		TRACE("motion detect : ch-%d \n", data.ch_id);

		DWORD tmCurrMotionChecked = GetTickCount();
		if (tmCurrMotionChecked - m_tmLastMotionChecked[data.ch_id] > 200)
		{
			m_tmLastMotionChecked[data.ch_id] = tmCurrMotionChecked;

//			TRACE("motion detect : ch-%d \n", data.ch_id);

			DATA_INFO	data_info;

			// motion data
			ZeroMemory(&data_info, sizeof(data_info));
			data_info.vs_idx		= 0;
			data_info.bd_idx		= 0;
			data_info.cam_idx		= data.ch_id;
			data_info.type			= ACT_CODIMAGE;
			data_info.data_size		= 1;
			data_info.pdata			= NULL;
			data_info.frame_type	= FT_MD_FULL;
			GetChannelResolution(data_info.cam_idx, 0, &data_info.res.w, &data_info.res.h);

			m_pAddr_Encoded_callback(&data_info);
		}
	}
#else
	UINT16	width, height;
	BOOL	b_data_rdy;

	if (uq_get_video_resolution(data.ch_id, data.ch_type, width, height))	// data.ch_type = UQ_VIDEO_SUB_STREAM
	{
		UINT16 mb_width = (width + 15) >> 4;
		UINT16 mb_height = (height + 15) >> 4;

		if (mb_width > 64 || mb_height > 32)
		{
			b_data_rdy = FALSE;
		}
		else
		{
			memset(m_motion_data[data.ch_id].motion_cell, 0, 32*sizeof(UINT64)); 
			for (UINT16 j=0, m =0; j<mb_height; ++j) 
				for (UINT16 i=0; i<mb_width; ++i, ++m)
					m_motion_data[data.ch_id].motion_cell[j] |= (UINT64) ((data.md_data[8 + (m << 2)] & 0x01)) << i;

			b_data_rdy = TRUE;
		}
	}

	if (b_data_rdy)
	{
		for (UINT16 j=0; j < MOTION_MAX_REGION; j++)
		{
			if (m_motion_data[data.ch_id].region[j].b_valid)
			{
				UINT64 u64temp;
				UINT16 mb_cnt = 0;
				for (UINT16 i= static_cast <UINT16>(m_motion_data[data.ch_id].region[j].mb_rect.top); i <= m_motion_data[data.ch_id].region[j].mb_rect.bottom; i++)
				{
					u64temp = m_motion_data[data.ch_id].motion_cell[i] & m_motion_data[data.ch_id].region[j].mask_cell[i];
					mb_cnt += _mm_popcnt_u32((UINT32)(u64temp & 0xFFFFFFFF)) + _mm_popcnt_u32((UINT32)((u64temp >> 32) & 0xFFFFFFFF));
				}

				if (mb_cnt >= m_motion_data[data.ch_id].region[j].spatial_th)
				{
					// motion is detected in the current frame!
//					m_motion_data[data.ch_id].region[j].temporal_sum += 1 - ((m_motion_data[data.ch_id].region[j].temporal_sbox >> (m_motion_data[data.ch_id].region[j].temporal_window-1)) & 0x1);
//					m_motion_data[data.ch_id].region[j].temporal_sbox = ((m_motion_data[data.ch_id].region[j].temporal_sbox << 1) | 0x1) & m_motion_data[data.ch_id].region[j].temporal_mask;


					DWORD tmCurrMotionChecked = GetTickCount();
					if (tmCurrMotionChecked - m_tmLastMotionChecked[data.ch_id] > 200)
					{
						m_tmLastMotionChecked[data.ch_id] = tmCurrMotionChecked;

						TRACE("motion detect : ch-%d \n", data.ch_id);

						DATA_INFO	data_info;

						// motion data
						ZeroMemory(&data_info, sizeof(data_info));
						data_info.vs_idx		= 0;
						data_info.bd_idx		= 0;
						data_info.cam_idx		= data.ch_id;
						data_info.type			= ACT_CODIMAGE;
						data_info.data_size		= 1;
						data_info.pdata			= NULL;
						data_info.frame_type	= FT_MD_FULL;
						GetChannelResolution(data_info.cam_idx, 0, &data_info.res.w, &data_info.res.h);

						m_pAddr_Encoded_callback(&data_info);
					}
				}
				else
				{
					// motion is NOT detected in the current frame!
//					m_motion_data[data.ch_id].region[j].temporal_sum -=  ((m_motion_data[data.ch_id].region[j].temporal_sbox >> (m_motion_data[data.ch_id].region[j].temporal_window-1)) & 0x1);
//					m_motion_data[data.ch_id].region[j].temporal_sbox = ((m_motion_data[data.ch_id].region[j].temporal_sbox << 1)) & m_motion_data[data.ch_id].region[j].temporal_mask;
				}

//				m_motion_data[data.ch_id].b_detected[j] = (m_motion_data[data.ch_id].region[j].temporal_sum >= m_motion_data[data.ch_id].region[j].temporal_th)? TRUE : FALSE;
			}  
//			else
//			{
//				m_motion_data[data.ch_id].b_detected[j] = FALSE;
//			}
		} 
	}
//	else
//	{
//		memset(m_motion_data[data.ch_id].b_detected, 0, MAX_MOTION_REGION*sizeof(BOOL));
//	}
#endif
}


BOOL CUXCapIF::SetVideoMode(int idx, int bd_id, ULONG uProperty[4])
{
	int		vformat = (uProperty[0]>>16)&0xffff;		// NTSC, PAL
	int		iformat = uProperty[0]&0xffff;				// YUV420
	DWORD	req_cam = uProperty[1];
	BOOL	b4=0;
	int		limit = GetCamNum(idx, bd_id);

	for (int i=0;i<limit;i++)
	{
//		m_VideoFormat[i] = vformat;

		b4 &= EnableVideo(idx, bd_id, i, (req_cam & (0x01 << i))?TRUE:FALSE);
	}

//	if (iformat == IMG_YUY2)
		m_ImageFormat = UQ_CF_420_PL;

//	//m_Log.Log(TRUE, "[INFO] Image Format = %s(%d)", (iformat==IMG_YUY2)?"YUY2":"Other", iformat);
//	//m_Log.Log(TRUE, "[INFO] Requested Cam = %x", req_cam);

	return b4;
}

BOOL CUXCapIF::SetImageRes(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (!uProperty[0])
		return TRUE;

//	m_nEncodeResolution[0][cam_id].w = LOWORD(uProperty[0]);
//	m_nEncodeResolution[0][cam_id].h = HIWORD(uProperty[0]);

//	//m_Log.Log(TRUE, "CUXCapIF::SetImageRes()    : cam_id=%d, w=%d, h=%d", cam_id, m_nEncodeResolution[0][cam_id].w, m_nEncodeResolution[0][cam_id].h);

	return TRUE;
}

BOOL CUXCapIF::SetImageResSub(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (!uProperty[0])
		return TRUE;

//	m_nEncodeResolution[1][cam_id].w = LOWORD(uProperty[0]);
//	m_nEncodeResolution[1][cam_id].h = HIWORD(uProperty[0]);

//	//m_Log.Log(TRUE, "CUXCapIF::SetImageResSub() : cam_id=%d, w=%d, h=%d", cam_id, m_nEncodeResolution[1][cam_id].w, m_nEncodeResolution[1][cam_id].h);

	return TRUE;
}

BOOL CUXCapIF::SetFrameRate(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	m_nEncodeFPS[0][cam_id] = (UINT8) uProperty[0];
	if (m_nEncodeFPS[0][cam_id] == 0)
	{
		m_nEncodeFPS[0][cam_id] = 30;
	}
//	//m_Log.Log(TRUE, "CUXCapIF::SetFrameRate()       : cam_id=%d, fps=%d", cam_id, m_nEncodeFPS[0][cam_id]);
	return TRUE;
}

BOOL CUXCapIF::SetFrameRateSub(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	m_nEncodeFPS[1][cam_id] = (UINT8) uProperty[0];
	if (m_nEncodeFPS[1][cam_id] == 0)
	{
		m_nEncodeFPS[1][cam_id] = 30;
	}
//	//m_Log.Log(TRUE, "CUXCapIF::SetFrameRateSub()    : cam_id=%d, fps=%d, fps_sub=%d", cam_id, m_nEncodeFPS[0][cam_id], m_nEncodeFPS[1][cam_id]);
	return TRUE;
}

BOOL CUXCapIF::SetCodecQuality(int idx, int bd_id, int cam_id, int w, int h, ULONG uProperty[4])
{
	DWORD image_quality = uProperty[0] - 1;		// BitRate
	DWORD encoding_type = uProperty[1];			// VBR, CBR
	DWORD record_res = uProperty[2];
	DWORD res_type = uProperty[3];				// resolution

	switch (encoding_type)
	{
	case 0 :
	default:
		m_nEncodeMode[0][cam_id] = UQ_RC_VBR;
		break;
	case 1 :		// Contant quality
		m_nEncodeMode[0][cam_id] = UQ_RC_CBR;
		break;
	}

//	switch (res_type)
//	{
//	case 3 :	m_nEncodeResolution[0][cam_id].w = 1920;
//				m_nEncodeResolution[0][cam_id].h = 1080;
//				break;
//	case 2 :	m_nEncodeResolution[0][cam_id].w = 1280;
//				m_nEncodeResolution[0][cam_id].h = 720;
//				break;
//	case 1 :	m_nEncodeResolution[0][cam_id].w = 720;
//				m_nEncodeResolution[0][cam_id].h = 480;		// NTSC로 설정.. 나중에 PAL check
//				break;
//	case 0 :
//	default:	m_nEncodeResolution[0][cam_id].w = 360;
//				m_nEncodeResolution[0][cam_id].h = 240;		// NTSC로 설정.. 나중에 PAL check
//				break;
//	}

	m_nEncodeLevel[0][cam_id] = min((UINT8) image_quality, 4);

//	//m_Log.Log(TRUE, "SetCodecQuality()    : cam_id=%2d, w=%4d, h=%4d, iq=%d, et=%d, res=%d", cam_id, m_nEncodeResolution[0][cam_id].w, m_nEncodeResolution[0][cam_id].h, image_quality, m_nEncodeMode[0][cam_id], res_type);
//	//m_Log.Log(TRUE, "SetCodecQuality()    : cam_id=%2d, iq=%d, et=%d, res=%d", cam_id, image_quality, m_nEncodeMode[0][cam_id], res_type);

	return TRUE;
}

BOOL CUXCapIF::SetCodecQualitySub(int idx, int bd_id, int cam_id, int w, int h, ULONG uProperty[4])
{
	DWORD image_quality = uProperty[0] - 1;		// BitRate
	DWORD encoding_type = uProperty[1];			// VBR, CBR
	DWORD record_res = uProperty[2];
	DWORD res_type = uProperty[3];				// resolution

	switch (encoding_type)
	{
	case 0 :		// Contant quality
	default:
		m_nEncodeMode[1][cam_id] = UQ_RC_VBR;
		break;
	case 1 :
		m_nEncodeMode[1][cam_id] = UQ_RC_CBR;
		break;
	}

	switch (res_type)
	{
//	case 3 :	m_nEncodeResolution[1][cam_id].w = 1920;
//				m_nEncodeResolution[1][cam_id].h = 1080;
//				break;
//	case 2 :	m_nEncodeResolution[1][cam_id].w = 1280;
//				m_nEncodeResolution[1][cam_id].h = 720;
//				break;
	case 1 :	m_nEncodeResolution[1][cam_id].w = 640;
				m_nEncodeResolution[1][cam_id].h = 480;		// NTSC로 설정.. 나중에 PAL check
				break;
	case 0 :
	default:	m_nEncodeResolution[1][cam_id].w = 320;
				m_nEncodeResolution[1][cam_id].h = 240;		// NTSC로 설정.. 나중에 PAL check
				break;
	}

	m_nEncodeLevel[1][cam_id] = min((UINT8) image_quality, 4);

	//m_Log.Log(TRUE, "SetCodecQualitySub() : cam_id=%2d, res=%d, w=%4d, h=%4d", cam_id, res_type, m_nEncodeResolution[1][cam_id].w, m_nEncodeResolution[1][cam_id].h);

	return TRUE;
}

BOOL CUXCapIF::SetCodecGOPSize(int cam_id, ULONG frame_rate)
{
	if (frame_rate <= 0)
		frame_rate = 1;

	if (m_VideoFormat[cam_id] == VF_NTSC)
	{
		m_nEncodeGOPSize[0][cam_id] = min((UINT8)frame_rate, 30);
		m_nEncodeGOPSize[1][cam_id] = min((UINT8)frame_rate, 30);
	}
	else
	{
		m_nEncodeGOPSize[0][cam_id] = min((UINT8)frame_rate, 25);
		m_nEncodeGOPSize[1][cam_id] = min((UINT8)frame_rate, 25);
	}

	//m_Log.Log(TRUE, "SetCodecGOPSize() : cam_id=%d, gop=%d", cam_id, m_nEncodeGOPSize[0][cam_id]);

	return TRUE;
}

void CUXCapIF::SetCaptureSizeLevel(int idx, int bd_id, int cam_id, int level, BOOL bDecrease)
{	// main preview
	if ( m_bDisplayStart[cam_id] && m_nDisplayResolutionLevel[cam_id] == level )
		return;

	m_nDisplayResolutionLevel[cam_id] = level;

//	//m_Log.Log(TRUE, "SetCaptureSizeLevel() : cam_id=%d, level=%d, bDecrease=%d", cam_id, level, bDecrease);

	if (m_bVideoLock[cam_id] && m_bVideoEnable[cam_id] && (m_dwStatus == API_ST_RUN))
	{
		m_bDisplayStart[cam_id] = TRUE;

		UINT16	displayX, displayY;
		GetDisplayResolution(m_nDisplayResolutionMax[cam_id].w, m_nDisplayResolutionMax[cam_id].h, m_nDisplayResolutionLevel[cam_id], displayX, displayY);

		uq_set_video_enable(cam_id, UQ_VIDEO_MAIN_PREVIEW, 0x00);
		//m_Log.Log(TRUE, "video_resolution(%2d, PREVIEW, %4d, %4d) = %d", cam_id, displayX, displayY,	uq_set_video_resolution(cam_id, UQ_VIDEO_MAIN_PREVIEW, displayX, displayY));
		uq_set_video_enable(cam_id, UQ_VIDEO_MAIN_PREVIEW, 0x01);

//		BOOL	enable;
//		if (uq_get_video_enable(cam_id, UQ_VIDEO_MAIN_PREVIEW, enable) == TRUE && enable == FALSE) {
//			//m_Log.Log(TRUE, "set_video_enable(%d, PREVIEW, %d) = %d", cam_id, 0x01,	uq_set_video_enable(cam_id, UQ_VIDEO_MAIN_PREVIEW, 0x01));
//		}
	}
	else
	{
//		//m_Log.Log(TRUE, "SetCaptureSizeLevel(cam=%d, lock=%d, enable=%d)", cam_id, m_bVideoLock[cam_id], m_bVideoEnable[cam_id]);
	}
}

void CUXCapIF::SetSecondarySizeLevel(int idx, int bd_id, int cam_id, int res, BOOL bEnable)
{	// sub preview
}

BOOL CUXCapIF::SetBright(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	m_brightness[cam_id] = (BYTE)uProperty[0];

	if (m_dwStatus >= API_ST_READY)
	{
		BOOL bret = TRUE;
		bret = uq_set_video_adjust(cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id]);
//		//m_Log.Log(TRUE, "video_adjust(%d, %d, %d, %d, %d) = %d", cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id],	bret);
		return bret;
	}
    return TRUE;
}

BOOL CUXCapIF::SetContrast(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	m_contrast[cam_id] = (BYTE)uProperty[0];

	if (m_dwStatus >= API_ST_READY)
	{
		BOOL bret = TRUE;
		bret = uq_set_video_adjust(cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id]);
//		//m_Log.Log(TRUE, "video_adjust(%d, %d, %d, %d, %d) = %d", cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id],	bret);
		return bret;
	}
    return TRUE;
}

BOOL CUXCapIF::SetSaturation(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	m_saturation[cam_id] = (BYTE)uProperty[0];

	if (m_dwStatus >= API_ST_READY)
	{
		BOOL bret = TRUE;
		bret = uq_set_video_adjust(cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id]);
//		//m_Log.Log(TRUE, "video_adjust(%d, %d, %d, %d, %d) = %d", cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id],	bret);
		return bret;
	}
    return TRUE;
}

BOOL CUXCapIF::SetHue(int idx, int bd_id, int cam_id, ULONG uProperty[4])
{
	m_hue[cam_id] = (BYTE)uProperty[0];

	if (m_dwStatus >= API_ST_READY)
	{
		BOOL bret = TRUE;
		bret = uq_set_video_adjust(cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id]);
//		//m_Log.Log(TRUE, "video_adjust(%d, %d, %d, %d, %d) = %d", cam_id, m_brightness[cam_id], m_contrast[cam_id], m_saturation[cam_id], m_hue[cam_id],	bret);
		return bret;
	}
    return TRUE;
}

BOOL CUXCapIF::SetGain(int idx, int bd_id, int aud_id, ULONG uProperty[4])
{
	UINT8	volume = (UINT8) uProperty[0];

//	//m_Log.Log(TRUE, "SetGain() : aud_id=%d, volume=%d", aud_id, volume);
//	TRACE("SetGain() : aud_id=%d, volume=%d \n", aud_id, volume);

	volume = volume * 16;

	UINT8 real_audio_id = GetAudioIndex(aud_id);
	uq_set_audio_encode_volume(real_audio_id, volume);

//	sx_uint16 input_data = (sx_uint16)uProperty[0];
	return TRUE;
}

void CUXCapIF::SetOSDColor(int idx, COLORREF osd_color)
{
	//m_Log.Log(TRUE, "SetOSDColor() : idx=%d, osd_color=%x", idx, osd_color);

	m_osd_color = osd_color;

	m_uq_osd->set_osd_font("Language\\NVR.ttf");
}

void CUXCapIF::SetOSDInfo(int idx, int bd_id, int cam_id, BOOL bShow, BOOL btime, int time_type, BOOL bosd, LPSTR osd, int pos, int translucent)
{
//	//m_Log.Log(TRUE, "SetOSDInfo() : cam_id=%d, bShow=%d, btime=%d, time_type=%d, bosd=%d, osd=%s, pos=%d", cam_id, bShow, btime, time_type, bosd, osd, pos);

	if (m_nBoardType == BD_UQ3388_2416)
	{
		m_osd_show[cam_id] = FALSE;
		return;
	}

	m_osd_show[cam_id] = bShow;

	if (!bShow)
	{
		return;
	}

	if (btime)
	{
		switch (time_type)
		{
		case 0 :	// mm-dd-yy
					m_osd_time_type[cam_id] = 1;
					break;
		case 1 :	// dd-mm-yy
					m_osd_time_type[cam_id] = 2;
					break;
		case 2 :	// yy-mm-dd
		default:	m_osd_time_type[cam_id] = 3;
					break;
		}
	}
	else {
		m_osd_time_type[cam_id] = 0;
	}

	ZeroMemory(m_osd_text[cam_id], sizeof(m_osd_text[cam_id]));
	if (bosd)
	{
		CString str;
		str.Format("%s", osd);
		MultiByteToWideChar(CP_ACP, 0, str, str.GetLength(), (LPWSTR) m_osd_text[cam_id], 128);
	}

	m_osd_pos[cam_id] = pos;
}

int CUXCapIF::SetWatchDog(int idx, int bd_id, int timeout, int beep_timeout)
{
	return 1;
}

BOOL CUXCapIF::StopWatchDog(int idx, int bd_id)
{
	return 1;
}

int CUXCapIF::RelayControl(int idx, int bd_id, int relay_id, int onoff)
{
	if (relay_id/8 >= MAX_DIO_BOARD)
		return FALSE;

	BOOL	bret = FALSE;
	UINT32	board_id = m_nDIDOBoardIndex[relay_id/8];

	if (board_id < 255)
	{
		bret = uq_set_dido_output(board_id, relay_id%8, onoff & 0x01);
		TRACE("RelayControl(board_id=%d, relay_id=%d, %d) - %d\n", board_id, relay_id, onoff, bret);
	}

	return bret;
}

BOOL CUXCapIF::GetRelayStatus(int idx, int bd_idx, ULONG* pdata)
{
	UINT32	relay = 0;

	*pdata = relay;

	return 1;
}

int CUXCapIF::GetVoutNum(int idx, int bd_id)
{
	return 1;
}

void CUXCapIF::SetOnBoardVout(int idx, DWORD bd, DWORD port, DWORD split, DWORD *cam_info)
{
	if (m_dwStatus != API_ST_RUN)
		return;

	int		i;
	UINT8	divide = 4;
	switch (split)
	{
	case 1 :	divide = 1;
				break;
	case 4 :	divide = 2;
				break;
	case 9 :	divide = 3;
				break;
	case 16 :	divide = 4;
				break;
	default :	return;
	}

	EnterCriticalSection(&m_criticalSectionTVout);

//	//m_Log.Log(TRUE, "SetOnBoardVout(%d,%d,%d)", bd, port, split);
#ifdef _DEBUG
	TRACE("SetOnBoardVout(%d,%d,%d)", bd, port, split);
	for (DWORD dw=0;dw<20;dw++)
	{
		TRACE(" %02d", cam_info[dw]);
	}
	TRACE(" \n");
#endif

	UINT8	enable;
	for(i = 0; i < m_nMaxVideo; i++)
	{
		uq_get_video_out_ch_enable(i, enable);
		if (enable)
		{
			uq_set_video_out_ch_enable_ext(i, m_nTVoutPort[i], 0x00);
//			//m_Log.Log(TRUE, "set_video_out_ch_enable_ext(%d, %d, 0x00)", i, m_nTVoutPort[i]);
		}
		m_nTVoutPort[i] = 0xff;
	}

	if (m_nTVoutSplit != split)
	{
		m_nTVoutSplit = split;

		//all board port disable
		for(i = 0; i < m_nBoardCount; i++){
			uq_set_video_out_hdmi_enable(i, 0x00);
			uq_set_video_out_cvbs_enable(i, 0x00);
		}

		//all board port setting
		for(i = 0; i < m_nBoardCount; i++){
			uq_set_video_out_hdmi_mode(i, UQ_HDMI_1280x720p, divide, divide);
			uq_set_video_out_cvbs_mode(i, UQ_CVBS_NTSC_M);
		}

		//all board port enable
		for(i = 0; i < m_nBoardCount; i++){
			uq_set_video_out_hdmi_enable(i, 0x01);
			uq_set_video_out_cvbs_enable(i, 0x01);
		}
	}

	//channel enable
	for(i = 0; i < split; i++)
	{
		if (cam_info[i] >= 0 && cam_info[i] < MAX_CAMERA_PER_BOARD)
		{
			m_nTVoutPort[cam_info[i]] = i;
			if (m_bVideoLock[cam_info[i]]){
				//channel enable
				uq_set_video_out_ch_enable_ext(cam_info[i], i, 0x01);
//				//m_Log.Log(TRUE, "set_video_out_ch_enable_ext(%d, %d, 0x01)", cam_info[i], i);
			}
		}
	}

//	//m_Log.Log(TRUE, "SetOnBoardVout()");
	LeaveCriticalSection(&m_criticalSectionTVout);
}

int CUXCapIF::CheckSecurity(int idx, int bd_idx)
{
	return 1;
}


/////////////////////////////////////////////////////////////////////////////
// serial port control

BOOL CUXCapIF::SerialPortGetState(int idx, int port, LPDCB pdcb)
{
	return  TRUE; // m_Serial[port].SerialPortGetState(port + 1, pdcb);
}

BOOL CUXCapIF::SerialPortSetState(int idx, int port, DCB dcb)
{
	/*if (m_Serial[port].Open(port+1, dcb.BaudRate, dcb.ByteSize, dcb.StopBits, dcb.Parity) == -1)
		return FALSE;*/

	return TRUE;
}

BOOL CUXCapIF::SerialPortSend(int idx, int port, LPSTR buf, int size)
{
	return TRUE; // m_Serial[port].Send((LPBYTE)buf, size);
}

BOOL CUXCapIF::SerialPortRecv(int idx, int port, LPSTR buf, LPINT psize)
{
	return FALSE;
}

BOOL CUXCapIF::SerialPortClearState(int idx, int port)
{
	return TRUE; // m_Serial[port].Close();
}
