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
#ifndef _LINUX_PN547_H
#define _LINUX_PN547_H
#define PN547_MAGIC 0xE9
#define PN547_SET_PWR _IOW(PN547_MAGIC, 0x01, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum pn547_state {
  PN547_STATE_UNKNOWN,
  PN547_STATE_OFF,
  PN547_STATE_ON,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  PN547_STATE_FWDL,
};
struct pn547_i2c_platform_data {
  void(* conf_gpio) (void);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int irq_gpio;
  int ven_gpio;
  int firm_gpio;
  int pvdd_en_gpio;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int configure_gpio;
  int configure_mpp;
  bool dynamic_config;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define board_nfc_parse_dt(x,...) 0
#define board_nfc_hw_lag_check(x,...) 0
#endif
