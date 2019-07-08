# Custom vendor health HAL

This repo is an implementation of the [android.hardware.health][health-ref] HAL,
based on the [official guide][guide] and existing device-specific
implementations.

It was written for Sony devices from 2016 onward, but it should work on other
devices with a few small modifications as well.
Most notably, the sysfs paths might be different and the persist partition might
be mounted at `/persist` instead of `/mnt/vendor/persist`.

## License
Apache License version 2, see [LICENSE.txt](LICENSE.txt).

## Implementing

To start using this HAL, you need to do the following:

Add the HAL to the [vendor device manifest][vintf-manifest].  
For SODP, that file is in [device-sony-common/vintf/manifest.xml][sodp-manifest].
```
    <hal format="hidl">
        <name>android.hardware.health</name>
        <transport>hwbinder</transport>
        <version>2.0</version>
        <interface>
            <name>IHealth</name>
            <instance>default</instance>
        </interface>
    </hal>
```

Add android.hardware.health@2.0-service-.sony to `PRODUCT_PACKAGES` and remove
the obsolete backup healthd instance that is shipped by default.  
For SODP, that is done in `common-treble.mk`:
```
# Health (in addition to own board libhealth)
PRODUCT_PACKAGES += \
    android.hardware.health@2.0-service.sony
# Remove deprecated backup healthd
DEVICE_FRAMEWORK_MANIFEST_FILE += \
    system/libhidl/vintfdata/manifest_healthd_exclude.xml
```

Add the binary to sepolicy `file_contexts`:
```
/(system/vendor|vendor)/bin/hw/android\.hardware\.health@2\.0-service\.sony  u:object_r:hal_health_default_exec:s0
```

And allow vendor_init to create the /mnt/vendor/persist/battery/ folder.  
```
# vendor_init.te:
allow vendor_init persist_battery_file:dir create_dir_perms;
```

# Internals for developers

Many features of this HAL were taken from the Pixel family of devices.
[Marlin][marlin](Pixel 1), [Wahoo][wahoo](Pixel 2) and
[Crosshatch][crosshatch](Pixel 3) all have more or less the same implementation.
We don't need to do quite as much logging as they do, so Pixelstats-related
functions have been stripped out.

Another difference are the battery cycles: The pixel battery driver is able to
report cycles for 8 individual "bins" for what are presumably groups/blocks of
battery cells instead of one single cycle count. If your device supports those
"bins" as well, take a look at the pixel `CycleCountBackupRestore`
implementation.

Relevant files and folders:

- system/core/healthd/
- hardware/interfaces/health/2.0/
- frameworks/native/services/batteryservice/include/batteryservice/BatteryService.h
- frameworks/native/services/sensorservice/BatteryService.h
- For the wakeup timers:
  - frameworks/base/services/core/java/com/android/server/am/ActivityManagerService.java
  - frameworks/base/services/core/jni/com_android_server_AlarmManagerService.cpp
  - kernel: fs/timerfd.c for timerfd_create() from healthd_common.cpp, included
    through the android.hardware.health@2.0-impl shared_lib
- Storage-related:
  - hardware/interfaces/health/2.0/IHealth.hal
  - hardware/interfaces/health/2.0/types.hal
  - system/core/storaged/storaged_diskstats.cpp

## Terminology

- SoC = State of Charge, e.g. from 0-100%
  (n.b. "SoC" is normally understood to mean "System-on-chip")
- FG = Fuel Gauge, similar to SoC
- BMS = Battery Management System

**USB terms**

- SDP = Standard downstream port(2.5-500mA)
- CDP = Charging downstream port(up to 1500mA)
- DCP = Dedicated charging port(up to 1500mA)

**Type-C terms:**

- UFP = "Upstream-facing port", i.e. the port that flows towards the computer
- DFP = "Downstream-facing port", i.e. the port that flows from the device
  towards another device, perhaps USB audio output

**Internals:**
- CC/CV = Constant current / constant voltage (CC/CV)
  For lithium batteries: When nearly depleted, use constant current for
  charging. When approaching max voltage, use contant voltage.
- OCV = Open circuit voltage
- CC = Coulomb counter
- PMIC = [Power Management Integrated Circuit][pmic]
- SPMI = [System Power Management Interface][spmi]
- FCC = Full charge capacity
- ESR = [Equivalent series resistance][esr]

## Sources

- [The Basics of USB charging](https://www.maximintegrated.com/en/app-notes/index.mvp/id/4803)
- [What is CC/CV mode?](http://www.bestgopower.com/faq/27-what-is-cc-cv-mode.html)

<!-- Markdown link references -->
[health-ref]: https://source.android.com/reference/hidl/android/hardware/health/2.0/IHealth
[guide]: https://android.googlesource.com/platform/hardware/interfaces/+/master/health/2.0/
[vintf-manifest]: https://source.android.com/devices/architecture/vintf/objects.html#device-manifest-file
[sodp-manifest]: https://github.com/sonyxperiadev/device-sony-common/blob/5945d6aff99a8188554c3e4f06032d254138db00/vintf/manifest.xml
[pmic]: https://en.wikipedia.org/wiki/Power_management_integrated_circuit
[spmi]: https://mipi.org/specifications/system-power-management-interface
[marlin]: https://android.googlesource.com/device/google/marlin/+/android-9.0.0_r22/health/
[wahoo]: https://android.googlesource.com/device/google/wahoo/+/android-9.0.0_r22/health/
[crosshatch]: https://android.googlesource.com/device/google/crosshatch/+/android-9.0.0_r22/health/
[esr]: https://en.wikipedia.org/wiki/ESR_meter
