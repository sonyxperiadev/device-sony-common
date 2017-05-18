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

#define PROP_VALUE_MAX 70

/* sysfs nodes */
#define CPUQUIET_NODE "/sys/devices/system/cpu/cpuquiet/"
#define RQBALANCE_NODE CPUQUIET_NODE "rqbalance/"

/* sysfs files */
#define SYS_MIN_CPUS		CPUQUIET_NODE  "nr_min_cpus"
#define SYS_MAX_CPUS		CPUQUIET_NODE  "nr_power_max_cpus"
#define SYS_THERM_CPUS		CPUQUIET_NODE  "nr_thermal_max_cpus"
#define SYS_BALANCE_LVL		RQBALANCE_NODE "balance_level"
#define SYS_UPCORE_THRESH	RQBALANCE_NODE "nr_run_thresholds"
#define SYS_DNCORE_THRESH	RQBALANCE_NODE "nr_down_run_thresholds"

/* Android properties */
#define PROP_MIN_CPUS			"cpuquiet.%s.min_cpus"
#define PROP_MAX_CPUS			"cpuquiet.%s.max_cpus"
#define PROP_BALANCE_LVL		"rqbalance.%s.balance_level"
#define PROP_UPCORE_THRESH		"rqbalance.%s.up_threshold"
#define PROP_DNCORE_THRESH		"rqbalance.%s.down_threshold"

#define PROP_EXTLIB			"ro.powerhal.extension_library"
#define PROP_DEBUGLVL			"powerhal.debug_level"

#define MODE_STR_BATTERYSAVE		"low"
#define MODE_STR_BALANCED		"normal"
#define MODE_STR_PERFORMANCE		"perf"

#define POWERSERVER_DIR			"/data/misc/powerhal/"
#define POWERSERVER_SOCKET		POWERSERVER_DIR "rqbsvr"
#define POWERSERVER_MAXCONN		10

#define RQBHAL_CONF_FILE		"/system/etc/rqbalance_config.xml"

typedef int (*lock_acq_t)(int, int, int*, int);
typedef int (*lock_rel_t)(int);

typedef enum {
	POWER_MODE_BATTERYSAVE,
	POWER_MODE_BALANCED,
	POWER_MODE_PERFORMANCE,
	/* Do not use this entry */
	POWER_MODE_MAX,
} rqb_pwr_mode_t;

struct rqbalance_params {
	char min_cpus[2];
	char max_cpus[2];
	char up_thresholds[PROP_VALUE_MAX];
	char down_thresholds[PROP_VALUE_MAX];
	char balance_level[PROP_VALUE_MAX];
};

/*
 * enum extended_hint_t
 * Provides extended power hints
 *
 * POWER_HINT_EXT_LOCK_ACQUIRE
 *   Acquires a performance lock in rqbalance driver.
 *   Calls the perf_lock_acquire function with the specified parameters.
 *   It accepts a struct rqbalance_ctl_locks parameter.
 *
 *       NO
 *   It accepts a string parameter with the following format:
 *   "id:%d time:%d locktype:%d lockstate:%d"
 *
 * POWER_HINT_EXT_LOCK_SET_PARAM
 *   Sets specific custom configuration parameters for the performance lock.
 *   Calls the lock_set_arg function with the specified parameters.
 *   It accepts a string parameter with the following format:
 *   "arg:%d"
 *
 * POWER_HINT_EXT_LOCK_RELEASE
 *   Releases a performance lock in rqbalance driver.
 *   Calls the perf_lock_release function with the lock ID parameter.
 *   It accepts an integer parameter containing the lock ID.
 */
typedef enum {
	POWER_HINT_EXT_LOCK_ACQUIRE	= 0x00007FE0,
	POWER_HINT_EXT_LOCK_SET_PARAM	= 0x00007FE1,
	POWER_HINT_EXT_LOCK_RELEASE	= 0x00007FE2,
	POWER_HINT_EXT_CLIENT_CONN	= 0x00007FE3,
} extended_hint_t;

/* Exported functions */
void __set_special_power_mode(char* max_cpus, char* min_cpus,
                              char* up_thresholds, char* down_thresholds,
                              char* balance_level);
void set_power_mode(rqb_pwr_mode_t mode);
