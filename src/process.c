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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "r40.h"

// -----------------------------------------------------------------------
struct crk5_process * crk5_process_unpack(uint16_t *ptr, uint16_t addr, int kern_mod)
{
	int offset = 0;

	struct crk5_process *proc = malloc(sizeof(struct crk5_process));
	if (!proc) {
		return NULL;
	}

	memcpy(proc, ptr, 2*3); // ICE

	if (kern_mod) {
		offset = 5;
	}

	memcpy(3+(uint16_t*)proc, ptr+3+offset, 2*36); // REJE..BLPASC

	if (kern_mod) {
		memset(&(proc->_ic), 0, 2*3); // IC
		offset = 2;
	} else {
		memcpy(&(proc->_ic), ptr+39, 2*3); // IC
	}

	memcpy(&(proc->_r1), ptr+42+offset, 2*13); // REJ..JLOLEN

	proc->addr = addr;
	proc->name = r40_to_ascii(proc->r40_name, 2, NULL);
	proc->segments &= 0xff;

	return proc;
}

// -----------------------------------------------------------------------
void crk5_process_delete(struct crk5_process *ptr)
{
	if (!ptr) return;
	free(ptr->name);
	free(ptr);
}

// vim: tabstop=4 shiftwidth=4 autoindent
