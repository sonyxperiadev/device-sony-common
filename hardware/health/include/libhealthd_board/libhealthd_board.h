#ifndef DEVICE_SONY_HEALTH_LIBHEALTHD_H
#define DEVICE_SONY_HEALTH_LIBHEALTHD_H

#include <batteryservice/BatteryService.h>
#include <healthd/healthd.h>

// Discard the pointer
void healthd_board_init(struct healthd_config *);

int healthd_board_battery_update(struct android::BatteryProperties *props);

#endif // DEVICE_SONY_HEALTH_LIBHEALTHD_H
