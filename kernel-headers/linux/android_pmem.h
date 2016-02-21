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
#ifndef _ANDROID_PMEM_H_
#define _ANDROID_PMEM_H_
#include <linux/fs.h>
#define PMEM_KERNEL_TEST_MAGIC 0xc0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_KERNEL_TEST_NOMINAL_TEST_IOCTL _IO(PMEM_KERNEL_TEST_MAGIC, 1)
#define PMEM_KERNEL_TEST_ADVERSARIAL_TEST_IOCTL _IO(PMEM_KERNEL_TEST_MAGIC, 2)
#define PMEM_KERNEL_TEST_HUGE_ALLOCATION_TEST_IOCTL _IO(PMEM_KERNEL_TEST_MAGIC, 3)
#define PMEM_KERNEL_TEST_FREE_UNALLOCATED_TEST_IOCTL _IO(PMEM_KERNEL_TEST_MAGIC, 4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_KERNEL_TEST_LARGE_REGION_NUMBER_TEST_IOCTL _IO(PMEM_KERNEL_TEST_MAGIC, 5)
#define PMEM_IOCTL_MAGIC 'p'
#define PMEM_GET_PHYS _IOW(PMEM_IOCTL_MAGIC, 1, unsigned int)
#define PMEM_MAP _IOW(PMEM_IOCTL_MAGIC, 2, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_GET_SIZE _IOW(PMEM_IOCTL_MAGIC, 3, unsigned int)
#define PMEM_UNMAP _IOW(PMEM_IOCTL_MAGIC, 4, unsigned int)
#define PMEM_ALLOCATE _IOW(PMEM_IOCTL_MAGIC, 5, unsigned int)
#define PMEM_CONNECT _IOW(PMEM_IOCTL_MAGIC, 6, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_GET_TOTAL_SIZE _IOW(PMEM_IOCTL_MAGIC, 7, unsigned int)
#define HW3D_REVOKE_GPU _IOW(PMEM_IOCTL_MAGIC, 8, unsigned int)
#define HW3D_GRANT_GPU _IOW(PMEM_IOCTL_MAGIC, 9, unsigned int)
#define HW3D_WAIT_FOR_INTERRUPT _IOW(PMEM_IOCTL_MAGIC, 10, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_CLEAN_INV_CACHES _IOW(PMEM_IOCTL_MAGIC, 11, unsigned int)
#define PMEM_CLEAN_CACHES _IOW(PMEM_IOCTL_MAGIC, 12, unsigned int)
#define PMEM_INV_CACHES _IOW(PMEM_IOCTL_MAGIC, 13, unsigned int)
#define PMEM_GET_FREE_SPACE _IOW(PMEM_IOCTL_MAGIC, 14, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define PMEM_ALLOCATE_ALIGNED _IOW(PMEM_IOCTL_MAGIC, 15, unsigned int)
struct pmem_region {
  unsigned long offset;
  unsigned long len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct pmem_addr {
  unsigned long vaddr;
  unsigned long offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long length;
};
struct pmem_freespace {
  unsigned long total;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long largest;
};
struct pmem_allocation {
  unsigned long size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int align;
};
#endif
