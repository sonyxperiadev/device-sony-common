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

#include "powermanagerhook_compat.h"

RQBalanceCTL::RQBalanceCTL()
{
}

int RQBalanceCTL::sendPowerHint(int hint, int data)
{
    Mutex::Autolock _l(mlock);
    if (mPowerManager == NULL) {
        const String16 serviceName("power");
        sp<IBinder> bs = defaultServiceManager()->checkService(serviceName);
        if (bs == NULL) {
            return NAME_NOT_FOUND;
        }
        mPowerManager = interface_cast<IPowerManager>(bs);
    }
    status_t status = mPowerManager->powerHint(hint, data);
    if (status == DEAD_OBJECT) {
        mPowerManager = NULL;
    }

    return static_cast<int>(status);
}

/* Access functions */
static RQBalanceCTL *RQBInstance = NULL;

EXPORT_C int RQBalanceCTL_sendPowerHint(int hint, int data)
{
    if (RQBInstance == NULL)
        RQBInstance = new RQBalanceCTL;

    return RQBInstance->sendPowerHint(hint, data);
}
