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

#include <cutils/properties.h>

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
#define PROP_DEBUGLVL			"powerhal.debug_level"

/* PowerServer definitions */
#define POWERSERVER_DIR			"/dev/socket/powerhal/"
#define POWERSERVER_SOCKET		POWERSERVER_DIR "rqbsvr"
#define POWERSERVER_MAXCONN		10

/* Others */
#define RQBHAL_CONF_FILE		"/system/etc/rqbalance_config.xml"

/*
 * enum rqb_pwr_mode_t
 * Provides RQBalance Power Modes definitions
 *
 * The POWER_MODE_MAX entry is used as commodity for code
 * and shall NEVER be used as a Power Mode.
 */
typedef enum {
	POWER_MODE_BATTERYSAVE,
	POWER_MODE_BALANCED,
	POWER_MODE_PERFORMANCE,
	POWER_MODE_OMXDECODE,
	POWER_MODE_OMXENCODE,
	/* Do not use this entry */
	POWER_MODE_MAX,
} rqb_pwr_mode_t;

/*
 * struct rqbalance_params
 * Main RQBalance-PowerHAL structure
 *
 * Contains the set of values for one full power mode configuration.
 * This struct HAS to contain ALL the parameters supported by the
 * RQBalance kernel driver, as per coding style spec.
 */
struct rqbalance_params {
	char min_cpus[2];
	char max_cpus[2];
	char up_thresholds[PROPERTY_VALUE_MAX];
	char down_thresholds[PROPERTY_VALUE_MAX];
	char balance_level[PROPERTY_VALUE_MAX];
};

/* Exported functions */
/* Note: These functions are meant to be used only INSIDE this source. */
void __set_special_power_mode(char* max_cpus, char* min_cpus,
                              char* up_thresholds, char* down_thresholds,
                              char* balance_level);
void set_power_mode(rqb_pwr_mode_t mode);
