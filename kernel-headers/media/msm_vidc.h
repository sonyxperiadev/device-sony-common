/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __MSM_VIDC_H__
#define __MSM_VIDC_H__
#include <linux/types.h>
struct msm_vidc_extradata_header {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int size;
  unsigned int : 32;
  unsigned int : 32;
  unsigned int type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int data_size;
  unsigned char data[1];
};
struct msm_vidc_interlace_payload {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int format;
};
struct msm_vidc_framerate_payload {
  unsigned int frame_rate;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vidc_ts_payload {
  unsigned int timestamp_lo;
  unsigned int timestamp_hi;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vidc_concealmb_payload {
  unsigned int num_mbs;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vidc_recoverysei_payload {
  unsigned int flags;
};
struct msm_vidc_aspect_ratio_payload {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int size;
  unsigned int version;
  unsigned int port_index;
  unsigned int aspect_width;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int aspect_height;
};
struct msm_vidc_mpeg2_seqdisp_payload {
  unsigned int video_format;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int color_descp;
  unsigned int color_primaries;
  unsigned int transfer_char;
  unsigned int matrix_coeffs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int disp_width;
  unsigned int disp_height;
};
struct msm_vidc_input_crop_payload {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int size;
  unsigned int version;
  unsigned int port_index;
  unsigned int left;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int top;
  unsigned int width;
  unsigned int height;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vidc_digital_zoom_payload {
  unsigned int size;
  unsigned int version;
  unsigned int port_index;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int zoom_width;
  unsigned int zoom_height;
};
struct msm_vidc_extradata_index {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int type;
  union {
    struct msm_vidc_input_crop_payload input_crop;
    struct msm_vidc_digital_zoom_payload digital_zoom;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
    struct msm_vidc_aspect_ratio_payload aspect_ratio;
  };
};
struct msm_vidc_panscan_window {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int panscan_height_offset;
  unsigned int panscan_width_offset;
  unsigned int panscan_window_width;
  unsigned int panscan_window_height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vidc_panscan_window_payload {
  unsigned int num_panscan_windows;
  struct msm_vidc_panscan_window wnd[1];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vidc_stream_userdata_payload {
  unsigned int type;
  unsigned int data[1];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vidc_frame_qp_payload {
  unsigned int frame_qp;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vidc_frame_bits_info_payload {
  unsigned int frame_bits;
  unsigned int header_bits;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vidc_s3d_frame_packing_payload {
  unsigned int fpa_id;
  unsigned int cancel_flag;
  unsigned int fpa_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int quin_cunx_flag;
  unsigned int content_interprtation_type;
  unsigned int spatial_flipping_flag;
  unsigned int frame0_flipped_flag;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int field_views_flag;
  unsigned int current_frame_is_frame0_flag;
  unsigned int frame0_self_contained_flag;
  unsigned int frame1_self_contained_flag;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int frame0_graid_pos_x;
  unsigned int frame0_graid_pos_y;
  unsigned int frame1_graid_pos_x;
  unsigned int frame1_graid_pos_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int fpa_reserved_byte;
  unsigned int fpa_repetition_period;
  unsigned int fpa_extension_flag;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vidc_vui_display_info_payload {
  unsigned int video_signal_present_flag;
  unsigned int video_format;
  unsigned int bit_depth_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int bit_depth_c;
  unsigned int video_full_range_flag;
  unsigned int color_description_present_flag;
  unsigned int color_primaries;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int transfer_characteristics;
  unsigned int matrix_coefficients;
  unsigned int chroma_location_info_present_flag;
  unsigned int chroma_format_idc;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int separate_color_plane_flag;
  unsigned int chroma_sample_loc_type_top_field;
  unsigned int chroma_sample_loc_type_bottom_field;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vidc_extradata_type {
  MSM_VIDC_EXTRADATA_NONE = 0x00000000,
  MSM_VIDC_EXTRADATA_MB_QUANTIZATION = 0x00000001,
  MSM_VIDC_EXTRADATA_INTERLACE_VIDEO = 0x00000002,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_EXTRADATA_VC1_FRAMEDISP = 0x00000003,
  MSM_VIDC_EXTRADATA_VC1_SEQDISP = 0x00000004,
  MSM_VIDC_EXTRADATA_TIMESTAMP = 0x00000005,
  MSM_VIDC_EXTRADATA_S3D_FRAME_PACKING = 0x00000006,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_EXTRADATA_FRAME_RATE = 0x00000007,
  MSM_VIDC_EXTRADATA_PANSCAN_WINDOW = 0x00000008,
  MSM_VIDC_EXTRADATA_RECOVERY_POINT_SEI = 0x00000009,
  MSM_VIDC_EXTRADATA_MPEG2_SEQDISP = 0x0000000D,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_EXTRADATA_STREAM_USERDATA = 0x0000000E,
  MSM_VIDC_EXTRADATA_FRAME_QP = 0x0000000F,
  MSM_VIDC_EXTRADATA_FRAME_BITS_INFO = 0x00000010,
  MSM_VIDC_EXTRADATA_INPUT_CROP = 0x0700000E,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_EXTRADATA_DIGITAL_ZOOM = 0x07000010,
  MSM_VIDC_EXTRADATA_MULTISLICE_INFO = 0x7F100000,
  MSM_VIDC_EXTRADATA_NUM_CONCEALED_MB = 0x7F100001,
  MSM_VIDC_EXTRADATA_INDEX = 0x7F100002,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_EXTRADATA_ASPECT_RATIO = 0x7F100003,
  MSM_VIDC_EXTRADATA_METADATA_LTR = 0x7F100004,
  MSM_VIDC_EXTRADATA_METADATA_FILLER = 0x7FE00002,
  MSM_VIDC_EXTRADATA_METADATA_MBI = 0x7F100005,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_EXTRADATA_VUI_DISPLAY_INFO = 0x7F100006,
};
enum msm_vidc_interlace_type {
  MSM_VIDC_INTERLACE_FRAME_PROGRESSIVE = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_INTERLACE_INTERLEAVE_FRAME_TOPFIELDFIRST = 0x02,
  MSM_VIDC_INTERLACE_INTERLEAVE_FRAME_BOTTOMFIELDFIRST = 0x04,
  MSM_VIDC_INTERLACE_FRAME_TOPFIELDFIRST = 0x08,
  MSM_VIDC_INTERLACE_FRAME_BOTTOMFIELDFIRST = 0x10,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_vidc_recovery_sei {
  MSM_VIDC_FRAME_RECONSTRUCTION_INCORRECT = 0x0,
  MSM_VIDC_FRAME_RECONSTRUCTION_CORRECT = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_FRAME_RECONSTRUCTION_APPROXIMATELY_CORRECT = 0x02,
};
enum msm_vidc_userdata_type {
  MSM_VIDC_USERDATA_TYPE_FRAME = 0x1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_VIDC_USERDATA_TYPE_TOP_FIELD = 0x2,
  MSM_VIDC_USERDATA_TYPE_BOTTOM_FIELD = 0x3,
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
