//  Copyright (c) 2015 Jakub Filipowicz <jakubf@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <inttypes.h>
#include <stdlib.h>

#include "emcrk/r40.h"
#include "emcrk/obj.h"

// -----------------------------------------------------------------------
void * crk5_obj_unpack(uint16_t *data, int *type)
{
	if (data[0] == CRK5_SPACE_STR) {
		*type = CRK5_OBJ_STREAM;
		return crk5_obj_stream_unpack(data);
	} else if (data[0] == CRK5_SPACE_DEV) {
		*type = CRK5_OBJ_DEVICE;
		return crk5_obj_dev_unpack(data);
	} else if (data[0] == CRK5_SPAVE_MES) {
		*type = CRK5_OBJ_MESSAGE;
		return crk5_obj_mes_unpack(data);
	} else if (data[0] == CRK5_SPAVE_RAM) {
		*type = CRK5_OBJ_MEM;
		return crk5_obj_mem_unpack(data);
	} else if (data[3] == 0) {
		*type = CRK5_OBJ_DISK;
		return crk5_obj_disk_unpack(data);
	} else {
		*type = CRK5_OBJ_FILE;
		return crk5_obj_file_unpack(data);
	}
}

// -----------------------------------------------------------------------
void crk5_obj_delete(void *obj, int type)
{
	if (!obj) return;

	if (type == CRK5_OBJ_STREAM) {
		crk5_obj_stream_delete(obj);
	} else if (type == CRK5_OBJ_DEVICE) {
		crk5_obj_dev_delete(obj);
	} else if (type == CRK5_OBJ_MESSAGE) {
		crk5_obj_mes_delete(obj);
	} else if (type == CRK5_OBJ_MEM) {
		crk5_obj_mem_delete(obj);
	} else if (type == CRK5_OBJ_DISK) {
		crk5_obj_disk_delete(obj);
	} else if (type == CRK5_OBJ_FILE) {
		crk5_obj_file_delete(obj);
	}
}

// -----------------------------------------------------------------------
struct crk5_obj_stream * crk5_obj_stream_unpack(uint16_t *data)
{
	struct crk5_obj_stream *obj = malloc(sizeof(struct crk5_obj_stream));
	if (!obj) return NULL;

	obj->id = data[3];

	return obj;
}

// -----------------------------------------------------------------------
void crk5_obj_stream_delete(struct crk5_obj_stream *obj)
{
	if (!obj) return;
	free(obj);
}

// -----------------------------------------------------------------------
struct crk5_obj_disk * crk5_obj_disk_unpack(uint16_t *data)
{
	struct crk5_obj_disk *obj = malloc(sizeof(struct crk5_obj_disk));
	if (!obj) return NULL;

	obj->space = data[0];
	obj->space_name = r40_to_ascii(data+1, 1, NULL);
	obj->start = data[1];
	obj->end = data[2];

	return obj;
}

// -----------------------------------------------------------------------
void crk5_obj_disk_delete(struct crk5_obj_disk *obj)
{
	if (!obj) return;
	free(obj->space_name);
	free(obj);
}
// -----------------------------------------------------------------------
struct crk5_obj_file * crk5_obj_file_unpack(uint16_t *data)
{
	struct crk5_obj_file *obj = malloc(sizeof(struct crk5_obj_file));
	if (!obj) return NULL;

	obj->space_name = r40_to_ascii(data, 1, NULL);
	obj->dir_name = r40_to_ascii(data+1, 2, NULL); 
	obj->file_name = r40_to_ascii(data+3, 2, NULL); 

	return obj;
}

// -----------------------------------------------------------------------
void crk5_obj_file_delete(struct crk5_obj_file *obj)
{
	if (!obj) return;
	free(obj->space_name);
	free(obj->dir_name);
	free(obj->file_name);
	free(obj);
}
// -----------------------------------------------------------------------
struct crk5_obj_dev * crk5_obj_dev_unpack(uint16_t *data)
{
	struct crk5_obj_dev *obj = malloc(sizeof(struct crk5_obj_dev));
	if (!obj) return NULL;

	obj->unit = data[3] % 8;
	obj->chan = (data[3] / 8) % 8;

	return obj;
}

// -----------------------------------------------------------------------
void crk5_obj_dev_delete(struct crk5_obj_dev *obj)
{
	if (!obj) return;
	free(obj);
}
// -----------------------------------------------------------------------
struct crk5_obj_mes * crk5_obj_mes_unpack(uint16_t *data)
{
	struct crk5_obj_mes *obj = malloc(sizeof(struct crk5_obj_mes));
	if (!obj) return NULL;

	obj->addr = data[1];
	obj->len = data[2];
	obj->receiver = data[3];

	return obj;
}

// -----------------------------------------------------------------------
void crk5_obj_mes_delete(struct crk5_obj_mes *obj)
{
	if (!obj) return;
	free(obj);
}
// -----------------------------------------------------------------------
struct crk5_obj_mem * crk5_obj_mem_unpack(uint16_t *data)
{
	struct crk5_obj_mem *obj = malloc(sizeof(struct crk5_obj_mem));
	if (!obj) return NULL;

	obj->name = r40_to_ascii(data+3, 2, NULL);

	return obj;
}

// -----------------------------------------------------------------------
void crk5_obj_mem_delete(struct crk5_obj_mem *obj)
{
	if (!obj) return;
	free(obj->name);
	free(obj);
}


// vim: tabstop=4 shiftwidth=4 autoindent
