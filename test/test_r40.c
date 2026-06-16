//  Copyright (c) 2026 Jakub Filipowicz <jakubf@gmail.com>
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
#include <string.h>

#include "r40.h"

static int checks = 0;
static int failures = 0;

#define CHECK(cond) do { \
	checks++; \
	if (!(cond)) { \
		failures++; \
		fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
	} \
} while (0)

// -----------------------------------------------------------------------
static void test_r40len(void)
{
	CHECK(r40len("") == 0);
	CHECK(r40len("A") == 1);
	CHECK(r40len("ABC") == 1);
	CHECK(r40len("ABCD") == 2);
	CHECK(r40len("ABCDEF") == 2);
	CHECK(r40len("ABCDEFG") == 3);
}

// -----------------------------------------------------------------------
static void test_valid_char(void)
{
	CHECK(r40_valid_char('A'));
	CHECK(r40_valid_char('Z'));
	CHECK(r40_valid_char('a'));     // lowercase is accepted (folded on encode)
	CHECK(r40_valid_char('z'));
	CHECK(r40_valid_char('0'));
	CHECK(r40_valid_char('9'));
	CHECK(r40_valid_char('_'));
	CHECK(r40_valid_char('%'));
	CHECK(r40_valid_char('#'));
	CHECK(r40_valid_char(0));       // NUL is the R40 blank slot

	CHECK(!r40_valid_char('-'));
	CHECK(!r40_valid_char(' '));
	CHECK(!r40_valid_char('!'));
	CHECK(!r40_valid_char('*'));
}

// -----------------------------------------------------------------------
static void test_valid_str(void)
{
	CHECK(r40_valid_str(""));
	CHECK(r40_valid_str("ABC123"));
	CHECK(r40_valid_str("a_b%c#"));
	CHECK(!r40_valid_str("AB-CD"));
	CHECK(!r40_valid_str("hello world"));
}

// -----------------------------------------------------------------------
static void test_encode_values(void)
{
	unsigned len;
	uint16_t *r;

	// F=6 O=15 O=15 -> 6*1600 + 15*40 + 15 = 0x27e7
	r = ascii_to_r40("FOO", &len, NULL);
	CHECK(r != NULL);
	CHECK(len == 1);
	if (r) CHECK(r[0] == 0x27e7);
	free(r);

	// A=1 B=2 (trailing 0) -> 1*1600 + 2*40 = 0x0690
	r = ascii_to_r40("AB", &len, NULL);
	CHECK(r != NULL);
	CHECK(len == 1);
	if (r) CHECK(r[0] == 0x0690);
	free(r);

	// #=39 in every position -> 39*1600 + 39*40 + 39 = 0xf9ff (max word)
	r = ascii_to_r40("###", &len, NULL);
	CHECK(r != NULL);
	if (r) CHECK(r[0] == 0xf9ff);
	free(r);

	// lowercase folds onto uppercase codes
	r = ascii_to_r40("foo", &len, NULL);
	CHECK(r != NULL);
	if (r) CHECK(r[0] == 0x27e7);
	free(r);
}

// -----------------------------------------------------------------------
static void test_encode_errors(void)
{
	unsigned len = 999;
	uint16_t *r;

	// invalid char: returns NULL and reports its position in r40_len
	r = ascii_to_r40("AB-CD", &len, NULL);
	CHECK(r == NULL);
	CHECK(len == 2);

	// empty input: valid, zero-length, must not return NULL (no malloc(0))
	len = 999;
	r = ascii_to_r40("", &len, NULL);
	CHECK(r != NULL);
	CHECK(len == 0);
	free(r);
}

// -----------------------------------------------------------------------
static void test_decode_strict(void)
{
	char *out;

	// (0,0,1): leading zeros then a non-zero -> not valid R40
	uint16_t bad1 = 0x0001;
	out = r40_to_ascii(&bad1, 1, NULL);
	CHECK(out == NULL);

	// (A,0,B) = 1*1600 + 0 + 2 = 1602: zero between non-zeros -> invalid
	uint16_t bad2 = 1602;
	out = r40_to_ascii(&bad2, 1, NULL);
	CHECK(out == NULL);

	// trailing zeros are fine: (A,B,0) decodes to "AB"
	uint16_t ok = 0x0690;
	out = r40_to_ascii(&ok, 1, NULL);
	CHECK(out != NULL);
	if (out) CHECK(strcmp(out, "AB") == 0);
	free(out);

	// all-zero word decodes to an empty string
	uint16_t zero = 0;
	out = r40_to_ascii(&zero, 1, NULL);
	CHECK(out != NULL);
	if (out) CHECK(strcmp(out, "") == 0);
	free(out);

	// a zero-padded word followed by a non-empty word is invalid across words
	uint16_t words[2] = { 0x0690 /* "AB" */, 1600 /* "A" */ };
	out = r40_to_ascii(words, 2, NULL);
	CHECK(out == NULL);
}

// -----------------------------------------------------------------------
static void roundtrip(const char *in, const char *expected)
{
	unsigned len;
	uint16_t *r = ascii_to_r40(in, &len, NULL);
	CHECK(r != NULL);
	if (!r) return;

	char *out = r40_to_ascii(r, len, NULL);
	CHECK(out != NULL);
	if (out) {
		CHECK(strcmp(out, expected) == 0);
		free(out);
	}
	free(r);
}

// -----------------------------------------------------------------------
static void test_roundtrip(void)
{
	roundtrip("A", "A");
	roundtrip("AB", "AB");
	roundtrip("ABC", "ABC");
	roundtrip("ABCD", "ABCD");        // spans two words, second is zero-padded
	roundtrip("ABCDEF", "ABCDEF");
	roundtrip("CROOK5", "CROOK5");
	roundtrip("a_b%c#", "A_B%C#");    // lowercase folds to uppercase
	roundtrip("", "");
}

// -----------------------------------------------------------------------
static void test_caller_buffers(void)
{
	// encode into a caller-supplied word buffer
	uint16_t words[2] = { 0xffff, 0xffff };
	unsigned len;
	uint16_t *r = ascii_to_r40("ABCDEF", &len, words);
	CHECK(r == words);
	CHECK(len == 2);

	// decode into a caller-supplied char buffer
	char buf[3 * 2 + 1];
	char *out = r40_to_ascii(words, 2, buf);
	CHECK(out == buf);
	CHECK(strcmp(buf, "ABCDEF") == 0);
}

// -----------------------------------------------------------------------
int main(void)
{
	test_r40len();
	test_valid_char();
	test_valid_str();
	test_encode_values();
	test_encode_errors();
	test_decode_strict();
	test_roundtrip();
	test_caller_buffers();

	printf("%d checks, %d failures\n", checks, failures);
	return failures ? 1 : 0;
}

// vim: tabstop=4 shiftwidth=4 autoindent
