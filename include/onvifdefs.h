#pragma once

enum ECodecID
{
	CID_NONE = 0,					///					NONE

	CID_RAW,						///					Raw Image ( not encoding )
	CID_MJPEG,						/// [JPEG]
	CID_MPEG4,						/// [MP4V-ES]
	CID_H264,						/// [H264]

	CID_PCM = 0x80,					///					PCM data ( not encoding )
	CID_ALAW,						/// [PCMA]			G.711 a-law
	CID_ULAW,						/// [PCMU]			G.711 u-law
	CID_G721,						///					G.721
	CID_G723_24,					///					G.723 24bit
	CID_G723_40,					///					G.723 40bit
	CID_G726_16U,					///					G.726 16bit u-law
	CID_G726_24U,					///					G.726 24bit u-law
	CID_G726_32U,					///					G.726 32bit u-law
	CID_G726_40U,					///					G.726 40bit u-law
	CID_G726_16A,					///					G.726 16bit a-law
	CID_G726_24A,					///					G.726 24bit a-law
	CID_G726_32A,					///					G.726 32bit a-law
	CID_G726_40A,					///					G.726 40bit a-law
	CID_G726_16L,					///	[G726-16]		G.726 16bit lenear
	CID_G726_24L,					///	[G726-24]		G.726 24bit lenear
	CID_G726_32L,					///	[G726-32]		G.726 32bit lenear
	CID_G726_40L,					///					G.726 40bit lenear
};

inline BOOL IsVideoCodec(INT nCodecID)
{
	return nCodecID != CID_NONE && nCodecID < 0x80 ;
}

inline BOOL IsAudioCodec(INT nCodecID)
{
	return nCodecID != CID_NONE && nCodecID >= 0x80;
}


typedef struct _tagOnvifParamProfile
{
	char strName[64];
	char strToken[64];
	char strVideoSource[64]; // 140314
	int nCodecID;
	int nWidth;
	int nHeight;
	int nQuality;
	int nFramerate;
	int nBitrate;
} OnvifParamProfile, *LPOnvifParamProfile;

// 140314
typedef struct _tagMinMaxValues
{
	int nMin;
	int nMax;
	int nCur;
} MINMAXVALUES;

typedef struct _tagOnvifParamImaging
{
	MINMAXVALUES Brightness;
	MINMAXVALUES ColorSaturation;
	MINMAXVALUES Contrast;
	MINMAXVALUES Sharpness;
	int IrisMode; // 0=AUTO 1=MANUAL
	MINMAXVALUES Iris;
	int FocusMode; // 0=AUTO 1=MANUAL
	int FocusType; // 0=Continuous 1=Absolute 2=Relative
	MINMAXVALUES FocusPosition; // absolute/relative 에서만 사용
	MINMAXVALUES FocusSpeed;
	MINMAXVALUES FocusNearLimit;
	MINMAXVALUES FocusFarLimit;
} OnvifParamImaging, *LPOnvifParamImaging;

typedef struct _tagDeviceInfo
{
	char name[64];
	char mac[64]; // 150416 size: 32 --> 64 (urn:uuid:A1E8A6A3-6040-40F5-A6DE-000ADE009900 형태로 넘어오는 애들이 있음)
	char ip[128]; // 150306 size: 16 --> MAX_STRING(128. defined in commondef.h)
	UINT port;
	char id[32];
	char pwd[64];
	char uri[128];
	char analytics_uri[128];
	char device_uri[128];
	char events_uri[128];
	char imaging_uri[128];
	char media_uri[128];
	char ptz_uri[128];
	char vendor[32];
	char model[32];
	char fwversion[64];
	int rtp_tcp; // 140314
	int rtp_rtsp_tcp; // 140314
	int camera;
	int sensor;
	int relay;
	char strRelay[64]; // 140515
	int audio_in;
	int audio_out;
	int ptz;
} DeviceInfo, *LPDeviceInfo;

