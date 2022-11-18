#pragma once

#ifdef WINDOWS
#else //LINUX
	#include <time.h>
#endif

#ifdef WINDOWS
	#define UQ3388_EXTC  extern "C" __declspec(dllexport)
#else //LINUX
	#define UQ3388_EXTC  extern "C"
#endif

//SYSTEM
UQ3388_EXTC BOOL		uq_get_instance();
UQ3388_EXTC void		uq_destroy_instance();
UQ3388_EXTC BOOL		uq_check_instance();
UQ3388_EXTC BOOL		uq_get_instance_condig(UINT8 board_id);

//SYSTEM
UQ3388_EXTC	BOOL		uq_init()																																																						;
UQ3388_EXTC	BOOL		uq_uninit()																																																					;
UQ3388_EXTC	BOOL		uq_setup()																																																					;
UQ3388_EXTC	BOOL		uq_unsetup()																																																				;
UQ3388_EXTC	BOOL		uq_run()																																																						;
UQ3388_EXTC	BOOL		uq_stop()																																																						;
//INFO
UQ3388_EXTC	UINT8		uq_get_board_count()																																																;
UQ3388_EXTC	UINT32	uq_get_device_version()																																															;

UQ3388_EXTC	UINT32	uq_get_api_version()																																																;
UQ3388_EXTC	UINT32	uq_get_board_type(UINT8 board_id)																																										;
UQ3388_EXTC	UINT8		uq_get_device_state()																																																;
UQ3388_EXTC	BOOL		uq_get_rtc_time(tm *time)																																														;
UQ3388_EXTC	UINT8		uq_get_max_video_ch()																																																;
UQ3388_EXTC	UINT8		uq_get_max_audio_ch()																																																;
UQ3388_EXTC	UINT8 	uq_get_vch_to_board_type(UINT8 chid)																																								;

//VIDEO GROP
UQ3388_EXTC	BOOL		uq_set_color_format(UINT8 color_format)																																							;
UQ3388_EXTC	UINT8		uq_get_color_format()																																																;
UQ3388_EXTC	BOOL		uq_set_video_privacy_color(UINT8 Y, UINT8  Cb, UINT8  Cr)																														;
UQ3388_EXTC	BOOL		uq_get_video_privacy_color(UINT8& Y, UINT8&  Cb, UINT8&  Cr)																												;

//VIDEO CHANNEL GROP
UQ3388_EXTC	BOOL		uq_set_video_flip(UINT8 ch_id, BOOL enable)																																					;
UQ3388_EXTC	BOOL		uq_get_video_flip(UINT8 ch_id, UINT8& enable)																																				;
UQ3388_EXTC	BOOL		uq_set_video_mirror(UINT8 ch_id, BOOL enable)																																				;
UQ3388_EXTC	BOOL		uq_get_video_mirror(UINT8 ch_id, UINT8& enable)																																			;
UQ3388_EXTC	BOOL		uq_set_video_privacy_param(UINT8 ch_id, UINT8 box_id, UINT16  left, UINT16  top, UINT16  right, UINT16  bottom)			;
UQ3388_EXTC	BOOL		uq_get_video_privacy_param(UINT8 ch_id, UINT8 box_id, UINT16& left, UINT16& top, UINT16& right, UINT16& bottom)			;
UQ3388_EXTC	BOOL		uq_set_video_privacy_enable(UINT8 ch_id, UINT8 box_id, UINT8 enable)																								;
UQ3388_EXTC	BOOL		uq_get_video_privacy_enable(UINT8 ch_id, UINT8 box_id,  UINT8& enable)																							;
UQ3388_EXTC	BOOL		uq_set_video_adjust(UINT8 ch_id, UINT8 brightness, UINT8 contrast, UINT8 saturation_u, UINT8  saturation_v)					;
UQ3388_EXTC	BOOL		uq_get_video_adjust(UINT8 ch_id, UINT8& brightness, UINT8& contrast, UINT8& saturation_u, UINT8&  saturation_v)			;
UQ3388_EXTC	BOOL		uq_set_video_deinterlace(UINT8 ch_id,  UINT8 deinterlace)																														;
UQ3388_EXTC	BOOL		uq_get_video_deinterlace(UINT8 ch_id,  UINT8& deinterlace)																													;
UQ3388_EXTC	BOOL		uq_set_video_2dnr(UINT8 ch_id, UINT8 enable)																																				;
UQ3388_EXTC	BOOL		uq_get_video_2dnr(UINT8 ch_id, UINT8& enable)																																				;
UQ3388_EXTC	BOOL		uq_set_video_2dnr_threshold(UINT8 ch_id, UINT8  threshold, UINT8 lowpass_enable)																		;
UQ3388_EXTC	BOOL		uq_get_video_2dnr_threshold(UINT8 ch_id, UINT8&  threshold, UINT8& lowpass_enable)																	;
UQ3388_EXTC	BOOL		uq_set_video_3dnr(UINT8 ch_id, UINT8 enable)																																				;
UQ3388_EXTC	BOOL		uq_get_video_3dnr(UINT8 ch_id, UINT8& enable)																																				;
UQ3388_EXTC	BOOL		uq_set_video_sharpness(UINT8 ch_id, UINT8 enable)																																		;
UQ3388_EXTC	BOOL		uq_get_video_sharpness(UINT8 ch_id, UINT8& enable)																																	;
UQ3388_EXTC	BOOL		uq_set_video_sharpness_alpha(UINT8 ch_id, UINT8 alpha)																															;
UQ3388_EXTC	BOOL		uq_get_video_sharpness_alpha(UINT8 ch_id, UINT8& alpha)																															;
UQ3388_EXTC	BOOL		uq_get_video_input_state(UINT8 ch_id, UQ_VIDEO_INPUT_STATE& state)																									;
UQ3388_EXTC	BOOL		uq_set_video_osd_box(UINT8 ch_id, UINT8 box_id, UINT16 st_x, UINT16 st_y, UINT16 size_x, UINT16 size_y)							;
UQ3388_EXTC	BOOL		uq_set_video_osd_palette(UINT8 ch_id, UINT32 *palette)																															;
UQ3388_EXTC	BOOL		uq_set_video_osd_ckey(UINT8 ch_id, UINT8 color, UINT8 enable)	  																										;
UQ3388_EXTC	BOOL		uq_set_video_osd_alpha(UINT8 ch_id, UINT8 box_id, UINT8 alpha)																											;
UQ3388_EXTC	BOOL		uq_set_video_osd_fill_rect(UINT8 ch_id, UINT8 box_id, UINT16 st_x, UINT16 st_y, UINT16 sizex, UINT16 sizey, UINT8 color);
UQ3388_EXTC	BOOL		uq_set_video_osd_fill_rect_with_image(UINT8 ch_id, UINT8 box_id, UINT16 st_x, UINT16 st_y, UINT16 sizex, UINT16 sizey, UINT8 *data);
UQ3388_EXTC	BOOL		uq_set_video_osd_box_enable(UINT8 ch_id, UINT8 box_id, UINT8 enable)																								;

//VIDEO SUB CHANNEL GROP
UQ3388_EXTC	BOOL		uq_set_video_enable(UINT8 ch_id, UINT8 ch_type, UINT8 enable)																												;
UQ3388_EXTC	BOOL		uq_get_video_enable(UINT8 ch_id, UINT8 ch_type, BOOL &enable)																												;
UQ3388_EXTC	BOOL		uq_set_video_resolution(UINT8 ch_id, UINT8 ch_type, UINT16 width,  UINT16 height)																		;
UQ3388_EXTC	BOOL		uq_get_video_resolution(UINT8 ch_id, UINT8 ch_type, UINT16& width, UINT16& height)																	;
UQ3388_EXTC	BOOL		uq_set_video_framerate(UINT8 ch_id, UINT8 ch_type, UINT8 fps)																												;
UQ3388_EXTC	BOOL		uq_get_video_framerate(UINT8 ch_id, UINT8 ch_type, UINT8& fps)																											;
UQ3388_EXTC	BOOL		uq_set_video_border(UINT8 ch_id, UINT8 ch_type, UINT16 left, UINT16 top, UINT16 right, UINT16 bottom, UINT8 enable)	;
UQ3388_EXTC	BOOL		uq_set_video_border_param(UINT8 ch_id, UINT8 ch_type, UINT8 sizex, UINT8 sizey, UINT8 luma, UINT8 cb, UINT8 cr)			;
UQ3388_EXTC	BOOL		uq_set_video_cropx(UINT8 ch_id, UINT8 ch_type, UINT16 crop_posx, UINT16 crop_sizex )																;
UQ3388_EXTC	BOOL		uq_get_video_cropx(UINT8 ch_id, UINT8 ch_type, UINT16& crop_posx, UINT16& crop_sizex )															;

//VIDEO_OUTPUT CVBS
UQ3388_EXTC	BOOL		uq_set_cvbs_mode(UINT8 board_id, UINT8 std, UINT8 nhd, UINT8 nvd)																										;
UQ3388_EXTC	BOOL		uq_get_cvbs_mode(UINT8 board_id, UINT8& std, UINT8& nhd, UINT8& nvd)																								;
UQ3388_EXTC	BOOL		uq_set_cvbs_enable(UINT8 board_id, UINT8 enable)																																		;
UQ3388_EXTC	BOOL		uq_get_cvbs_enable(UINT8 board_id, UINT8& enable)																																		;
UQ3388_EXTC	BOOL		uq_set_cvbs_ch_enable(UINT8 ch_id, UINT8 enable)																																		;
UQ3388_EXTC	BOOL		uq_get_cvbs_ch_enable(UINT8 ch_id, UINT8& enable)																																		;
UQ3388_EXTC	BOOL		uq_set_cvbs_ch_enable_ext(UINT8 src_chid, UINT8 dst_chid, UINT8 enable)																							;
UQ3388_EXTC	BOOL		uq_set_cvbs_size(UINT8 ch_id, UINT32 sizex, UINT32 sizey)																														;
UQ3388_EXTC	BOOL		uq_get_cvbs_size(UINT8 chid,  UINT32& sizex,  UINT32& sizey)																												;

//VIDEO_OUTPUT CVBS OSD
UQ3388_EXTC	BOOL		uq_set_cvbs_osd_palette(UINT8 board_id, UINT32 *palette)																														;
UQ3388_EXTC	BOOL		uq_set_cvbs_osd_ckey(UINT8 board_id, UINT32 ckey, UINT8 enable)																											;
UQ3388_EXTC	BOOL		uq_set_cvbs_osd_alpha(UINT8 board_id, UINT8 alpha, UINT8 enable)																										;
UQ3388_EXTC	BOOL		uq_set_cvbs_draw_point(UINT8 board_id, UINT16 x, UINT16 y, UINT8 color)																							;
UQ3388_EXTC	BOOL		uq_set_cvbs_fill_rect(UINT8 board_id, UINT16 x, UINT16 y, UINT16 width, UINT16 height, UINT8 color)									;
UQ3388_EXTC	BOOL		uq_set_cvbs_fill_rect_with_image(UINT8 board_id, UINT16 x, UINT16 y, UINT16 width, UINT16 height, UINT8 *data)			;
UQ3388_EXTC	BOOL		uq_set_cvbs_osd_enable(UINT8 board_id, UINT8 enable)																																;

//VIDEO_OUTPUT HDMI
UQ3388_EXTC	BOOL		uq_set_hdmi_mode(UINT8 board_id, UINT8 std, UINT8 nhd, UINT8 nvd)																										;
UQ3388_EXTC	BOOL		uq_get_hdmi_mode(UINT8 board_id, UINT8& std,  UINT8& nhd, UINT8& nvd)																								;
UQ3388_EXTC	BOOL		uq_set_hdmi_enable(UINT8 board_id, UINT8 enable)																																		;
UQ3388_EXTC	BOOL		uq_get_hdmi_enable(UINT8 board_id, UINT8& enable)																																		;
UQ3388_EXTC	BOOL		uq_set_hdmi_ch_enable(UINT8 ch_id, UINT8 enable)																																		;
UQ3388_EXTC	BOOL		uq_get_hdmi_ch_enable(UINT8 ch_id, UINT8& enable)																																		;
UQ3388_EXTC	BOOL		uq_set_hdmi_ch_enable_ext(UINT8 src_chid, UINT8 dst_chid, UINT8 enable)																							;
UQ3388_EXTC	BOOL		uq_set_hdmi_size(UINT8 ch_id, UINT32 sizex, UINT32 sizey)																														;
UQ3388_EXTC	BOOL		uq_get_hdmi_size(UINT8 chid,  UINT32& sizex,  UINT32& sizey)																												;

//VIDEO_OUTPUT HDMI OSD
UQ3388_EXTC	BOOL		uq_set_hdmi_osd_palette(UINT8 board_id, UINT32 *palette)																														;
UQ3388_EXTC	BOOL		uq_set_hdmi_osd_ckey(UINT8 board_id, UINT32 ckey, UINT8 enable)																											;
UQ3388_EXTC	BOOL		uq_set_hdmi_osd_alpha(UINT8 board_id, UINT8 alpha, UINT8 enable)																										;
UQ3388_EXTC	BOOL		uq_set_hdmi_draw_point(UINT8 board_id, UINT16 x, UINT16 y, UINT8 color)																							;
UQ3388_EXTC	BOOL		uq_set_hdmi_fill_rect(UINT8 board_id, UINT16 x, UINT16 y, UINT16 width, UINT16 height, UINT8 color)									;
UQ3388_EXTC	BOOL		uq_set_hdmi_fill_rect_with_image(UINT8 board_id, UINT16 x, UINT16 y, UINT16 width, UINT16 height, UINT8 *data)			;
UQ3388_EXTC	BOOL		uq_set_hdmi_osd_enable(UINT8 board_id, UINT8 enable)																																;

//VIDEO OUTPUT NEW
UQ3388_EXTC	BOOL		uq_set_video_out_cvbs_mode(UINT8 board_id, UINT8 std)																																;
UQ3388_EXTC	BOOL		uq_get_video_out_cvbs_mode(UINT8 board_id, UINT8& std)																															;
UQ3388_EXTC	BOOL		uq_set_video_out_cvbs_enable(UINT8 board_id, UINT8 enable)																													;
UQ3388_EXTC	BOOL		uq_get_video_out_cvbs_enable(UINT8 board_id, UINT8& enable)																													;
UQ3388_EXTC	BOOL		uq_set_video_out_hdmi_mode(UINT8 board_id, UINT8 std, UINT8 nhd, UINT8 nvd)																					;
UQ3388_EXTC	BOOL		uq_get_video_out_hdmi_mode(UINT8 board_id, UINT8& std,  UINT8& nhd, UINT8& nvd)																			;
UQ3388_EXTC	BOOL		uq_set_video_out_hdmi_enable(UINT8 board_id, UINT8 enable)																													;
UQ3388_EXTC	BOOL		uq_get_video_out_hdmi_enable(UINT8 board_id, UINT8& enable)																													;
UQ3388_EXTC	BOOL		uq_set_video_out_ch_enable(UINT8 src_chid, UINT8 enable)																														;
UQ3388_EXTC	BOOL		uq_set_video_out_ch_enable_ext(UINT8 src_chid, UINT8 dst_chid, UINT8 enable)																				;
UQ3388_EXTC	BOOL		uq_get_video_out_ch_enable(UINT8 src_chid, UINT8& enable)																														;

//CODEC DECODING
UQ3388_EXTC	BOOL		uq_set_video_decode(UINT8 ch_id, UINT16 width,  UINT16 height)																											;
UQ3388_EXTC	BOOL		uq_get_video_decode(UINT8 ch_id, UINT16 &width,  UINT16 &height)																										;
UQ3388_EXTC	BOOL		uq_set_video_decode_data(UINT8 ch_id, UINT8 *buf, UINT32 buf_size)																									;
UQ3388_EXTC	BOOL		uq_set_video_decode_sync(UINT8 ch_id, float speed)																																	;
UQ3388_EXTC	BOOL		uq_get_video_decode_sync(UINT8 ch_id, float& speed)																																	;
UQ3388_EXTC	BOOL		uq_get_video_decode_buf_size(UINT8 ch_id)																																						;
UQ3388_EXTC	BOOL		uq_get_video_codec_cmd_size(UINT8 ch_id)																																						;

//CODEC ENCODING
UQ3388_EXTC	BOOL		uq_set_video_gop(UINT8 ch_id, UINT8 ch_type, UINT8 gop)																															;
UQ3388_EXTC	BOOL		uq_get_video_gop(UINT8 ch_id, UINT8 ch_type,  UINT8& gop)																														;	
UQ3388_EXTC	BOOL		uq_set_video_gop_ex(UINT8 ch_id, UINT8 ch_type, UINT8 gop)																													;	
UQ3388_EXTC	BOOL		uq_set_video_skip(UINT8 ch_id, UINT8 ch_type, UINT8 skip)																														;	
UQ3388_EXTC	BOOL		uq_get_video_skip(UINT8 ch_id, UINT8 ch_type, UINT8& skip)																													;	
UQ3388_EXTC	BOOL		uq_set_video_user_data(UINT8 ch_id, UINT8 ch_type, UINT32 data0, UINT32 data1)																			;	
UQ3388_EXTC	BOOL		uq_get_video_user_data(UINT8 ch_id, UINT8 ch_type, UINT32& data0, UINT32& data1)																		;	
UQ3388_EXTC	BOOL		uq_set_video_bit_rate_mode(UINT8 ch_id, UINT8 ch_type, UINT8 bit_rate_mode )																				;	
UQ3388_EXTC	BOOL		uq_get_video_bit_rate_mode(UINT8 ch_id, UINT8 ch_type,  UINT8& bit_rate_mode)																				;	
UQ3388_EXTC	BOOL		uq_set_video_bit_rate_qp(UINT8 ch_id, UINT8 ch_type, UINT8 iqp, UINT8 pqp)																					;	
UQ3388_EXTC	BOOL		uq_get_video_bit_rate_qp(UINT8 ch_id, UINT8 ch_type,  UINT8& iqp,  UINT8& pqp)																			;	
UQ3388_EXTC	BOOL		uq_set_video_bit_rate_bps(UINT8 ch_id, UINT8 ch_type, UINT32 high_bps , UINT32 low_bps)															;	
UQ3388_EXTC	BOOL		uq_get_video_bit_rate_bps(UINT8 ch_id, UINT8 ch_type, UINT32& high_bps,  UINT32& low_bps)														;	

//CODEC MOTION
UQ3388_EXTC	BOOL		uq_set_video_motion_enable(UINT8 ch_id, UINT8 ch_type, UINT8 enable)																								;	
UQ3388_EXTC	BOOL		uq_get_video_motion_enable(UINT8 ch_id, UINT8 ch_type, UINT8& enable)																								;	
UQ3388_EXTC	BOOL		uq_set_video_motion_mode(UINT8 ch_id, UINT8 ch_type, UINT8 mode)																										;	
UQ3388_EXTC	BOOL		uq_get_video_motion_mode(UINT8 ch_id, UINT8 ch_type,  UINT8& mode)																									;	
UQ3388_EXTC	BOOL		uq_set_video_motion_threshold(UINT8 ch_id, UINT8 ch_type, UINT16 threshold)																					;	
UQ3388_EXTC	BOOL		uq_get_video_motion_threshold(UINT8 ch_id, UINT8 ch_type, UINT16& threshold)																				;	

//JPEG ENC
UQ3388_EXTC	BOOL		uq_set_jpeg_qp(UINT8 ch_id, UINT8 qp)																																								;	
UQ3388_EXTC	BOOL		uq_get_jpeg_qp(UINT8 ch_id, UINT8& qp)																																							;	
UQ3388_EXTC	BOOL		uq_capture_ejpeg_image(UINT8 ch_id)																																									;	

//AUDIO GROP
UQ3388_EXTC	BOOL		uq_set_audio_encode_sample_rate(UINT32 sample_rate)																																	;	
UQ3388_EXTC	BOOL		uq_get_audio_encode_sample_rate(UINT32& sample_rate)																																;	
UQ3388_EXTC	BOOL		uq_set_audio_encode_sample_num(UINT32 sample_num)																																		;	
UQ3388_EXTC	BOOL		uq_get_audio_encode_sample_num(UINT32& sample_num)																																	;	
UQ3388_EXTC	BOOL		uq_set_audio_detection(UINT8 dec_mode, UINT8 dec_avg_mode, UINT8 dec_th)																						;	
UQ3388_EXTC	BOOL		uq_get_audio_detection(UINT8& dec_mode, UINT8& dec_avg_mode, UINT8& dec_th)																					;	

//AUDIO CHANNEL GROP
UQ3388_EXTC	BOOL		uq_set_audio_out(UINT8 ch_id, UINT8 ch_type,		UINT8 enable)																												;	
UQ3388_EXTC	BOOL		uq_get_audio_out(UINT8 ch_id, UINT8 ch_type,  UINT8& enable)																												;	
UQ3388_EXTC	BOOL		uq_set_audio_interrupt(UINT8 ch_id, UINT8 enable)																																		;	
UQ3388_EXTC	BOOL		uq_get_audio_interrupt(UINT8 ch_id, UINT8& enable)																																	;	
UQ3388_EXTC	BOOL		uq_set_audio_mute(UINT8 ch_id, UINT8 ch_type, UINT8 enable)																													;	
UQ3388_EXTC	BOOL		uq_get_audio_mute(UINT8 ch_id, UINT8 ch_type,  UINT8& enable)																												;	
UQ3388_EXTC	BOOL		uq_set_audio_codec(UINT8 ch_id, UINT8 ch_type, UINT8 audio_codec,  UINT8 mode_8bit)																	;	
UQ3388_EXTC	BOOL		uq_get_audio_codec(UINT8 ch_id, UINT8 ch_type,  UINT8& audio_codec,  UINT8& mode_8bit)															;	
UQ3388_EXTC	BOOL		uq_set_audio_enable(UINT8 ch_id, UINT8 ch_type, UINT8 enable)																												;	
UQ3388_EXTC	BOOL		uq_get_audio_enable(UINT8 ch_id, UINT8 ch_type,  UINT8& enable)																											;	
UQ3388_EXTC	BOOL		uq_set_audio_decode_data(UINT8 ch_id, UINT8 *buf, UINT32 buf_size)																									;	
UQ3388_EXTC	BOOL		uq_set_audio_decode_volume(UINT8 ch_id, UINT8 volume)																																;	
UQ3388_EXTC	BOOL		uq_get_audio_decode_volume(UINT8 ch_id,  UINT8& volume)																															;	
UQ3388_EXTC	BOOL		uq_set_audio_encode_volume(UINT8 ch_id, UINT8 volume)																																;	
UQ3388_EXTC	BOOL		uq_get_audio_encode_volume(UINT8 ch_id, UINT8& volume)																															;	

//callback
UQ3388_EXTC	void		uq_set_video_preview_callback(void(*fp)(UQ_VIDEO_PREVIEW_DATA& data))																								;
UQ3388_EXTC	void		uq_set_video_stream_callback(void(*fp)(UQ_VIDEO_STREAM_DATA& data))																									;	
UQ3388_EXTC	void		uq_set_audio_stream_callback(void(*fp)(UQ_AUDIO_STREAM_DATA& data))																									;
UQ3388_EXTC	void		uq_set_jpeg_stream_callback(void(*fp)(UQ_JPEG_STREAM_DATA& data))																										;
UQ3388_EXTC	void		uq_set_motion_data_callback(void(*fp)(UQ_MOTION_STREAM_DATA& data))																									;
UQ3388_EXTC	void		uq_set_irq_callback(void(*fp)(UQ_INTERRUPT_DATA& data))																															;
UQ3388_EXTC	void		uq_set_video_input_callback(void(*fp)(UQ_VIDEO_INPUT_STATE& state))																									;

//dido 
UQ3388_EXTC	BOOL		uq_init_dido(UINT8 board_id)																																												;
UQ3388_EXTC	BOOL		uq_detect_dido_board(UINT8 board_id,  UINT8& board_detect)																													;
UQ3388_EXTC	BOOL		uq_get_dido_input(UINT8 board_id, UINT8 id, UINT8& data)																														;
UQ3388_EXTC	BOOL		uq_set_dido_output(UINT8 board_id, UINT8 id, UINT8 wdata)																														;
UQ3388_EXTC	BOOL		uq_set_dido_buzzer(UINT8 board_id,  UINT8 wdata)																																		;

//EEPROM
UQ3388_EXTC BOOL		uq_get_vender_id(UINT8 board_id, UINT32& vender_id)																																	;
UQ3388_EXTC UINT32	uq_get_production_date(UINT8 board_id)																																							;
UQ3388_EXTC	BOOL		uq_write_rom(UINT8 board_id, UINT8 address, UINT8 data)																															;
UQ3388_EXTC BOOL		uq_read_rom(UINT8 board_id, UINT8 address, UINT8& data)																															;

// Peripheral Function
UQ3388_EXTC	BOOL		uq_set_rtc_interrupt(UINT8 rtc_int_type, UINT8 enable)																															;	
UQ3388_EXTC	BOOL		uq_get_rtc_interrupt(UINT8 rtc_int_type, UINT8 &enable)																															;	
UQ3388_EXTC	BOOL		uq_set_utc_data(UINT8 ch_id, UINT8 std, UINT8 mode, UINT8 data[8])																									;	
UQ3388_EXTC	BOOL		uq_set_watchdog_enable(UINT8 board_id, UINT8 enable)																																;	
UQ3388_EXTC	BOOL		uq_set_watchdog_trigger(UINT8 board_id)																																							;	
UQ3388_EXTC	BOOL		uq_set_watchdog_time(UINT8 board_id, UINT16 time)																																		;	
UQ3388_EXTC	UINT32	uq_get_watchdog_version(UINT8 board_id)																																							;	
UQ3388_EXTC	BOOL		uq_set_sw_irq(UINT8 board_id, UINT32 data)																																					;

//util
UQ3388_EXTC BOOL 		uq_get_video_motion_detected(UINT32 *stream);
