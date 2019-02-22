/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * *    * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __POWER_HELPER_H__
#define __POWER_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

enum legacy_stats_source {
    RPM_VOTER_APSS = 0,
    RPM_VOTER_MPSS,
    RPM_VOTER_ADSP,
    RPM_VOTER_SLPI,
    RPM_MAX_VOTER_STATS,

    //WLAN Stats
    WLAN_POWER_DEBUG_STATS = 0,
    MAX_WLAN_STATS,
};


// These values are used as indices in getSubsystemLowPowerStats(), as source IDs
// in stats_section instances, and (in the case of the _COUNT values) to dimension
// containers.  The values used as indices need to be contiguous, but others do
// not (which is why SYSTEM_STATES is all the way at the end; it is not used as
// an index, but only as a source ID).
enum stats_source {
    // Master stats
    MASTER_APSS = 0,
    MASTER_MPSS,
    MASTER_ADSP,
    MASTER_SLPI,
    MASTER_CDSP,  // Not technically used, but included for precautionary stats tracking
    // The following masters are supported by the RPMh stats driver, but not
    // in use on our devices.
    // MASTER_GPU,
    // MASTER_DISPLAY,
    MASTER_COUNT, // Total master sources

    // Subsystem stats.  (Numbering starts at MASTER_COUNT to preserve
    // contiguous source numbering.)
    SUBSYSTEM_WLAN = MASTER_COUNT,

    // Don't add any lines after this line
    STATS_SOURCE_COUNT, // Total sources of any kind excluding system states
    SUBSYSTEM_COUNT = STATS_SOURCE_COUNT - MASTER_COUNT,

    SYSTEM_STATES
};

enum master_sleep_states {
    MASTER_SLEEP = 0,

    // Don't add any lines after this line
    MASTER_SLEEP_STATE_COUNT
};

enum legacy_master_stats {
    SLEEP_XO_DURATION = 0,
    SLEEP_XO_COUNT,

    // Don't add any lines after this line
    RPM_MASTER_STATS_COUNT
};

enum master_stats {
    SLEEP_CUMULATIVE_DURATION_MS = 0,
    SLEEP_ENTER_COUNT,
    SLEEP_LAST_ENTER_TSTAMP_MS,

    // Don't add any lines after this line
    MASTER_STATS_COUNT
};

enum wlan_sleep_states {
    WLAN_STATE_ACTIVE = 0,
    WLAN_STATE_DEEP_SLEEP,

    // Don't add any lines after this line
    WLAN_SLEEP_STATE_COUNT
};

// Note that stats for both WLAN sleep states are in a single section of the
// source file, so there's only 1 stats section despite having 2 states
enum wlan_stats {
    CUMULATIVE_SLEEP_TIME_MS = 0,
    CUMULATIVE_TOTAL_ON_TIME_MS,
    DEEP_SLEEP_ENTER_COUNTER,
    LAST_DEEP_SLEEP_ENTER_TSTAMP_MS,

    // Don't add any lines after this line
    WLAN_STATS_COUNT
};

enum system_sleep_states {
    SYSTEM_STATE_AOSD = 0,
    SYSTEM_STATE_CXSD,

    //Don't add any lines after this line
    SYSTEM_SLEEP_STATE_COUNT
};

enum legacy_system_sleep_states {
    SYSTEM_STATE_VLOW = 0,
    SYSTEM_STATE_VMIN,

    //Don't add any lines after this line
    RPM_SYSTEM_SLEEP_STATE_COUNT
};

enum system_state_stats {
    TOTAL_COUNT = 0,
    ACCUMULATED_TIME_MS,

    //Don't add any lines after this line
    SYSTEM_STATE_STATS_COUNT
};

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))
#endif

struct stats_section {
    int source;
    const char *label;
    const char **stats_labels;
    size_t num_stats;
};

int extract_rpm_master_stats(uint64_t *list, size_t list_length);
int extract_rpmh_master_stats(uint64_t *list, size_t list_length);
int extract_wlan_stats(uint64_t *list, size_t list_length);
int extract_rpm_system_stats(uint64_t *list, size_t list_length);
int extract_rpmh_system_stats(uint64_t *list, size_t list_length);
bool power_helper_is_legacy(void);

#ifdef __cplusplus
}
#endif

#endif //__POWER_HELPER_H__
