# RQBalance based PowerHAL (binder) #

This is an Open Source Android HAL, made specifically to manage power states
on devices that are using the RQBalance hotplugging governor kernel driver.

## Overview ##

RQBalance-PowerHAL includes the possibility to get advanced interaction with
the RQBalance driver by using an "HAL Extension" which code gets used by the
main PowerHAL PowerServer that is needed to communicate, through UNIX sockets,
with external libraries (currently, librqbalance) running in either 64-bits or
32-bits mode.

It supports easily extensible and configurable (through Android Properties)
Power Modes, Special PowerHints (non-standard, forced parameters,
non-compliant to the user configuration, only for really critical situations)
and Timed Optimizations (using POSIX timers).


## Notes ##

The external librqbalance library is made for Qualcomm based devices,
implementing Qualcomm specific functions to send advanced performance hints,
but the concept of the modularity of RQBalance-PowerHAL is done for easy
porting across various SoC vendors' specific way of sending special hints.


## License ##

Copyright (C) 2017-2019 AngeloGioacchino Del Regno <kholk11@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
