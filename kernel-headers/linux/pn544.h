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
#ifndef _PN544_H_
#define _PN544_H_
#define PN544_MAGIC 0xE9
#define PN544_SET_PWR _IOW(PN544_MAGIC, 0x01, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PN544_SET_GPIO 0
#define PN544_DEVICE_NAME "pn544"
enum pn544_set_pwr_cmd {
  PN544_SET_PWR_OFF,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  PN544_SET_PWR_ON,
  PN544_SET_PWR_FWDL,
};
enum pn544_state {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  PN544_STATE_UNKNOWN,
  PN544_STATE_OFF,
  PN544_STATE_ON,
  PN544_STATE_FWDL,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct pn544_i2c_platform_data {
  int irq_gpio;
  int fwdl_en_gpio;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int ven_gpio;
  int pvdd_en_gpio;
  int gpio_fwdl_enable[4];
  int gpio_fwdl_disable[4];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int(* chip_config) (enum pn544_state, void *);
  int(* driver_loaded) (void);
  void(* driver_unloaded) (void);
  int(* driver_opened) (void);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void(* driver_closed) (void);
};
#endif
