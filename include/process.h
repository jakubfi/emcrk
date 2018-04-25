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

#ifndef CRK5_PROCESS_H
#define CRK5_PROCESS_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
CROOK-5 v8 process structure for N and P kernels:

Field   Offset
        N  P
------------------
ICE		0  0	process vector: IC, R0, SR
REJE	3  8	registers: R1 - R7
NEXT	10 15	next process on the list
NXCH	11 16	next child on the children list
STATE	12 17	process state
PIOR	13 18	process priority
PRNU	14 19	process number
PAPR	15 20	parent process
CHLS	16 21	children list start
ALLS	17 22	?
CHTIM	18 23	?
DEVI	19 24	current input?
DEVO	20 25	current output?
USAL	21 26	?
ROB		22 27	working area?
STRLI	30 35	początek listy strumieni?
BUFLI	31 36	początek listy buforów?
LARUS	32 37	TABLICA KODOW DOSTEPU
LISMEM	33 38	POCZ LISTY BLOKOW PAO
CORSIZ	34 39	number of 4kw segments occupied by the process
NXTMEM	35 40	LISTA PROCESOW Z PAO DO ODEBRANIA
BAR		36 41	?
SZABMEM	37 42	SZABLON PRZYDZIALU PAO
BLPASC	38 43	blokada PASC
IC		39 -4	IC, R0, SR
REJ		42 44	R1-R7
JDAD	49 51	ADRES POCZ ZRODLA LADOWANIA
JPAD	50 52	process entry address
JACN	51 53	POZYCJA PRG W FILDIC
JNAME	52 54	process name (r40-encoded)
JLOLEN	54 56	process image size (words loaded)
--------------------------------------------------
TABUJB	55 61	total size of process structure
*/

#define CRK5_BPROG 0x62
#define CRK5N_PROCESS_SIZE 55
#define CRK5P_PROCESS_SIZE 61

struct crk5_process {
	uint16_t ic, r0, sr;	// ICE
	uint16_t r1, r2, r3, r4, r5, r6, r7; // REJE
	uint16_t next_proc;		// NEXT
	uint16_t next_child;	// NXCH
	uint16_t state;			// STATE
	int16_t prio;			// PIOR
	uint16_t num;			// PRNU
	uint16_t parent;		// PAPR
	uint16_t children;		// CHLS
	uint16_t ALLS;			// ALLS
	uint16_t CHTIM;			// CHTIM
	uint16_t DEVI;			// DEVI
	uint16_t DEVO;			// DEVO
	uint16_t USAL;			// USAL
	uint16_t rob[8];		// ROB
	uint16_t STRLI;			// STRLI
	uint16_t BUFLI;			// BUFLI
	uint16_t LARUS;			// LARUS
	uint16_t LISMEM;		// LISMEM
	uint16_t segments;		// CORSIZ
	uint16_t NXTMEM;		// NXTMEM
	uint16_t BAR;			// BAR
	uint16_t segmap;		// SZABME
	uint16_t BLPASC;		// BLPASC
	uint16_t _ic, _r0, _sr; // IC
	uint16_t _r1, _r2, _r3, _r4, _r5, _r6, _r7; // REJ
	uint16_t JDAD;			// JDAD
	uint16_t start;			// JPAD
	uint16_t JACN;			// JACN
	uint16_t r40_name[2];	// JNAME
	uint16_t size;			// JLOLEN

	uint16_t addr;
	char *name;
};

struct crk5_process * crk5_process_unpack(uint16_t *ptr, uint16_t addr, int kern_mod);
void crk5_process_delete(struct crk5_process *ptr);

#ifdef __cplusplus
}
#endif

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
