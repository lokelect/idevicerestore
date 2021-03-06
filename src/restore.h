/*
 * restore.h
 * Functions for handling idevices in restore mode
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

#ifndef RESTORED_H
#define RESTORED_H

#include <libimobiledevice/restore.h>

#include "restore.h"

int restore_handle_progress_msg(restored_client_t client, plist_t msg);
int restore_handle_status_msg(restored_client_t client, plist_t msg);
int asr_send_system_image_data_from_file(idevice_t device, restored_client_t client, const char *filesystem);
int restore_send_kernelcache(restored_client_t client, char *kernel_data, int len);
int restore_send_nor_data(restored_client_t client, char* ipsw, plist_t tss);
const char* restore_progress_string(unsigned int operation);

#endif
