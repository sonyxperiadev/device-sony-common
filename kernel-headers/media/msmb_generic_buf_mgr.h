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
#ifndef __UAPI_MEDIA_MSMB_BUF_MNGR_H__
#define __UAPI_MEDIA_MSMB_BUF_MNGR_H__
#include <media/msmb_camera.h>
enum msm_camera_buf_mngr_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  MSM_CAMERA_BUF_MNGR_CONT_MAP,
  MSM_CAMERA_BUF_MNGR_CONT_UNMAP,
  MSM_CAMERA_BUF_MNGR_CONT_MAX,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum msm_camera_buf_mngr_buf_type {
  MSM_CAMERA_BUF_MNGR_BUF_PLANAR,
  MSM_CAMERA_BUF_MNGR_BUF_USER,
  MSM_CAMERA_BUF_MNGR_BUF_INVALID,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_buf_mngr_info {
  uint32_t session_id;
  uint32_t stream_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t frame_id;
  struct timeval timestamp;
  uint32_t index;
  uint32_t reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum msm_camera_buf_mngr_buf_type type;
  struct msm_camera_user_buf_cont_t user_buf;
};
struct msm_buf_mngr_main_cont_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t session_id;
  uint32_t stream_id;
  enum msm_camera_buf_mngr_cmd cmd;
  uint32_t cnt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int32_t cont_fd;
};
struct v4l2_subdev * msm_buf_mngr_get_subdev(void);
#define VIDIOC_MSM_BUF_MNGR_GET_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 33, struct msm_buf_mngr_info)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_BUF_MNGR_PUT_BUF _IOWR('V', BASE_VIDIOC_PRIVATE + 34, struct msm_buf_mngr_info)
#define VIDIOC_MSM_BUF_MNGR_BUF_DONE _IOWR('V', BASE_VIDIOC_PRIVATE + 35, struct msm_buf_mngr_info)
#define VIDIOC_MSM_BUF_MNGR_CONT_CMD _IOWR('V', BASE_VIDIOC_PRIVATE + 36, struct msm_buf_mngr_main_cont_info)
#define VIDIOC_MSM_BUF_MNGR_INIT _IOWR('V', BASE_VIDIOC_PRIVATE + 37, struct msm_buf_mngr_info)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VIDIOC_MSM_BUF_MNGR_DEINIT _IOWR('V', BASE_VIDIOC_PRIVATE + 38, struct msm_buf_mngr_info)
#endif
