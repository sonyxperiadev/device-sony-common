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

#define LOG_TAG "rqbalance_ctl"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <cutils/android_filesystem_config.h>
#include <hardware/hardware.h>
#include <hardware/power.h>
#include <log/log.h>

#define POWERSERVER_SOCKET		"/dev/socket/powerhal/rqbsvr"

#define MAX_ARGUMENTS	20
struct rqbalance_halext_params {
	uint8_t acquire;
	uint32_t id;
	uint32_t time;
	uint32_t argument[MAX_ARGUMENTS];
	uint32_t arraysz;
};

static int send_powerserver_data(struct rqbalance_halext_params params)
{
	register int sock;
	int ret, len = sizeof(struct sockaddr_un);
	int32_t halext_reply;
	fd_set receivefd;
	struct sockaddr_un server_address;
	struct timeval timeout;

	/* Get socket in the UNIX domain */
	sock = socket(PF_UNIX, SOCK_SEQPACKET, 0);
	if (sock < 0) {
		ALOGE("Could not get the PowerServer from client");
		return -EPROTO;
	}

	memset(&server_address, 0, sizeof(struct sockaddr_un));
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, POWERSERVER_SOCKET);

	/* Set nonblocking I/O for socket to avoid stall */
	fcntl(sock, F_SETFL, O_NONBLOCK);

	ret = connect(sock, (struct sockaddr*)&server_address, len);
	if (ret < 0) {
		ALOGE("Cannot connect to PowerServer socket");
		goto end;
	}

	/* Send the filled struct */
	ret = send(sock, &params, sizeof(struct rqbalance_halext_params), 0);
	if (ret < 0) {
		ALOGE("Cannot send data to PowerServer");
		goto end;
	}

	/* Setup for receiving server reply (handle) */
	/* Initialize and set a new FD for receive operation */
	FD_ZERO(&receivefd);
	FD_SET(sock, &receivefd);

	/* Set a one second timeout for select operation */
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	/* Wait until the socket is ready for receive operation */
	ret = select(sock+1, &receivefd, NULL, NULL, &timeout);
	if (ret < 0) {
		ALOGE("Socket error. Cannot continue.");
		if (ret == -1) {
			ALOGE("Socket not ready: timed out");
			ret = -ETIMEDOUT;
		}
		goto end;
	}

	/* New FD is set and the socket is ready to receive data */
	ret = recv(sock, &halext_reply, sizeof(int32_t), 0);
	if (ret == -1) {
		ALOGE("Cannot receive reply from PowerServer");
		ret = halext_reply = -EINVAL;
	} else ret = halext_reply;
end:
	if (sock)
		close(sock);
	return ret;
}

/*
 * perf_lock_acq - Sends an extended type power hint to the RQBalance
 *                 based PowerHAL to acquire performance lock.
 *
 * \return Returns success (0) or negative errno.
 */
int perf_lock_acq(int id, int time, int argument[], int arraysz)
{
	struct rqbalance_halext_params params;
	uint32_t i;

	params.acquire = 1;
	params.id = id;
	params.time = time;
	params.arraysz = arraysz;

	if (MAX_ARGUMENTS < arraysz) {
		ALOGE("Maximum number of arguments exceeded!!");
		params.arraysz = MAX_ARGUMENTS;
	}

	for (i = 0; i <= params.arraysz; i++)
		params.argument[i] = argument[i];

	return send_powerserver_data(params);
}

/*
 * perf_lock_rel - Sends an extended type power hint to the RQBalance
 *                 based PowerHAL to release performance lock.
 *
 * \return Returns success (0) or negative errno.
 */
int perf_lock_rel(int id)
{
	struct rqbalance_halext_params params;

	params.acquire = 0;
	params.id = id;

	return send_powerserver_data(params);
}
