/*
 * ipsw.c
 * Definitions for IPSW utilities
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

#ifndef IPSW_H
#define IPSW_H

#include <zip.h>

typedef struct {
	int index;
	char* name;
	unsigned int size;
	unsigned char* data;
} ipsw_file;

int ipsw_extract_to_memory(const char* ipsw, const char* infile, char** pbuffer, int* psize);
void ipsw_free_file(ipsw_file* file);

#endif
