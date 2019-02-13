/*
 * RQBalance-based PowerHAL
 * External interactor
 *
 * Copyright (C) 2017-2019 AngeloGioacchino Del Regno <kholk11@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "RQBalance-PowerHAL-HALEXT"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#include <android-base/stringprintf.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>

#include <cutils/properties.h>
#include <utils/Log.h>
#include <utils/Trace.h>

#include "common.h"
#include "Hints.h"
#include "Power.h"
#include "RQBalanceHALExt.h"

RQBalanceHALExt::RQBalanceHALExt(RQBalanceHintsHandler* rqb_handler):
    mRQBHandler(rqb_handler) { }

RQBalanceHALExt::~RQBalanceHALExt() { }

/*******************************************************************/

/*
 * The argument[] array:
 *
 * This array contains a (positive) integer number, which values
 * are used as described.
 *
 * If no ID is passed to the function, argument[] is used to
 * recognize which kind of hint we are trying to initialize
 * and which driver or Android HAL is requesting init of a
 * performance lock.
 *
 *             argument[] = 0x AA BB
 *                             || ||
 *                             /   \
 *                            /     \
 *               Type of lock    State of the lock
 *                (0x00-0x7F)        (00-FF)
 *                 LOCKTYPE         LOCKSTATE
 *
 * For the description of the known lock types and lock states,
 * please look at the enumerations in the library header(s).
 *
 * If an ID is passed to the function, this means that we are
 * requested to modify the parameters of the performance lock
 * that was initialized and being used by the driver or Android
 * HAL identified by the specified ID.
 *
 * Please note that there's no fixed ID.
 * The ID is dynamically assigned on a first-come, first-served
 * logic.
 */

static struct rqbalance_ctl_locks current_locks[MAX_PERMITTED_LOCKS];
static short number_of_locks = 0;
static int possible_cores = -1;

int get_locktype_by_tid(timer_t timerid);
int get_locktype_by_id(unsigned int id);

/* Timer Handling */

/*
 * timer_expired - Callback function for rqbalance_ctl timers
 *
 * \param sig - Structure passed by *(sigev_notify_function)
 */
void RQBalanceHALExt::timer_expired(union sigval sig)
{
        RQBalanceHALExt *obj = (RQBalanceHALExt *)sig.sival_ptr;
	int ltid;

	if (sig.sival_int == 0)
		return;

	ltid = get_locktype_by_id((unsigned int)sig.sival_int);

	obj->locktype_action(ltid, 0);

	return;
}

/*
 * start_timer - Initialize itimerspec and start the timer
 *
 * \param timerid - ID of the timer
 * \param duration_ms - Timer duration in milliseconds
 * \return Returns success (0) or failure (negative errno)
 */
int RQBalanceHALExt::start_timer(timer_t timerid, int duration_ms)
{
	struct itimerspec tspec;
	int entryno;

	tspec.it_value.tv_sec = MSEC_TO_SEC(duration_ms);
	tspec.it_value.tv_nsec = MSEC_TO_NSEC(duration_ms);
	/* Do not auto-rearm */
	tspec.it_interval.tv_sec = 0;
	tspec.it_interval.tv_nsec = 0;

	entryno = get_locktype_by_tid(timerid);
	if (!entryno) {
		ALOGE("WTF: Tried to start timer for unexistant perflock!");
		return entryno;
	}

	locktype_action(entryno, 1);

	return timer_settime(timerid, 0, &tspec, NULL);
}

/*
 * new_timer - Create a new monotonic timer with unique ID
 *
 * \param timerid - ID of the timer
 * \param luid - Lock unique identifier
 * \return Returns success (0) or failure (negative errno)
 */
int RQBalanceHALExt::new_timer(timer_t timerid, int luid)
{
	struct sigevent sev;

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_notify_function = &RQBalanceHALExt::timer_expired;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = (void *)this;
	sev.sigev_value.sival_int = luid;

	if (!timer_create(CLOCK_MONOTONIC, &sev, &timerid))
		return -ETIME;

	return 0;
}

const char* RQBalanceHALExt::lock_type_str(int t)
{
	switch (t) {
		case OMX_DECODER:
			return "VIDC Decoder";
		case DISPLAY_LAYER:
			return "Display Layer";
		case OMX_ENCODER:
			return "VIDC Encoder";
		default:
			break;
	}

	return "Unknown";
}

/* Search functions */
int get_locktype_by_tid(timer_t tid)
{
	int i;

	for (i = 0; i < number_of_locks; i++) {
		if (current_locks[number_of_locks].tid == tid)
			return i;
	}

	return -EINVAL;
}

int get_locktype_by_id(unsigned int id)
{
	int i;

	for (i = 0; i < number_of_locks; i++) {
		if (current_locks[number_of_locks].luid == id)
			return i;
	}

	return -EINVAL;
}

int get_locktype_by_type(unsigned short type)
{
	int i;

	for (i = 0; i < number_of_locks; i++) {
		if (current_locks[number_of_locks].drid == type)
			return i;
	}

	return -EINVAL;
}

/* HALExt Logic */

/*
 * locktype_action - Take an action for the input lock type
 *
 * \param entry - Position in the locks array
 * \param state - Requested action
 * \return Returns success (0) or failure (negative errno)
 */
int RQBalanceHALExt::locktype_action(int entry, int state)
{
	int type = current_locks[entry].drid;

	switch (type)
	{
		case OMX_DECODER:
			ALOGI("OMX Decoder hint received.");
			if (state) {
				mRQBHandler->SetPowerMode(
                                                POWER_MODE_OMXDECODE);
			} else {
				mRQBHandler->SetPowerMode(
                                                POWER_MODE_BALANCED);
			}
			break;
		case DISPLAY_LAYER:
			ALOGI("Display layer hint received.");
			if (state) {
			/* do stuff */
			} else {
			/* revert stuff */
			}
			break;
		case OMX_ENCODER:
			ALOGI("OMX Encoder hint received.");
			if (state) {
				mRQBHandler->SetPowerMode(
                                                POWER_MODE_OMXENCODE);
			} else {
				mRQBHandler->SetPowerMode(
                                                POWER_MODE_BALANCED);
			}
			break;
		case RQB_POWERHAL:
			ALOGI("RQBalance PowerHAL hint received.");
			if (state) {
			/* do stuff */
			} else {
			/* revert stuff */
			}
			break;
		default:
			ALOGE("ERROR: Unknown hint type %d !!!!", type);
			break;
	}

	return 0;
}

int RQBalanceHALExt::new_lock_init(unsigned int time,
                                   unsigned short type, int state)
{
	unsigned int luid;
	int locknum, ret;

	/* If lock is already present */
	ret = get_locktype_by_type(type);
	if (ret < 0) {
		luid = rand();
	} else {
		locknum = ret;
		luid = current_locks[locknum].luid;

		if (time > 0)
			start_timer(current_locks[locknum].tid, time);

		goto do_action;
	}

	number_of_locks++;

	/* Initialize current_locks array element with new lock infos */
	current_locks[number_of_locks].luid = luid;
	current_locks[number_of_locks].time = time;
	current_locks[number_of_locks].drid = type;
	current_locks[number_of_locks].state = state;
	locknum = number_of_locks;

do_action:
	if (time) {
		ret = new_timer(current_locks[locknum].tid,
				current_locks[locknum].luid);
		if (ret) {
			ALOGE("ERROR: Cannot create timed lock!!");
			return ret;
		}
		ret = start_timer(current_locks[locknum].tid, time);
		if (ret) {
			ALOGE("ERROR: Cannot start timer!!");
			return ret;
		}
	} else {
		locktype_action(locknum, 1);
	}

	return luid;
}

int lock_set_arg(int lparm)
{
	int ret = -EINVAL;

	switch (lparm)
	{
		case ALL_CPUS_PWR_CLPS_DIS:
			ALOGE("Power collapse disable not implemented");
			return -EINVAL;
		case CPU0_MIN_FREQ_TURBO_MAX:
		case CPU4_MIN_FREQ_TURBO_MAX:
			ALOGE("Frequency optimizations not implemented");
			return -EINVAL;
		case ALL_CORES_ONLINE:
			ALOGE("Special core modes not implemented");
			//snprintf(tmp, 2, "%d", possible_cores);
			//__set_spc_power_mode(tmp, tmp, NULL, NULL, NULL);
			/*
			 * TODO: Create a timer? When should we put cores back
			 *       to the balanced/previous profile???
			 */
			return -EINVAL;
		default:
			ALOGD("Parameter not implemented: 0x%x", lparm);
			return -EINVAL;
	}

	return ret;
}

void RQBalanceHALExt::remove_and_reorder(int entryno)
{
	/* This is paranoid, but avoids possible future coding mistakes */
	/* Note: Will probably get optimized out by the compiler */
	if (entryno == number_of_locks)
		goto end;

	if (entryno < number_of_locks) {
		memcpy(&current_locks[entryno], &current_locks[number_of_locks],
			sizeof(struct rqbalance_ctl_locks));
		return;
	}

end:
	/* Run optimization parameter deactivation */
	locktype_action(entryno, 0);
	/*
	 * Remove only the ID. That's the only sensible information
	 * in there (may contain a possible hook to a kernel driver)
	 */
	current_locks[entryno].luid = 0;
	number_of_locks--;
	return;
}

static ssize_t sysfs_read(const char *path, char *s, int num_bytes)
{
    char buf[80];
    ssize_t count;
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error reading from %s: %s\n", path, buf);
        return -1;
    }
    if ((count = read(fd, s, (num_bytes - 1))) < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error reading from  %s: %s\n", path, buf);
    } else {
        if ((count >= 1) && (s[count-1] == '\n')) {
            s[count-1] = '\0';
        } else {
            s[count] = '\0';
        }
    }
    close(fd);
    ALOGV("read '%s' from %s", s, path);
    return count;
}

/*
 * get_possible_cores - Read configured cores from sysfs
 *
 * \return Returns number of cores or failure (negative errno)
 */
static int get_possible_cores(void)
{
	char buf[10];
	int mincore, maxcore;

	if (sysfs_read(SYS_CPUPOSS_PATH, buf, 10) < 0) {
		ALOGE("Cannot get number of cores in the system!!!");
		return -ENXIO;
	}

	if (sscanf(buf, "%d-%d", &mincore, &maxcore) < 2) {
		ALOGE("Error while scanning cores!!");
		return -ENXIO;
	}

	return maxcore;
}

/*
 * PerfLockAcquire -   Acquires a performance lock in rqbalance driver.
 *                     The rqbalance driver will then set performance mode
 *                     depending on predefined or special parameters.
 *
 * \return Returns success (0) or negative errno.
 */
int RQBalanceHALExt::PerfLockAcquire(struct rqbalance_halext_params *params)
{
	int arraysz, id, lock_type, lock_state, i, ret = -EINVAL;

	if (possible_cores < 0)
		possible_cores = get_possible_cores();

	arraysz = params->arraysz;
	id = params->id;

	if (!id) {
		if (arraysz > 1) {
			ALOGE("Unexpected argument. Bailing out.");
			ALOGD("Arguments: %d", arraysz);
			for (i = 0; i <=arraysz; i++)
				ALOGD("Arg[%d]: 0x%x", i, params->argument[i]);
			return -EINVAL;
		}

		lock_type = (params->argument[0] & 0xff00) >> 8;
		lock_state = (params->argument[0] & 0xff);

		return new_lock_init((unsigned int)params->time,
					(unsigned short)lock_type, lock_state);
	}

	for (i = 0; i <= arraysz; i++) {
		ret = lock_set_arg(params->argument[i]);
		if (ret < 0)
			return ret;
	}

	return ret;
}

int RQBalanceHALExt::PerfLockRelease(int id)
{
	int i;

	/*
	 * Many times, it happens that a driver
	 * requests a lock for a very small amount
	 * of time, hence search in reverse order.
	 */
	for (i = number_of_locks; i >=0; i--) {
		if (i < 0) {
			ALOGD("WTF: FIXME FIXME FIXME FIXME FIXME FIXME");
			ALOGD("WTF: Tried to remove an unexistant lock.");
			ALOGD("WTF: FIXME FIXME FIXME FIXME FIXME FIXME");
			return -ENXIO;
		} else if ((unsigned int)id == current_locks[i].luid) {
			remove_and_reorder(i);
			break;
		}
	}

	return 0;
}

