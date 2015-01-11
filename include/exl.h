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

#ifndef CRK5_EXL_H
#define CRK5_EXL_H

#include <inttypes.h>

enum crk5_exls_codes {
	CRK5_EXL_ASG = 128,
	CRK5_EXL_CASG,
	CRK5_EXL_SETP,
	CRK5_EXL_LOAP,
	CRK5_EXL_TMEM,
	CRK5_EXL_NASG,
	CRK5_EXL_ERF,
	CRK5_EXL_ERS,
	CRK5_EXL_ERAS,
	CRK5_EXL_FBOF,
	CRK5_EXL_INPR,
	CRK5_EXL_PRIR,
	CRK5_EXL_PINP,
	CRK5_EXL_PRIN,
	CRK5_EXL_EOF,
	CRK5_EXL_FEOF,
	CRK5_EXL_INAM,
	CRK5_EXL_INUM,
	CRK5_EXL_WADR,
	CRK5_EXL_OVL,
	CRK5_EXL_REAP,
	CRK5_EXL_WRIP,
	CRK5_EXL_READ,
	CRK5_EXL_WRIT,
	CRK5_EXL_OES,
	CRK5_EXL_ERR,
	CRK5_EXL_CORE,
	CRK5_EXL_CPRF,
	CRK5_EXL_JUMP,
	CRK5_EXL_SDIR,
	CRK5_EXL_TDIR,
	CRK5_EXL_CDIR,
	CRK5_EXL_DEFP,
	CRK5_EXL_DELP,
	CRK5_EXL_SREG,
	CRK5_EXL_TREG,
	CRK5_EXL_RUNP,
	CRK5_EXL_HANG,
	CRK5_EXL_TERR,
	CRK5_EXL_WAIT,
	CRK5_EXL_STOP,
	CRK5_EXL_RELD,
	CRK5_EXL_DATE,
	CRK5_EXL_TIME,
	CRK5_EXL_CHPI = 172,
	CRK5_EXL_WAIS,
	CRK5_EXL_SIGN,
	CRK5_EXL_TLAB,
	CRK5_EXL_PINF,
	CRK5_EXL_CSUM,
	CRK5_EXL_CSYS,
	CRK5_EXL_UNL,
	CRK5_EXL_LOD,
	CRK5_EXL_TAKS,
	CRK5_EXL_RELS,
	CRK5_EXL_GMEM,
	CRK5_EXL_RMEM,
	CRK5_EXL_LRAM,
	CRK5_EXL_OPPI = 189,
	CRK5_EXL_WFPI,
	CRK5_EXL_CAMAC,
	CRK5_EXL_RWMT,
	CRK5_EXL_FBMT,
	CRK5_EXL_FFMT,
	CRK5_EXL_BBMT,
	CRK5_EXL_BFMT,
	CRK5_EXL_FMMT,
	CRK5_EXL_REMT,
	CRK5_EXL_WRMT,
	CRK5_EXL_SCON = 249,
	CRK5_EXL_TCON,
	CRK5_EXL_END,
	CRK5_EXL_BACK,
	CRK5_EXL_ABO,
	CRK5_EXL_KILL,
	CRK5_EXL_EOSL,
};

enum crk5_exl_arg_types {
	CRK5_EXL_ARG_NONE = 0,
	CRK5_EXL_ARG_FIL,
	CRK5_EXL_ARG_TMEM,
	CRK5_EXL_ARG_STR,
	CRK5_EXL_ARG_REC,
	CRK5_EXL_ARG_BLOCK,
	CRK5_EXL_ARG_ERR,
	CRK5_EXL_ARG_R4,
	CRK5_EXL_ARG_PROC,
	CRK5_EXL_ARG_DIR,
	CRK5_EXL_ARG_DAT,
	CRK5_EXL_ARG_TIM,
	CRK5_EXL_ARG_MET,
	CRK5_EXL_ARG_PINF,
	CRK5_EXL_ARG_TABL,
	CRK5_EXL_ARG_PI,
	CRK5_EXL_ARG_CAM,
	CRK5_EXL_ARG_MT,
	CRK5_EXL_ARG_REJE,
};

#define CRK5_EXL_FIL_SIZE 12
#define CRK5_EXL_TMEM_SIZE 5
#define CRK5_EXL_DAT_SIZE 3
#define CRK5_EXL_TIM_SIZE 3
#define CRK5_EXL_PROC_SIZE 12
#define CRK5_EXL_PINF_SIZE 12
#define CRK5_EXL_ERR_SIZE 3
#define CRK5_EXL_STR_SIZE 2
#define CRK5_EXL_DIR_SIZE 33
#define CRK5_EXL_MET_SIZE 5
#define CRK5_EXL_REC_SIZE 5
#define CRK5_EXL_BLOCK_SIZE 5
#define CRK5_EXL_MT_SIZE 4
#define CRK5_EXL_PI_SIZE 2
#define CRK5_EXL_CAM_SIZE 4

struct crk5_exl {
	int type;
	int size;
	char *name;
	char *desc;
};

struct crk5_exl_fil {
	int16_t err;
	uint16_t stream_id;
	uint16_t type;
	uint16_t len;
	uint16_t param[2];
	int rights;
	int attributes;
	int mem;
	int obj_type;
	void *obj;
};

struct crk5_exl_tmem {
	int16_t err;
	uint16_t stream_id;
	uint16_t addr;
	uint16_t seg;
};

struct crk5_exl_dat {
	uint16_t year;
	uint16_t month;
	uint16_t day;
};

struct crk5_exl_tim {
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
};

struct crk5_exl_proc {
	int16_t err;
	uint16_t num;
	uint16_t ic;
	uint16_t r0;
	uint16_t prio_sr;
	uint16_t uregs[7];
};

struct crk5_exl_pinf {
	int sys_ver_maj;
	int sys_ver_min;
	uint8_t seg_total;
	uint8_t user_rights;
	uint16_t prio;
	uint16_t spec_file_len;
	uint16_t entry_addr;
	uint16_t words_used;
	uint16_t attr;
	char *space_name;
	char *user_name;
	char *proc_name;
};

struct crk5_exl_err {
	uint16_t proc_addr;
	uint16_t alarm_addr;
	uint8_t alarm_nr;
};

struct crk5_exl_str {
	int16_t err;
	uint16_t stream_id;
};

struct crk5_exl_dir {
	uint16_t count;
	uint16_t *user_id;
	uint16_t *dir_id;
};

struct crk5_exl_met {
	uint16_t disk_id;
	char *disk_name;
	uint16_t dicdic;
	uint16_t fildic;
	uint16_t map;
	uint16_t len;
};

struct crk5_exl_rec {
	uint16_t position;
	uint16_t stream_id;
	uint16_t buf_addr;
	uint8_t end_char;
	uint8_t max_len;
	char prechars[3];
};

struct crk5_exl_block {
	uint16_t transmitted;
	uint16_t stream_id;
	uint16_t addr;
	uint16_t count;
	uint16_t start_sector;
};

struct crk5_exl_mt {
	int tape_end;
	int tape_rev_start;
	int file_mark;
	int transmitted;
	uint16_t unit_num;
	uint16_t addr;
	uint16_t len;
};

struct crk5_exl_pi {
	uint16_t comm;
	int o;
	int f;
	int fc;
	int chan_num;
	int addr_kas;
	int addr_pak;
};

struct crk5_exl_cam {
	int request;
	int response;
	int transmitted;
	int c;
	int f;
	int a;
	int n;
	uint16_t addr;
	uint16_t count;
};

const struct crk5_exl * crk5_get_exl(int num);
struct crk5_exl_fil * crk5_exl_fil_unpack(uint16_t *data);
void crk5_exl_fil_delete(struct crk5_exl_fil *exl);
struct crk5_exl_tmem * crk5_exl_tmem_unpack(uint16_t *data);
void crk5_exl_tmem_delete(struct crk5_exl_tmem *exl);
struct crk5_exl_dat * crk5_exl_dat_unpack(uint16_t *data);
void crk5_exl_dat_delete(struct crk5_exl_dat *exl);
struct crk5_exl_tim * crk5_exl_tim_unpack(uint16_t *data);
void crk5_exl_tim_delete(struct crk5_exl_tim *exl);
struct crk5_exl_proc * crk5_exl_proc_unpack(uint16_t *data);
void crk5_exl_proc_delete(struct crk5_exl_proc *exl);
struct crk5_exl_pinf *crk5_exl_pinf_unpack(uint16_t *data);
void crk5_exl_pinf_delete(struct crk5_exl_pinf *exl);
struct crk5_exl_err * crk5_exl_err_unpack(uint16_t *data);
void crk5_exl_err_delete(struct crk5_exl_err *exl);
struct crk5_exl_str * crk5_exl_str_unpack(uint16_t *data);
void crk5_exl_str_delete(struct crk5_exl_str *exl);
struct crk5_exl_dir * crk5_exl_dir_unpack(uint16_t *data);
void crk5_exl_dir_delete(struct crk5_exl_dir *exl);
struct crk5_exl_met * crk5_exl_met_unpack(uint16_t *data);
void crk5_exl_met_delete(struct crk5_exl_met *exl);
struct crk5_exl_rec * crk5_exl_rec_unpack(uint16_t *data);
void crk5_exl_rec_delete(struct crk5_exl_rec *exl);
struct crk5_exl_block * crk5_exl_block_unpack(uint16_t *data);
void crk5_exl_block_delete(struct crk5_exl_block *exl);
struct crk5_exl_mt * crk5_exl_mt_unpack(uint16_t *data);
void crk5_exl_mt_delete(struct crk5_exl_mt *exl);
struct crk5_exl_pi * crk5_exl_pi_unpack(uint16_t *data);
void crk5_exl_pi_delete(struct crk5_exl_pi *exl);
struct crk5_exl_cam * crk5_exl_cam_unpack(uint16_t *data);
void crk5_exl_cam_delete(struct crk5_exl_cam *exl);

#endif

// vim: tabstop=4 shiftwidth=4 autoindent
