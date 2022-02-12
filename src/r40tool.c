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
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "r40.h"

enum modes { NONE, ENCODE, DECODE };

// -----------------------------------------------------------------------
static void help()
{
	fprintf(stderr,
		"Usage: r40 -e string [string ...] - encode ascii strings into R40 values\n"
		"       r40 [-d] value [value ...] - decode R40 values into ascii string\n"
		"\n"
		"R40 encodes 3 characters into one 16-bit integer.\n"
		"Characters allowed in R40 code are: A-Z, 0-9, _, %%, #.\n"
		"Lowercase letters are encoded in uppercase.\n"
	);
}

// -----------------------------------------------------------------------
static int encode(int argc, char **argv, int indx)
{
	unsigned len;
	uint16_t *out = NULL;

	while (indx < argc) {
		out = ascii_to_r40(argv[indx], &len, NULL);
		if (!out) {
			fprintf(stderr, "Input contains characters invalid in R40.\n");
			return 1;
		}

		for (int i=0 ; i<len ; i++) {
			printf("0x%04x ", out[i]);
		}
		printf("\n");
		indx++;
	}

	free(out);
	return 0;
}

// -----------------------------------------------------------------------
static int decode(int argc, char **argv, int indx)
{
	int i = 0;
	uint16_t buf[64 * 1024];
	char *out;
	int val;
	int res;

	while (optind < argc) {
		res = sscanf(argv[optind], "%i", &val);
		if ((val < 0) || (val > 0xffff) || (res != 1)) {
			fprintf(stderr, "Value '%s' is not a 16-bit integer\n", argv[optind]);
			return 1;
		}
		buf[i] = val & 0xffff;
		optind++;
		i++;
	}

	out = r40_to_ascii(buf, i, NULL);
	printf("%s\n", out);

	free(out);

	return 0;
}

// -----------------------------------------------------------------------
int main(int argc, char **argv)
{
	int opt;
	int mode = DECODE;
	int ret;

	while ((opt = getopt(argc, argv, "ed")) != -1) {
		switch (opt) {
			case 'e':
				mode = ENCODE;
				break;
			case 'd':
				mode = DECODE;
				break;
			default:
				help();
				exit(1);
		}
	}

	if (optind == argc) {
		fprintf(stderr, "Missing argument\n");
		help();
		ret = 1;
	} else if (mode == ENCODE) {
		ret = encode(argc, argv, optind);
	} else if (mode == DECODE) {
		ret = decode(argc, argv, optind);
	} else {
		fprintf(stderr, "Unknown error\n");
		ret = 1;
	}

	return ret;
}


// vim: tabstop=4 shiftwidth=4 autoindent
