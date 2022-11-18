#ifndef _COMMONDEF
#define _COMMONDEF

// system os type
// ---> nebutton.h 에도 중복 선언 되어 있음.
#define OS_WIN95					0
#define OS_WIN2K					1
#define OS_WIN7						2

#define OVERLAY_COLOR	RGB(10,0,10)	// NewBoard

// dvr type by board for network
#define NETEYE_IP		0x37
#define NETEYE_PC		0x38
#define NETEYE_HYBRID	0x39	
#define SECURUS_HYBRID	0x40   // For NewBoard
#define SECURUS_MPEG	0x41	// New MPEG
#define HD_HYBRID		0x42	// HD DVR
//#define HANNET_DVR	0x43
#define SECURUS_NVR		0x44

typedef VOID (CALLBACK*	VIDEO_DATA_CALLBACK)(LPVOID lpData);
typedef VOID (CALLBACK*	AUDIO_DATA_CALLBACK)(LPVOID lpData);

// user messages
// WM_USER+1 ~ WM_USER+100 : api와 application 사이에서 사용
// WM_USER+101 ~ WM_USER+200 : NetEye 내에서 사용
// WM_USER+201 ~ WM_USER+300 : NESetup 내에서 사용
// WM_USER+301 ~ WM_USER+500 : 기타 다른 application 내에서 사용
// WM_USER+501 ~ WM_USER+1000 : application 과 application 사이에서 사용
// WM_USER+1001 이상 : 공통으로 정의되어야 하는 메시지
#define WM_MOUSEINVOID				WM_USER+11
#define WM_MOUSEINLEFT				WM_USER+12
#define WM_MOUSEINRIGHT				WM_USER+13
#define WM_MOUSEINUPPER				WM_USER+14
#define WM_MOUSEINLOWER				WM_USER+15
#define WM_MOUSEINUPPERLEFT			WM_USER+16
#define WM_MOUSEINUPPERRIGHT		WM_USER+17
#define WM_MOUSEINLOWERLEFT			WM_USER+18
#define WM_MOUSEINLOWERRIGHT		WM_USER+19
#define WM_MOUSEWHEELINCLIENT		WM_USER+20

// 120127 RemoteService 와 ServerA 간에 사용
#define WM_CLIENT_WAITING			WM_USER+51 // Client 가 접속해왔을 때 RemoteService가 ServerA로 보냄. ServerA는 RemoteService에 Client의 정보를 묻게됨
#define WM_CLIENT_SLOT_REMOVE		WM_USER+52

// 140203 USB dongle (NE_DGL_API) 에서 dongle 이 없을 때 메인으로 보내는 메시지
#define WM_DONGLE_REMOVED			WM_USER+61

// 160130 Lotte
#define UM_ONECHANNEL_REQUEST		WM_USER+71 // Clilent가 1 채널만 요청했을 때 RService 에서 MainFrame 으로 보냄

// tree에서 item을 drag했을 경우 발생하는 메시지
#define WM_BEGIN_DRAG				WM_USER+1001
// button에서 마우스 왼쪽 버튼을 클릭했을 경우 발생하는 메시지
//--> nebutton.h 로 옮김
//#define UM_BUTTON_PRESSED			WM_USER+1011
//#define UM_BUTTON_RELEASED			WM_USER+1012

// interface for cms
#define WM_CMS_NOTIFY_MESSAGE		WM_USER+0
#define WM_QUERY_SYSTEMINFO			WM_USER+501
#define WM_QUERY_CAMCONTROL			WM_USER+502
#define WM_QUERY_RELAYCONTROL		WM_USER+503
#define WM_QUERY_SYSTEMCONTROL		WM_USER+504

// interface for setup
#define WM_RELOAD					WM_USER+601	// nesetup sends to neteye.
												//	and used by neteye on receiving
												//	WM_QUERY_SYSTEMCONTROL from agent (CMS)
#define WM_MEMMAP_START				WM_USER+602	// used by WM_COPYDATA from nesetup
#define WM_MEMMAP_STOP				WM_USER+603	// used by WM_COPYDATA from nesetup
#define WM_QUERY_VIDEO_EXISTS		WM_USER+604	// used by WM_COPYDATA from nesetup
#define WM_QUERY_MD_EXISTS			WM_USER+605	// used by WM_COPYDATA from nesetup
#define WM_COLOR_BRIGHT				WM_USER+606	// used by WM_COPYDATA from nesetup
#define WM_COLOR_CONTRAST			WM_USER+607	// used by WM_COPYDATA from nesetup
#define WM_COLOR_SATURATIONU		WM_USER+608	// used by WM_COPYDATA from nesetup
#define WM_COLOR_SATURATIONV		WM_USER+609	// used by WM_COPYDATA from nesetup
#define WM_COLOR_HUE				WM_USER+610	// used by WM_COPYDATA from nesetup
#define WM_MOTION_SENSITIVITY		WM_USER+611	// used by WM_COPYDATA from nesetup
#define WM_MOTION_RATE				WM_USER+612	// used by WM_COPYDATA from nesetup
#define WM_SETUP_CHANGED			WM_USER+613	// used by WM_COPYDATA from nesetup

#define WM_PTZ_CAMUP				WM_USER+614
#define WM_PTZ_CAMRIGHTUP			WM_USER+615
#define WM_PTZ_CAMRIGHT				WM_USER+616
#define WM_PTZ_CAMRIGHTDOWN			WM_USER+617
#define WM_PTZ_CAMDOWN				WM_USER+618
#define WM_PTZ_CAMLEFTDOWN			WM_USER+619
#define WM_PTZ_CAMLEFT				WM_USER+620
#define WM_PTZ_CAMLEFTUP			WM_USER+621
#define WM_PTZ_COMMAND_STOP			WM_USER+622
#define WM_PTZ_FOCUSIN				WM_USER+623
#define WM_PTZ_FOCUSOUT				WM_USER+624
#define WM_PTZ_ZOOMIN				WM_USER+625
#define WM_PTZ_ZOOMOUT				WM_USER+626
#define WM_PTZ_POWERON				WM_USER+627
#define WM_PTZ_POWEROFF				WM_USER+628
#define WM_PTZ_LAMPON				WM_USER+629
#define WM_PTZ_LAMPOFF				WM_USER+630
#define WM_PTZ_AUXON				WM_USER+631
#define WM_PTZ_AUXOFF				WM_USER+632
#define WM_PTZ_AUTOPANON			WM_USER+633
#define WM_PTZ_AUTOPANOFF			WM_USER+634
#define WM_PTZ_IRIS_OPEN			WM_USER+635
#define WM_PTZ_IRIS_CLOSE			WM_USER+636
#define WM_PTZ_PRESET_SET			WM_USER+637
#define WM_PTZ_GOTO_PRESET			WM_USER+638
#define WM_PTZ_TOUR_SET				WM_USER+639
#define WM_PTZ_TOURING_START		WM_USER+640
#define WM_PTZ_TOURING_STOP			WM_USER+641
#define WM_PTZ_SCAN_START			WM_USER+642
#define WM_PTZ_SCAN_STOP			WM_USER+643
#define WM_PTZ_CLEAR				WM_USER+644
#define WM_PTZ_ENTER				WM_USER+645
#define WM_PTZ_MENU					WM_USER+646
#define WM_PTZ_PTRN					WM_USER+647
#define WM_PTZ_GLOBAL				WM_USER+648
#define WM_PTZ_HOME					WM_USER+649
#define WM_PTZ_CAMUPSTOP			WM_USER+650
#define WM_PTZ_CAMRIGHTSTOP			WM_USER+651
#define WM_PTZ_CAMDOWNSTOP			WM_USER+652
#define WM_PTZ_CAMLEFTSTOP			WM_USER+653
#define WM_PTZ_PTZSETSPEED			WM_USER+654
//*** New MPEG
#define WM_MOTION_VIEW_SETUP		WM_USER+660	// used by WM_COPYDATA from nesetup
#define WM_MOTION_TEST				WM_USER+661 // used by WM_COPYDATA from nesetup
#define WM_MOTION_DETECTED			WM_USER+662 // used by WM_COPYDATA from main to nesetup
#define WM_POPUP_CLOSE				WM_USER+663
#define WM_CONNECT_DONE				WM_USER+664

// mouse position in a display window
#define MOUSE_ENTER_LEFT			0x01
#define MOUSE_ENTER_RIGHT			0x02
#define MOUSE_ENTER_UPPER			0x04
#define MOUSE_ENTER_LOWER			0x08
#define MOUSE_ENTER_UPPERLEFT		0x10
#define MOUSE_ENTER_UPPERRIGHT		0x20
#define MOUSE_ENTER_LOWERLEFT		0x40
#define MOUSE_ENTER_LOWERRIGHT		0x80

#define MAX_PCIBOARD				2 

//#define _HONEYWELL

#ifdef _HONEYWELL
	#define MAX_VIDEOSERVER				20
	#define MAX_CAMERA					20
	#define MAX_CHANNEL					MAX_CAMERA
#else
	#define MAX_VIDEOSERVER				64
	#define MAX_CAMERA					64
	#define MAX_CHANNEL					64
#endif

#define MAX_BOARD_PER_VS			2//4
#define MAX_CAMERA_PER_BOARD		64//140930 tizone//16//min(32,MAX_CAMERA)
#define MAX_AUDIO_PER_BOARD			64//140930 tizone//16//min(32,MAX_CAMERA)
#define MAX_DI_PER_BOARD			16//min(16,MAX_CAMERA)
#define MAX_DO_PER_BOARD			16//min(16,MAX_CAMERA)

#define MAX_MOTIONWINDOW_COUNT		16
#define MAX_TVOUT_SCENE				6

#define MAX_EVENTS					128		// event trigger or notify 관련 setting 개수는 최대 128개
#define MAX_EVENT_NOTIFY_SITE		32		// event notify site 수는 최대 32개
#define MAX_EVENT_EMAIL_SITE		32		// event notify email 수는 최대 32개
#define MAX_GAIN					15		// maximum volume 15
#define MIN_GAIN					0		// minimum volume 0
#define MAX_SERIAL_PORT				8		// 최대 serial port 개수는 8개
#define MAX_SERIAL_DATA_SIZE		1024	// maximum size of serial port in/out data is 1024
#define MAX_SCHEDULE_ROW			8
#define MAX_SCHEDULE_COL			25
#define MAX_STRING					128

#define MAX_HOLIDAYS				128
#define MAX_USER					32

#define MAX_PRESET					16
#define MAX_TOUR_MEMBER				16
#define MAX_TOUR					16
#define MAX_SCAN					16

#define MAX_TVOUT					8

#define MAX_YUV_VIDEO_DATA			1920*1080*2

// 20201016
#define MAX_LPRBOX					4

#define SDCAP_AUDIO_CHUNK_SIZE		4000	// capture 보드의 오디오 청크 하나는 4000바이트.
#define SDCAP_AUDIO_CHUNK_COUNT		1		// 1개가 0.5초 분량의 오디오.
#define SNCAP_AUDIO_CHUNK_SIZE		8192	// capture 보드의 오디오 청크 하나는 4000바이트.	// NewBoard
#define SNCAP_AUDIO_CHUNK_COUNT		2		// 2개가 0.5초 분량의 오디오.	// NewBoard

// message from
#define FROM_MANAGER				1
#define FROM_SETUP					2

// communication message field size
#define FLD_SZ_LEN					4
#define FLD_SZ_CMD					2
#define FLD_SZ_IP					16
#define FLD_SZ_URL					50
#define FLD_SZ_PORT					4
#define FLD_SZ_ACTION				4
#define FLD_SZ_ACTPARAM				4
#define FLD_SZ_ID					16
#define FLD_SZ_PWD					16
#define FLD_SZ_DESC					32
#define FLD_SZ_ERRNO				4
#define FLD_SZ_DWORD				4
#define FLD_SZ_STRING				MAX_STRING

// number of events
#define NUM_EVENTS					6
#define VIDEO_EVENTS				2		// NewBoard [Video Thread]
#define EXT_EVENTS					4

// video server type

// Board Type
#define VS_UNKNOWN					0		// unknown
#define VS_SHH_UDP3000				33		// H.264 Board (UDP)
#define VS_SHH_UDP4000				34		// H.264 Stretch Board (UDP)
#define VS_UBIQ_3388				50
#define VS_STRETCH_7				64
//--- IP Camera -------------------------------------
#define VS_IP_CAMERA				70
// 아래 Type 값은 CreateDevice() 호출 시에만 사용하는 값이고
// 모든  IP camera 의 Type 값은 'VS_IP_CAMERA' 만 사용한다.
#define VS_TRUEN					701
#define VS_TRUEN_PTZ				702
#define VS_TRUEN_ZOOM				703
#define VS_LG						706
#define VS_LG_PTZ					707
#define VS_VISIONHI					711
#define VS_VISIONHI_PTZ				712
#define VS_PRIME					716
#define VS_HIK						721
#define VS_HIK_PTZ					722
#define VS_ACTI						726
#define VS_ACTI_PTZ					727
#define VS_PROBED					731
#define VS_CELLINX					736
#define VS_SEYEONTECH				741
#define VS_HITRON					746
#define VS_NEOCOM					751
#define VS_DAHUA					756 // 200728
#define VS_DAHUA_LPR				757 // 200728
//---------------------------------------------------

#define BD_UQ3388_1204				900
#define BD_UQ3388_2408				901
#define BD_UQ3388_2416				902
#define BD_UQ3388_4816				903
#define BD_STRETCH_D1				1003
#define BD_STRETCH_HD7				1004
#define IP_CAMERA					1005
// 100번이상은 비디오 카메라 혹은 비디오 서버


// ip type
#define IP_NONE						0
#define IP_FIXED					1
#define IP_DYNAMIC					2

// video format
#define VF_NTSC						1
#define VF_PAL						2

// video mode (single/quad/switching)
#define VM_NOTHING					0
#define VM_SINGLE					1
#define VM_QUAD						2
#define VM_SWITCHING				3

// sensor type
#define ALM_NO						0		// normal open
#define ALM_NC						1		// normal close

// relay operation schedule
#define RELAY_INACTIVE				0x01
#define RELAY_ACTIVE				0x02

#define NOTIFY_INACTIVE				0x01
#define NOTIFY_ACTIVE				0x02

// event type for setup
#define EVT_REC_START				11
#define EVT_REC_STOP				12
#define EVT_MOTION					13
#define EVT_SENSOR					14
#define EVT_VIDEO					15
#define EVT_VCABRIDGE				21
#define EVT_LPRBOX					31		// 20201016
#define EVT_USER					99		// 141030 tizone firewatch

// event type for cms
#define EVT_SHUTDOWN				1		// shutdown
#define EVT_VIDEO_RECOVER			2		// 비디오 복구
#define EVT_VIDEO_LOSS				3		// 비디오 단락
#define EVT_MOTION_START			4		// 모션시작
#define EVT_MOTION_END				5		// 모션끝
#define EVT_SENSOR_START			6		// 센서시작
#define EVT_SENSOR_END				7		// 센서끝
// 141030 tizone firewatch
#define EVT_USER_FIRE				11		// 화재
#define EVT_USER_SMOKE				12		// 연기,분진
#define EVT_USER_HEAT_SENSOR		13		// 온도
#define EVT_USER_FIRE_ALARM			14		// 화재감지기
#define EVT_VCABRIDGE_START			21		// VCA event ON
#define EVT_VCABRIDGE_END			22		// VCA event OFF
// 20201016 event subtype
#define EVT_LPRBOX_START			31
#define EVT_LPRBOX_END				32
// event notify type
#define EVT_NOTIFY_TYPE_TRIGGER		1
#define EVT_NOTIFY_TYPE_LOCAL		2
#define EVT_NOTIFY_TYPE_REMOTE		3
#define EVT_NOTIFY_TYPE_EMAIL		4

// notify status define
#define EVT_NOTI_FULLSCREEN			0x00000001
#define EVT_NOTI_BEEP				0x00000002
#define EVT_NOTI_BLINK				0x00000004
#define EVT_NOTI_SITE				0x00000008
#define EVT_NOTI_EMAIL				0x00000010
#define EVT_NOTI_RELAY				0x00010000
#define EVT_NOTI_PRESET				0x00020000
#define EVT_NOTI_AUDIOSEND			0x00000020 // 140515

// video server status
#define VS_ST_ERROR					-1
#define VS_ST_NOTHING				0
#define VS_ST_ACTIVE				1		// connected
#define VS_ST_RECONNECT				2

// camera status flags
#define CAM_ST_NOTHING				0x00
#define CAM_ST_REC_ACTIVE			0x01
#define CAM_ST_VIEW_ACTIVE			0x02
#define CAM_ST_ERROR				0x10

// channel status flags
#define CH_ST_NOTHING				0
#define CH_ST_ACTIVE				1
#define CH_ST_MOTION				2
#define CH_ST_SENSOR				3
#define CH_ST_VIEW_LOSS				4		// 2008.03.24
#define CH_ST_ERROR					5		// 2008.06.25	비디오 서버 연결이 안된 상태

// sensor status
#define SNS_ST_OPEN					1
#define SNS_ST_CLOSE				2

// relay status
#define RELAY_ST_OFF				0
#define RELAY_ST_ON					1

// PTZ Use
#define PTZ_NOT_USE					0
#define PTZ_LOCAL_USE				1
#define PTZ_REMOTE_USE				2

// 다른 application에서 WM_QUERY_*로 문의한 상태에 대한 return값
#define QUERY_ST_NOTHING			1
#define QUERY_ST_CONNECTED			2
#define QUERY_ST_VIDEO_EXISTS		3
#define QUERY_ST_MD_EXISTS			4

// camera type
#define CT_NORMAL					1
#define CT_PTZ						2
#define CT_ZOOM						3

// serial port type
#define SPT_NONE					0
#define SPT_RS232					1
#define SPT_RS422					2

// recording method - neteye definition
#define REC_NOT						0x01
#define REC_CONTINUOUS				0x02
#define REC_MOTION					0x04
#define REC_SENSOR					0x08		// NewBoard
#define REC_MOTION_SENSOR			0x10
#define REC_NORMAL_IN_EVENT			0x20
#define REC_EVENT					REC_NORMAL_IN_EVENT
// recording method - db manager definition
#define REC_DB_CONTINUOUS			1
#define REC_DB_SENSOR				2
#define REC_DB_MOTION				3
#define REC_DB_SENSOR_OR_MOTION		4
#define REC_DB_NORMAL_IN_EVENT		5

// NewBoard

#define RGB_NOREC					RGB(196,196,255)//흰색
#define RGB_CONTINUOUS				RGB(000,250,128)//녹색
#define RGB_MOTION					RGB(255,250,000)//노랑
#define RGB_SENSOR					RGB(255,178,111)//오랜지색
#define RGB_SENSORANDMOTION			RGB(255,191,220)//빨강
#define RGB_CONTIANDEVENT			RGB(158,217,251)// 하늘 92,166,242  //134,190,249 // 

#define RGB_SEL_NOREC				RGB(196,196,255)//흰색
#define RGB_SEL_CONTINUOUS			RGB(000,255,128)//녹색
#define RGB_SEL_MOTION				RGB(255,255,000)//노랑
#define RGB_SEL_SENSOR				RGB(255,183,111)//오랜지색
#define RGB_SEL_SENSORANDMOTION		RGB(255,196,220)//빨강
#define RGB_SEL_CONTIANDEVENT		RGB(158,222,251)// 하늘 92,166,242  //134,190,249 // 

// NewBoard

// draw channel boundary as ... 20091208
#define DR_NORMAL					0
#define DR_MOTION					1
#define DR_SENSOR					2
#define DR_VIDEOLOSS				3

//*** For Data structure define value between DVR with dll
// audio frame type
#define FT_PCM						1
#define FT_G723						2
#define FT_MP2						3
#define FT_G721						4			// New MPEG

// frame type
#define FT_UNKNOWN					0
#define FT_AUDIO					1
#define FT_VIDEO					2
#define FT_MD_FULL					3

// 141030 tizone firewatch
#define FT_USER_EVENT				10	//9// 140404
#define FT_USER_EVENT_FIRE			11	// 화재
#define FT_USER_EVENT_SMOKE			12	// 연기,분진
#define FT_USER_EVENT_HEAT_SENSOR	13	// 온도
#define FT_USER_EVENT_FIRE_ALARM	14	// 화재감지기

// video frame type
#define FT_MJPEG					20
#define FT_MP4_I					21
#define FT_MP4_P					22
#define FT_MP4_B					23
#define FT_H263_I					24
#define FT_H263_P					25
#define FT_H264_I					26
#define FT_H264_P					27

// 150204 tizone udp vca
#define FT_META_UDPVCA				40

// image format
#define IMG_RGB24					1
#define IMG_RGB32					2
#define IMG_YUY2					3
#define IMG_YV12					4
//***

// image resolution
#define VIDEO_360_240				2
#define VIDEO_720_480				2
#define VIDEO_1280_720				2
#define VIDEO_1920_1080				2
#define	VIDEO_720P60				2
#define VIDEO_720P50				2
#define VIDEO_720P30				2
#define VIDEO_720P25				2
#define VIDEO_1080P30				2
#define VIDEO_1080P25				2
#define VIDEO_1080P60				2
#define VIDEO_1080P50				2
#define VIDEO_1080I60				2
#define VIDEO_1080I50				2

//*** For Data structure define value FileSystem and network header
// data type	=> use in network header
#define DATA_AUDIO					1
#define DATA_VIDEO					2
#define DATA_META_FIREWATCH			3
#define DATA_META_UDPVCA			4

// recording object
#define REC_VIDEO					0x00
#define REC_AUDIO					0x01

#define REC_I_FRAME					0x01
#define REC_P_FRAME					0x02
#define REC_B_FRAME					0x03

// audio codec type
#define CODEC_PCM_101				101			// ( 8000, 8) PCM with Header
#define CODEC_MPEG1_102				102			// (32000,16) uucore (modified mp4)
#define CODEC_PCM_103				103			// ( 8000, 8) PCM with Header
#define CODEC_PCM_104				104			// (16000,16) PCM with Header
#define CODEC_PCM_105				105			// ( 8192,16) PCM with Header
#define CODEC_PCM_106				106			// (32000,16) PCM without Header
#define CODEC_G721_107				107			// ( 8000,16) g721 without Header
#define CODEC_PCM_108				108			// ( 8000,16) PCM with Header
#define CODEC_UNKNOWN				CODEC_NONE

// video codec type
#define CODEC_NONE					0
#define CODEC_HWMP4					1			// uucore (mp4)
#define CODEC_MULTIVIA_R			2			// multivia remote
#define CODEC_MULTIVIA_L			3			// multivia local
#define CODEC_XVID					4			// xvid (mp4)
#define CODEC_MV_MPEG4				5			// 
#define CODEC_MVCODEC				CODEC_MV_MPEG4// MV codec (modified mp4)
#define CODEC_HUVIRON				6			// huviron sdk (mp4)
#define CODEC_MICROWEB				7			// miroweb (mp4)
#define CODEC_H264_SEO				8			// H.264
#define CODEC_MNC					9			// mnc
#define CODEC_ACTI					10
#define CODEC_MJPEG					11
#define CODEC_H264_COMART			12			// Comart Codec
#define CODEC_H264_STRETCH			13
#define CODEC_H264_UDPD1			14
// refer to netdevdef.h of servera ///////////////////////////////////////
#define CODEC_NET_H264				21			// servera - sntsdk codec
#define CODEC_NET_MP4				22			// servera - sntsdk codec
#define CODEC_NET_MJPEG				23			// servera - sntsdk codec
/////////////////////////////////////////////////////////////////////////////
#define CODEC_JPEG					51
#define CODEC_UNKNOWN				CODEC_NONE
//***

// schedule type
#define SCHEDULE_RECORD				0x01
#define SCHEDULE_RELAY				0x02
#define SCHEDULE_NOTIFY				0x04

// overlay mode
#define OVR_MODE_SINGLE				1
#define OVR_MODE_CASCADE			2
#define OVR_MODE_COMBINE			3

// network action flags
#define ACT_NETWORK					0x00800000	// network action
#define ACT_NCONNECTION				0x00800001	// connection status
#define ACT_NPROPERTY				0x00800002	// property changed
#define ACT_NSERIALDATA				0x00800004  // serial data arrived. 20080624

// request action flags
#define ACT_AUDIO					0x10000000	// command "QA"
#define ACT_CODIMAGE				0x20000000	// command "QC"
#define ACT_RAWIMAGE				0x40000000	// command "QV"
#define ACT_SALARM					0x80000000	// command "QS"
#define ACT_MALARM					0x01000000	// command "QS"
#define ACT_VALARM					0x02000000	// command "QS"
#define ACT_RELAYON					0x04000000	// command "QR"
#define ACT_RELAYOFF				0x08000000	// command "QR"
#define ACT_PROPVF					0x00000001	// command "QP". Video Format
#define ACT_PROPIF					0x00000002	// command "QP". Image Format
#define ACT_PROPRES					0x00000004	// command "QP". Image Resolution
#define ACT_PROPFR					0x00000008	// command "QP". Frame Rate
#define ACT_PROPGOP					0x00000010	// command "QP". GOP size
#define ACT_PROPST_AUD				0x00000020	// command "QP". Audio enable/disable
#define ACT_PROPST_VID				0x00000040	// command "QP". Video enable/disable
#define ACT_PROPST_MD				0x00000080	// command "QP". MD/MD Sensitivity enable/disable
#define ACT_PROPBR_VBRQ				0x00000100	// command "QP". Quant
#define ACT_PROPBR_CBR				0x00000200	// command "QP". Quant+CBR
#define ACT_PROPBR_VBRH				0x00000400	// command "QP". Quant+Max+Min
#define ACT_PROPAUD_GAIN			0x00000800	// command "QP". Audio Gain
#define ACT_PROPAUD_BPS				0x00001000	// command "QP". Audio Bits Per Sample
#define ACT_PROPAUD_FREQ			0x00002000	// command "QP". Audio Sampling Frequency
#define ACT_PROPIQ_BRIGHT			0x00004000	// command "QP". Bright
#define ACT_PROPIQ_CONTRAST			0x00008000	// command "QP". Contrast
#define ACT_PROPIQ_SATURATION_U		0x00010000	// command "QP". Saturation U
#define ACT_PROPIQ_SATURATION_V		0x00020000	// command "QP". Saturation V
#define ACT_PROPIQ_HUE				0x00040000	// command "QP". Hue
#define ACT_PROPVM					0x00080000	// command "QP". Video Mode (single/quad/switching)

// data on/off request flags
#define DATA_OFF					0
#define DATA_ON						1

// tv out type
#define TV_ONBOARD_VGA				0
#define TV_ONBOARD_USER				1
#define TV_EXT_BD					2
#define TV_ONBOARD_HDMI				3

#define TV_EXT1604					2
#define TV_EXT3204					3
#define TV_EXT6404					4

// error #
#define ERRNO_NO_ERROR				0
#define ERRNO_BASE					10000
#define ERRNO_NO_ACCOUNTS			ERRNO_BASE+1
#define ERRNO_PASSWORD_ERROR		ERRNO_BASE+2
#define ERROR_NO_RIGHTS				ERRNO_BASE+3
#define ERRNO_NOT_LOGON				ERRNO_BASE+4
#define ERRNO_CONNECT_FAIL			ERRNO_BASE+5

// ptz commands
enum ptz_commands
{
	PTZ_INIT,
	PTZ_CAM_UP,
	PTZ_CAM_RIGHTUP,
	PTZ_CAM_RIGHT,
	PTZ_CAM_RIGHTDOWN,
	PTZ_CAM_DOWN,
	PTZ_CAM_LEFTDOWN,
	PTZ_CAM_LEFT,
	PTZ_CAM_LEFTUP,
	PTZ_CAM_AUTOPAN_ON,
	PTZ_CAM_AUTOPAN_OFF,
	PTZ_CAM_FOCUSIN,
	PTZ_CAM_FOCUSOUT,
	PTZ_CAM_ZOOMIN,
	PTZ_CAM_ZOOMOUT,
	PTZ_CAM_FLIP,
	PTZ_IRC,						// iris
	PTZ_IRO,						// iris
	PTZ_IRA,						// iris
	PTZ_PRESET_ADD,
	PTZ_PRESET_REMOVE,
	PTZ_PRESET_GOTO,
	PTZ_TOURING_ADD,
	PTZ_TOURING_REMOVE,
	PTZ_TOURING_START,
	PTZ_TOURING_STOP,
	PTZ_SCANNING_START,
	PTZ_SCANNING_END,
	PTZ_SCANNING_ON,
	PTZ_SCANNING_OFF,
	PTZ_POWER_ON,
	PTZ_POWER_OFF,
	PTZ_LAMP_ON,
	PTZ_LAMP_OFF,
	PTZ_AUX_ON,
	PTZ_AUX_OFF,
	PTZ_PAN_SPEED_DEFAULT,
	PTZ_PAN_SPEED_UP,
	PTZ_PAN_SPEED_DOWN,
	PTZ_TILT_SPEED_DEFAULT,
	PTZ_TILT_SPEED_UP,
	PTZ_TILT_SPEED_DOWN,
	PTZ_ZOOM_SPEED_DEFAULT,
	PTZ_ZOOM_SPEED_UP,
	PTZ_ZOOM_SPEED_DOWN,
	PTZ_FOCUS_SPEED_DEFAULT,
	PTZ_FOCUS_SPEED_UP,
	PTZ_FOCUS_SPEED_DOWN,

	PTZ_SPEED_DEFAULT,
	PTZ_SPEED_UP,
	PTZ_SPEED_DOWN,
	PTZ_MENU_PRESET,	// added 20090513
	PTZ_MENU_TOUR,		// added 20090513
	PTZ_MENU_PATTERN,	// added 20090513
	PTZ_MENU_SCAN,		// added 20090513
	PTZ_MENU,			// added 20090513
	PTZ_GLOBAL,			// added 20090513
	PTZ_CLEAR,			// added 20090513
	PTZ_ENTER,			// added 20090513
	PTZ_HOME,			// added 20090513

	PTZ_SET_SPEED,		// 20180115

	PTZ_COMMAND_STOP,
	PTZ_CLOSE,

	// Maximum number of PTZ commands
	MAX_PTZ_COMMANDS
};

// structures
//- dims   newboard 
typedef struct _tagDIMS
{
	int				use;
	int				registed;
	char			server_ip[FLD_SZ_STRING];
	int				server_port;
	char			host_name[FLD_SZ_STRING];
	char			web_id[FLD_SZ_ID];
	char			web_pwd[FLD_SZ_PWD];
	char			web_name[FLD_SZ_STRING];
	char			email[FLD_SZ_STRING];
} DIMS, *LPDIMS;  

typedef struct _tagAuto
{
	int				use;
	int				hour;
	int				minute;
} AUTO, *LPAUTO;

typedef struct _tagAutoSchedule
{
	AUTO			auto_reboot[8];
	AUTO			auto_shutdown[8];
} AutoSchedule, *LPAutoSchedule;

// structure for serial port
typedef struct _tagSERIAL_PORT
{
	int				port;
	int				type;
	int				baudrate;
	int				parity;
	int				databits;
	int				stopbits;

} SERIAL_PORT, *LPSERIAL_PORT;

//- system
typedef struct _tagNETEYE_SYSTEM
{
	char			desc[FLD_SZ_DESC];
	char			adminname[FLD_SZ_DESC];			//newboard 
	char			admintelno[FLD_SZ_DESC];			//newboard 

	int				max_camera;

	int				use_liveconnect;								//newboard 
	int				bandwidth;										//newboard 
	int				r_port;												//newboard 
	int				w_port;
	int				search_port;										//newboard 
	int				web_port;											//newboard 	
	int				audio_port;										//newboard 
	int				agent_port;										//newboard 
	
	DIMS			dims;
	int				video_format;
	int				use_watchdog;
	int				use_sbackup;
	int				use_daylight;
	int				use_logoff_time;							//newboard 
	int				logoff_time;									//newboard 
	char			daylight_save_path[MAX_PATH];
	char			odd_format_tool[MAX_PATH];				// 일단 안쓴다.

	int				motion_log;
	int				sensor_log;
	int				event_log;
	AutoSchedule	auto_shedule;

	BOOL			bVideoloss_Beep;
	BOOL			bVideoloss_CMS;
	BOOL			bVideoloss_EMail;

	BOOL			bBackup_config;
	char			backup_config_drive;

	BOOL			disp_heat_error;
	BOOL			disp_hdd_error;

	int				dvr_id;								// for GSP
	BOOL			use_keyboard;						// for GSP
	SERIAL_PORT		keyboard_serial_port;				// for GSP

	DWORD			window_key_lock;
} NETEYE_SYSTEM, *LPNETEYE_SYSTEM;    

//- pci board
typedef struct _tagPCI_NAME
{
	int				type;
	char			name[FLD_SZ_STRING];

} PCI_NAME, *LPPCI_NAME;

// camera
typedef struct _tagRESOLUTION
{
	int				w;
	int				h;

} RESOLUTION, *LPRESOLUTION;

typedef struct _tagCOLOR
{
	int				brightness;
	int				contrast;
	int				saturationU;
	int				saturationV;
	int				hue;
	int				modified;

} COLOR, *LPCOLOR;

typedef struct _tagFUNCTION
{
	WORD			lsave:1;		// requested for save video
	WORD			lsave_aud:1;	// requested for save audio
	WORD			ldisp:1;		// requested for display video
	WORD			lplay_aud:1;	// requested for play audio
	WORD			rvideo:1;		// requested for remote view
	WORD			raudio:1;		// requested for remote audio
	WORD			reserved:10;	// reserved for more functions

} FUNCTION, *LPFUNCTION;

typedef struct _tagAUDIO
{
	int				l_idx;			// logical audio index matched with the camera
	int				bd_idx;			// physical audio board index
	int				aud_idx;		// physical audio index
	int				gain;			// audio gain
	int				modified;
} AUDIO, *LPAUDIO;

// structure for preset
typedef struct _tagPRESET
{
	int				idx;
	char			desc[FLD_SZ_DESC];
} PRESET, *LPPRESET;

typedef struct _tagPRESETINFO
{
	int				count;
	PRESET			preset[MAX_PRESET];
} PRESETINFO, *LPPRESETINFO;

// structure for touring
typedef struct _tagTOURMEMBER
{
	int				preset_idx;
	int				duration;

} TOURMEMBER, *LPTOURMEMBER;

typedef struct _tagTOURING
{
	int				idx;
	char			desc[FLD_SZ_DESC];
	int				member_count;
	TOURMEMBER		member[MAX_TOUR_MEMBER];
	int				current_member_idx;

} TOURING, *LPTOURING;

typedef struct _tagTOURINGINFO
{
	int				count;
	TOURING			touring[MAX_TOUR];
	int				current_tour_idx;

} TOURINGINFO, *LPTOURINGINFO;

//** scan
typedef struct _tagSCANNING
{
	int				idx;
	char			desc[FLD_SZ_DESC];
} SCANNING, *LPSCANNING;

typedef struct _tagSCANINFO
{
	int				count;
	SCANNING		scanning[MAX_SCAN];
	int				current_scan_idx;
} SCANINFO, *LPSCANINFO;
//**

// structure for ptz camera
typedef struct _tagPTZINFO
{
	int				id;
	int				ptz_manufact;	// index of ptz of pt.dll
	SERIAL_PORT		serial_port;
	PRESETINFO		preset_info;
	TOURINGINFO		touring_info;
	SCANINFO		scanning_info;	// scan
} PTZINFO, *LPPTZINFO;

typedef struct _tagSCHEDULE_INFO
{
	WORD			cells[MAX_SCHEDULE_ROW][MAX_SCHEDULE_COL];

} SCHEDULE_INFO, *LPSCHEDULE_INFO;

typedef struct _tagMOTION
{
	int				sensitivity;	// hardware에서 motion 발생으로 인정하는 임계값 (percent. 1-100)
									// 모션으로 표현된 부분이 (100-임계값)을 넘으면 모션으로 인정
	int				rate;			// software에서 motion 발생으로 인정하는 임계값 (percent. 1-100)
									// 모션으로 표현된 부분이 임계값을 넘으면 모션으로 인정
	__int64			last_compared_time;
									// motion compare gap... 0.5sec.
} MOTION, *LPMOTION;

typedef struct _tagMOTIONWINDOW
{
	RECT			rect;
	MOTION			motion;

} MOTIONWINDOW, *LPMOTIONWINDOW;

typedef struct _tagMOTIONINFO
{
	BOOL			bPatialArea;
	int				window_count;
	MOTIONWINDOW	motion_window[MAX_MOTIONWINDOW_COUNT];

} MOTIONINFO, *LPMOTIONINFO;

typedef struct _tagTDSINFO        // new board 
{
	int                            use;                 // 0: not used   1: used
	LOGFONT			font;
	COLORREF		fgColor;	
	int				txtmode;         // 1: DateTime  2: Camera information 3: ALL
	int             position;         // Text Alignment    1: TopLeft  2:Top Center 3: Top Right
	                                  // Text Alignment     4: BottomLeft  5:Bottom Center  6: Bottom Right      	
	int				format_datetime;
}TDSINFO, *LPTDSINFO;             // new board 

typedef struct _tagCAMERA_INFO
{
	int				idx;
	int				use;
	int				hide;
	char			desc[FLD_SZ_DESC];
	int				vs_idx;
	int				bd_idx;
	int				type;			// 1 : CT_NORMAL, 2 : CT_PTZ, 3 : CT_ZOOM
	int				codec_id;

	PTZINFO			ptz_info;

	int				res_type;
	int				res_type_sub;
	RESOLUTION		res;
	RESOLUTION		res_sub;
	int				recording_res_decimation;
	int				frate;
	int				frate_sub;
	int				encoding_type;		// 0 : vbr, 1: cbr
	int				encoding_type_sub;	// 0 : vbr, 1: cbr
	int				img_quality;
	int				img_quality_sub;

	int				record_stream;	// 1:firststream 2:secondstream
	int				display_stream;	// 1:firststream 2:secondstream
	int				transmit_stream;// 1:firststream 2:secondstream

	COLOR			color;			// bright, contrast, saturation, hue

	int				use_audio;		// camera has an assigned audio?
	AUDIO			audio;			// audio characteristics

	TDSINFO			tds_info;		// new board 
	
	MOTIONINFO		motion_info;

	int				use_sensor;
	int				sensor_count;					// 카메라에 연동되는 센서의 개수
	int				sensor_idx[MAX_DI_PER_BOARD];	// 카메라에 연동되는 sensor_count만큼의 센서의 인덱스들 (0-based)

	int				use_vcabridge;
	int				vcabridge_count;				// 카메라에 연동되는 VCABridge 의 개수
	int				vcabridge_idx[MAX_CHANNEL];		// 카메라에 연동되는 vcabridge_count만큼의 VCABridge의 인덱스들 (0-based)

	// 20201016
	int				use_lprbox;		// lprbox 와 연동되는지 여부
	int				lprbox_idx;		// 연동된 lprbox의 인덱스
	int				lprbox_show;      // 2021.01.05 csnam
	char			lprData[20];
	//////////////////////////////////////////////////////////////

	SCHEDULE_INFO	schedule;		// 0x01 : not record
									// 0x02 : continuous
									// 0x04 : motion
									// 0x08 : sensor
	int				cur_schedule;	// 현재 시간대의 스케줄
	int				pre_seconds;	// 이벤트 저장 시 저장 할 pre-event 시간
	int				post_seconds;	// 이벤트 저장 시 저장 할 post-event 시간
	BOOL			bMotion;
	BOOL			bSensor;

	FUNCTION		func;			// local save video
									// local display video
									// local save audio
									// local play audio
									// remote display video
									// remote play audio

	int				save_idx;		// index for save. saved by saver and used by playback
	int				disp_idx;		// channel index for display
	LPBYTE			video_data;		// decoding된 YUY2형태의 비디오 데이터
	int				is_valid_video_data;
									// video_data buffer의 유효성 check flag
	LPBYTE			audio_data;		// audio data (0.5초 분랑 = 약 8000바이트)
	int				status;			// CAM_ST_ERROR			= 0xff : error
									// CAM_ST_NOTHING		= 0x00 : nothing
									// CAM_ST_REC_ACTIVE	= 0x01 : active recording
									// CAM_ST_VIEW_ACTIVE	= 0x02 : active view
	int				savefps;
	int				savefps_prev;
	int				dispfps;
	int				dispfps_prev;

} CAMERA_INFO, *LPCAMERA_INFO;

//- sensor
typedef struct _tagSENSOR_INFO
{
	int				idx;
	int				type;			// 0 : normal open  (ALM_NO)
									// 1 : normal close (ALM_NC)
	char			desc[FLD_SZ_DESC];
	int				vs_idx;
	int				bd_idx;
	int				status;

} SENSOR_INFO, *LPSENSOR_INFO;

//- relay
typedef struct _tagRELAY_INFO
{
	int				idx;
	char			desc[FLD_SZ_DESC];
	int				vs_idx;
	int				bd_idx;
	int				status;
	SCHEDULE_INFO	schedule;		// 0x01 : RELAY_OFF (not operate)
									// 0x02 : RELAY_ON (do operate)
	int				post_seconds;	// RELAY ON 을 유지해야하는 시간

} RELAY_INFO, *LPRELAY_INFO;

//- video server
typedef struct _tagDIPM
{
	char			ip_url[FLD_SZ_STRING];
	char			id[FLD_SZ_STRING];
	char			pwd[FLD_SZ_STRING];
	int				port; // dipm port

} DIPM, *LPDIPM;

typedef struct _tagDYNDNS
{
	char			hostname[FLD_SZ_STRING];
	char			id[FLD_SZ_STRING];
	char			pwd[FLD_SZ_STRING];

} DYNDNS, *LPDYNDNS;

typedef struct _tagDDNSINFO
{
	BOOL			ddns_type;		// 0 : dipm
									// 1 : dynddns
	union {
		DIPM		dipm;
		DYNDNS		dyndns;
	};

} DDNSINFO, *LPDDNSINFO;

typedef struct _tagNETWORK_INFO
{
	int				protocol_type;	// 0=Onvif 1=RTSP
	int				ip_type;		// 0=IP_NONE
									// 1=IP_FIXED
									// 2=IP_DYNAMIC
	TCHAR			ip_url[FLD_SZ_STRING];
	TCHAR			ip[FLD_SZ_IP];
	int				http_port;

	TCHAR			uri[MAX_STRING];	// general service uri. get in Discovery phase
	TCHAR			analytics_uri[MAX_STRING];
	TCHAR			device_uri[MAX_STRING];
	TCHAR			events_uri[MAX_STRING];
	TCHAR			imaging_uri[MAX_STRING]; // color control
	TCHAR			media_uri[MAX_STRING]; // getprofiles, getstreamuri
	TCHAR			ptz_uri[MAX_STRING];

	TCHAR			profile_name[64]; // if onvif, use profile
	TCHAR			profile_token[64];
	TCHAR			videosource_token[64]; // 140314
	TCHAR			relay_token[64]; // 140515 
	TCHAR			url_add[MAX_STRING];
	int				rtsp_protocol;	// 0=RTSP UDP, 1=RTSP TCP, 2=Over HTTP
	int				rtp_protocol;	// 0=rtp_udp, 1=rtp_tcp // 140314
	int				rtsp_port;

	// added for second stream
	TCHAR			profile_name2[64]; // if onvif, use profile
	TCHAR			profile_token2[64];
	TCHAR			videosource_token2[64]; // 140314
	TCHAR			url_add2[MAX_STRING]; // second stream
	int				rtsp_protocol2;	// second stream 0=RTSP UDP, 1=RTSP TCP, 2=Over HTTP
	int				rtp_protocol2;	// 0=rtp_udp, 1=rtp_tcp // 140314
	int				rtsp_port2;		// second stream

	int				record_stream;	// 1:firststream 2:secondstream
	int				display_stream;	// 1:firststream 2:secondstream
	int				transmit_stream;// 1:firststream 2:secondstream
	int				internet_stream;// 1:firststream 2:secondstream
	/////////////////////////////////////////////////////////////////////////////

	TCHAR			id[32];
	TCHAR			pwd[64];

	int				vendor;
	int				model;

} NETWORK_INFO, *LPNETWORK_INFO;

typedef struct _tagVIDEO_SERVER
{
	int				idx;
	int				type;			// 0  : unknown
									// x  : local, means board (capture)
									// 2x : local, means board (overlay)
									// 1xx : NVS series
									// 2xx : NVE series
	int				use;
	NETWORK_INFO	ni;
	char			desc[FLD_SZ_DESC];
	int				bd_count;
	int				bd_type[MAX_BOARD_PER_VS];
	int				cam_count[MAX_BOARD_PER_VS];
	int				audio_count[MAX_BOARD_PER_VS];
	CAMERA_INFO		ci[MAX_BOARD_PER_VS][MAX_CAMERA_PER_BOARD];
	int				sensor_count[MAX_BOARD_PER_VS];
	SENSOR_INFO		si[MAX_BOARD_PER_VS][MAX_DI_PER_BOARD];
	int				relay_count[MAX_BOARD_PER_VS];
	RELAY_INFO		ri[MAX_BOARD_PER_VS][MAX_DO_PER_BOARD];

	int				video_format[MAX_BOARD_PER_VS];	// 1 : VF_NTSC  // 2 : VF_PAL
	int				nSupportSpot[MAX_BOARD_PER_VS];

	HANDLE			events[NUM_EVENTS];
									// [0] : connection event
									// [1] : encoded image data event
									// [2] : raw image data event
									// [3] : audio event
									// [4] : alarm event
									// [5] : video status changed event
	CWinThread*		pEventThread;
	HANDLE			hEventThread;

	int				network_frate;			// NewBoard
	int				reconnect_count;
	int				reconnect_interval;
	int				reconnect_time;

	int				status;			// VS_ST_ERROR	   = -1: error
									// VS_ST_NOTHING   = 0 : nothing
									// VS_ST_ACTIVE    = 1 : connected
									// VS_ST_RECONNECT = 2 : reconnecting

} VIDEO_SERVER, *LPVIDEO_SERVER;

typedef struct _tagVIDEO_SERVER_INFO
{
	int				vs_count;
	VIDEO_SERVER	vs[MAX_VIDEOSERVER];

} VIDEO_SERVER_INFO, *LPVIDEO_SERVER_INFO;

//- hardware properties
typedef struct _tagHWPROPERTY {
	ULONG				uCommand;
	ULONG				uBoard;
	ULONG				uChannel;
	union {
		ULONG			uParam[4];
		struct {
			ULONG		uParam0;
			ULONG		uParam1;
			ULONG		uParam2;
			ULONG		uParam3;
		};
	};
	BOOL				bRet;

} HWPROPERTY, *LPHWPROPERTY;

//- holiday
typedef struct _tagHOLIDAY
{
	int				year;
	int				month;
	int				day;
	char			desc[FLD_SZ_DESC];

} HOLIDAY, *LPHOLIDAY;

typedef struct _tagHOLIDAY_INFO
{
	int				count;
	HOLIDAY			holidays[MAX_HOLIDAYS];

} HOLIDAY_INFO, *LPHOLIDAY_INFO;

//- display channel
typedef struct _tagCHANNEL
{
	int				idx;			// array상의 순서
	int				order;			// 화면상의 순서
//	NETWORK_INFO	ni;
	int				vs_type;
	int				vs_idx;
	int				bd_idx;
	int				cam_idx;
	int				w;
	int				h;
	BOOL			bZoom;
	float			zoom_tl_x;
	float			zoom_tl_y;
	float			zoom_dr_x;
	float			zoom_dr_y;
	RECT			zoom_rect;
	int				status;			// CH_ST_NOTHING = 0 : nothing
									// CH_ST_ACTIVE  = 1 : active
									// CH_ST_MOTION  = 2 : motion detected
									// CH_ST_SENSOR  = 3 : sensor detected

} CHANNEL, *LPCHANNEL;

typedef struct _tagCHANNEL_INFO
{
	DWORD			enabled_splits[2];
									// 32bit*2 operation - 사용가능한 split의 종류는 "VPSplits.h"에 있다.
	int				split_idx;
	int             use_fullsplit;		// NewBoard
	int				switching_time;
	int				display_ratio;
	int				use_deinterlace;
	int				use_osd;
	int				latency; // 141003 tizone. display buffering time (100~1000 ms)
	LOGFONT			font;
	COLORREF		fgColor[3];
	CHANNEL			channel[MAX_CHANNEL];

} CHANNEL_INFO, *LPCHANNEL_INFO;

//- recording
typedef struct _tagRECORD
{
	int				idx;
	int				use;
	
//	NETWORK_INFO	ni;
	int				vs_type;
	int				vs_idx;
	int				bd_idx;
	int				cam_idx;
	
//	BYTE			method;
/*	SCHEDULE_INFO	schedule;		// 0x01 : not record
									// 0x02 : continuous
									// 0x04 : motion
									// 0x08 : sensor
	int				sensor_count;	// 저장 방법에 센서가 포함되는 경우에 사용
									// 저장을 위해 대기해야 하는 센서의 개수
	int				sensor_idx[MAX_DI_PER_BOARD];
									// 저장 방법에 센서가 포함되는 경우에 사용
									// 저장을 위해 대기해야 하는 sensor_count만큼의 센서의 인덱스들 (0-based)
	int				pre_seconds;	// 이벤트 저장 시 저장 할 pre-event 시간
	int				post_seconds;	// 이벤트 저장 시 저장 할 post-event 시간
*/	int				audio;

} RECORD, *LPRECORD;

typedef struct _tagRECORD_INFO
{
	RECORD			record[MAX_CAMERA];

} RECORD_INFO, *LPRECORD_INFO;

//- notify
typedef struct _tagSITE
{
	NETWORK_INFO	ni;

} SITE, *LPSITE;

typedef struct _tagSITE_INFO
{
	int				count;
	SITE			site[MAX_EVENT_NOTIFY_SITE];
	int				duration;
} SITE_INFO, *LPSITE_INFO;

typedef struct _tagEMAIL
{
	BOOL		bDefaultServer;
	char		addr[FLD_SZ_STRING];
	int			port;
	char		id[FLD_SZ_STRING];
	char		pwd[FLD_SZ_STRING];

	char		from[FLD_SZ_STRING];     // newboard 
	char		to[FLD_SZ_STRING];			// newboard 
	char		cc[FLD_SZ_STRING];			// newboard 
	int			uselogin;								// newboard 	
	int			useattach;							// newboard 
	int			usessl;  // 111226 SSL
} EMAIL, *LPEMAIL;

typedef struct _tagEMAIL_INFO
{
	int				count;
	EMAIL			email[MAX_EVENT_EMAIL_SITE];
	int				duration;
} EMAIL_INFO, *LPEMAIL_INFO;

// Attach Image in Email	2008.06.10
#define		EMAIL_ST_NOTHING		0
#define		EMAIN_ST_REQ_JPG		1
#define		EMAIL_ST_READY_SEND		2
#define		EMAIL_ST_SENDED			3

typedef struct _tagEMAIL_NOTIINFO
{
	int				status;
	int				type;
	SYSTEMTIME		tm_sended;
	int				tm_duration;
	int				related_record_idx;
	char			body[511];
	int				vs_idx;
} EMAIL_NOTIINFO, *LPEMAIL_NOTIINFO;
//***

typedef struct _tagTRIGGER
{
	int				r_count;
	int				r_actor[MAX_BOARD_PER_VS*MAX_DO_PER_BOARD];
									// MAKELONG(bd_idx,relay_idx)
//	int				p_count;
//	int				p_actor[MAX_BOARD_PER_VS*MAX_CAMERA_PER_BOARD];
//									// MAKELONG(bd_idx,cam_idx)
//	int				p_preset[MAX_BOARD_PER_VS*MAX_CAMERA_PER_BOARD];
									// preset 번호

} TRIGGER, *LPTRIGGER;

typedef struct _tagNOTIFY_LOCAL
{
	int				method;			// EVT_NOTI_FULLSCREEN 0x01
									// EVT_NOTI_BEEP       0x02
									// EVT_NOTI_BLINK      0x04
	int				displayduration;		// duration for notifying event (seconds).

	int				audio_out;
	int				audio_out_to_system;
	int				audio_out_to_IPCam;
	char			audio_file[MAX_PATH];
	int				audio_repeat;
} NOTIFY_LOCAL, *LPNOTIFY_LOCAL;

typedef struct _tagNOTIFY_REMOTE
{
	int				method;			//
//	char			info[FLD_SZ_STRING];
	SCHEDULE_INFO	schedule;
	
	int				site_count;
	int				site[MAX_EVENT_NOTIFY_SITE];		// 대상 list의 인덱스
} NOTIFY_REMOTE, *LPNOTIFY_REMOTE;

typedef struct _tagEVENT_INFO
{
	NOTIFY_LOCAL	notify_local;
	NOTIFY_REMOTE	notify_remote;
	NOTIFY_REMOTE	notify_email;
	TRIGGER			trigger;
} EVENT_INFO, *LPEVENT_INFO;

typedef struct _tagEVENTS
{
	EVENT_INFO		event_info[MAX_VIDEOSERVER][MAX_BOARD_PER_VS][MAX_CAMERA_PER_BOARD];
} EVENTS, *LPEVENTS;


//- vca
typedef struct _tagVCABRIDGE_INFO
{
	BOOL			use;
	TCHAR			desc[FLD_SZ_STRING];
	TCHAR			ip_url[FLD_SZ_STRING];
	TCHAR			tag[FLD_SZ_STRING];
	int				status;						// 0:Alarm OFF, 1:Alarm ON

} VCABRIDGE_INFO, *LPVCABRIDGE_INFO;

typedef struct _tagVCA_INFO
{
	HANDLE			vcabridge_event;
	int				vcabridge_port;
	int				vcabridge_licensecount;
	int				vcabridge_usecount;
	VCABRIDGE_INFO	vcabridge[MAX_CHANNEL];
	EVENT_INFO		vcabridge_eventinfo[MAX_CHANNEL];

	CWinThread*		pEventThread;
	HANDLE			hEventThread;

} VCA_INFO, *LPVCA_INFO;

// 20201016 LPR /////////////////////////////////////////////////////
typedef struct
{
	int				lprbox_idx;
	int				vs_idx;
	int				bd_idx;
	int				cam_idx;
	int				save_idx;
	char			desc[FLD_SZ_STRING];

} LPRBOX_EVENT, *LPLPRBOX_EVENT;
typedef struct 
{
	BOOL			use;
	TCHAR			desc[FLD_SZ_STRING];
	TCHAR			ip_url[FLD_SZ_STRING];
	int				status;						// 0:Alarm OFF, 1:Alarm ON

} LPRBOX_INFO, *LPLPRBOX_INFO;
typedef struct 
{
	HANDLE			lprbox_event;	// lpr event thread에 event 발생을 알리는 이벤트 핸들

	int				lprbox_use;		// lprbox 하나라도 사용하면 TRUE

	int				lprbox_port;
	BOOL			lprbox_autorun;
	TCHAR			lprbox_user[FLD_SZ_STRING];
	TCHAR			lprbox_pwd[FLD_SZ_STRING];
	TCHAR			lprbox_exefilename[MAX_PATH];

	LPRBOX_INFO		lprbox[MAX_LPRBOX];
	EVENT_INFO		lprbox_eventinfo[MAX_LPRBOX];

	CWinThread*		pEventThread;	// lpr event thread 포인터
	HANDLE			hEventThread;	// lpr event thread 핸들

} LPR_INFO, *LPLPR_INFO;
////////////////////
//2020.12.23 csnam 
///////////////////////////////////////////////////////////////////////
using namespace std;
//#include <iostream>
//#include <vector>
////
//typedef struct
//{
//	int				aibox_idx;
//	int				vs_idx;
//	int				bd_idx;
//	int				cam_idx;
//	int				save_idx;
//
//
//	UINT 			event_id;
//	string			device_id;
//	UINT			channel_id;
//	string 			happen_time;  // time_t
//	UINT			time_zone;
//	string			notifyName;
//	vector<AIEvent> events;
//
//} AIBOX_EVENT, *LPAIBOX_EVENT;

typedef struct            /// aibox 장비에 대한 정보  
{
	BOOL			use;
	TCHAR			desc[FLD_SZ_STRING];
	TCHAR			ip_url[FLD_SZ_STRING];
	int				status;						// 0:Alarm OFF, 1:Alarm ON

} AIBOX_INFO, * LPAIBOX_INFO;
// <<---- 20201223 CSNAM EVENT
typedef struct
{

	HANDLE			lpaibox_event;	// ai event thread에 event 발생을 알리는 이벤트 핸들
	int				lpaibox_use;	// aibox 하나라도 사용하면 TRUE
	int				lpaibox_port;

	LPAIBOX_INFO	lpaibox_info[MAX_LPRBOX];
	EVENT_INFO		lprbox_eventinfo[MAX_LPRBOX];

	CWinThread*		pEventThread;	// ai event thread 포인터
	HANDLE			hEventThread;	// ai event thread 핸들

}AIBOX_BASIC_INFO, * LPAIBOX_BASIC_INFO;

///////////////////////////////////////////////////////////
typedef struct _tagEVENTDATA
{
	int				ch;						// channel
	char			desc[FLD_SZ_STRING];

} EVENT_DATA, *LPEVENT_DATA;



//- data
typedef struct _tagACTIONS
{
	DWORD			action;			// 0x00010000 : audio
									// 0x00020000 : image
									// 0x00100000 : sensor alarm
									// 0x00200000 : motion alarm
									// 0x00400000 : video loss alarm
									// 0x10000001 : relay on
									// 0x10000000 : relay off
									// 0x20000000 : change property of video format
									// 0x40000000 : change property of image resolution
	DWORD			param;			// action == 0x00010000 : audio
									// action == 0x00020000 : image
									// action == 0x00200000 : motion alarm
									// action == 0x00400000 : video loss alarm
									// action == 0x20000000 : change property of video format
									// action == 0x40000000 : change property of image resolution
									//		WORD low = board index
									//		WORD high = camera index
									// action == 0x00100000 : sensor alarm
									//		WORD low = board index
									//		WORD high = alarm index
									// action == 0x10000001 : relay on
									// action == 0x10000000 : relay off
									//		WORD low = board index
									//		WORD high = relay index

} ACTIONS, *LPACTIONS;

typedef struct _tagDATA
{
	int				vs_idx;
	int				type;			// action
	int				bd_idx;
	int				cam_idx;
	int				frame_type;
	RESOLUTION		res;
	int				header_size;
	LPVOID			pheader;
	int				data_size;
	LPVOID			pdata;
	__int64			dts;

} DATA_INFO, *LPDATA_INFO;

typedef struct _tagMIXED_SPLIT
{
	int		split;
	int		rotation_interval;
	int		channel[MAX_CAMERA];
} MIXED_SPLIT, *LPMIXED_SPLIT;

typedef struct _tagTVOUT
{
	int				type;
	
	int				use;
	int				vs_idx;
	int				bd_idx;
	int				port;

	int				rotation_interval;

	int				split;			// 40이면 mixed split 사용
	DWORD			nEnableCamera;
	int				channel_sequence[MAX_CAMERA];
	MIXED_SPLIT		mixed_split[MAX_TVOUT_SCENE];

	BOOL			one_split;
	BOOL			motion;
	BOOL			sensor;
	int				event_interval;
	int				priority; // 0:same, 1:1split, 2:motion, 3:sensor
} TVOUT, *LPTVOUT;

typedef struct _tagTVOUT_INFO
{
	int				nTVOUT;
	TVOUT			tv_out[MAX_TVOUT];
} TVOUT_INFO, *LPTVOUT_INFO;

/////////////////////////////////////////////////////////////////////////////////////////
// for nesetup interfaces

typedef struct _tagMYCOPYDATA
{
	NETWORK_INFO	ni;
	int				vs_type;
	int				bd_idx;
	int				cam_idx;
	int				value;

} MYCOPYDATA, *LPMYCOPYDATA;

// setup result
#define SR_NOTHING					0		// nothing

#define SR_MODIFIED					1		// modified
#define SR_REMOVED					2		// removed vs
#define SR_ADDED					3		// added vs
#define SR_MODIFIED_NEED_CONNET		4		// modified but need to connect

typedef struct _tagSETUPVS
{
	NETWORK_INFO	ni;
	int				setup;			// SR_NOTHING
									// SR_MODIFIED
									// SR_ADDED
} SETUPVS, *LPSETUPVS;

typedef struct _tagSETUPRESULT
{
	int				modified;		// SR_NOTHING
									// SR_MODIFIED
									// SR_REMOVED
	SETUPVS			vs[MAX_VIDEOSERVER];

} SETUPRESULT, *LPSETUPRESULT;

//*** New MPEG
typedef struct tagSM_POSITION
{
	int		bd_idx;
	int		cam_idx;
	RECT	rectOverlay;
}SM_POSITION,*LPSM_POSITION;

typedef struct tagSM_MOTION_TEST
{
	BOOL		bTest;
	int			vs_idx;
	int			bd_idx;
	int			cam_idx;
	MOTIONINFO	motion_info;
}SM_MOTION_TEST, *LPSM_MOTION_TEST;

typedef struct tagSM_MOTION_DETECT
{
	BOOL		bDetect;
	int			vs_idx;
	int			bd_idx;
	int			cam_idx;
}SM_MOTION_DETECT, *LPSM_MOTION_DETECT;
//***

// end of nesetup interfaces
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// for PC-DVR interfaces
#pragma pack(1)
typedef union tagORIGIN
{
    BYTE		cno;		// 카메라번호
    BYTE		sno;		// 센서번호    
} ORIGIN; 

typedef struct tagNETDATAHDR
{
    SYSTEMTIME	tm;			// 생성시간(16)
    BYTE		origintype;	// 영상/음성/문자/센서/모션(1)
    ORIGIN		origin;		// 카메라 번호/센서 번호(1)
    DWORD		savetype;	// 저장종류&백업카운트&압축종류(4)
    DWORD		size;		// 실 데이터 크기(4)
} NETDATAHDR, *LPNETDATAHDR, FSDATAHDR, *LPFSDATAHDR;
#pragma pack()

typedef struct _tagNEW_NETDATAHDR			// New MPEG
{
	SYSTEMTIME		tm;
	WORD			wChannel;
	WORD			wFrameType;
	WORD			wDataType;
	WORD			wSaveMode;
	WORD			wCodecType;
	WORD			wHeight;
	WORD			wWidth;
	WORD			wGabage;
	DWORD			dwSize;		
} NEW_NETDATAHDR, *LPNEW_NETDATAHDR;
// end of PC-DVR interfaces
/////////////////////////////////////////////////////////////////////////////////////////

// 20090612 change port
typedef struct _tagPORTINFO
{
	unsigned int v_stream;
	unsigned int a_stream;
	unsigned int http;
	unsigned int event;
	unsigned int control;
	unsigned int ptz;
	unsigned int reserved[10];

} PORTINFO, *LPPORTINFO;

typedef struct _tagTEXTDATA
{
	int			nCam;
	int			nStartX;
	int			nStartY;
	int			nEndX;
	int			nEndY;
	int         nLen;
	char  	    szData[255];
}TEXTDATA, *LPTEXTDATA;

//////////////////////////////////////////////////////////////////////////////////////
// servera - netdevdef.h

typedef struct tagNetDevDataHdr {
	int version;
	int	reserved1;
	__int64 dts;
	int slot_idx;
	int codec_id;
	int data_type; // video/audio
	int frame_no;
	int width;
	int height;
	int length;
	int reserved2;

} stNetDevDataHdr, NETDEVDATAHDR, *LPNETDEVDATAHDR;
///////////////////////////////////////////////////////////////////////////////////////

#endif

