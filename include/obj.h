//  Copyright (c) 2014 Jakub Filipowicz <jakubf@gmail.com>
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

#ifndef CRK5_OBJ_H
#define CRK5_OBJ_H

#include <inttypes.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
#endif

#define CRK5_SPACE_STR 0x79f2
#define CRK5_SPACE_DEV 0x19de
#define CRK5_SPACE_MES 0x521b
#define CRK5_SPACE_RAM 0x70b5

enum crk5_obj_types {
	CRK5_OBJ_INVALID,
	CRK5_OBJ_STREAM,
	CRK5_OBJ_DISK,
	CRK5_OBJ_FILE,
	CRK5_OBJ_DEVICE,
	CRK5_OBJ_MESSAGE,
	CRK5_OBJ_MEM,
};

#define CRK5_OBJ_SIZE 5

struct crk5_obj_stream {
	uint16_t id;
};

struct crk5_obj_disk {
	uint16_t space;
	char *space_name;
	uint16_t start;
	uint16_t end;
};

struct crk5_obj_file {
	char *space_name;
	char *dir_name;
	char *file_name;
};

struct crk5_obj_dev {
	int chan;
	int unit;
};

struct crk5_obj_mes {
	uint16_t addr;
	int len;
	uint16_t recv_process;
};

struct crk5_obj_mem {
	char *name;
};

void * crk5_obj_unpack(uint16_t *data, int *type);

struct crk5_obj_stream * crk5_obj_stream_unpack(uint16_t *data);
void crk5_obj_stream_delete(struct crk5_obj_stream *obj);
struct crk5_obj_disk * crk5_obj_disk_unpack(uint16_t *data);
void crk5_obj_disk_delete(struct crk5_obj_disk *obj);
struct crk5_obj_file * crk5_obj_file_unpack(uint16_t *data);
void crk5_obj_file_delete(struct crk5_obj_file *obj);
struct crk5_obj_dev * crk5_obj_dev_unpack(uint16_t *data);
void crk5_obj_dev_delete(struct crk5_obj_dev *obj);
struct crk5_obj_mes * crk5_obj_mes_unpack(uint16_t *data);
void crk5_obj_mes_delete(struct crk5_obj_mes *obj);
struct crk5_obj_mem * crk5_obj_mem_unpack(uint16_t *data);
void crk5_obj_mem_delete(struct crk5_obj_mem *obj);

#ifdef __cplusplus
}
#endif

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
