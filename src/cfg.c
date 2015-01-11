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

#include "cfg.h"

// -----------------------------------------------------------------------
struct crk5_cfg_mem * crk5_cfg_mem_unpack(uint16_t *d)
{
	struct crk5_cfg_mem *mem = malloc(sizeof(struct crk5_cfg_mem));
	mem->silicon = (*d >> 15) & 0b1;
	mem->mega = (*d >> 14) & 0b1;
	mem->blocks = ((*d >> 8) & 0b111111) + 1;
	mem->block_start = (*d >> 4) & 0b1111;
	mem->module_start = (*d >> 0) & 0b1111;

	return mem;
}

// -----------------------------------------------------------------------
int crk5_cfg_mem_pack(struct crk5_cfg_mem *mem, uint16_t *d)
{
	if (!mem) return 0;
	mem->blocks--;
	if (mem->blocks > 0b111111) return 0;
	if (mem->block_start > 0b1111) return 0;
	if (mem->module_start > 0b1111) return 0;
	if (mem->mega && !mem->silicon) return 0;

	*d = 0;

	if (mem->silicon) *d |= 1 << 15;
	if (mem->mega) *d |= 1 << 14;
	*d |= mem->blocks << 8;
	*d |= mem->block_start << 4;
	*d |= mem->module_start << 0;

	return 1;
}

// -----------------------------------------------------------------------
void crk5_cfg_mem_delete(struct crk5_cfg_mem *mem)
{
	free(mem);
}

// -----------------------------------------------------------------------


// vim: tabstop=4 shiftwidth=4 autoindent
