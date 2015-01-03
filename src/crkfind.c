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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "emcrk/kfind.h"

// -----------------------------------------------------------------------
static void help()
{
	fprintf(stderr,
		"crkfind - Look for CROOK-5 kernels in a file.\n"
		"Usage: crkfind filename\n"
	);
}

// -----------------------------------------------------------------------
int main(int argc, char **argv)
{
	off_t words;

	struct stat sb;

	if (argc != 2) {
		help();
		exit(1);
	}

	if (stat(argv[1], &sb) == -1) {
		fprintf(stderr, "Cannot stat file: %s\n", argv[1]);
		exit(1);
	}

	uint16_t *buf = malloc(sb.st_size);
	if (!buf) {
		fprintf(stderr, "Memory allocation error.\n");
		exit(1);
	}

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		fprintf(stderr, "Cannot open file %s for reading.\n", argv[1]);
		free(buf);
		exit(1);
	}

	words = fread(buf, 2, sb.st_size/2, f);
	fclose(f);

	for (int i=0 ; i<words ; i++) {
		buf[i] = ntohs(buf[i]);
	}

	struct crk5_kern_result *kern = crk5_kern_findall(buf, words);
	struct crk5_kern_result *tkern = kern;

	while (tkern) {
		printf(
			"kernel @ %li bytes\n"
			"	CPU: %s\n"
			"	current entry point: 0x%04x, start addr: 0x%04x (kernel is %s)\n"
			"	cksum @ 0x%04x = 0x%04x, computed = 0x%04x -> %s\n",
			tkern->offset,
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
	free(buf);
	return 0;
}


// vim: tabstop=4 shiftwidth=4 autoindent
