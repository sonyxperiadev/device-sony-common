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

/* Exported functions */
void __set_power_mode(struct rqbalance_params *rqparm);
void set_power_mode(rqb_pwr_mode_t mode);
