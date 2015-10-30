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

int r40len(char *s);
uint16_t * ascii_to_r40(char *ascii, unsigned *r40_len, uint16_t *r40);
char * r40_to_ascii(uint16_t *r40, unsigned r40_len, char *ascii);

int r40_valid_char(char ch);
int r40_valid_str(char *str);

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
