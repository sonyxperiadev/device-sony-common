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
#ifndef _UAPI_LSM_PARAMS_H__
#define _UAPI_LSM_PARAMS_H__
#include <linux/types.h>
#include <sound/asound.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_LSM_VERSION SNDRV_PROTOCOL_VERSION(0, 1, 0)
#define LSM_OUT_FORMAT_PCM (0)
#define LSM_OUT_FORMAT_ADPCM (1 << 0)
#define LSM_OUT_DATA_RAW (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define LSM_OUT_DATA_PACKED (1)
#define LSM_OUT_DATA_EVENTS_DISABLED (0)
#define LSM_OUT_DATA_EVENTS_ENABLED (1)
#define LSM_OUT_TRANSFER_MODE_RT (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define LSM_OUT_TRANSFER_MODE_FTRT (1)
enum lsm_app_id {
  LSM_VOICE_WAKEUP_APP_ID = 1,
  LSM_VOICE_WAKEUP_APP_ID_V2 = 2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum lsm_detection_mode {
  LSM_MODE_KEYWORD_ONLY_DETECTION = 1,
  LSM_MODE_USER_KEYWORD_DETECTION
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum lsm_vw_status {
  LSM_VOICE_WAKEUP_STATUS_RUNNING = 1,
  LSM_VOICE_WAKEUP_STATUS_DETECTED,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  LSM_VOICE_WAKEUP_STATUS_END_SPEECH,
  LSM_VOICE_WAKEUP_STATUS_REJECTED
};
enum LSM_PARAM_TYPE {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  LSM_ENDPOINT_DETECT_THRESHOLD = 0,
  LSM_OPERATION_MODE,
  LSM_GAIN,
  LSM_MIN_CONFIDENCE_LEVELS,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  LSM_REG_SND_MODEL,
  LSM_DEREG_SND_MODEL,
  LSM_CUSTOM_PARAMS,
  LSM_PARAMS_MAX,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct snd_lsm_ep_det_thres {
  __u32 epd_begin;
  __u32 epd_end;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct snd_lsm_detect_mode {
  enum lsm_detection_mode mode;
  bool detect_failure;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct snd_lsm_gain {
  __u16 gain;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct snd_lsm_sound_model_v2 {
  __u8 __user * data;
  __u8 * confidence_level;
  __u32 data_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  enum lsm_detection_mode detection_mode;
  __u8 num_confidence_levels;
  bool detect_failure;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct snd_lsm_session_data {
  enum lsm_app_id app_id;
};
struct snd_lsm_event_status {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u16 status;
  __u16 payload_size;
  __u8 payload[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct snd_lsm_detection_params {
  __u8 * conf_level;
  enum lsm_detection_mode detect_mode;
  __u8 num_confidence_levels;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  bool detect_failure;
};
struct lsm_params_info {
  __u32 module_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u32 param_id;
  __u32 param_size;
  __u8 __user * param_data;
  enum LSM_PARAM_TYPE param_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct snd_lsm_module_params {
  __u8 __user * params;
  __u32 num_params;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u32 data_size;
};
struct snd_lsm_output_format_cfg {
  __u8 format;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u8 packing;
  __u8 events;
  __u8 mode;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_LSM_DEREG_SND_MODEL _IOW('U', 0x01, int)
#define SNDRV_LSM_EVENT_STATUS _IOW('U', 0x02, struct snd_lsm_event_status)
#define SNDRV_LSM_ABORT_EVENT _IOW('U', 0x03, int)
#define SNDRV_LSM_START _IOW('U', 0x04, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_LSM_STOP _IOW('U', 0x05, int)
#define SNDRV_LSM_SET_SESSION_DATA _IOW('U', 0x06, struct snd_lsm_session_data)
#define SNDRV_LSM_REG_SND_MODEL_V2 _IOW('U', 0x07, struct snd_lsm_sound_model_v2)
#define SNDRV_LSM_LAB_CONTROL _IOW('U', 0x08, uint32_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_LSM_STOP_LAB _IO('U', 0x09)
#define SNDRV_LSM_SET_PARAMS _IOW('U', 0x0A, struct snd_lsm_detection_params)
#define SNDRV_LSM_SET_MODULE_PARAMS _IOW('U', 0x0B, struct snd_lsm_module_params)
#define SNDRV_LSM_OUT_FORMAT_CFG _IOW('U', 0x0C, struct snd_lsm_output_format_cfg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_LSM_SET_PORT _IO('U', 0x0D)
#endif
