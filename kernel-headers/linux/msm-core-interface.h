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
#ifndef __MSM_CORE_LIB_H__
#define __MSM_CORE_LIB_H__
#include <linux/ioctl.h>
#define TEMP_DATA_POINTS 13
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAX_NUM_FREQ 200
enum msm_core_ioctl_params {
  MSM_CORE_LEAKAGE,
  MSM_CORE_VOLTAGE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define MSM_CORE_MAGIC 0x9D
struct sched_params {
  uint32_t cpumask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t cluster;
  uint32_t power[TEMP_DATA_POINTS][MAX_NUM_FREQ];
  uint32_t voltage[MAX_NUM_FREQ];
  uint32_t freq[MAX_NUM_FREQ];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define EA_LEAKAGE _IOWR(MSM_CORE_MAGIC, MSM_CORE_LEAKAGE, struct sched_params)
#define EA_VOLT _IOWR(MSM_CORE_MAGIC, MSM_CORE_VOLTAGE, struct sched_params)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
