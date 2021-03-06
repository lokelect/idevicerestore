/*
 * recovery.c
 * Functions for handling idevices in recovery mode
 *
 * Copyright (c) 2010 Joshua Hill. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libirecovery.h>

#include "tss.h"
#include "img3.h"
#include "recovery.h"
#include "idevicerestore.h"

int recovery_send_signed_component(irecv_client_t client, char* ipsw, plist_t tss, char* component) {
	int size = 0;
	char* data = NULL;
	char* path = NULL;
	char* blob = NULL;
	img3_file* img3 = NULL;
	irecv_error_t error = 0;

	if (get_signed_component_by_name(ipsw, tss, component, &data, &size) < 0) {
		error("ERROR: Unable to get signed component: %s\n", component);
		return -1;
	}

	info("Sending %s...\n", component);
	error = irecv_send_buffer(client, data, size);
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to send IMG3: %s\n", path);
		img3_free(img3);
		free(data);
		free(path);
		return -1;
	}

	if (data) {
		free(data);
		data = NULL;
	}

	return 0;
}

irecv_error_t recovery_open_with_timeout(irecv_client_t* client) {
	int i = 0;
	irecv_error_t error = 0;
	for (i = 10; i > 0; i--) {
		error = irecv_open(client);
		if (error == IRECV_E_SUCCESS) {
			return error;
		}

		sleep(2);
		info("Retrying connection...\n");
	}

	error("ERROR: Unable to connect to recovery device.\n");
	return error;
}

int recovery_send_ibec(char* ipsw, plist_t tss) {
	irecv_error_t error = 0;
	irecv_client_t client = NULL;
	char* component = "iBEC";

	error = recovery_open_with_timeout(&client);
	if (error != IRECV_E_SUCCESS) {
		return -1;
	}

	error = irecv_send_command(client, "setenv auto-boot true");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to set auto-boot environmental variable\n");
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "saveenv");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to save environmental variable\n");
		irecv_close(client);
		client = NULL;
		return -1;
	}

	if (recovery_send_signed_component(client, ipsw, tss, component) < 0) {
		error("ERROR: Unable to send %s to device.\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "go");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to execute %s\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	if (client) {
		irecv_close(client);
		client = NULL;
	}
	return 0;
}

int recovery_send_applelogo(char* ipsw, plist_t tss) {
	irecv_error_t error = 0;
	irecv_client_t client = NULL;
	char* component = "AppleLogo";

	info("Sending %s...\n", component);

	error = recovery_open_with_timeout(&client);
	if (error != IRECV_E_SUCCESS) {
		return -1;
	}

	if (recovery_send_signed_component(client, ipsw, tss, component) < 0) {
		error("ERROR: Unable to send %s to device.\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "setpicture 1");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to set %s\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "bgcolor 0 0 0");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to display %s\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	if (client) {
		irecv_close(client);
		client = NULL;
	}
	return 0;
}

int recovery_send_devicetree(char* ipsw, plist_t tss) {
	irecv_error_t error = 0;
	irecv_client_t client = NULL;
	char *component = "RestoreDeviceTree";

	error = recovery_open_with_timeout(&client);
	if (error != IRECV_E_SUCCESS) {
		return -1;
	}

	if (recovery_send_signed_component(client, ipsw, tss, component) < 0) {
		error("ERROR: Unable to send %s to device.\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "devicetree");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to execute %s\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	if (client) {
		irecv_close(client);
		client = NULL;
	}
	return 0;
}

int recovery_send_ramdisk(char* ipsw, plist_t tss) {
	irecv_error_t error = 0;
	irecv_client_t client = NULL;
	char *component = "RestoreRamDisk";

	error = recovery_open_with_timeout(&client);
	if (error != IRECV_E_SUCCESS) {
		return -1;
	}

	if (recovery_send_signed_component(client, ipsw, tss, component) < 0) {
		error("ERROR: Unable to send %s to device.\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "ramdisk");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to execute %s\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	if (client) {
		irecv_close(client);
		client = NULL;
	}
	return 0;
}

int recovery_send_kernelcache(char* ipsw, plist_t tss) {
	irecv_error_t error = 0;
	irecv_client_t client = NULL;
	char *component = "RestoreKernelCache";

	error = recovery_open_with_timeout(&client);
	if (error != IRECV_E_SUCCESS) {
		return -1;
	}

	if (recovery_send_signed_component(client, ipsw, tss, component) < 0) {
		error("ERROR: Unable to send %s to device.\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	error = irecv_send_command(client, "bootx");
	if (error != IRECV_E_SUCCESS) {
		error("ERROR: Unable to execute %s\n", component);
		irecv_close(client);
		client = NULL;
		return -1;
	}

	if (client) {
		irecv_close(client);
		client = NULL;
	}
	return 0;
}


int recovery_get_ecid(uint64_t* ecid) {
	return 0;
}
