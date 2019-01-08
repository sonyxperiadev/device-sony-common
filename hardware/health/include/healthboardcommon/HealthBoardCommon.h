#ifndef DEVICE_SONY_HEALTH_HEALTH_COMMON_H
#define DEVICE_SONY_HEALTH_HEALTH_COMMON_H

#include <batteryservice/BatteryService.h>

namespace device {
namespace sony {
namespace health {

void health_board_battery_init();

void health_board_battery_update(struct android::BatteryProperties *props);

}  // namespace health
}  // namespace sony
}  // namespace device

#endif // DEVICE_SONY_HEALTH_HEALTH_COMMON_H
