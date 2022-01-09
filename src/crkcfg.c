//  Copyright (c) 2022 Jakub Filipowicz <jakubf@gmail.com>
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
#include <arpa/inet.h>
#include <string.h>

#include "cfg.h"

#define FAIL_SYNTAX	1
#define FAIL_DECODE 2
#define FAIL_FILE	3
#define FAIL_ENCODE 4
#define FAIL_RECODE 5

// -----------------------------------------------------------------------
static void help()
{
	fprintf(stderr,
		"crkcfg - Dump CROOK-5 configuration.\n"
		"Usage: crkcfg [-d|-c] filename offset\n"
		"\n"
		" -d - dump configuration\n"
		" -c - check if decoded == encoded (for test purposes)\n"
	);
}

// -----------------------------------------------------------------------
void cfg_dump(struct crk5_cfg *cfg)
{
	printf("sys.sem = %i\n", cfg->sys.sem);
	printf("sys.dw3 = %i\n", cfg->sys.dw3);
	printf("sys.lod_unl = %i\n", cfg->sys.lod_unl);
	printf("sys.tick1x = %i\n", cfg->sys.tick1x);
	printf("sys.no_swap = %i\n", cfg->sys.no_swap);
	printf("sys.buf_4k = %i\n", cfg->sys.buf_4k);
	printf("sys.all_resident = %i\n", cfg->sys.all_resident);
	printf("sys.dis_wait = %i\n", cfg->sys.disk_wait);
	printf("sys.io_bu_count = %i\n", cfg->sys.io_buf_count);
	printf("sys.auto_ram_files = %i\n", cfg->sys.auto_ram_files);
	printf("sys.dir_guard = %i\n", cfg->sys.dir_guard);
	printf("sys.dir_write_check = %i\n", cfg->sys.dir_write_check);
	printf("sys.dir_vec_count = %i\n", cfg->sys.dir_vec_count);
	printf("sys.os_blocks = %i\n", cfg->sys.os_blocks);
	printf("sys.line_buf_len = %i\n", cfg->sys.line_buf_len);

	printf("mx.auto_multix = %i\n", cfg->mx.auto_multix);
	printf("mx.no_parity = %i\n", cfg->mx.no_parity);
	printf("mx.odd_parity = %i\n", cfg->mx.odd_parity);
	printf("mx.bits8 = %i\n", cfg->mx.bits8);
	printf("mx.xon_xoff = %i\n", cfg->mx.xon_xoff);
	printf("mx.bs_can = %i\n", cfg->mx.bs_can);
	printf("mx.uppercase = %i\n", cfg->mx.uppercase);
	printf("mx.stream_vectors = %i\n", cfg->mx.stream_vectors);

	for (int i=0 ; i<CRK5_CFG_MEM_SLOTS ; i++) {
		printf("mem.%i.configured = %i\n", i, cfg->mem[i].configured);
		if (cfg->mem[i].configured) {
			printf("mem.%i.init_required = %i\n", i, cfg->mem[i].init_required);
			printf("mem.%i.mega = %i\n", i, cfg->mem[i].mega);
			printf("mem.%i.block_count = %i\n", i, cfg->mem[i].block_count);
			printf("mem.%i.block_start = %i\n", i, cfg->mem[i].block_start);
			printf("mem.%i.module_start = %i\n", i, cfg->mem[i].module_start);
		}
	}

	for (int i=0 ; i<CRK5_CFG_DISK_SLOTS ; i++) {
		printf("disk.%i.configured = %i\n", i, cfg->disk[i].configured);
		if (cfg->disk[i].configured) {
			printf("disk.%i.foreign = %i\n", i, cfg->disk[i].foreign);
			printf("disk.%i.type = %s\n", i, crk5_cfg_disk_type_name(cfg->disk[i].type));

			switch (cfg->disk[i].type) {
				case CRK5_CFG_DISK_9425:
					printf("disk.%i.chan = %i\n", i, cfg->disk[i].d.m9425.chan);
					printf("disk.%i.dev = %i\n", i, cfg->disk[i].d.m9425.dev);
					printf("disk.%i.fixed = %i\n", i, cfg->disk[i].d.m9425.fixed);
					break;
				case CRK5_CFG_DISK_WINCH:
					printf("disk.%i.start_quant = %i\n", i, cfg->disk[i].d.winch.start_quant);
					printf("disk.%i.winch_type = %i\n", i, cfg->disk[i].d.winch.type);
					printf("disk.%i.num = %i\n", i, cfg->disk[i].d.winch.num);
					break;
				case CRK5_CFG_DISK_FLOP8:
					printf("disk.%i.dev = %i\n", i, cfg->disk[i].d.flop8.dev);
					printf("disk.%i.door = %i\n", i, cfg->disk[i].d.flop8.door);
					break;
				case CRK5_CFG_DISK_FLOP5:
					printf("disk.%i.density = %s\n", i, crk5_cfg_disk_floppy_density_name(cfg->disk[i].d.flop5.density));
					printf("disk.%i.dev = %i\n", i, cfg->disk[i].d.flop5.dev);
					break;
				case CRK5_CFG_DISK_PLIX:
					printf("disk.%i.plix_type = %i\n", i, cfg->disk[i].d.plix.dev);
					printf("disk.%i.controller = %i\n", i, cfg->disk[i].d.plix.controller);
					printf("disk.%i.dev = %i\n", i, cfg->disk[i].d.plix.dev);
					break;
				default:
					break;
			}
		}
	}

	for (int i=0 ; i<CRK5_CFG_TAPE_SLOTS ; i++) {
		printf("tape.%i.raw = 0x%04x\n", i, cfg->tape[i].raw);
	}

	for (int i=0 ; i<CRK5_CFG_CAMAC_SLOTS ; i++) {
		printf("camac.%i.configured = %i\n", i, cfg->ch_camac[i].configured);
		if (cfg->ch_camac[i].configured) {
			printf("camac.%i.chan = %i\n", i, cfg->ch_camac[i].chan);
		}
	}

	printf("plix.configured = %i\n", cfg->ch_plix.configured);
	if (cfg->ch_plix.configured) {
		printf("plix.chan = %i\n", cfg->ch_plix.chan);
	}

	printf("multix.configured = %i\n", cfg->ch_multix.configured);
	if (cfg->ch_multix.configured) {
		printf("multix.chan = %i\n", cfg->ch_multix.chan);
	}

	for (int i=0 ; i<CRK5_CFG_CHAR_SLOTS ; i++) {
		printf("char.%i.configured = %i\n", i, cfg->ch_char[i].configured);
		if (cfg->ch_char[i].configured) {
			printf("char.%i.chan = %i\n", i, cfg->ch_char[i].chan);
		}
	}

	printf("rtc.configured = %i\n", cfg->rtc.configured);
	if (cfg->rtc.configured) {
		printf("rtc.chan = %i\n", cfg->rtc.chan);
		printf("rtc.dev = %i\n", cfg->rtc.dev);
	}

	printf("net.configured = %i\n", cfg->net.configured);
	if (cfg->net.configured) {
		printf("net.chan = %i\n", cfg->net.chan);
		printf("net.dev = %i\n", cfg->net.dev);
	}

	printf("winch_line = %i\n", cfg->winch_line);
	printf("floppy_line = %i\n", cfg->floppy_line);

	printf("mongroup.configured = %i\n", cfg->mongroup.configured);
	if (cfg->mongroup.configured) {
		printf("mongroup.dev = %i\n", cfg->mongroup.dev);
	}

	if (cfg->oprq.multix_reset) {
		printf("oprq.multix_reset = 1\n");
	} else {
		printf("oprq.ini_line = %i\n", cfg->oprq.ini_line);
	}

	for (int i=0 ; i<CRK5_CFG_LINE_SLOTS ; i++) {
		printf("lines.%i.configured = %i\n", i, cfg->lines[i].configured);
		if (cfg->lines[i].configured) {
			printf("lines.%i.multix = %i\n", i, cfg->lines[i].multix);
			if (cfg->lines[i].multix) {
				printf("lines.%i.dir = %s\n", i, crk5_cfg_line_dir_name(cfg->lines[i].d.mx.dir));
				printf("lines.%i.used = %i\n", i, cfg->lines[i].d.mx.used);
				printf("lines.%i.type = %s\n", i, crk5_cfg_line_type_name(cfg->lines[i].d.mx.type));
				printf("lines.%i.protocol = %s\n", i, crk5_cfg_line_protocol_name(cfg->lines[i].d.mx.protocol));
				printf("lines.%i.line_count = %i\n", i, cfg->lines[i].d.mx.count);
			} else {
				printf("lines.%i.device = %i\n", i, cfg->lines[i].d.ch.dev);
			}
		}
	}

	printf("winch.quant = %i\n", cfg->winch_quant);
	for (int i=0 ; i<CRK5_CFG_WINCH_TYPE_SLOTS ; i++) {
		printf("winch.type.%i.big = %i\n", i, cfg->winch_type[i].big);
		printf("winch.type.%i.heads = %i\n", i, cfg->winch_type[i].heads);
		printf("winch.type.%i.autopark = %i\n", i, cfg->winch_type[i].autopark);
		if (!cfg->winch_type[i].autopark) {
			printf("winch.type.%i.park_cyl = %i\n", i, cfg->winch_type[i].park_cyl);
		}
	}
}

// -----------------------------------------------------------------------
int main(int argc, char **argv)
{
	if (argc != 4) {
		help();
		exit(FAIL_SYNTAX);
	}

	const char *opt = argv[1];
	const char *filename = argv[2];
	int offset = atoi(argv[3]);
	FILE *f = NULL;
	uint16_t buf[CRK5_CFG_SIZE_WORDS];
	struct crk5_cfg cfg;

	if (!(f = fopen(filename, "r"))) {
		printf("ERROR: Failed to open file: %s\n", filename);
		goto fail;
	}
	if (fseek(f, offset*2, SEEK_SET)) {
		printf("ERROR: Seek to %i failed\n", offset*2);
		goto fail;
	}
	if (fread(buf, sizeof(uint16_t), CRK5_CFG_SIZE_WORDS, f) != CRK5_CFG_SIZE_WORDS) {
		printf("ERROR: Failed to read %i words from file %s at offset %i\n", CRK5_CFG_SIZE_WORDS, filename, offset);
		goto fail;
	}

	fclose(f);
	f = NULL;

	for (int i=0 ; i<CRK5_CFG_SIZE_WORDS ; i++) {
		buf[i] = ntohs(buf[i]);
	}

	bool decode_ok = crk5_cfg_decode(buf, &cfg);
	if (!decode_ok) {
		printf("ERROR: Failed to decode congfiguration correctly.\n");
		exit(FAIL_DECODE);
	}

	if (!strcmp(opt, "-d")) {
		cfg_dump(&cfg);
		exit(0);
	} else if (!strcmp(opt, "-c")) {
		bool recode_ok = true;
		uint16_t encoded[CRK5_CFG_SIZE_WORDS];

		if (!crk5_cfg_encode(&cfg, encoded)) {
			printf("ERROR: Failed to encode configuration correctly.\n");
			exit(FAIL_ENCODE);
		}

		int ranges[2][2] = {
			{ CRK5_CFG_CFG_START, CRK5_CFG_CFG_END },
			{ CRK5_CFG_WTYPES_START, CRK5_CFG_WTYPES_END },
		};
		for (int j=0 ; j<2 ; j++) {
			for (int i=ranges[j][0] ; i<=ranges[j][1] ; i++) {
				if (encoded[i] != buf[i]) {
					printf("ERR @ %i 0x%04x -> 0x%04x\n", i, buf[i], encoded[i]);
					recode_ok = false;
				}
			}
		}

		if (recode_ok) {
			exit(0);
		} else {
			printf("ERROR: Failed to recode congfiguration correctly.\n");
			exit(FAIL_RECODE);
		}
	} else {
		printf("ERROR: Unknown option: %s\n", opt);
		help();
		exit(FAIL_SYNTAX);
	}

	exit(0);

fail:
	if (f) fclose(f);
	exit(FAIL_FILE);
}

// vim: tabstop=4 shiftwidth=4 autoindent
