#if !defined(AFX_uq3388_osd_lib_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_)
#define AFX_uq3388_osd_lib_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_

#pragma once

///////////////////////////////////////////////////////////////////////////////
// FreeType
///////////////////////////////////////////////////////////////////////////////
#include "../include/Ubiqmicro/FreeType/ft2build.h"
#include FT_FREETYPE_H
#pragma comment(lib,"../library/Ubiqmicro/FreeType/vs2010_x86/freetype.lib")

#define UQ_OSD_ALIGN_LEFT                     0
#define UQ_OSD_ALIGN_CENTER                   1
#define UQ_OSD_ALIGN_RIGHT                    2

class uq3388_osd_lib
{
public:
  uq3388_osd_lib();
  ~uq3388_osd_lib();
	void set_osd_font(char fname[]);
//	void set_osd_font_size(int font_size);
	void set_osd_box(UINT8 ch_id, int st_x, int st_y, int size_x, int size_y);
	void set_osd_string(UINT8 ch_id, wchar_t *string, UINT8 align,int font_size);
	void set_osd_string_color(UINT8 ch_id, UINT32 color);
	void set_osd_box_enable(UINT8 ch_id, UINT8 enable);

	FT_Library m_ft_lib;
	FT_Face	m_face;	
	int m_box_size_x[64];
	int m_box_size_y[64];
	int m_box_st_x[64];
	int m_box_st_y[64];		
	int m_box_enable[64];		
};

#endif // !defined(AFX_uq3388_osd_lib_H__CBA71F38_7D03_4F80_8146_1019DC6F48EF__INCLUDED_)
