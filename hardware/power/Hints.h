/*
 * Copyright (C) 2016-2019 AngeloGioacchino Del Regno <kholk11@gmail.com>
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

#ifndef __RQBALANCE_POWERHAL_HINTS_H__
#define __RQBALANCE_POWERHAL_HINTS_H__

#include <atomic>
#include <thread>
#include <android/hardware/power/1.3/IPower.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include "common.h"

struct RQBalanceHALExt;

bool sysfs_write(const char *path, const char *s);

struct RQBalanceHintsHandler {
    RQBalanceHintsHandler();
    ~RQBalanceHintsHandler();

    int InitializeHAL(void);
    int ManagePowerServer(bool is_starting);
    int ManagePowerServerSafe(bool is_starting);
    void SetPowerMode(rqb_pwr_mode_t mode);

    int PerfLockAcquire(struct rqbalance_halext_params *extparams);
    int PerfLockRelease(uint32_t id);

 private:
    std::unique_ptr<RQBalanceHALExt> mHALExt;
    int manage_powerserver(bool start);

    void __set_special_power_mode(char* max_cpus, char* min_cpus,
                              char* up_thresholds, char* down_thresholds,
                              char* balance_level);
    void print_params(rqb_pwr_mode_t pwrmode, int dbg_lvl);

    static bool init_all_rqb_params(void);

    rqb_pwr_mode_t cur_pwrmode;
};

#endif /* __RQBALANCE_POWERHAL_HINTS_H__ */
