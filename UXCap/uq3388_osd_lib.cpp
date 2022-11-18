#include "stdafx.h"
#include "uq3388_osd_lib.h"

//-------------------------------------------------------------------------
//	uq3388_osd_lib
//-------------------------------------------------------------------------
uq3388_osd_lib::uq3388_osd_lib()
{
	m_face = NULL;
	for(int i=0;i<64;i++){
		m_box_size_x[i] = 0;
		m_box_size_y[i] = 0;
		m_box_st_x[i] = 0;
		m_box_st_y[i] = 0;
		m_box_enable[i] = 0;
	}
	FT_Init_FreeType(&m_ft_lib);
}

uq3388_osd_lib::~uq3388_osd_lib()
{
	if(m_face) FT_Done_Face(m_face);
	FT_Done_FreeType(m_ft_lib);
}

//-------------------------------------------------------------------------
//	set_osd_font
//-------------------------------------------------------------------------
void uq3388_osd_lib::set_osd_font(char fname[])
{
	if(m_face) FT_Done_Face(m_face);
	FT_New_Face(m_ft_lib, fname, 0, &m_face);
}

//-------------------------------------------------------------------------
//	set_osd_box
//-------------------------------------------------------------------------
void uq3388_osd_lib::set_osd_box(UINT8 ch_id, int st_x, int st_y, int size_x, int size_y)
{
	m_box_size_x[ch_id] = size_x;
	m_box_size_y[ch_id] = size_y;
	m_box_st_x[ch_id]   = st_x;
	m_box_st_y[ch_id]   = st_y;

	uq_set_video_osd_box(ch_id, 0x00, m_box_st_x[ch_id], m_box_st_y[ch_id], m_box_size_x[ch_id], m_box_size_y[ch_id]);
	uq_set_video_osd_alpha(ch_id, 0x00, 0);
	uq_set_video_osd_fill_rect(ch_id, 0x00, 0, 0, m_box_size_x[ch_id], m_box_size_y[ch_id], 255);
}

//-------------------------------------------------------------------------
//	set_osd_string
//-------------------------------------------------------------------------
void uq3388_osd_lib::set_osd_string_color(UINT8 ch_id, UINT32 color)
{
	RGBQUAD	hRGB[256];
	for (int i = 0; i < 256; i++) {
		hRGB[i].rgbBlue  = (color >> 16) & 0xFF;
		hRGB[i].rgbGreen = (color >>  8) & 0xFF;
		hRGB[i].rgbRed	 = (color >>  0) & 0xFF;
	}
	uq_set_video_osd_palette(ch_id, (UINT32 *)hRGB);
	uq_set_video_osd_ckey(ch_id, 255, 1);
}

//-------------------------------------------------------------------------
//	set_osd_string
//-------------------------------------------------------------------------
void uq3388_osd_lib::set_osd_string(UINT8 ch_id, wchar_t *string, UINT8 align, int font_size)
{
	if(!m_face) return;

	int length;
	int	dx,dy, sizex, sizey;
	int space_x;
	int adv_sizex, adv_sizey;
	int buf_pos;
	int buf_max;

	if(!m_box_enable[ch_id])
		return;

	length = (int)wcslen(string);
	UINT8 *osd_buf0 = new BYTE [m_box_size_x[ch_id] * m_box_size_y[ch_id]];
	UINT8 *osd_buf1 = new BYTE [m_box_size_x[ch_id] * m_box_size_y[ch_id]];
	memset(osd_buf0, 0xFF, m_box_size_x[ch_id] * m_box_size_y[ch_id]);
	memset(osd_buf1, 0xFF, m_box_size_x[ch_id] * m_box_size_y[ch_id]);
	buf_max = m_box_size_x[ch_id] * m_box_size_y[ch_id];
	dx = dy = 0;
	FT_Set_Char_Size(m_face, font_size * 64, 0, 0, 0);
	FT_Load_Char(m_face, '1', FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);
	space_x = (m_face->glyph->advance.x >> 6);
	dx = dy = 0;
	for (int cnt = 0; cnt < length; cnt++) {
		if(string[cnt] == ' '){
			dx += space_x;
		}	else if(string[cnt] == '\t'){
			dx += space_x * 4;
		}	else if(string[cnt] == '\n'){
			dx = 0;
			dy += font_size;
		}else{
			FT_Load_Char(m_face, string[cnt], FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);
			sizex = m_face->glyph->bitmap.width;
			sizey = m_face->glyph->bitmap.rows;
			adv_sizex = (m_face->glyph->advance.x >> 6);
			adv_sizey = font_size - m_face->glyph->bitmap_top;


			for (int i = 0; i < sizey; i++) {
				for (int j = 0; j < sizex; j++){
					if(m_face->glyph->bitmap.buffer[i * sizex + j] == 0){
						//osd_image[((adv_sizey + dy) * m_box_sizex + dx) + (i * m_box_sizex + j)] = 255;
					}else{
						buf_pos = ((dy + adv_sizey) * m_box_size_x[ch_id] + dx + m_face->glyph->bitmap_left) + (i * m_box_size_x[ch_id] + j);
						if(buf_pos < buf_max)
							osd_buf0[buf_pos] = 0;//255 - app_osd_face->glyph->bitmap.buffer[i * sizex + j];
					}
				}
			}
			if(m_box_size_x[ch_id] < (dx + adv_sizex)){
				dx = 0;
				dy += font_size + 2;
			}
			dx = dx + adv_sizex;
		}
		int st_x;
		if(align == 1)	st_x = ((m_box_size_x[ch_id] - dx) / 2) & 0xFFFC;
		else if(align == 2)	st_x = (m_box_size_x[ch_id] - dx) & 0xFFFC;
		else st_x = 0;

		for (int k = 0; k < m_box_size_y[ch_id]; k++){
			memcpy(&osd_buf1[(k*m_box_size_x[ch_id])+st_x],&osd_buf0[(k*m_box_size_x[ch_id])], dx);
		}
	}
	uq_set_video_osd_fill_rect_with_image(ch_id, 0x00, 0, 0, m_box_size_x[ch_id], m_box_size_y[ch_id], osd_buf1);
	delete [] osd_buf0;
	delete [] osd_buf1;
}

//-------------------------------------------------------------------------
//	set_osd_box_enable
//-------------------------------------------------------------------------
void uq3388_osd_lib::set_osd_box_enable(UINT8 ch_id, UINT8 enable)
{
	uq_set_video_osd_box_enable(ch_id, 0x00, enable);
	m_box_enable[ch_id] = enable;
}


