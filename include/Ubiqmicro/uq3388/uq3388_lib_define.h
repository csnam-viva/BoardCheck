#pragma once

///////////////////////////////////////////////////////////////////////////////
// SYSTEM
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Generic Define
///////////////////////////////////////////////////////////////////////////////
#define 	UQ_MAX_BOARD												4
#define 	UQ_MAX_CHIP													16
#define 	UQ_MAX_VCH													64
#define 	UQ_MAX_ACH													64

//-----------------------------------------------------------------------------
// Process State
//-----------------------------------------------------------------------------
#define		UQ_STATE_NULL 		                  1
#define		UQ_STATE_INIT 		                  2
#define		UQ_STATE_SETUP 	                    4
#define		UQ_STATE_RUN 		                    8

//-----------------------------------------------------------------------------
// VIDEO INPUT
//-----------------------------------------------------------------------------
#define		UQ_NO_INPUT		                      0
#define		UQ_HD_SDI			                      1
#define		UQ_EX_SDI			                      2
#define		UQ_AHD				                      3
#define		UQ_CVI				                      4
#define		UQ_TVI				                      5
#define		UQ_NTSC				                      6
#define		UQ_PAL				                      7
#define		UQ_AHDX				                      8
#define		UQ_CVIX				                      9

#define		UQ_720P24			                      0
#define		UQ_720P25			                      1
#define		UQ_720P30			                      2
#define		UQ_720P50			                      3
#define		UQ_720P60			                      4
#define		UQ_1080P24	    	                  5
#define		UQ_1080P25	    	                  6
#define		UQ_1080P30	    	                  7
#define		UQ_480i60			                      8
#define		UQ_576i60			                      9

#define		UQ_LOCK				                      1
#define		UQ_LOSS				                      0

//-----------------------------------------------------------------------------
// CORE_MODE
//-----------------------------------------------------------------------------
#define UQ3388_HB_HD_04CH_RT									0
#define UQ3388_AN_HD_08CH_RT									1
#define UQ3388_AN_HD_16CH_NRT									2
//#define UQ3388_AN_HD_16CH_NRTA							3
#define UQ3388_DEC_04CH_RT										4
#define UQ3388_DEC_16CH_RT										5
#define UQ3388_AN_HD_16CH_RT									6

//-----------------------------------------------------------------------------
// STREAM TYPE
//-----------------------------------------------------------------------------
#define UQ_VIDEO_PREVIEW                      0
#define UQ_VIDEO_MAIN_PREVIEW 				 	      0
#define UQ_VIDEO_SUB_PREVIEW    				      1
#define UQ_VIDEO_STREAM                       2
#define UQ_VIDEO_MAIN_STREAM    				      2
#define UQ_VIDEO_SUB_STREAM      			        3
#define UQ_VIDEO_DECODE                       4
#define UQ_AUDIO_ENCODE                       5
#define UQ_AUDIO_DECODE               	      6
#define UQ_VIDEO_JPEG               	        7

//-----------------------------------------------------------------------------
// Color Format
//-----------------------------------------------------------------------------
#define UQ_CF_422_PL 			                    0
#define UQ_CF_420_PL 			                    1
#define UQ_CF_NV12 					                  2
#define UQ_CF_YUY2 					                  3
#define UQ_CF_ARGB 														4	//unsigned int	data[0] => {A[31:24], R[23:16], G[15:8], B[7:0]}	//The horizontal size should be a multiple of 4
#define UQ_CF_RGB 														5	//unsigned char data[0] => R , data[1] => G, data[2] => B					//The horizontal size should be a multiple of 16
#define UQ_CF_BGR 														6	//unsigned char data[0] => B , data[1] => G, data[2] => R					//The horizontal size should be a multiple of 16
//-----------------------------------------------------------------------------
// CVBS
//-----------------------------------------------------------------------------
#define	UQ_CVBS_LINE_525			 	        			0
#define	UQ_CVBS_LINE_625			 	        			1
#define	UQ_CVBS_NTSC_M			 		        			0
#define	UQ_CVBS_NTSC_J					        			1
#define	UQ_CVBS_NTSC_443				      				2
#define	UQ_CVBS_NTSC_N					        			3
#define	UQ_CVBS_PAL_BDGHI			      					4
#define	UQ_CVBS_PAL_N					            		5
#define	UQ_CVBS_PAL_M					            		6
#define	UQ_CVBS_PAL_NC					          		7
#define	UQ_CVBS_PAL_60					          		8

//-----------------------------------------------------------------------------
// HDMI
//-----------------------------------------------------------------------------
#define	UQ_HDMI_1920x1080p 			      				0
#define	UQ_HDMI_1280x960p 				      			1
#define	UQ_HDMI_1280x720p 				      			2
#define	UQ_HDMI_1024x768p 				      			3
#define	UQ_HDMI_800x600p 				        			4
#define	UQ_HDMI_640x480p 				        			5
#define	UQ_HDMI_1920x1080i 			        			6
#define	UQ_HDMI_24BIT	 				          			0
#define	UQ_HDMI_16BIT	 				          			1
#define	UQ_HDMI_RGB	 				            			0
#define	UQ_HDMI_YUV	 				            			1

//-----------------------------------------------------------------------------
// RATECTRL
//-----------------------------------------------------------------------------
#define UQ_RC_CBR 				                    0
#define UQ_RC_VBR 					                  1
#define UQ_RC_FIX 				   	                2

//-----------------------------------------------------------------------------
// AUDIO CODEC TYPE
//-----------------------------------------------------------------------------
#define UQ_AUDIO_CODEC_PCM		                0x01
#define UQ_AUDIO_CODEC_ALAW	                  0x06
#define UQ_AUDIO_CODEC_MULAW	                0x07
#define UQ_AUDIO_CODEC_ADPCM	                0x11

//-----------------------------------------------------------------------------
// IRQ ID
//-----------------------------------------------------------------------------
#define UQ_INT_SRQ						                0
#define UQ_INT_VIP_ENC 			                  1
#define UQ_INT_VIP_SS				                  2
#define UQ_INT_VIP_PV				                  3
#define UQ_INT_VIP_CC0 			                  4
#define UQ_INT_VIP_CC1 			                  5
#define UQ_INT_VIP_SV				                  6
#define UQ_INT_IE0_PV				                  7
#define UQ_INT_IE0_SV				                  8
#define UQ_INT_IE0_MS				                  9
#define UQ_INT_IE0_SS				                  10
#define UQ_INT_IE1_PV				                  11
#define UQ_INT_IE1_SV				                  12
#define UQ_INT_IE1_MS				                  13
#define UQ_INT_IE1_SS				                  14
#define UQ_INT_CODEC0				                  15
#define UQ_INT_CODEC1				                  16
#define UQ_INT_MD0						                17
#define UQ_INT_MD1						                18
#define UQ_INT_MT0						                19
#define UQ_INT_MT1						                20
#define UQ_INT_EJPEG					                21
#define UQ_INT_ACD						                22
#define UQ_INT_AIP						                23
#define UQ_INT_ADT						                24
#define UQ_INT_AOP						                25
#define UQ_INT_RTC						                26
#define UQ_INT_SAR						                27
#define UQ_INT_DCAS					                  28
#define UQ_INT_PCI_DMA 			                  29
#define UQ_INT_VLB						                30
#define UQ_INT_MEM_DMA 			                  31
#define UQ_INT_EXT						                32
#define UQ_INT_GPIO					                  33
#define UQ_INT_DEC_ERROR											34

#ifndef TRUE
#define TRUE																	1
#endif 
#ifndef FALSE
#define FALSE																	0
#endif 
//-----------------------------------------------------------------------------
// UQ_TIME_TEG
//-----------------------------------------------------------------------------
typedef struct _UQ_TIME_TEG
{
	WORD		wYear;
	WORD		wMonth;
	WORD		wDayOfWeek;
	WORD		wDay;
	WORD		wHour;
	WORD		wMinute;
	WORD		wSecond;
	WORD		wMilliseconds;
} UQ_TIME_TEG;

//-----------------------------------------------------------------------------
// UQ_VIDEO_PREVIEW_DATA
//-----------------------------------------------------------------------------
typedef struct _UQ_VIDEO_PREVIEW_DATA
{
  UINT8		device_id;
  UINT8		ch_id;
  UINT8		ch_type;
  UINT16	size_x;
  UINT16	size_y;
  UINT16	pitch;
  UINT8		cfmt;
  UINT8		*video_y;
  UINT8		*video_u;
  UINT8		*video_v;
	UQ_TIME_TEG	time_tag;
} UQ_VIDEO_PREVIEW_DATA;


//-----------------------------------------------------------------------------
// UQ_VIDEO_STREAM_DATA
//-----------------------------------------------------------------------------
typedef struct _UQ_VIDEO_STREAM_DATA
{
  UINT8		device_id;
  UINT8		ch_id;
  UINT8		ch_type;
  UINT8		enc;
  UINT8		key_frame;
  UINT8   ref_frame;
  UINT32	size;
  UINT32	*motion;
  UINT8		*vstream;
	UQ_TIME_TEG	time_tag;
} UQ_VIDEO_STREAM_DATA;

//-----------------------------------------------------------------------------
// UQ_AUDIO_STREAM_DATA
//-----------------------------------------------------------------------------
typedef struct  _UQ_AUDIO_STREAM_DATA
{
  UINT8		device_id;
  UINT8		ch_id;
  UINT8		ch_type;
  UINT8		enc;
  UINT16	mode;
  UINT8		rate;
  UINT32	size;
  UINT8		*astream;
	UQ_TIME_TEG	time_tag;
} UQ_AUDIO_STREAM_DATA;


//-----------------------------------------------------------------------------
// UQ_JPEG_STREAM_DATA
//-----------------------------------------------------------------------------
typedef struct _UQ_JPEG_STREAM_DATA
{
  UINT8		device_id;
  UINT8		ch_id;
  UINT8		mjpeg;
  UINT8		qp;
  UINT32	sizex;
  UINT32	sizey;
  UINT32	size;
  UINT8		*jstream;
	UQ_TIME_TEG	time_tag;
} UQ_JPEG_STREAM_DATA;


//-----------------------------------------------------------------------------
// UQ_MOTION_DATA
//-----------------------------------------------------------------------------
typedef struct _UQ_MOTION_STREAM_DATA
{
  UINT8		device_id;
  UINT8		ch_id;
  UINT8		ch_type;
  UINT32	size;
  UINT8		*md_data;
} UQ_MOTION_STREAM_DATA;


//-----------------------------------------------------------------------------
// UQ_INTERRUPT_DATA
//-----------------------------------------------------------------------------
typedef struct _UQ_INTERRUPT_DATA
{
  UINT8   board_id;
  UINT8   device_id;
  UINT8		irq_id;
  UINT32	irq_data[8];
} UQ_INTERRUPT_DATA;


//-----------------------------------------------------------------------------
// UQ_VIDEO_INPUT_STATE
//-----------------------------------------------------------------------------
typedef struct _UQ_VIDEO_INPUT_STATE
{
  UINT8   board_id;
  UINT8   ch_id;
  UINT8		std;
  UINT8		mode;
  UINT8		lock;
  UINT8		fps;
  UINT8		interlace;
  UINT16	sizex;
  UINT16	sizey;
} UQ_VIDEO_INPUT_STATE;

