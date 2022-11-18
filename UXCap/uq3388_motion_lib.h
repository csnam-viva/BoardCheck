#pragma once
#define 	MOTION_MAX_CH								64
#define 	MOTION_MAX_REGION						4
#define 	MOTION_MAX_WINDOWS					32

typedef struct _UQ_MOTION_REGION
{
	BOOL		detected;				
  BOOL 		enable;
  RECT 		rect;	
  RECT 		mb_rect;	
  UINT16  spatial_th;  // spatial threshold
  UINT64 	mask_cell_low[68];
  UINT64 	mask_cell_high[68];
  UINT8 	temporal_th;	
  UINT64 	temporal_sbox;
  UINT8 	temporal_sum;
  UINT8 	temporal_window;
  UINT64 	temporal_mask;
} UQ_MOTION_REGION;

typedef struct _UQ_MOTION_CH
{
  UINT16  enable;				
  UINT16  size_x;
  UINT16  size_y;
  UINT16  size_mbx;
  UINT16  size_mby;
	UINT64  motion_cell_low[68];
	UINT64  motion_cell_high[68];	
	UQ_MOTION_REGION	region[MOTION_MAX_REGION];
} UQ_MOTION_CH;

class uq3388_motion_lib
{
public:
  uq3388_motion_lib();
  ~uq3388_motion_lib();

	BOOL 	check_motion(UQ_MOTION_STREAM_DATA data);
	void 	set_channel(UINT8 chid, UINT16 size_x, UINT16 size_y);	
	BOOL 	set_region(UINT8 chid, UINT8 region_id, RECT rect, UINT8 spatial_th, UINT8 temporal_th, UINT8 temporal_window);
	BOOL 	set_region_enable(UINT8 chid, UINT8 region_id, UINT8 enable);
	BOOL 	set_channel_enable(UINT8 chid, UINT8 enable);
	UQ_MOTION_CH m_chp[MOTION_MAX_CH];
private:
	int 	popcnt(unsigned int data);	
	UINT8 m_sse4_enable;		
};

