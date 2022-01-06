//  Copyright (c) 2014, 2022 Jakub Filipowicz <jakubf@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "kfind.h"

// -----------------------------------------------------------------------
static void help()
{
	fprintf(stderr,
		"crkfind - Search for CROOK-5 kernels in a file.\n"
		"Usage: crkfind filename\n"
	);
}

// -----------------------------------------------------------------------
int main(int argc, char **argv)
{
	if (argc != 2) {
		help();
		exit(1);
	}

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		fprintf(stderr, "Cannot open file %s for reading.\n", argv[1]);
		exit(1);
	}

	struct crk5_kern_result *kern = crk5_kern_findall_file(f, true);
	struct crk5_kern_result *tkern = kern;

	while (tkern) {
		printf(
			"kernel @ %li words\n"
			"	version: %i/%i for CPU: %s\n"
			"	current entry point: 0x%04x, start addr: 0x%04x (kernel is %s)\n"
			"	cksum @ 0x%04x = 0x%04x, computed = 0x%04x -> %s\n",
			tkern->offset,
			tkern->vmaj,
			tkern->vmin,
			tkern->mod ? "MX-16" : "MERA-400",
			tkern->entry_point,
			tkern->start_addr,
			tkern->raw ? "raw" : "installed",
			tkern->cksum_addr,
			tkern->cksum_stored,
			tkern->cksum_computed,
			tkern->cksum_stored == tkern->cksum_computed ? "OK" : "mismatch"
		);
		tkern = tkern->next;
	}

	crk5_kern_res_drop(kern);
	return 0;
}


// vim: tabstop=4 shiftwidth=4 autoindent
