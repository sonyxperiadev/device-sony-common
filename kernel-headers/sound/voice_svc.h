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
#ifndef __VOICE_SVC_H__
#define __VOICE_SVC_H__
#include <linux/types.h>
#include <linux/ioctl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VOICE_SVC_DRIVER_NAME "voice_svc"
#define VOICE_SVC_MVM_STR "MVM"
#define VOICE_SVC_CVS_STR "CVS"
#define MAX_APR_SERVICE_NAME_LEN 64
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSG_REGISTER 0x1
#define MSG_REQUEST 0x2
#define MSG_RESPONSE 0x3
struct voice_svc_write_msg {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u32 msg_type;
  __u8 payload[0];
};
struct voice_svc_register {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  char svc_name[MAX_APR_SERVICE_NAME_LEN];
  __u32 src_port;
  __u8 reg_flag;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct voice_svc_cmd_response {
  __u32 src_port;
  __u32 dest_port;
  __u32 token;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u32 opcode;
  __u32 payload_size;
  __u8 payload[0];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct voice_svc_cmd_request {
  char svc_name[MAX_APR_SERVICE_NAME_LEN];
  __u32 src_port;
  __u32 dest_port;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u32 token;
  __u32 opcode;
  __u32 payload_size;
  __u8 payload[0];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif
