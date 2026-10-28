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
	r = ascii_to_r40("FOO", &len, NULL, R40_STRICT);
	CHECK(r != NULL);
	CHECK(len == 1);
	if (r) CHECK(r[0] == 0x27e7);
	free(r);

	// A=1 B=2 (trailing 0) -> 1*1600 + 2*40 = 0x0690
	r = ascii_to_r40("AB", &len, NULL, R40_STRICT);
	CHECK(r != NULL);
	CHECK(len == 1);
	if (r) CHECK(r[0] == 0x0690);
	free(r);

	// #=39 in every position -> 39*1600 + 39*40 + 39 = 0xf9ff (max word)
	r = ascii_to_r40("###", &len, NULL, R40_STRICT);
	CHECK(r != NULL);
	if (r) CHECK(r[0] == 0xf9ff);
	free(r);

	// lowercase folds onto uppercase codes
	r = ascii_to_r40("foo", &len, NULL, R40_STRICT);
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
	r = ascii_to_r40("AB-CD", &len, NULL, R40_STRICT);
	CHECK(r == NULL);
	CHECK(len == 2);

	// empty input: valid, zero-length, must not return NULL (no malloc(0))
	len = 999;
	r = ascii_to_r40("", &len, NULL, R40_STRICT);
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
	out = r40_to_ascii(&bad1, 1, NULL, R40_STRICT);
	CHECK(out == NULL);

	// (A,0,B) = 1*1600 + 0 + 2 = 1602: zero between non-zeros -> invalid
	uint16_t bad2 = 1602;
	out = r40_to_ascii(&bad2, 1, NULL, R40_STRICT);
	CHECK(out == NULL);

	// trailing zeros are fine: (A,B,0) decodes to "AB"
	uint16_t ok = 0x0690;
	out = r40_to_ascii(&ok, 1, NULL, R40_STRICT);
	CHECK(out != NULL);
	if (out) CHECK(strcmp(out, "AB") == 0);
	free(out);

	// all-zero word decodes to an empty string
	uint16_t zero = 0;
	out = r40_to_ascii(&zero, 1, NULL, R40_STRICT);
	CHECK(out != NULL);
	if (out) CHECK(strcmp(out, "") == 0);
	free(out);

	// a zero-padded word followed by a non-empty word is invalid across words
	uint16_t words[2] = { 0x0690 /* "AB" */, 1600 /* "A" */ };
	out = r40_to_ascii(words, 2, NULL, R40_STRICT);
	CHECK(out == NULL);
}

// -----------------------------------------------------------------------
static void roundtrip(const char *in, const char *expected)
{
	unsigned len;
	uint16_t *r = ascii_to_r40(in, &len, NULL, R40_STRICT);
	CHECK(r != NULL);
	if (!r) return;

	char *out = r40_to_ascii(r, len, NULL, R40_STRICT);
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
	uint16_t *r = ascii_to_r40("ABCDEF", &len, words, R40_STRICT);
	CHECK(r == words);
	CHECK(len == 2);

	// decode into a caller-supplied char buffer
	char buf[3 * 2 + 1];
	char *out = r40_to_ascii(words, 2, buf, R40_STRICT);
	CHECK(out == buf);
	CHECK(strcmp(buf, "ABCDEF") == 0);
}

// -----------------------------------------------------------------------
static void test_decode_overflow(void)
{
	char *out;

	// 63999 = "###" is the largest word with every code < 40
	uint16_t max = 63999;
	out = r40_to_ascii(&max, 1, NULL, R40_STRICT);
	CHECK(out && !strcmp(out, "###"));
	free(out);

	// 64000: top code 40, wraps to "" without the check
	uint16_t over = 64000;
	CHECK(r40_to_ascii(&over, 1, NULL, R40_STRICT) == NULL);
	out = r40_to_ascii(&over, 1, NULL, R40_RAW);
	CHECK(out && !strcmp(out, ""));
	free(out);
}

// -----------------------------------------------------------------------
static int decodes_raw(const uint16_t *words, unsigned len, const char *expected)
{
	char *out = r40_to_ascii(words, len, NULL, R40_RAW);
	int res = out && !strcmp(out, expected);
	free(out);
	return res;
}

// -----------------------------------------------------------------------
static void test_decode_raw(void)
{
	uint16_t gap = 2 * 1600 + 27;		// (B,0,0-digit)
	uint16_t lead = 7;					// (0,0,G)
	uint16_t ab = 0x0690;				// (A,B,0)
	uint16_t zero = 0;
	uint16_t words[2] = { 0x0690, 1600 };	// "AB" then "A"
	uint16_t trail[2] = { 0x27e7, 0 };	// "FOO" then nothing

	CHECK(decodes_raw(&gap, 1, "B 0"));
	CHECK(decodes_raw(&lead, 1, "  G"));
	CHECK(decodes_raw(&ab, 1, "AB"));
	CHECK(decodes_raw(&zero, 1, ""));
	CHECK(decodes_raw(words, 2, "AB A"));
	CHECK(decodes_raw(trail, 2, "FOO"));

	// raw decode of a valid value equals the strict decode
	char *s = r40_to_ascii(&ab, 1, NULL, R40_STRICT);
	CHECK(s && decodes_raw(&ab, 1, s));
	free(s);
}

// -----------------------------------------------------------------------
static void test_encode_raw(void)
{
	unsigned len;
	uint16_t *r;

	// space is not an R40 character in strict mode
	r = ascii_to_r40("B 0", &len, NULL, R40_STRICT);
	CHECK(r == NULL);
	CHECK(len == 1);

	r = ascii_to_r40("B 0", &len, NULL, R40_RAW);
	CHECK(r && (len == 1) && (r[0] == 2 * 1600 + 27));
	free(r);

	r = ascii_to_r40("  G", &len, NULL, R40_RAW);
	CHECK(r && (len == 1) && (r[0] == 7));
	free(r);

	r = ascii_to_r40("AB A", &len, NULL, R40_RAW);
	CHECK(r && (len == 2) && (r[0] == 0x0690) && (r[1] == 1600));
	free(r);
}

// -----------------------------------------------------------------------
// raw decode then raw encode gives back every word below 64000
static void test_roundtrip_raw(void)
{
	int bad = 0;
	char buf[4];
	uint16_t back[1];
	unsigned len;

	for (unsigned w=0 ; w<64000 ; w++) {
		uint16_t word = w;
		back[0] = 0xffff;
		if (!r40_to_ascii(&word, 1, buf, R40_RAW)) {
			bad++;
			continue;
		}
		if (!ascii_to_r40(buf, &len, back, R40_RAW)) {
			bad++;
			continue;
		}
		// trailing zero codes are trimmed, so an all-zero word encodes to no words
		if (((len == 0) && (word != 0)) || ((len == 1) && (back[0] != word))) {
			bad++;
		}
	}
	CHECK(bad == 0);
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
	test_decode_overflow();
	test_decode_raw();
	test_encode_raw();
	test_roundtrip_raw();

	printf("%d checks, %d failures\n", checks, failures);
	return failures ? 1 : 0;
}

// vim: tabstop=4 shiftwidth=4 autoindent
