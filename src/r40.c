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
int r40len(char *s)
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
uint16_t * ascii_to_r40(char *ascii, int *r40_len, uint16_t *r40)
{
	char *s = ascii;
    int mul = 1600;
	int result_len = r40len(ascii);
	uint16_t *dest;

	if (r40) {
		dest = r40;
	} else {
		dest = malloc(sizeof(uint16_t) * result_len);
	}

	uint16_t *r40p = dest;

	if (r40_len) {
		*r40_len = 0;
	}

	*r40p = 0;

    while (s && *s) {
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
			*r40p = 0;
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
	int div;

	if (ascii) {
		buf = ascii;
	} else {
		buf = malloc(3*r40_len + 1);
	}

	while (r40_len > 0) {
		div = 1600;
		while (div >= 1) {
			int ch = r40_to_char((*r40/div) % 40);
			if (ch < 0) {
				free(buf);
				return NULL;
			}
			buf[bpos] = ch;
			div /= 40;
			bpos++;
		}
		r40++;
		r40_len--;
	}
	buf[bpos] = '\0';
	return buf;
}

// vim: tabstop=4 shiftwidth=4 autoindent
