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
#include <hidl/Status.h>

#include <algorithm>
#include <mutex>
#include <vector>

#include <cutils/properties.h>

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <linux/msm_mdp.h>

#define DEFAULT_LOW_PERSISTENCE_MODE_BRIGHTNESS 255

#define LP_MODE_BRIGHTNESS_PROPERTY "sys.display.low_persistence_mode_brightness"

static constexpr int RAMP_SIZE = 8;
static constexpr int BRIGHTNESS_RAMP[RAMP_SIZE] = { 0, 14, 28, 42, 56, 70, 84, 100 };

const static std::string LEDS_CLASS_BASE
    = "/sys/class/leds/";

static const std::string RED_LED_BASE
    = LEDS_CLASS_BASE + "red/";

static const std::string GREEN_LED_BASE
    = LEDS_CLASS_BASE + "green/";

static const std::string BLUE_LED_BASE
    = LEDS_CLASS_BASE + "blue/";

static const std::string RED_LED_FILE
    = RED_LED_BASE + "brightness";

static const std::string GREEN_LED_FILE
    = GREEN_LED_BASE + "brightness";

static const std::string BLUE_LED_FILE
    = BLUE_LED_BASE + "brightness";

const static std::string RED_LED_BREATH_FILE
    = RED_LED_BASE + "breath";

const static std::string GREEN_LED_BREATH_FILE
    = GREEN_LED_BASE + "breath";

const static std::string BLUE_LED_BREATH_FILE
    = BLUE_LED_BASE + "breath";

static const std::string RED_BLINK_FILE
    = RED_LED_BASE + "blink";

static const std::string GREEN_BLINK_FILE
    = GREEN_LED_BASE + "blink";

static const std::string BLUE_BLINK_FILE
    = BLUE_LED_BASE + "blink";

#ifdef DRMSDE_BACKLIGHT
static const std::string LCD_FILE
    = "/sys/class/backlight/panel0-backlight/brightness";

static const std::string LCD_MAX_FILE
    = "/sys/class/backlight/panel0-backlight/max_brightness";
#else
static const std::string LCD_FILE
    = "/sys/class/leds/lcd-backlight/brightness";

static const std::string LCD_MAX_FILE
    = "/sys/class/leds/lcd-backlight/max_brightness";
#endif

static const std::string PERSISTENCE_FILE
    = "/sys/class/graphics/fb0/msm_fb_persist_mode";

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {
using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::light::V2_0::ILight;
using ::android::hardware::light::V2_0::LightState;
using ::android::hardware::light::V2_0::Status;
using ::android::hardware::light::V2_0::Type;

struct Light : public ILight {
public:
    Light();

    // Methods from ::android::hardware::light::V2_0::ILight follow.
    Return<Status> setLight(Type type, const LightState &state) override;
    Return<void> getSupportedTypes(getSupportedTypes_cb _hidl_cb) override;

private:
    std::mutex mLock;
    std::mutex mLcdLock;
    int mBacklightMax = 0;
    int mBacklightShift = 0;

    Brightness mLastBacklightMode;
    LightState batteryState;
    LightState notificationState;

    int setLightBacklight(const LightState &state);
    int setLightBattery(const LightState &state);
    int setLightNotifications(const LightState &state);
    void handleSpeakerBatteryLocked();
    int setSpeakerLightLocked(const LightState &state);

    static std::string getScaledDutyPcts(int brightness);
    static int isLit(const LightState &state);
    static bool isRgbSyncAvailable();
    static int rgbToBrightness(const LightState &state);
    static int writeInt(const std::string &path, int value);
    static int readInt(const std::string &path);
    static int writeStr(const std::string &path, const std::string &value);
};
} // namespace implementation
} // namespace V2_0
} // namespace light
} // namespace hardware
} // namespace android

#endif // ANDROID_HARDWARE_LIGHT_V2_0_LIGHT_H
