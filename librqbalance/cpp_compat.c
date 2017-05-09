#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>

#include <hardware/hardware.h>
#include <hardware/power.h>
#include <utils/Log.h>

#include "cpp_compat.h"


RQBalanceCTL* myInstance = RQBalanceCTL_new();

int test(void)
{
	int i = RQBalanceCTL_sendPowerHint(myInstance, POWER_HINT_WHATEVER, thepowerhint_rqbalance_struct);

	RQBalanceCTL_delete(myInstance);
}

