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

#include "r40.h"

// -----------------------------------------------------------------------
int r40len(const char *s)
{
	return (strlen(s) + 2) / 3;
}

// -----------------------------------------------------------------------
static int char_to_r40(char c)
{
	if ((c >= 'A') && (c <= 'Z')) {
		return c - 64;
	} else if ((c >= 'a') && (c <= 'z')) {
		return c - 96;
	} else if ((c >= '0') && (c <= '9')) {
		return c - 21;
	} else if (c == '_') {
		return 37;
	} else if (c == '%') {
		return 38;
	} else if (c == '#') {
		return 39;
	} else if (c == 0) {
		return 0;
	} else {
		return -1;
	}
}

// -----------------------------------------------------------------------
static int r40_to_char(unsigned i)
{
	if (i == 0) {
		return 0;
	} else if ((i >= 1) && (i <= 26)) {
		return i+64;
	} else if ((i >= 27) && (i <= 36)) {
		return i+21;
	} else if (i == 37) {
		return '_';
	} else if (i == 38) {
		return '%';
	} else if (i == 39) {
		return '#';
	} else {
		return -1;
	}
}

// -----------------------------------------------------------------------
int r40_valid_char(char ch)
{
	if (char_to_r40(ch) != -1) {
		return 1;
	} else {
		return 0;
	}
}

// -----------------------------------------------------------------------
int r40_valid_str(const char *str)
{
	while (*str) {
		if (char_to_r40(*str) == -1) {
			return 0;
		}
		str++;
	}

	return 1;
}

// -----------------------------------------------------------------------
uint16_t * ascii_to_r40(const char *ascii, unsigned *r40_len, uint16_t *r40, enum r40_mode mode)
{
	const char *s = ascii;
	int mul = 1600;
	int result_len = r40len(ascii);
	uint16_t *dest;

	if (r40_len) {
		*r40_len = 0;
	}

	if (r40) {
		dest = r40;
	} else {
		// always allocate at least one element so "" never hits malloc(0)
		dest = malloc(sizeof(uint16_t) * (result_len ? result_len : 1));
		if (!dest) {
			return NULL;
		}
	}

	uint16_t *r40p = dest;

	for (int i=0 ; i<result_len ; i++) {
		dest[i] = 0;
	}

	while (*s) {
		int ch = ((mode == R40_RAW) && (*s == ' ')) ? 0 : char_to_r40(*s);

		if (ch < 0) {
			if (r40_len) {
				*r40_len = s-ascii;
			}
			if (!r40) {
				free(dest);
			}
			return NULL;
		}

		*r40p += mul * ch;

		if (mul == 1) {
			r40p++;
			mul = 1600;
		} else {
			mul /= 40;
		}

		s++;
	}

	if (r40_len) {
		*r40_len = result_len;
	}

	return dest;
}

// -----------------------------------------------------------------------
char * r40_to_ascii(const uint16_t *r40, unsigned r40_len, char *ascii, enum r40_mode mode)
{
	char *buf;
	int bpos = 0;
	int term = 0; // seen a code 0; in strict mode every following code must be 0 as well

	if (ascii) {
		buf = ascii;
	} else {
		buf = malloc(3*r40_len + 1);
		if (!buf) {
			return NULL;
		}
	}

	for (unsigned i=0 ; i<r40_len ; i++) {
		// the top code would be 40 or more
		if ((mode == R40_STRICT) && (r40[i] >= 64000)) {
			goto invalid;
		}
		for (int d=1600 ; d>=1 ; d/=40) {
			// (r40[i]/d) % 40 is always 0..39, so r40_to_char never fails here
			int code = (r40[i]/d) % 40;
			if (mode == R40_RAW) {
				buf[bpos++] = code ? r40_to_char(code) : ' ';
			} else if (!code) {
				term = 1;
			} else if (term) {
				goto invalid;
			} else {
				buf[bpos++] = r40_to_char(code);
			}
		}
	}

	while ((bpos > 0) && (buf[bpos-1] == ' ')) {
		bpos--;
	}
	buf[bpos] = '\0';
	return buf;

invalid:
	if (!ascii) free(buf);
	return NULL;
}

// vim: tabstop=4 shiftwidth=4 autoindent
