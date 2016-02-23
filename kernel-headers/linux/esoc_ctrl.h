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
#ifndef _UAPI_ESOC_CTRL_H_
#define _UAPI_ESOC_CTRL_H_
#define ESOC_CODE 0xCC
#define ESOC_CMD_EXE _IOW(ESOC_CODE, 1, u32)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ESOC_WAIT_FOR_REQ _IOR(ESOC_CODE, 2, u32)
#define ESOC_NOTIFY _IOW(ESOC_CODE, 3, u32)
#define ESOC_GET_STATUS _IOR(ESOC_CODE, 4, u32)
#define ESOC_WAIT_FOR_CRASH _IOR(ESOC_CODE, 6, u32)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ESOC_REG_REQ_ENG _IO(ESOC_CODE, 7)
#define ESOC_REG_CMD_ENG _IO(ESOC_CODE, 8)
#define HSIC "HSIC"
#define HSICPCIe "HSIC+PCIe"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PCIe "PCIe"
enum esoc_evt {
  ESOC_RUN_STATE = 0x1,
  ESOC_UNEXPECTED_RESET,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ESOC_ERR_FATAL,
  ESOC_IN_DEBUG,
  ESOC_REQ_ENG_ON,
  ESOC_REQ_ENG_OFF,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ESOC_CMD_ENG_ON,
  ESOC_CMD_ENG_OFF,
  ESOC_INVALID_STATE,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum esoc_cmd {
  ESOC_PWR_ON = 1,
  ESOC_PWR_OFF,
  ESOC_FORCE_PWR_OFF,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ESOC_RESET,
  ESOC_PREPARE_DEBUG,
  ESOC_EXE_DEBUG,
  ESOC_EXIT_DEBUG,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum esoc_notify {
  ESOC_IMG_XFER_DONE = 1,
  ESOC_BOOT_DONE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ESOC_BOOT_FAIL,
  ESOC_IMG_XFER_RETRY,
  ESOC_IMG_XFER_FAIL,
  ESOC_UPGRADE_AVAILABLE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ESOC_DEBUG_DONE,
  ESOC_DEBUG_FAIL,
  ESOC_PRIMARY_CRASH,
  ESOC_PRIMARY_REBOOT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum esoc_req {
  ESOC_REQ_IMG = 1,
  ESOC_REQ_DEBUG,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  ESOC_REQ_SHUTDOWN,
};
#endif
