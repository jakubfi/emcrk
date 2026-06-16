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
uint16_t * ascii_to_r40(const char *ascii, unsigned *r40_len, uint16_t *r40)
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
		int ch = char_to_r40(*s);

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
char * r40_to_ascii(uint16_t *r40, unsigned r40_len, char *ascii)
{
	char *buf;
	int bpos = 0;
	int d;
	int term = 0; // seen a \0; every lower position must be \0 as well

	if (ascii) {
		buf = ascii;
	} else {
		buf = malloc(3*r40_len + 1);
		if (!buf) {
			return NULL;
		}
	}

	while (r40_len > 0) {
		d = 1600;
		while (d >= 1) {
			// (*r40/d) % 40 is always 0..39, so r40_to_char never fails here
			int ch = r40_to_char((*r40/d) % 40);
			if (ch == 0) {
				term = 1;
			} else if (term) {
				// non-zero char after a \0: words fill left-to-right, so this is not valid R40
				if (!ascii) free(buf);
				return NULL;
			} else {
				buf[bpos++] = ch;
			}
			d /= 40;
		}
		r40++;
		r40_len--;
	}
	buf[bpos] = '\0';
	return buf;
}

// vim: tabstop=4 shiftwidth=4 autoindent
