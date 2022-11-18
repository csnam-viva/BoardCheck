#include "stdafx.h"
#include "intrin.h"
#include "uq3388_motion_lib.h"

uq3388_motion_lib::uq3388_motion_lib()
{
  int cpuinfo[4] ={0};
	__cpuid(cpuinfo, 0x1);

  if(cpuinfo[2] & (1 << 19) || false)
		m_sse4_enable = 1;
	else
		m_sse4_enable = 0;

	memset(m_chp, 0, MOTION_MAX_CH * sizeof(UQ_MOTION_CH)); 					
}

uq3388_motion_lib::~uq3388_motion_lib()
{
}

//-------------------------------------------------------------------------
//	set_video_channel
//-------------------------------------------------------------------------
void uq3388_motion_lib::set_channel(UINT8 chid, UINT16 size_x, UINT16 size_y)
{
	m_chp[chid].size_x 		= size_x;
	m_chp[chid].size_y 		= size_y;
	m_chp[chid].size_mbx	= (size_x + 15) >> 4;
	m_chp[chid].size_mby	=	(size_y + 15) >> 4;
}

//-------------------------------------------------------------------------
//	set_video_channel
//-------------------------------------------------------------------------
int uq3388_motion_lib::popcnt(unsigned int data)
{
	if(m_sse4_enable){
		return _mm_popcnt_u32(data); 										
	}else{
	  data = data - ((data >> 1) & 0x55555555);                      // reuse input as temporary
	  data = (data & 0x33333333) + ((data >> 2) & 0x33333333);       // temp
	  return (((data + (data >> 4) & 0xF0F0F0F) * 0x1010101) >> 24); // count
	}	
}

//-------------------------------------------------------------------------
//	check_m_chp
//-------------------------------------------------------------------------
BOOL uq3388_motion_lib:: check_motion(UQ_MOTION_STREAM_DATA data)
{
	UINT16	i, j, m;
	UINT64 	u64temp0, u64temp1;
	UINT16 	mb_cnt;
	UINT8		chid  = data.ch_id;
	BOOL  	detected = 0;		
	memset(m_chp[chid].motion_cell_low,  0, 68 * sizeof(UINT64)); 
	memset(m_chp[chid].motion_cell_high, 0, 68 * sizeof(UINT64)); 

	if(m_chp[chid].enable)
	{
		for(j=0, m =0; j < m_chp[chid].size_mby; ++j)
		{
			for(i=0; i < m_chp[chid].size_mbx; ++i, ++m)
			{
				if (!(i & 0x40))
				{
					m_chp[chid].motion_cell_low[j]  |= (UINT64) ((data.md_data[8 + (m << 2)] & 0x01)) << i;
				}else { //if (i & 0x40)
					m_chp[chid].motion_cell_high[j] |= (UINT64) ((data.md_data[8 + (m << 2)] & 0x01)) << (i & 0x3F);
				}
			}
		}

		for (j=0; j < MOTION_MAX_REGION; j++)
		{
			if (m_chp[chid].region[j].enable)
			{
				mb_cnt = 0;
				for (i = (UINT16)(m_chp[chid].region[j].mb_rect.top); i <= m_chp[chid].region[j].mb_rect.bottom; i++)
				{
					u64temp0 = m_chp[chid].motion_cell_low[i] & m_chp[chid].region[j].mask_cell_low[i];
					u64temp1 = m_chp[chid].motion_cell_high[i] & m_chp[chid].region[j].mask_cell_high[i];
					mb_cnt += popcnt((UINT32)(u64temp0 & 0xFFFFFFFF)) + popcnt((UINT32)((u64temp0 >> 32) & 0xFFFFFFFF)) + popcnt((UINT32)(u64temp1 & 0xFFFFFFFF)) + popcnt((UINT32)((u64temp1 >> 32) & 0xFFFFFFFF));
				}

#ifdef _DEBUG
				//spatial
				UINT16 region_hsize = (UINT16)((m_chp[chid].region[j].mb_rect.right - m_chp[chid].region[j].mb_rect.left + 1));
				UINT16 region_vsize = (UINT16)((m_chp[chid].region[j].mb_rect.bottom - m_chp[chid].region[j].mb_rect.top + 1));
				UINT16 region_size  = region_hsize * region_vsize;
				TRACE("check_motion ch=%d  region_size=%d  mb_cnt=%d  spatial_th=%d\n", chid, region_size, mb_cnt, m_chp[chid].region[j].spatial_th);
#endif
				if (mb_cnt >= m_chp[chid].region[j].spatial_th)
				{	// motion is detected in the current frame!
					m_chp[chid].region[j].temporal_sum += 1 - ((m_chp[chid].region[j].temporal_sbox >> (m_chp[chid].region[j].temporal_window-1)) & 0x1);
					m_chp[chid].region[j].temporal_sbox = ((m_chp[chid].region[j].temporal_sbox << 1) | 0x1) & m_chp[chid].region[j].temporal_mask;
				}else{
					// motion is NOT detected in the current frame!
					m_chp[chid].region[j].temporal_sum -=  ((m_chp[chid].region[j].temporal_sbox >> (m_chp[chid].region[j].temporal_window-1)) & 0x1);
					m_chp[chid].region[j].temporal_sbox = ((m_chp[chid].region[j].temporal_sbox << 1)) & m_chp[chid].region[j].temporal_mask;
				}

				//temporal
				if (m_chp[chid].region[j].temporal_sum >= m_chp[chid].region[j].temporal_th)
				{
					m_chp[chid].region[j].detected = TRUE;
//					printf("check_motion chid %d region %d motion detected \n",chid, j);
					detected = 1;
				}else{
					m_chp[chid].region[j].detected = FALSE;							
				}
			}else{
				m_chp[chid].region[j].detected = FALSE;
			}
		}
		return detected;
	}else{
		return 0;					
	}
}

//-------------------------------------------------------------------------
//	set_motion_region
//-------------------------------------------------------------------------
BOOL uq3388_motion_lib:: set_region(UINT8 chid, UINT8 region_id, RECT rect, UINT8 spatial_th, UINT8 temporal_th, UINT8 temporal_window)
{
	// mb-wise quantization
	m_chp[chid].region[region_id].rect.left		= rect.left & 0xFFF0;
	m_chp[chid].region[region_id].rect.top		= rect.top  & 0xFFF0;
	m_chp[chid].region[region_id].rect.right		= (rect.right  + 15) & 0xFFF0;       // width bound ceiling
	m_chp[chid].region[region_id].rect.bottom		= (rect.bottom + 15) & 0xFFF0;       // height bound ceiling

	// mb rect
	m_chp[chid].region[region_id].mb_rect.left	= m_chp[chid].region[region_id].rect.left >> 4; 
	m_chp[chid].region[region_id].mb_rect.right	= m_chp[chid].region[region_id].rect.right >> 4; 
	m_chp[chid].region[region_id].mb_rect.top		= m_chp[chid].region[region_id].rect.top >> 4; 
	m_chp[chid].region[region_id].mb_rect.bottom	= m_chp[chid].region[region_id].rect.bottom >> 4; 

	// region size
	UINT16 region_hsize = (UINT16)((m_chp[chid].region[region_id].mb_rect.right - m_chp[chid].region[region_id].mb_rect.left + 1));
	UINT16 region_vsize = (UINT16)((m_chp[chid].region[region_id].mb_rect.bottom - m_chp[chid].region[region_id].mb_rect.top + 1));
	UINT16 region_size  = region_hsize * region_vsize;
	
	// mask generation
	memset(m_chp[chid].region[region_id].mask_cell_low,  0, 68*sizeof(UINT64)); 
	memset(m_chp[chid].region[region_id].mask_cell_high, 0, 68*sizeof(UINT64)); 	

	UINT64  u64temp  = 0;
	UINT64  u64temp1 = 0;
	for (UINT16 n = (UINT16)(m_chp[chid].region[region_id].mb_rect.left); n < m_chp[chid].region[region_id].mb_rect.right; n++)
	{
		if (n < 64)
			u64temp  |= (1ULL << n);
		else
			u64temp1 |= (1ULL << (n - 64));
	}	

	for (UINT16 k = (UINT16)(m_chp[chid].region[region_id].mb_rect.top); k < m_chp[chid].region[region_id].mb_rect.bottom; k++)
	{
		m_chp[chid].region[region_id].mask_cell_low[k]  = u64temp;
		m_chp[chid].region[region_id].mask_cell_high[k] = u64temp1;
	}

	// spatial threshold setting
#if 1
	UINT16 u16temp = (UINT16)((float)region_size * (float)spatial_th/300.0);
	u16temp = (u16temp == 0)? 1 : (u16temp > region_size)? region_size : u16temp;
	m_chp[chid].region[region_id].spatial_th = u16temp;
#else
	UINT16 u16temp = (300 * (float)spatial_th/100.0);
	u16temp = (u16temp == 0)? 1 : (u16temp > region_size)? region_size : u16temp;
	m_chp[chid].region[region_id].spatial_th = u16temp;
//	m_chp[chid].region[region_id].spatial_th = spatial_th * 5;
#endif

	// temporal threshold setting
	UINT8 u8temp = (temporal_window == 0)? 1 : (temporal_window > MOTION_MAX_WINDOWS) ? MOTION_MAX_WINDOWS : temporal_window; 
	m_chp[chid].region[region_id].temporal_window =  u8temp;
	m_chp[chid].region[region_id].temporal_th = (temporal_th > u8temp)? u8temp : (temporal_th == 0)? 1 : temporal_th; 
	m_chp[chid].region[region_id].temporal_sbox = 0;
	m_chp[chid].region[region_id].temporal_sum = 0;
	m_chp[chid].region[region_id].temporal_mask = ((UINT64)0xFFFFFFFFFFFFFFFF) >> (64 - m_chp[chid].region[region_id].temporal_window);

	return TRUE;
}

//-------------------------------------------------------------------------
//	set_region_enable
//-------------------------------------------------------------------------
BOOL uq3388_motion_lib:: set_region_enable(UINT8 chid, UINT8 region_id, UINT8 enable)
{
	m_chp[chid].region[region_id].enable = enable;		
	return TRUE;	 
}
//-------------------------------------------------------------------------
//	set_channel_enable
//-------------------------------------------------------------------------
BOOL uq3388_motion_lib:: set_channel_enable(UINT8 chid, UINT8 enable)
{
	m_chp[chid].enable = enable;		
	return TRUE;	 
}


