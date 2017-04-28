#ifndef FINGERPRINT_COMMON_H
#define FINGERPRINT_COMMON_H

#include <stdint.h>

typedef int32_t err_t;
err_t sysfs_write(char *path, char *s);
err_t sys_fs_irq_poll(char *path);

#endif //FINGERPRINT_COMMON_H
