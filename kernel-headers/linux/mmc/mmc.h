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
#ifndef UAPI_MMC_MMC_H
#define UAPI_MMC_MMC_H
#define MMC_GO_IDLE_STATE 0
#define MMC_SEND_OP_COND 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_ALL_SEND_CID 2
#define MMC_SET_RELATIVE_ADDR 3
#define MMC_SET_DSR 4
#define MMC_SLEEP_AWAKE 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_SWITCH 6
#define MMC_SELECT_CARD 7
#define MMC_SEND_EXT_CSD 8
#define MMC_SEND_CSD 9
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_SEND_CID 10
#define MMC_READ_DAT_UNTIL_STOP 11
#define MMC_STOP_TRANSMISSION 12
#define MMC_SEND_STATUS 13
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_BUS_TEST_R 14
#define MMC_GO_INACTIVE_STATE 15
#define MMC_BUS_TEST_W 19
#define MMC_SPI_READ_OCR 58
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_SPI_CRC_ON_OFF 59
#define MMC_SET_BLOCKLEN 16
#define MMC_READ_SINGLE_BLOCK 17
#define MMC_READ_MULTIPLE_BLOCK 18
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_SEND_TUNING_BLOCK 19
#define MMC_SEND_TUNING_BLOCK_HS200 21
#define MMC_SEND_TUNING_BLOCK_HS400 MMC_SEND_TUNING_BLOCK_HS200
#define MMC_WRITE_DAT_UNTIL_STOP 20
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_SET_BLOCK_COUNT 23
#define MMC_WRITE_BLOCK 24
#define MMC_WRITE_MULTIPLE_BLOCK 25
#define MMC_PROGRAM_CID 26
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_PROGRAM_CSD 27
#define MMC_SET_WRITE_PROT 28
#define MMC_CLR_WRITE_PROT 29
#define MMC_SEND_WRITE_PROT 30
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_ERASE_GROUP_START 35
#define MMC_ERASE_GROUP_END 36
#define MMC_ERASE 38
#define MMC_FAST_IO 39
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MMC_GO_IRQ_STATE 40
#define MMC_LOCK_UNLOCK 42
#define MMC_APP_CMD 55
#define MMC_GEN_CMD 56
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
