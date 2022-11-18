// VideoSvrClass.cpp : implementation file
//

#include "stdafx.h"
#include "../include/devapiclass.h"
//#include "../include/SDC/SDCapIF.h"	// UDP Board
#include "../include/UXCapIF.h"			// Ubiqmicro Board
//#include "../include/STCapIF.h"			// Stretch Board
//#include "../include/NetDevNvrIF.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDevApiClass, CWnd)

 

#pragma comment (lib, "../Library/UXCap2.lib")
#pragma message ("linking 'UXCap2.lib'")


//#pragma comment (lib, "../Library/NetDevNvr.lib")
//#pragma message ("linking 'NetDev.lib'")

static int s_dwUsePTZCtrl = 0;
/////////////////////////////////////////////////////////////////////////////
// CDevApiClass

CDevApiClass::CDevApiClass()
{
	// newapi
	m_pDevice = 0;
}

CDevApiClass::~CDevApiClass()
{
	if (m_pDevice) delete m_pDevice;
}

/////////////////////////////////////////////////////////////////////////////
// public interfaces

// newapi
BOOL CDevApiClass::CreateDevice(int type)
{   // create device type and load dll
	DestroyDevice();

	switch (type) {
	case VS_UBIQ_3388:
		{
			CUXCapIF* pUXCapif = new CUXCapIF;
			m_pDevice = (CDevLibIF*)pUXCapif->CreateDevice(type);
			if (!m_pDevice)
				delete pUXCapif;
			
			break;
		}

	default:
		break;
	}

#ifdef _DEBUG
	if (_heapchk() != _HEAPOK)
		DebugBreak();
#endif
	if (m_pDevice) return TRUE;
	return FALSE;
}
// newapi
void CDevApiClass::DestroyDevice()
{
	if (m_pDevice) {
		m_pDevice->DestroyDevice();
		m_pDevice = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vs-vendor supported dll wrapper APIs

// 보드가 있으면 보드의 타입을 리턴한다.
// 보드가 없으면 항상 0을 리턴한다.
int CDevApiClass::GetVSType()
{
	// get board type
	if (m_pDevice) {
		m_pDevice->GetQueryInterface(0);
		if (m_pDevice->ExtractSystemInfo())
		{
			int vs_type = m_pDevice->GetVSType(0);
			//m_pDevice->ResetQueryInterface(0);
			return vs_type;
		}
		else
		{
			//m_pDevice->ResetQueryInterface(0);
		}
	}

	return 0;
}

// vs-vendor supported dll 초기화
int CDevApiClass::LoadDll(int type)
{
	if (m_pDevice)
		return m_pDevice->LoadDll(type);
		
	return 0;
}

// unload dll
void CDevApiClass::UnloadDll(int type)
{
	if (m_pDevice) {
		m_pDevice->UnloadDll();
	}
}

// query interface 가져오기
void CDevApiClass::InitVSIF(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		m_pDevice->GetQueryInterface(pvs->idx);
	}
}

void CDevApiClass::UninitVSIF(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		m_pDevice->ResetQueryInterface(pvs->idx);
	}
}

// vs에 접속
int CDevApiClass::Connect(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->Connect(pvs->idx, pvs->ni.ip, pvs->ni.http_port, pvs->ni.id, pvs->ni.pwd)?1:0;
	}
	return 0;
}

// vs 접속 해제
void CDevApiClass::Disconnect(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		m_pDevice->Disconnect(pvs->idx);
	}
}
// serverA
int CDevApiClass::Connect(LPVIDEO_SERVER pvs, int slot_idx, int cam_idx, DWORD data_type)
{	// 오디오는 1st stream 에서만 받도록.. 2nd stream 은 비디오만..
	int ret = 0;
	if (m_pDevice) {
		if (pvs->ni.record_stream==1 || pvs->ni.display_stream==1 || pvs->ni.transmit_stream==1) {
			if (m_pDevice->Connect(pvs->idx, slot_idx, pvs->ni, cam_idx, data_type)==TRUE)
				ret |= 0x1;
		}
		if (pvs->ni.record_stream==2 || pvs->ni.display_stream==2 || pvs->ni.transmit_stream==2) {
			if (m_pDevice->Connect2(pvs->idx, slot_idx, pvs->ni, cam_idx, data_type)==TRUE)
				ret |= 0x2;
		}
	}
	// ret 0:실패, 1st bit is 1:1st stream 성공, 2nd bit is 1:2nd stream 성공 ... And so on..
	return ret;
}
void CDevApiClass::Disconnect(LPVIDEO_SERVER pvs, int slot_idx)
{
	if (m_pDevice) {
		m_pDevice->Disconnect(pvs->idx, slot_idx);
	}
}

// vs-vendor supported dll을 사용할 준비 후 event handle 가져오기
int CDevApiClass::Ready(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Video_callback, AUDIO_DATA_CALLBACK pAddr_Audio_callback)
{
	if (m_pDevice) {
		if (IsPCI(pvs->type))
		{
			if(!s_dwUsePTZCtrl)
				//SetPTInit();
			s_dwUsePTZCtrl++;
		}

		if (m_pDevice->SetUp(pvs->idx) == 0)
			return 0;

		m_pDevice->GetEventHandles(pvs->idx, pvs->events);
		return 1;
	}
	return 0;
}

void CDevApiClass::SetVideoCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Video_callback)
{
	if (m_pDevice)
		m_pDevice->SetVideoCallback(pAddr_Video_callback);
}

void CDevApiClass::SetAudioCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Audio_callback)
{
	if (m_pDevice)
		m_pDevice->SetAudioCallback(pAddr_Audio_callback);
}
	
void CDevApiClass::SetEncodedCallback(LPVIDEO_SERVER pvs, VIDEO_DATA_CALLBACK pAddr_Encoded_callback, VIDEO_DATA_CALLBACK pAddr_Encoded_callback_Secondary)
{
	if (m_pDevice)
		m_pDevice->SetEncodedCallback(pAddr_Encoded_callback, pAddr_Encoded_callback_Secondary);
}

// vs-vendor supported dll에 사용 중지를 알림
void CDevApiClass::CleanUp(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		if (IsPCI(pvs->type))
		{
			s_dwUsePTZCtrl--;
			//if(s_dwUsePTZCtrl <= 0)
				//SetPTUnInit();
		}

		m_pDevice->ReleaseEventHandles(pvs->idx, pvs->events);
		m_pDevice->CleanUp(pvs->idx);
	}
}

// vs-vendor supported dll을 구동시킴
BOOL CDevApiClass::Run(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		if(m_pDevice->Run(pvs->idx))
			return TRUE;
	}
	return FALSE;
}

// vs-vendor supported dll의 작동을 중단함
void CDevApiClass::Stop(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		m_pDevice->Stop(pvs->idx);
	}
}

// security check
int CDevApiClass::CheckSecurity(LPVIDEO_SERVER pvs, int bd_idx)
{
	if (m_pDevice) {
		return m_pDevice->CheckSecurity(pvs->idx, bd_idx);
	}
	return -1;
}

// 데이터 가져오기
int CDevApiClass::GetData(int action, LPVIDEO_SERVER pvs, LPDATA_INFO pdata)
{
	if (m_pDevice) {
		if (action == ACT_NETWORK)
			return m_pDevice->GetNetworkData(pvs->idx, pdata);
		else if (action == ACT_AUDIO)
			return m_pDevice->GetAudioData(pvs->idx, pdata);
		else if (action == ACT_CODIMAGE)
			return m_pDevice->GetCodImageData(pvs->idx, pdata);
		else if (action == ACT_RAWIMAGE)
			return m_pDevice->GetRawImageData(pvs->idx, pdata);
		else if (action == ACT_SALARM)
			return m_pDevice->GetSensorData(pvs->idx, pdata);
		else if (action == ACT_VALARM)
			return m_pDevice->GetVStatusData(pvs->idx, pdata);
	}
	return 0;
}

// 데이터 해제하기
void CDevApiClass::ReleaseData(int action, LPVIDEO_SERVER pvs, LPDATA_INFO pdata)
{
	if (m_pDevice) {
		if (action == ACT_NETWORK)
			m_pDevice->ReleaseNetworkData(pvs->idx, pdata);
		else if (action == ACT_AUDIO)
			m_pDevice->ReleaseAudioData(pvs->idx, pdata);
		else if (action == ACT_CODIMAGE)
			m_pDevice->ReleaseCodImageData(pvs->idx, pdata);
		else if (action == ACT_RAWIMAGE)
			m_pDevice->ReleaseRawImageData(pvs->idx, pdata);
		else if (action == ACT_SALARM)
			m_pDevice->ReleaseSensorData(pvs->idx, pdata);
		else if (action == ACT_VALARM)
			m_pDevice->ReleaseVStatusData(pvs->idx, pdata);
	}
}

void CDevApiClass::GetKeyPacket(int idx, LPSTR pKey)
{
	if (m_pDevice) {
		m_pDevice->GetKeyPacket(idx, pKey);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// audio

BOOL CDevApiClass::SetAudioGain(LPVIDEO_SERVER pvs, int bd_id, int aud_id, int gain)
{
	if (m_pDevice) {
		ULONG prop[4] = {gain,};
		return m_pDevice->SetGain(pvs->idx, bd_id, aud_id, prop);
	}
	return FALSE;
}

BOOL CDevApiClass::SetAudioBPSandFreq(LPVIDEO_SERVER pvs, int bd_id, int aud_id, int BPS, int Freq)
{
	if (m_pDevice) {
		BOOL bret = FALSE;
		ULONG prop[4] = {0,};

		prop[0] = 16;			// BPS
		bret = m_pDevice->SetBPS(pvs->idx, bd_id, aud_id, prop);
		//prop[0] = 8000;			// frequency
		prop[0] = 16000;			// frequency
		bret &= m_pDevice->SetFrequency(pvs->idx, bd_id, aud_id, prop);
		return bret;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////
// image 특성값

BOOL CDevApiClass::SetImageRes(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) {
		return m_pDevice->SetImageRes(pvs->idx, bd_id, cam_id, uProperty);
	}
	return FALSE;
}

BOOL CDevApiClass::SetImageResSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) {
		return m_pDevice->SetImageResSub(pvs->idx, bd_id, cam_id, uProperty);
	}
	return FALSE;
}

BOOL CDevApiClass::SetBright(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) 
		return m_pDevice->SetBright(pvs->idx, bd_id, cam_id, uProperty);
	
	return FALSE;
}
BOOL CDevApiClass::SetContrast(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) 
		return m_pDevice->SetContrast(pvs->idx, bd_id, cam_id, uProperty);
	
	return FALSE;
}
BOOL CDevApiClass::SetSaturation(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) {
		return m_pDevice->SetSaturation(pvs->idx, bd_id, cam_id, uProperty);
	}
	return FALSE;
}
BOOL CDevApiClass::SetHue(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice)
		return m_pDevice->SetHue(pvs->idx, bd_id, cam_id, uProperty);
	
	return FALSE;
}
BOOL CDevApiClass::SetOSD(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	return FALSE;
}
BOOL CDevApiClass::EnableOSD(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable)
{
	return FALSE;
}

BOOL CDevApiClass::SetFrameRate(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) 
		return m_pDevice->SetFrameRate(pvs->idx, bd_id, cam_id, uProperty);
	else return FALSE;
}

BOOL CDevApiClass::SetFrameRateSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, ULONG uProperty[4])
{
	if (m_pDevice) 
		return m_pDevice->SetFrameRateSub(pvs->idx, bd_id, cam_id, uProperty);
	else return FALSE;
}

BOOL CDevApiClass::SetCodecQuality(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int w, int h, ULONG uProperty[4])
{
	if (m_pDevice) 
		return m_pDevice->SetCodecQuality(pvs->idx, bd_id, cam_id, w, h, uProperty);
	else return FALSE;
}

BOOL CDevApiClass::SetCodecQualitySub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int w, int h, ULONG uProperty[4])
{
	if (m_pDevice) 
		return m_pDevice->SetCodecQualitySub(pvs->idx, bd_id, cam_id, w, h, uProperty);
	else return FALSE;
}

void CDevApiClass::SetSecondarySizeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int res, BOOL bEnable)
{
	if (m_pDevice)
		m_pDevice->SetSecondarySizeLevel(pvs->idx, bd_id, cam_id, res, bEnable);
}

void CDevApiClass::SetCaptureSizeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int level, BOOL bDecrease)
{
	if (m_pDevice)
		m_pDevice->SetCaptureSizeLevel(pvs->idx, bd_id, cam_id, level, bDecrease);
}

// enable a/v
BOOL CDevApiClass::EnableAudio(LPVIDEO_SERVER pvs, int bd_id, int aud_id, BOOL bEnable)
{
	if (m_pDevice)
		return m_pDevice->EnableAudio(pvs->idx, bd_id, aud_id, bEnable);

	return FALSE;
}
BOOL CDevApiClass::EnableVideo(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable)
{
	if (m_pDevice) {
		return m_pDevice->EnableVideo(pvs->idx, bd_id, cam_id, bEnable);
	}

	return FALSE;
}

BOOL CDevApiClass::EnableMotionDetection(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bEnable, MOTIONINFO motion_info)
{
	if (!m_pDevice)
		return FALSE;
	
	return m_pDevice->EnableMotionDetection(pvs->idx, bd_id, cam_id, bEnable, FALSE, motion_info);
}

BOOL CDevApiClass::MotionDetectionTest(LPVIDEO_SERVER pvs, int bd_id, int cam_id, MOTIONINFO motion_info)		// New MPEG
{
	return m_pDevice->EnableMotionDetection(pvs->idx, bd_id, cam_id, TRUE, TRUE, motion_info);
}

BOOL CDevApiClass::StopWatchDog(LPVIDEO_SERVER pvs, int bd_id)
{
	if (m_pDevice) {
		return m_pDevice->StopWatchDog(pvs->idx, bd_id);
	}

	return FALSE;
}
int CDevApiClass::SetWatchDog(LPVIDEO_SERVER pvs, int bd_id, int timeout, int beep_timeout)
{
	if (m_pDevice) {
		return m_pDevice->SetWatchDog(pvs->idx, bd_id, timeout, beep_timeout);
	}
	return -4;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

// set video mode
BOOL CDevApiClass::SetVideoMode(LPVIDEO_SERVER pvs, int bd_id, ULONG uProperty[4])
{
	if (m_pDevice) {
		return m_pDevice->SetVideoMode(pvs->idx, bd_id, uProperty);
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// PCI는 frame rate을 camera 별로 조정하도록 수정함. 20070109 tizone
// frame rate 개별 control (fps control per channel)
// 20090907 tizone VS_MNC modified
BOOL CDevApiClass::SetUserFrameRate(LPVIDEO_SERVER pvs, int bd_id, unsigned char pFPS[], int length, BOOL DropAtNight)
{
	if (m_pDevice) {
		return m_pDevice->SetUserFrameRate(pvs->idx, bd_id, pFPS, length, DropAtNight);
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////

// relay control
int CDevApiClass::RelayControl(int action, int act_param, LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		int bd_id = LOWORD(act_param);
		int relay_id = HIWORD(act_param);
		int onoff = 1;
		if (action == ACT_RELAYOFF)
			onoff = 0;
		// 140515
		//return m_pDevice->RelayControl(pvs->idx, bd_id, relay_id, onoff);
		int ret = m_pDevice->RelayControl(pvs->idx, bd_id, relay_id, onoff);
		if (0 == ret) {
			// 140515 onvif relay control
			return m_pDevice->RelayControl(pvs->ni.relay_token, onoff);
		}
		// 140515
		else return ret;
		////////////////////////////////////////////////////////////////
	}
	return 0;
}

// 보드가 몇개?
int CDevApiClass::GetBoardNum(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->GetBoardNum(pvs->idx);
	}
	return 0;
}

// 보드 종류는?
int CDevApiClass::GetBoardType(LPVIDEO_SERVER pvs, int bd_idx)
{
	if (m_pDevice) {
		return m_pDevice->GetBoardType(pvs->idx, bd_idx);
	}
	return -1;
}

// 오디오 보드 몇개?
int CDevApiClass::GetAudBoardNum(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->GetAudBoardNum(pvs->idx);
	}
	return 0;
}

// 카메라가 몇개?
int CDevApiClass::GetCamNum(LPVIDEO_SERVER pvs, int bd_idx)
{
	if (m_pDevice) {
		int cam_num = 0;
		int bd_num = 0;
		int i;

		if (IsPCI(pvs->type)) {
			if (bd_idx == -1) {
				bd_num = m_pDevice->GetBoardNum(pvs->idx);
				for (i = 0; i < bd_num; i++)
					 cam_num += m_pDevice->GetCamNum(pvs->idx, i);
			}
			else
				cam_num = m_pDevice->GetCamNum(pvs->idx, bd_idx);
		}
		else {
			cam_num = m_pDevice->GetCamNum(pvs->idx, 0);
		}

		return cam_num;
	}
	return 0;
}
// 오디오 몇개?
int CDevApiClass::GetAudioNum(LPVIDEO_SERVER pvs, int bd_idx)
{
	if (m_pDevice) {
		int aud_num = 0;
		int bd_num = 0;
		int i;

		if (IsPCI(pvs->type)) {
			if (bd_idx == -1) {
				bd_num = m_pDevice->GetAudBoardNum(pvs->idx);
				for (i = 0; i < bd_num; i++)
					aud_num += m_pDevice->GetAudioNum(pvs->idx, i);
			}
			else
				aud_num = m_pDevice->GetAudioNum(pvs->idx, bd_idx);
		}
		else {
			aud_num = m_pDevice->GetAudioNum(pvs->idx, 0);
		}

		return aud_num;
	}
	return 0;
}
// 센서 몇개?
int CDevApiClass::GetSensorNum(LPVIDEO_SERVER pvs, int bd_idx)
{
	if (m_pDevice) {
		int sensor_num = 0;
		int bd_num = 0;
		int i;

		if (IsPCI(pvs->type)) {
			if (bd_idx == -1) {
				bd_num = m_pDevice->GetBoardNum(pvs->idx);
				for (i = 0; i < bd_num; i++)
					 sensor_num += m_pDevice->GetSensorNum(pvs->idx, i);
			}
			else
				sensor_num = m_pDevice->GetSensorNum(pvs->idx, bd_idx);
		}
		else {
			sensor_num += m_pDevice->GetSensorNum(pvs->idx, 0);
		}

		return sensor_num;
	}
	return 0;
}

// 릴레이 몇개?
int CDevApiClass::GetRelayNum(LPVIDEO_SERVER pvs, int bd_idx)
{
	if (m_pDevice) {
		int relay_num = 0;
		int bd_num = 0;
		int i;

		if (IsPCI(pvs->type)) {
			if (bd_idx == -1) {
				bd_num = m_pDevice->GetBoardNum(pvs->idx);
				for (i = 0; i < bd_num; i++)
					 relay_num += m_pDevice->GetRelayNum(pvs->idx, i);
			}
			else
				relay_num = m_pDevice->GetRelayNum(pvs->idx, bd_idx);
		}
		else {
			relay_num = m_pDevice->GetRelayNum(pvs->idx, 0);
		}

		return relay_num;
	}
	return 0;
}

// 카메라 상태?
void CDevApiClass::GetCameraStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONGLONG* pdata)
{
	if (m_pDevice) {
		m_pDevice->GetCameraStatus(pvs->idx, bd_idx, pdata);
	}
}

// 센서 상태?
void CDevApiClass::GetSensorStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata)
{
	if (m_pDevice) {
		m_pDevice->GetSensorStatus(pvs->idx, bd_idx, pdata);
	}
}

// 릴레이 상태?
void CDevApiClass::GetRelayStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata)
{
	if (m_pDevice) {
		m_pDevice->GetRelayStatus(pvs->idx, bd_idx, pdata);
	}
}

// motion 상태?
void CDevApiClass::GetMotionStatus(LPVIDEO_SERVER pvs, int bd_idx, ULONG* pdata)
{
	if (m_pDevice) {
		m_pDevice->GetMotionStatus(pvs->idx, bd_idx, pdata);
	}
}


BOOL CDevApiClass::GetVSTime(LPVIDEO_SERVER pvs, LPSYSTEMTIME psysTime)
{
	if (m_pDevice) {
		return m_pDevice->GetVSTime(pvs->idx, psysTime);
	}
	return FALSE;
}

BOOL CDevApiClass::SetVSTime(LPVIDEO_SERVER pvs, LPSYSTEMTIME psysTime)
{
	if (m_pDevice) {
		return m_pDevice->SetVSTime(pvs->idx, psysTime);
	}
	return FALSE;
}

DWORD CDevApiClass::GetImageResType(LPVIDEO_SERVER pvs, int bd_id, int cam_id)
{
	if (m_pDevice)
		return m_pDevice->GetImageResType(pvs->idx, bd_id, cam_id);
	else return 0;
}
/////////////////////////////////////////////////////////////////////////////
// PCI stream encoding operations

// encoder 지정
void CDevApiClass::SetEncoder(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int codec)
{
	if (m_pDevice) {
		m_pDevice->SetEncoder(pvs->idx, bd_idx, cam_idx, (BYTE)codec);
	}
}

// encoder 해제
void CDevApiClass::ResetEncoder(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int codec)
{
	if (m_pDevice) {
		m_pDevice->ResetEncoder(pvs->idx, bd_idx, cam_idx, (BYTE)codec);
	}
}

// 압축 파라미터 지정하기. level이 높을수록 저화질
int CDevApiClass::SetEncodeLevel(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate)
{
	if (m_pDevice) {
		return m_pDevice->SetEncodeLevel(pvs, bd_id, cam_id, enc_level, frate);
	}
	return -1;
}

int CDevApiClass::SetEncodeLevelSub(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int enc_level, int frate)
{
	if (m_pDevice) {
		return m_pDevice->SetEncodeLevelSub(pvs, bd_id, cam_id, enc_level, frate);
	}
	return -1;
}

// PCI 영상 압축하기
int CDevApiClass::EncodeImage(LPVIDEO_SERVER pvs, int bd_id, int cam_id, LPBYTE pimage, LPBYTE dst, BOOL* pKeyOK)
{
	if (m_pDevice) {
		return m_pDevice->EncodeImage(pvs, bd_id, cam_id, pimage, dst, pKeyOK);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// serial port control

BOOL CDevApiClass::SerialPortGetState(LPVIDEO_SERVER pvs, int port, LPDCB pdcb)
{
	if (m_pDevice) {
		return m_pDevice->SerialPortGetState(pvs->idx, port, pdcb);
	}
	return FALSE;
}

BOOL CDevApiClass::SerialPortSetState(LPVIDEO_SERVER pvs, int port, DCB dcb)
{
	if (m_pDevice) {
		return m_pDevice->SerialPortSetState(pvs->idx, port, dcb);
	}
	return FALSE;
}

BOOL CDevApiClass::SerialPortSend(LPVIDEO_SERVER pvs, int port, LPSTR buf, int size)
{
	if (m_pDevice) {
		return m_pDevice->SerialPortSend(pvs->idx, port, buf, size);
	}
	return FALSE;
}

BOOL CDevApiClass::SerialPortRecv(LPVIDEO_SERVER pvs, int port, LPSTR buf, LPINT psize)
{
	if (m_pDevice) {
		return m_pDevice->SerialPortRecv(pvs->idx, port, buf, psize);
	}
	return FALSE;
}

BOOL CDevApiClass::SerialPortClearState(LPVIDEO_SERVER pvs, int port)
{
	if (m_pDevice) {
		return m_pDevice->SerialPortClearState(pvs->idx, port);
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// PTZ control

int CDevApiClass::GetPTZCommandString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int ptz_addr, int command, LPSTR buf)
{
	return 0;
}

BOOL CDevApiClass::IsPresetable(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx)
{			
	
	return FALSE;
}

BOOL CDevApiClass::SetParamIdx(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int param_idx)
{
	
	return TRUE;
}

BOOL CDevApiClass::SetParamString(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, LPSTR param_str)
{
	if (m_pDevice) {
		return m_pDevice->PresetSubInfo(pvs->idx, param_str);
	}
	return FALSE;
}

BOOL CDevApiClass::IsAutoScanUsed(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx)
{
	if (pvs->type == VS_IP_CAMERA) {
#if 0 // 140410
		switch (pvs->ni.vendor) {
		case TrueN:
			switch (pvs->ni.model) {
			case TCAM_570:		return TRUE;
			default:			return FALSE;
			} break;
		case ProbeD:
			switch (pvs->ni.model) {
			case PTI_H2100:		return TRUE;
			default:		return FALSE;
			}
		default:				return FALSE;
		}
#else
		return FALSE;
#endif
	}
	else {
		int ptz_idx = pvs->ci[bd_idx][cam_idx].ptz_info.ptz_manufact;
		return FALSE; // GetPtAutoScanUsed_NET(ptz_idx);
	}
	return FALSE;
}

BOOL CDevApiClass::SetAutoScanParamIdx(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx, int param_idx)
{
	
	return FALSE;
}

BOOL CDevApiClass::IsSpeedControlUsed(LPVIDEO_SERVER pvs, int bd_idx, int cam_idx)
{
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// overlay operations
void CDevApiClass::OvrSetOverlayWindow(CWnd* pWnd)
{
	if (m_pDevice) {
		m_pDevice->OvrSetOverlayWindow(pWnd);
	}
}

void CDevApiClass::OvrSetOverlayWindow(HWND hwnd)
{
	if (m_pDevice) {
		m_pDevice->OvrSetOverlayWindow(hwnd);
	}
}

BOOL CDevApiClass::OvrSetVideoFormat(ULONG vf)
{
	if (m_pDevice) {
		return m_pDevice->OvrSetVideoFormat(vf);
	}
	return FALSE;
}

BOOL CDevApiClass::OvrSetScreenMode(ULONG sm)
{
	if (m_pDevice) {
		return m_pDevice->OvrSetScreenMode(sm);
	}
	return FALSE;
}

BOOL CDevApiClass::OvrSetOverlayMode(int mode, int tx_mode)
{
	if (m_pDevice) {
		return m_pDevice->OvrSetOverlayMode(mode, tx_mode);
	}
	return FALSE;
}

void CDevApiClass::OvrShowOverlay(BOOL bInterlace)
{
	if (m_pDevice) {
		m_pDevice->OvrShowOverlay(bInterlace);
	}
}

void CDevApiClass::OvrMoveWantedRect(RECT over_rect)
{
	if (m_pDevice) {
		m_pDevice->OvrMoveWantedRect(over_rect);
	}
}

void CDevApiClass::OvrHideOverlay()
{
	if (m_pDevice) {
		m_pDevice->OvrHideOverlay();
	}
}

BOOL CDevApiClass::OvrCaptureEnable(int bd_id, BOOL bEnable)
{
	if (m_pDevice) {
		return m_pDevice->OvrCaptureEnable(bd_id, bEnable);
	}
	return FALSE;
}

BOOL CDevApiClass::OvrEnableVideo(int bd_id, int cam_id, int ch_id, BOOL bEnable)
{
	if (m_pDevice) {
		return m_pDevice->OvrEnableVideo(bd_id, cam_id, ch_id, bEnable);
	}
	return FALSE;
}

BOOL CDevApiClass::OvrSetSplit(int split_id, int start_ch, int count)
{
	if (m_pDevice) {
		return m_pDevice->OvrSetSplit(split_id, start_ch, count);
	}
	return FALSE;
}

void CDevApiClass::OvrArrayClear()
{
	if (m_pDevice) {
		m_pDevice->OvrArrayClear();
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// private functions

BOOL CDevApiClass::IsPCI(int type)
{
	return (type == VS_UBIQ_3388 ||
			type == VS_STRETCH_7);		// HD Board
}

/////////////////////////////////////////////////////////////////////////////
// V2.0.0.5
void CDevApiClass::SetDisplayArea(LPVIDEO_SERVER pvs, int bd_id, int cam_id, CRect rt)
{
	if (m_pDevice) {
		m_pDevice->SetDisplayArea(pvs->idx, 0, 0, rt);
	}
}

void CDevApiClass::SetOSDColor(LPVIDEO_SERVER pvs, COLORREF osd_color)
{
	if (m_pDevice) {
		m_pDevice->SetOSDColor(pvs->idx, osd_color);
	}
}

void CDevApiClass::SetOSDInfo(LPVIDEO_SERVER pvs, int bd_id, int cam_id, BOOL bShow, BOOL btime, int time_type, BOOL bosd, LPSTR osd, int pos, int translucent)
{
	if (m_pDevice) {
		m_pDevice->SetOSDInfo(pvs->idx, bd_id, cam_id, bShow, btime, time_type, bosd, osd, pos, translucent);
	}
}

/////////////////////////////////////////////////////////////////////////////
// V2.0.0.1
void CDevApiClass::SetWindowForDraw(LPVIDEO_SERVER pvs, int bd_id, int cam_id, HWND drawhwnd)
{
	if (m_pDevice) {
		m_pDevice->SetWindowForDraw(pvs->idx, 0, 0, drawhwnd);
	}
}

void CDevApiClass::SetPTZSpeed(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int ptz_addr, int cmd, DWORD dwSpeed)
{
	if (m_pDevice) {
		m_pDevice->SetPTZSpeed(pvs->idx, cmd, dwSpeed);
	}
}

BOOL CDevApiClass::SendPTZCommand(LPVIDEO_SERVER pvs, int bd_id, int cam_id, int ptz_addr, int cmd)
{
	if (m_pDevice) {
		if (pvs->type == VS_IP_CAMERA) {
			m_pDevice->SendPTZCommand(pvs->idx, cmd, 3000);
		}
		else {
			char buf[MAX_SERIAL_DATA_SIZE] = {0,};
			int len = GetPTZCommandString(pvs, bd_id, cam_id, ptz_addr, cmd, buf);
			if ((cmd == PTZ_INIT&&len==1) || cmd == PTZ_CLOSE)
				return TRUE;
			else if (len <= 0)
				return FALSE;

			Sleep(200);		// 2008.05.02
			if (SerialPortSend(pvs,	pvs->ci[bd_id][cam_id].ptz_info.serial_port.port, buf, len) == FALSE)
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// V2.0.0.6
//
// TV out
// 	m_TVlinkedBDid : tv가 연결된 보드의 id
//	m_VPid : tv의 id

/*BOOL CDevApiClass::ActivateExtVout(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->ActivateExtVout();
	}
	return FALSE;
}

BYTE CDevApiClass::GetExtVoutCardId(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->GetExtVoutCardId();
	}
	return 0;
}

void CDevApiClass::SetExtVout(LPVIDEO_SERVER pvs, unsigned long uSel, unsigned long uOut)
{
	if (m_pDevice) {
		m_pDevice->SetExtVout(uSel, uOut);
	}
}

void CDevApiClass::SetVoutMask(LPVIDEO_SERVER pvs, DWORD bd, DWORD port, DWORD type, DWORD uMask)
{
	if (m_pDevice) {
		m_pDevice->SetVoutMask(bd, port, type, uMask);
	}
}*/

int CDevApiClass::GetVoutNum(LPVIDEO_SERVER pvs, int bd_id)
{
	if (m_pDevice)
		return m_pDevice->GetVoutNum(pvs->idx, bd_id);
	return FALSE;
}

void CDevApiClass::SetOnBoardVout(LPVIDEO_SERVER pvs, DWORD bd, DWORD port, DWORD split, DWORD *cam_info)
{
	if (m_pDevice) {
		m_pDevice->SetOnBoardVout(pvs->idx, bd, port, split, cam_info);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 20090907 tizone VS_MNC for 360 camera - 업체 SDK를 이용해 display할 때의 mode
BOOL CDevApiClass::SetViewMode(int view_mode, BOOL reverse)
{
	if (m_pDevice) {
		return m_pDevice->SetViewMode(view_mode, reverse);
	}
	return FALSE;
}
BOOL CDevApiClass::SetViewAngle(int angle)
{
	if (m_pDevice) {
		return m_pDevice->SetViewAngle(angle);
	}
	return FALSE;
}
BOOL CDevApiClass::SetMotionArea(int area_idx, BOOL bEnable)
{
	if (m_pDevice) {
		return m_pDevice->SetMotionArea(area_idx, bEnable);
	}
	return FALSE;
}
BOOL CDevApiClass::ConfirmMotionArea(UINT uiMotionHoldTime)
{
	if (m_pDevice) {
		return m_pDevice->ConfirmMotionArea(uiMotionHoldTime);
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 140515 Audio UpLoad
BOOL CDevApiClass::Init2Way(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->Init2Way(pvs->idx);
	}
	return FALSE;
}
BOOL CDevApiClass::AudioUpload(LPVIDEO_SERVER pvs, LPBYTE pData, int size)
{
	if (m_pDevice) {
		return m_pDevice->AudioUpload(pvs->idx, pData, size);
	}
	return FALSE;
}
BOOL CDevApiClass::Close2Way(LPVIDEO_SERVER pvs)
{
	if (m_pDevice) {
		return m_pDevice->Close2Way(pvs->idx);
	}
	return FALSE;
}
BOOL CDevApiClass::GetAudioUploadFormat(LPVIDEO_SERVER pvs, WAVEFORMATEX* pWave)
{
	if (m_pDevice) {
		return m_pDevice->GetAudioUploadFormat(pvs->idx, pWave);
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////



BEGIN_MESSAGE_MAP(CDevApiClass, CWnd)
	//{{AFX_MSG_MAP(CDevApiClass)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDevApiClass message handlers
