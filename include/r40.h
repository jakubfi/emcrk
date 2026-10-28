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

#ifndef CRK5_R40_H
#define CRK5_R40_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

enum r40_mode {
	R40_STRICT,	// decode: NULL unless left-packed and every word < 64000; encode: reject space
	R40_RAW,	// decode: every code, 0 as space, trailing spaces trimmed; encode: space as 0
};

int r40len(const char *s);
uint16_t * ascii_to_r40(const char *ascii, unsigned *r40_len, uint16_t *r40, enum r40_mode mode);
char * r40_to_ascii(const uint16_t *r40, unsigned r40_len, char *ascii, enum r40_mode mode);

int r40_valid_char(char ch);
int r40_valid_str(const char *str);

#ifdef __cplusplus
}
#endif

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
