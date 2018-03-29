/*
 * Copyright (C) 2016 The Android Open Source Project
 * Copyright (C) 2018 Shane Francis
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
#ifndef ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
#define ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H

#include <android/hardware/light/2.0/ILight.h>
#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>
#include <map>

#include <cutils/properties.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#include <linux/msm_mdp.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <hardware/lights.h>

#define DEFAULT_LOW_PERSISTENCE_MODE_BRIGHTNESS 255

#define LP_MODE_BRIGHTNESS_PROPERTY "sys.display.low_persistence_mode_brightness"

struct lights_t {
    pthread_mutex_t g_lock;
    pthread_mutex_t g_lcd_lock;
    int g_last_backlight_mode;
    int backlight_bits;
};

static constexpr int RAMP_SIZE = 8;
static constexpr int BRIGHTNESS_RAMP[RAMP_SIZE] = {0, 14, 28, 42, 56, 70, 84, 100};

const static std::string RED_LED_FILE
        = "/sys/class/leds/led:rgb_red/brightness";

const static std::string GREEN_LED_FILE
        = "/sys/class/leds/led:rgb_green/brightness";

const static std::string BLUE_LED_FILE
        = "/sys/class/leds/led:rgb_blue/brightness";

const static std::string RED_LED_DUTY_PCTS_FILE
        = "/sys/class/leds/led:rgb_red/duty_pcts";

const static std::string GREEN_LED_DUTY_PCTS_FILE
        = "/sys/class/leds/led:rgb_green/duty_pcts";

const static std::string BLUE_LED_DUTY_PCTS_FILE
        = "/sys/class/leds/led:rgb_blue/duty_pcts";

const static std::string RED_BLINK_FILE
        = "/sys/class/leds/led:rgb_red/blink";

const static std::string GREEN_BLINK_FILE
        = "/sys/class/leds/led:rgb_green/blink";

const static std::string BLUE_BLINK_FILE
        = "/sys/class/leds/led:rgb_blue/blink";

const static std::string RGB_BLINK_FILE
        = "/sys/class/leds/rgb/rgb_blink";

const static std::string LCD_FILE
        = "/sys/class/leds/lcd-backlight/brightness";

const static std::string LCD_MAX_FILE
        = "/sys/class/leds/lcd-backlight/max_brightness";

const static std::string PERSISTENCE_FILE
        = "/sys/class/graphics/fb0/msm_fb_persist_mode";

namespace android {
    namespace hardware {
        namespace light {
            namespace V2_0 {
                namespace implementation {
                    using::android::hardware::light::V2_0::ILight;
                    using::android::hardware::light::V2_0::LightState;
                    using::android::hardware::light::V2_0::Status;
                    using::android::hardware::light::V2_0::Type;
                    using::android::hardware::Return;
                    using::android::hardware::Void;
                    using::android::hardware::hidl_vec;
                    using::android::hardware::hidl_string;
                    using::android::sp;

                    struct Light : public ILight {
                        public:
                        Light();

                        static ILight *getInstance();

                        // Methods from ::android::hardware::light::V2_0::ILight follow.
                        Return <Status> setLight(Type type, const LightState &state) override;
                        Return<void> getSupportedTypes(getSupportedTypes_cb _hidl_cb) override;

                        private:
                        static Light *sInstance;
                        lights_t *mDevice;
                        LightState batteryState;
                        LightState notificationState;
                        int setLightBacklight(const LightState &state);
                        int setLightBattery(const LightState &state);
                        int setLightNotifications(const LightState &state);
                        void handleSpeakerBatteryLocked();
                        int setSpeakerLightLocked(const LightState &state);
                        std::string getScaledDutyPcts(int brightness);
                        int isLit(const LightState &state);
                        bool isRgbSyncAvailable();
                        int rgbToBrightness(const LightState &state);
                        static int writeInt(const std::string &path, int value);
                        static int readInt(const std::string &path);
                        static int writeStr(const std::string &path, const std::string &value);
                        void openHal();
                    };
                }  // namespace implementation
            }  // namespace V2_0
        }  // namespace light
    }  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
