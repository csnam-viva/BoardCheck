#ifndef _NETDEVDEF
#define _NETDEVDEF

//#include "stdafx.h"

#define MAX_NETDEV_COUNT 64

//bitrate_type
#define BITRATE_ANY 0
#define BITRATE_CBR 1
#define BITRATE_VBR 2

#define FPS_ANY 0

#define MODEL_ANY 0

#define GOP_ANY 0

//data_type
#define STREAM_ANY 0x0
#define STREAM_VIDEO 0x1
#define STREAM_AUDIO 0x2

#define PORT_ANY 0

// RemoteService 와 ServerA 간에 사용
#ifndef WM_CLIENT_WAITING
#define WM_CLIENT_WAITING			WM_USER+51 // Client 가 접속해왔을 때 RemoteService가 ServerA로 보냄. ServerA는 RService에 Client의 정보를 묻게됨
#endif
#ifndef WM_CLIENT_SLOT_REMOVE
#define WM_CLIENT_SLOT_REMOVE		WM_USER+52 // 접속이 끊어졌을 때 RemoteService가 ServerA로 보냄.
#endif
#ifndef WM_GET_PROFILES
#define WM_GET_PROFILES				WM_USER+53 // Client 가 프로파일 정보를 요청했을 때 RemoteService가 ServerA로 보냄.
#endif

// MainFrame 과 NetDevInst 간에 사용
#ifndef TM_START_SERVICE
#define TM_START_SERVICE			WM_USER+61 // Slot 에 새로운 network device 가 할당되었을 때, 접속과 영상 요청을 시작하라는 메시지 
#endif
#ifndef TM_STOP_SERVICE
#define TM_STOP_SERVICE				WM_USER+62 // network device 에 대한 영상 요청을 중지하고 접속을 끊으라는 메시지 
#endif
#ifndef TM_RESTART_SERVICE
#define TM_RESTART_SERVICE			WM_USER+63 // network device 에 대한 영상 요청을 중지하고 접속을 끊으라는 메시지 
#endif

// NetDevInst 내에서 사용
#ifndef TM_UPDATE_PROFILES
#define TM_UPDATE_PROFILES			WM_USER+64 // Network Device가 지원하는 Profile 의 Type들을 수신한 후 분석하기 위해 사용
#endif

// refer to commondef.h
#define CODEC_NET_H264				21			// servera - sntsdk codec
#define CODEC_NET_MP4				22			// servera - sntsdk codec
#define CODEC_NET_MJPEG				23			// servera - sntsdk codec

//#define _IFRAMEHEADER_V2	// 150511 tizone
typedef struct tagIFrameHeader {
#ifdef _IFRAMEHEADER_V2
	char* acSPS;
	char* acPPS;
#else
	char acSPS[256];
	char acPPS[256];
#endif
	int  nSPSLength;
	int  nPPSLength;

} stIFrameHeader, IFRAMEHEADER, *LPIFRAMEHEADER;
typedef struct tagStreamInfo {
	UINT rtsp_port;		// offset:0
	DWORD res_w;
	DWORD res_h;		// offset:8
	int codec_id;
	DWORD bitrate_type; // offset:16
	DWORD bitrate;
	DWORD fps;			// offset:24
	DWORD gop;
	DWORD data_type;	// offset:32
	DWORD conn_type;	// rtsp connection type 0:udp,1:tcp // 140314
	DWORD stream_type;	// rtp streaming type 0:udp,1:tcp // 143014
	char videosource_token[64]; // offset:40 140314
	char profile_token[64];// offset:40
	char media_addr[256];// offset:168 --> total 424 bytes(8*53)

} stStreamInfo, STREAMINFO, *LPSTREAMINFO;
typedef struct tagSlotInfo {
	int slot_id;		// offset:0
	UINT vendor_id;		// offset:4
	char vendor[32];	// offset:8
	UINT model_id;		// offset:40
	char model[32];		// offset:44
	char ip_addr[16];	// offset:76
	UINT http_port;		// offset:92
	char user_id[32];	// offset:96
	char user_pwd[64];	// offset:128
	DWORD protocol;		// 0=onvif,1=rtsp offset:192
	int cam_id;			// offset:196

	char uri[128];
	char analytics_uri[128];
	char device_uri[128];
	char events_uri[128];
	char imaging_uri[128];
	char media_uri[128];
	char ptz_uri[128];

	STREAMINFO first_stream; // +424 offset:200
	STREAMINFO second_stream;// +424 offset:624

	int record_stream;	// offset:1048
	int display_stream;	// offset:1052
	int transmit_stream;// offset:1056 --> total 1056 bytes (8*132)

} stSlotInfo, SLOTINFO, *LPSLOTINFO;

#endif // _NETDEVDEF
