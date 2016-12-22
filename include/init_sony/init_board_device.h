/*
 * Copyright (C) 2016 The CyanogenMod Project
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

#ifndef __INIT_BOARD_DEVICE_H__
#define __INIT_BOARD_DEVICE_H__

#include "init_board_common.h"
#include "init_prototypes.h"

#define LED_RED_PATH "/sys/class/leds/led:rgb_red/brightness"
#define LED_GREEN_PATH "/sys/class/leds/led:rgb_green/brightness"
#define LED_BLUE_PATH "/sys/class/leds/led:rgb_blue/brightness"
#define VIBRATOR_PATH "/sys/class/timed_output/vibrator/enable"

// Class: init_board_device
class init_board_device : public init_board_common
{
public:
    // Board: Introduction for Keycheck
    virtual void introduce_keycheck()
    {
        // Short vibration
        vibrate(75);

        // LED purple
        led_color(255, 0, 255);
    }

    // Board: finalization of keycheck
    virtual void finish_keycheck(bool recoveryBoot)
    {
        // Short vibration
        if (recoveryBoot)
        {
            vibrate(75);
            msleep(75);
        }
    }

    // Board: Introduction for Android
    virtual void introduce_android()
    {
        // LED off
        led_color(0, 0, 0);
    }

    // Board: Introduction for Recovery
    virtual void introduce_recovery()
    {
        // LED orange
        led_color(255, 100, 0);
    }

    // Board: Finish init execution
    virtual void finish_init()
    {
        // Power off vibrator
        vibrate(0);
    }

    // Board: Set LED colors
    void led_color(uint8_t red, uint8_t green, uint8_t blue)
    {
        write_int(LED_RED_PATH, red);
        write_int(LED_GREEN_PATH, green);
        write_int(LED_BLUE_PATH, blue);
    }

    // Board: Set hardware vibrator
    void vibrate(uint8_t strength)
    {
        write_int(VIBRATOR_PATH, strength);
    }
};

#endif //__INIT_BOARD_DEVICE_H__
