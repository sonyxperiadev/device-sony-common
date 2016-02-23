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
#ifndef __MSMB_PPROC_H
#define __MSMB_PPROC_H
#ifdef MSM_CAMERA_BIONIC
#include <sys/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#include <linux/videodev2.h>
#include <linux/types.h>
#include <media/msmb_generic_buf_mgr.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAX_PLANES VIDEO_MAX_PLANES
#define PARTIAL_FRAME_STRIPE_COUNT 4
#define MAX_NUM_CPP_STRIPS 8
#define MSM_CPP_MAX_NUM_PLANES 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSM_CPP_MIN_FRAME_LENGTH 13
#define MSM_CPP_MAX_FRAME_LENGTH 4096
#define MSM_CPP_MAX_FW_NAME_LEN 32
#define MAX_FREQ_TBL 10
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_cpp_frame_type {
  MSM_CPP_OFFLINE_FRAME,
  MSM_CPP_REALTIME_FRAME,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_vpe_frame_type {
  MSM_VPE_OFFLINE_FRAME,
  MSM_VPE_REALTIME_FRAME,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_cpp_frame_strip_info {
  uint32_t scale_v_en;
  uint32_t scale_h_en;
  uint32_t upscale_v_en;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t upscale_h_en;
  int32_t src_start_x;
  uint32_t src_end_x;
  int32_t src_start_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t src_end_y;
  int32_t extra_src_start_x;
  int32_t extra_src_end_x;
  int32_t extra_src_start_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t extra_src_end_y;
  int32_t extra_initial_vertical_count[2];
  int32_t extra_initial_horizontal_count[2];
  int32_t extra_left_crop;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t extra_top_crop;
  int32_t extra_pad_bottom;
  int32_t extra_pad_top;
  int32_t extra_pad_right;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t extra_pad_left;
  int32_t extra_upscale_width;
  int32_t extra_upscale_height;
  uint32_t temporal_pad_bottom;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t temporal_pad_top;
  uint32_t temporal_pad_right;
  uint32_t temporal_pad_left;
  int32_t temporal_src_start_x;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t temporal_src_end_x;
  int32_t temporal_src_start_y;
  uint32_t temporal_src_end_y;
  uint32_t pad_bottom;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t pad_top;
  uint32_t pad_right;
  uint32_t pad_left;
  uint32_t v_init_phase;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t h_init_phase;
  uint32_t h_phase_step;
  uint32_t v_phase_step;
  uint32_t spatial_denoise_crop_width_first_pixel;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t spatial_denoise_crop_width_last_pixel;
  uint32_t spatial_denoise_crop_height_first_line;
  uint32_t spatial_denoise_crop_height_last_line;
  uint32_t sharpen_crop_height_first_line;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t sharpen_crop_height_last_line;
  uint32_t sharpen_crop_width_first_pixel;
  uint32_t sharpen_crop_width_last_pixel;
  uint32_t temporal_denoise_crop_width_first_pixel;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t temporal_denoise_crop_width_last_pixel;
  uint32_t temporal_denoise_crop_height_first_line;
  uint32_t temporal_denoise_crop_height_last_line;
  uint32_t prescaler_spatial_denoise_crop_width_first_pixel;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t prescaler_spatial_denoise_crop_width_last_pixel;
  uint32_t prescaler_spatial_denoise_crop_height_first_line;
  uint32_t prescaler_spatial_denoise_crop_height_last_line;
  uint32_t state_crop_width_first_pixel;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t state_crop_width_last_pixel;
  uint32_t state_crop_height_first_line;
  uint32_t state_crop_height_last_line;
  int32_t dst_start_x;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t dst_end_x;
  int32_t dst_start_y;
  uint32_t dst_end_y;
  int32_t temporal_dst_start_x;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t temporal_dst_end_x;
  int32_t temporal_dst_start_y;
  uint32_t temporal_dst_end_y;
  uint32_t input_bytes_per_pixel;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t output_bytes_per_pixel;
  uint32_t temporal_bytes_per_pixel;
  uint32_t source_address[2];
  uint32_t extra_source_address[2];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t destination_address[2];
  uint32_t temporal_source_address[2];
  uint32_t temporal_destination_address[2];
  uint32_t src_stride;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t dst_stride;
  uint32_t rotate_270;
  uint32_t horizontal_flip;
  uint32_t vertical_flip;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t scale_output_width;
  uint32_t scale_output_height;
  uint32_t spatial_denoise_crop_en;
  uint32_t sharpen_crop_en;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t temporal_denoise_crop_en;
  uint32_t prescaler_spatial_denoise_crop_en;
  uint32_t state_crop_en;
  int32_t we_h_init;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t we_v_init;
  int32_t we_h_step;
  int32_t we_v_step;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_cpp_buffer_info_t {
  int32_t fd;
  uint32_t index;
  uint32_t offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t native_buff;
  uint8_t processed_divert;
  uint32_t identity;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_cpp_stream_buff_info_t {
  uint32_t identity;
  uint32_t num_buffs;
  struct msm_cpp_buffer_info_t * buffer_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum msm_cpp_batch_mode_t {
  BATCH_MODE_NONE,
  BATCH_MODE_VIDEO,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  BATCH_MODE_PREVIEW
};
struct msm_cpp_batch_info_t {
  enum msm_cpp_batch_mode_t batch_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t batch_size;
  uint32_t intra_plane_offset[MAX_PLANES];
  uint32_t pick_preview_idx;
  uint32_t cont_idx;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_cpp_frame_info_t {
  int32_t frame_id;
  struct timeval timestamp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t inst_id;
  uint32_t identity;
  uint32_t client_id;
  enum msm_cpp_frame_type frame_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t num_strips;
  struct msm_cpp_frame_strip_info __user * strip_info;
  uint32_t msg_len;
  uint32_t * cpp_cmd_msg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int src_fd;
  int dst_fd;
  struct timeval in_time, out_time;
  void __user * cookie;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t * status;
  int32_t duplicate_output;
  uint32_t duplicate_identity;
  uint32_t feature_mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t we_disable;
  struct msm_cpp_buffer_info_t input_buffer_info;
  struct msm_cpp_buffer_info_t output_buffer_info[8];
  struct msm_cpp_buffer_info_t duplicate_buffer_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_cpp_buffer_info_t tnr_scratch_buffer_info[2];
  uint32_t reserved;
  uint8_t partial_frame_indicator;
  uint8_t first_payload;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t last_payload;
  uint32_t first_stripe_index;
  uint32_t last_stripe_index;
  uint32_t stripe_info_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t stripe_info;
  struct msm_cpp_batch_info_t batch_info;
};
struct msm_cpp_pop_stream_info_t {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t frame_id;
  uint32_t identity;
};
struct cpp_hw_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t cpp_hw_version;
  uint32_t cpp_hw_caps;
  unsigned long freq_tbl[MAX_FREQ_TBL];
  uint32_t freq_tbl_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vpe_frame_strip_info {
  uint32_t src_w;
  uint32_t src_h;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t dst_w;
  uint32_t dst_h;
  uint32_t src_x;
  uint32_t src_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t phase_step_x;
  uint32_t phase_step_y;
  uint32_t phase_init_x;
  uint32_t phase_init_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vpe_buffer_info_t {
  int32_t fd;
  uint32_t index;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t offset;
  uint8_t native_buff;
  uint8_t processed_divert;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_vpe_stream_buff_info_t {
  uint32_t identity;
  uint32_t num_buffs;
  struct msm_vpe_buffer_info_t * buffer_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_vpe_frame_info_t {
  int32_t frame_id;
  struct timeval timestamp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t inst_id;
  uint32_t identity;
  uint32_t client_id;
  enum msm_vpe_frame_type frame_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_vpe_frame_strip_info strip_info;
  unsigned long src_fd;
  unsigned long dst_fd;
  struct ion_handle * src_ion_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct ion_handle * dest_ion_handle;
  unsigned long src_phyaddr;
  unsigned long dest_phyaddr;
  unsigned long src_chroma_plane_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long dest_chroma_plane_offset;
  struct timeval in_time, out_time;
  void * cookie;
  struct msm_vpe_buffer_info_t input_buffer_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct msm_vpe_buffer_info_t output_buffer_info;
};
struct msm_pproc_queue_buf_info {
  struct msm_buf_mngr_info buff_mgr_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint8_t is_buf_dirty;
};
struct msm_cpp_clock_settings_t {
  unsigned long clock_rate;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint64_t avg;
  uint64_t inst;
};
#define VIDIOC_MSM_CPP_CFG _IOWR('V', BASE_VIDIOC_PRIVATE, struct msm_camera_v4l2_ioctl_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_CPP_GET_EVENTPAYLOAD _IOWR('V', BASE_VIDIOC_PRIVATE + 1, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_GET_INST_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 2, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_LOAD_FIRMWARE _IOWR('V', BASE_VIDIOC_PRIVATE + 3, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_GET_HW_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 4, struct msm_camera_v4l2_ioctl_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_CPP_FLUSH_QUEUE _IOWR('V', BASE_VIDIOC_PRIVATE + 5, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_ENQUEUE_STREAM_BUFF_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 6, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_DEQUEUE_STREAM_BUFF_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 7, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_VPE_CFG _IOWR('V', BASE_VIDIOC_PRIVATE + 8, struct msm_camera_v4l2_ioctl_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_VPE_TRANSACTION_SETUP _IOWR('V', BASE_VIDIOC_PRIVATE + 9, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_VPE_GET_EVENTPAYLOAD _IOWR('V', BASE_VIDIOC_PRIVATE + 10, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_VPE_GET_INST_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 11, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_VPE_ENQUEUE_STREAM_BUFF_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 12, struct msm_camera_v4l2_ioctl_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_VPE_DEQUEUE_STREAM_BUFF_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 13, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_QUEUE_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 14, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_APPEND_STREAM_BUFF_INFO _IOWR('V', BASE_VIDIOC_PRIVATE + 15, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_SET_CLOCK _IOWR('V', BASE_VIDIOC_PRIVATE + 16, struct msm_camera_v4l2_ioctl_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_CPP_POP_STREAM_BUFFER _IOWR('V', BASE_VIDIOC_PRIVATE + 17, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_IOMMU_ATTACH _IOWR('V', BASE_VIDIOC_PRIVATE + 18, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_IOMMU_DETACH _IOWR('V', BASE_VIDIOC_PRIVATE + 19, struct msm_camera_v4l2_ioctl_t)
#define VIDIOC_MSM_CPP_DELETE_STREAM_BUFF _IOWR('V', BASE_VIDIOC_PRIVATE + 20, struct msm_camera_v4l2_ioctl_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_EVENT_CPP_FRAME_DONE (V4L2_EVENT_PRIVATE_START + 0)
#define V4L2_EVENT_VPE_FRAME_DONE (V4L2_EVENT_PRIVATE_START + 1)
struct msm_camera_v4l2_ioctl_t {
  uint32_t id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  size_t len;
  int32_t trans_code;
  void __user * ioctl_ptr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
