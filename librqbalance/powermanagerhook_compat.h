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

#ifdef __cplusplus // only actually define the class if this is C++

#include <stdint.h>
#include <sys/types.h>

#include <utils/Mutex.h>

#include <binder/IServiceManager.h>
#include <powermanager/IPowerManager.h>
#include <powermanager/PowerManager.h>


using namespace android;

class RQBalanceCTL
{
    public:
	RQBalanceCTL();
        int sendPowerHint(int, int);

    private:
        sp<IPowerManager> mPowerManager;
        Mutex mlock;
};

#else

/* Hack it in C as a struct */
typedef struct void* RQBalanceCTL; 

#endif

/* Access functions */
#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif

EXPORT_C int RQBalanceCTL_sendPowerHint(int, int);
