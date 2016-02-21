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
#ifndef __TFA98XX_H
#define __TFA98XX_H
struct tfa98xx_param_data {
  unsigned int size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int type;
  unsigned char * data;
};
#define TFA98XX_IOCTL_MAGIC 't'
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TFA98XX_PATCH_PARAM _IOW(TFA98XX_IOCTL_MAGIC, 0, struct tfa98xx_param_data)
#define TFA98XX_CONFIG_PARAM _IOW(TFA98XX_IOCTL_MAGIC, 1, struct tfa98xx_param_data)
#define TFA98XX_SPEAKER_PARAM _IOW(TFA98XX_IOCTL_MAGIC, 2, struct tfa98xx_param_data)
#define TFA98XX_PRESET_PARAM _IOW(TFA98XX_IOCTL_MAGIC, 3, struct tfa98xx_param_data)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TFA98XX_EQ_PARAM _IOW(TFA98XX_IOCTL_MAGIC, 4, struct tfa98xx_param_data)
#define PARAM_SIZE_MAX (16 * 1024)
enum {
  PATCH_DSP = 0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  PATCH_COLDBOOT,
  PATCH_MAX
};
enum {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  AMP_TOP = 0,
  AMP_BOTTOM,
  AMP_RECEIVER,
  AMP_MAX
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum {
  TYPE_HIFISPEAKER_TOP = 0,
  TYPE_HIFISPEAKER_BOTTOM,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  TYPE_HIFISPEAKER_RING_TOP,
  TYPE_HIFISPEAKER_RING_BOTTOM,
  TYPE_HIFISPEAKER_SFORCE_TOP,
  TYPE_HIFISPEAKER_SFORCE_BOTTOM,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  TYPE_VOICECALLSPEAKER_TOP,
  TYPE_VOICECALLSPEAKER_BOTTOM,
  TYPE_FMSPEAKER_TOP,
  TYPE_FMSPEAKER_BOTTOM,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  TYPE_VOICECALLEARPICE_TOP,
  TYPE_MAX
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
