#include "common.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

#define LOG_TAG "FPC COMMON"

#include <cutils/log.h>

err_t sysfs_write(char *path, char *s)
{
    char buf[80];
    ssize_t len;
    int ret = 0;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);

        ret = -1;
    }

    close(fd);

    return ret;
}

err_t sys_fs_irq_poll(char *path)
{

    char buf[80];
    int ret = 0;
    int result;
    struct pollfd pollfds[2];

    pollfds[0].fd = open(path, O_RDONLY | O_NONBLOCK);

    if (pollfds[0].fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    char dummybuf;
    read(pollfds[0].fd, &dummybuf, 1);
    pollfds[0].events = POLLPRI;

    result = poll(pollfds, 1, 1000);

    switch (result) {
        case 0:
            ALOGD ("timeout\n");
            close(pollfds[0].fd);
            return -1;
        case -1:
            ALOGE ("poll error \n");
            close(pollfds[0].fd);
            return -1;
        default:
            ALOGD ("IRQ GOT \n");
            close(pollfds[0].fd);
            break;
    }

    close(pollfds[0].fd);

    return ret;
}