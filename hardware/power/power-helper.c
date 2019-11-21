/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
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

#define LOG_NIDEBUG 0
#define LOG_TAG "RQBalance-PowerHAL-PowerHelper"

#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdlib.h>

#include <log/log.h>

#include "power-helper.h"

#ifndef RPM_SYSTEM_STAT
#define RPM_SYSTEM_STAT "/d/system_stats"
#endif

#ifndef RPMH_MASTER_STATS_FILE
#define RPMH_MASTER_STATS_FILE "/sys/power/rpmh_stats/master_stats"
#endif

#ifndef SYSTEM_STATS_FILE
#define SYSTEM_STATS_FILE "/sys/power/system_sleep/stats"
#endif

#ifndef WLAN_STATS_FILE
#define WLAN_STATS_FILE "/sys/kernel/wlan/power_stats"
#endif

#define LINE_SIZE 128

/* RPM */
const char *rpm_master_stat_params[RPM_MASTER_STATS_COUNT] = {
    "Accumulated XO duration",
    "XO Count",
};
struct stats_section rpm_master_sections[] = {
    { RPM_VOTER_APSS, "APSS",   rpm_master_stat_params, ARRAY_SIZE(rpm_master_stat_params) },
    { RPM_VOTER_MPSS, "MPSS",   rpm_master_stat_params, ARRAY_SIZE(rpm_master_stat_params) },
    { RPM_VOTER_ADSP, "ADSP",   rpm_master_stat_params, ARRAY_SIZE(rpm_master_stat_params) },
    { RPM_VOTER_SLPI, "SLPI",   rpm_master_stat_params, ARRAY_SIZE(rpm_master_stat_params) },
};


/* RPMh */
const char *master_stats_labels[MASTER_STATS_COUNT] = {
    "Sleep Accumulated Duration",
    "Sleep Count",
    "Sleep Last Entered At",
};

struct stats_section master_sections[MASTER_COUNT] = {
    { MASTER_APSS,       "APSS", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
    { MASTER_MPSS,       "MPSS", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
    { MASTER_ADSP,       "ADSP", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
    { MASTER_SLPI,       "SLPI", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
    { MASTER_CDSP,       "CDSP", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
    // The following masters are currently unused
    //{ MASTER_GPU,         "GPU", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
    //{ MASTER_DISPLAY, "DISPLAY", master_stats_labels, ARRAY_SIZE(master_stats_labels) },
};

const char *wlan_stats_labels[WLAN_STATS_COUNT] = {
    "cumulative_sleep_time_ms",
    "cumulative_total_on_time_ms",
    "deep_sleep_enter_counter",
    "last_deep_sleep_enter_tstamp_ms"
};

struct stats_section wlan_sections[] = {
    { SUBSYSTEM_WLAN, "POWER DEBUG STATS", wlan_stats_labels, ARRAY_SIZE(wlan_stats_labels) },
};

/* Common */
const char *system_stats_labels[SYSTEM_STATE_STATS_COUNT] = {
    "count",
    "actual last sleep(msec)"
};

struct stats_section rpm_system_sections[] = {
    { SYSTEM_STATES, "RPM Mode:vlow", system_stats_labels, ARRAY_SIZE(system_stats_labels) },
    { SYSTEM_STATES, "RPM Mode:vmin", system_stats_labels, ARRAY_SIZE(system_stats_labels) },
};

struct stats_section rpmh_system_sections[] = {
    { SYSTEM_STATES, "RPM Mode:aosd", system_stats_labels, ARRAY_SIZE(system_stats_labels) },
    { SYSTEM_STATES, "RPM Mode:cxsd", system_stats_labels, ARRAY_SIZE(system_stats_labels) },
};

static int parse_stats(const char **stat_labels, size_t num_stats,
        uint64_t *list, FILE *fp) {
    ssize_t nread;
    size_t len = LINE_SIZE;
    char *line;
    size_t stats_read = 0;
    size_t i;

    line = malloc(len);
    if (!line) {
        ALOGE("%s: no memory to hold line", __func__);
        return -ENOMEM;
    }

    while ((stats_read < num_stats) &&
        (nread = getline(&line, &len, fp) > 0)) {
        char *key = line + strspn(line, " \t");
        char *value = strchr(key, ':');
        if (!value || (value > (line + len)))
            continue;
        *value++ = '\0';

        for (i = 0; i < num_stats; i++) {
            if (!strncmp(key, stat_labels[i], strlen(stat_labels[i]))) {
                list[i] = strtoull(value, NULL, 0);
                stats_read++;
                break;
            }
        }
    }

    free(line);

    return stats_read;
}

static int extract_stats(uint64_t *stats_list, size_t entries_per_section, char *file,
        struct stats_section *sections, size_t num_sections) {
    FILE *fp;
    ssize_t read;
    size_t len = LINE_SIZE;
    char *line;
    size_t i;
    size_t sections_read = 0;
    size_t stats_read = 0;
    int ret = 0;

    fp = fopen(file, "re");
    if (fp == NULL) {
        ALOGE("%s: failed to open: %s Error = %s", __func__, file, strerror(errno));
        return -errno;
    }

    line = malloc(len);
    if (!line) {
        ALOGE("%s: no memory to hold line", __func__);
        fclose(fp);
        return -ENOMEM;
    }

    // Ensure that any missing stats default to 0
    for (i = 0; i < (entries_per_section * num_sections); i++) {
        stats_list[i] = 0L;
    }

    // Iterate over the sections we expect to find in the file, calling parse_stats()
    // to process each section as we detect section headers
    while ((sections_read < num_sections) && (read = getline(&line, &len, fp) != -1)) {
        size_t begin = strspn(line, " \t");

        for (i = 0; i < num_sections; i++) {
            if (!strncmp(line + begin, sections[i].label, strlen(sections[i].label))) {
                sections_read++;
                break;
            }
        }

        if (i == num_sections) {
            continue;
        }

        stats_read = parse_stats(sections[i].stats_labels, sections[i].num_stats,
                &stats_list[i * entries_per_section], fp);
        // If we don't find all of the stats we expect in this section, our understanding of
        // the input is wrong, and we can't just carry on as if everything is okay.  Better
        // to log the error and give up than potentially return incorrect data as stats.
        if (stats_read != sections[i].num_stats) {
            ALOGE("%s: failed to read all stats for %s section (%zu of %zu)", __func__,
                    sections[i].label, stats_read, sections[i].num_stats);
            break;
        }
    }

    free(line);
    fclose(fp);

    return ret;
}

int extract_rpm_master_stats(uint64_t *list, size_t list_length)
{
    size_t entries_per_section = list_length / ARRAY_SIZE(rpm_master_sections);
    if (list_length % entries_per_section != 0) {
        ALOGW("%s: stats list size not an even multiple of section count", __func__);
    }

    return extract_stats(list, entries_per_section, RPM_SYSTEM_STAT,
            rpm_master_sections, ARRAY_SIZE(rpm_master_sections));
}

int extract_rpmh_master_stats(uint64_t *list, size_t list_length)
{
    size_t entries_per_section = list_length / ARRAY_SIZE(master_sections);
    if (list_length % entries_per_section != 0) {
        ALOGW("%s: stats list size not an even multiple of section count", __func__);
    }

    return extract_stats(list, entries_per_section, RPMH_MASTER_STATS_FILE,
            master_sections, ARRAY_SIZE(master_sections));
}

int extract_wlan_stats(uint64_t *list, size_t list_length)
{
    size_t entries_per_section = list_length / ARRAY_SIZE(wlan_sections);
    if (list_length % entries_per_section != 0) {
        ALOGW("%s: stats list size not an even multiple of section count", __func__);
    }

    return extract_stats(list, entries_per_section, WLAN_STATS_FILE,
            wlan_sections, ARRAY_SIZE(wlan_sections));
}

int extract_rpm_system_stats(uint64_t *list, size_t list_length)
{
    size_t entries_per_section = list_length / ARRAY_SIZE(rpm_system_sections);
    if (list_length % entries_per_section != 0) {
        ALOGW("%s: stats list size not an even multiple of section count", __func__);
    }

    return extract_stats(list, entries_per_section, SYSTEM_STATS_FILE,
            rpm_system_sections, ARRAY_SIZE(rpm_system_sections));
}

int extract_rpmh_system_stats(uint64_t *list, size_t list_length)
{
    size_t entries_per_section = list_length / ARRAY_SIZE(rpmh_system_sections);
    if (list_length % entries_per_section != 0) {
        ALOGW("%s: stats list size not an even multiple of section count", __func__);
    }

    return extract_stats(list, entries_per_section, SYSTEM_STATS_FILE,
            rpmh_system_sections, ARRAY_SIZE(rpmh_system_sections));
}

bool power_helper_is_legacy(void)
{
    FILE *fp;

    /*
     * If the RPMh master stats file exists, we are indeed using a
     * SoC with the RPMh. Otherwise, we're on legacy platforms.
     */
    fp = fopen(RPMH_MASTER_STATS_FILE, "re");
    if (fp != NULL) {
        fclose(fp);
        return false;
    }

    return true;
}

