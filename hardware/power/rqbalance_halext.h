/*
 * Copyright (C) 2017 AngeloGioacchino Del Regno <kholk11@gmail.com>
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

#define SYS_CPUPOSS_PATH	"/sys/devices/system/cpu/possible"

/* HalExt definitions */
#define MAX_PERMITTED_LOCKS	10	/* Maximum number of locks */
#define MAX_ARGUMENTS		20	/* Maximum number of params */

#define MSEC_TO_SEC(x)		x/1000
#define MSEC_TO_NSEC(x)		x*1000000

struct rqbalance_ctl_locks {
	unsigned int luid;	/* Lock unique identifier */
	unsigned int time;	/* Time to hold the lock */
	unsigned short drid;	/* Driver/HAL identifier */
	timer_t tid;		/* ID of the POSIX timer */
	int state;		/* State: enable/disable */
};

struct rqbalance_halext_params {
	int8_t acquire;
	int32_t id;
	int32_t time;
	int32_t argument[MAX_ARGUMENTS];
	int32_t arraysz;
};

typedef enum {
	ALL_CPUS_PWR_CLPS_DIS		= 0x101,
	MINCORES			= 0x700,	/* 0x7XX XX=NCores */
	CPU0_MIN_FREQ_TURBO_MAX		= 0x2FE,
	ALL_CORES_ONLINE		= 0x7FE,
        CPU4_MIN_FREQ_TURBO_MAX		= 0x1FFE,
} LOCKPARAM;

typedef enum {
	OMX_DECODER	= 0x44,
	DISPLAY_LAYER	= 0x45,
	OMX_ENCODER	= 0x46,
	RQB_POWERHAL	= 0x88,
} LOCKTYPE;

typedef enum {
	STATE_DISABLE = 0,
	STATE_ENABLE,
} LOCKSTATE;

/* Exported functions */
int halext_perf_lock_acquire(struct rqbalance_halext_params *params);
int halext_perf_lock_release(int id);
