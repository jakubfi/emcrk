//  Copyright (c) 2015 Jakub Filipowicz <jakubf@gmail.com>
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

#include "r40.h"
#include "obj.h"
#include "exl.h"

#define CRK5_EXL_MIN 128
#define CRK5_EXL_MAX 255

const struct crk5_exl crk5_exl_invalid = { CRK5_EXL_ARG_NONE, 0, NULL, NULL };

const struct crk5_exl crk5_exl_list[] = {
	{ /* 128 */ CRK5_EXL_ARG_FIL, CRK5_EXL_FIL_SIZE, "ASG", "Assign stream to file" },
	{ /* 129 */ CRK5_EXL_ARG_FIL, CRK5_EXL_FIL_SIZE, "CASG", "Create file and assign stream" },
	{ /* 130 */ CRK5_EXL_ARG_FIL, CRK5_EXL_FIL_SIZE, "SETP", "Set file parameters" },
	{ /* 131 */ CRK5_EXL_ARG_FIL, CRK5_EXL_FIL_SIZE, "LOAP", "Get file parameters" },
	{ /* 132 */ CRK5_EXL_ARG_TMEM, CRK5_EXL_TMEM_SIZE, "TMEM", "Add memory for process" },
	{ /* 133 */ CRK5_EXL_ARG_FIL, CRK5_EXL_FIL_SIZE, "NASG", "Add stream to file" },
	{ /* 134 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "ERF", "Remove file" },
	{ /* 135 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "ERS", "Remove stream" },
	{ /* 136 */ CRK5_EXL_ARG_NONE, 0, "ERAS", "Remove streams" },
	{ /* 137 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "FBOF", "Seek stream to beginning" },
	{ /* 138 */ CRK5_EXL_ARG_REC, CRK5_EXL_REC_SIZE, "INPR", "Read record" },
	{ /* 139 */ CRK5_EXL_ARG_REC, CRK5_EXL_REC_SIZE, "PRIR", "Write record" },
	{ /* 140 */ CRK5_EXL_ARG_REC, CRK5_EXL_REC_SIZE, "PINP", "Write 2-char and read record" },
	{ /* 141 */ CRK5_EXL_ARG_REC, CRK5_EXL_REC_SIZE, "PRIN", "Write record no ending char" },
	{ /* 142 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "EOF", "Write end char" },
	{ /* 143 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "FEOF", "Seek stream to end" },
	{ /* 144 */ CRK5_EXL_ARG_REC, CRK5_EXL_REC_SIZE, "INAM", "Get parameter from buffer" },
	{ /* 145 */ CRK5_EXL_ARG_REC, CRK5_EXL_REC_SIZE, "INUM", "Get number from buffer" },
	{ /* 146 */ CRK5_EXL_ARG_BLOCK, CRK5_EXL_BLOCK_SIZE, "WADR", "Write disk addresses" },
	{ /* 147 */ CRK5_EXL_ARG_BLOCK, CRK5_EXL_BLOCK_SIZE, "OVL", "Read overlay" },
	{ /* 148 */ CRK5_EXL_ARG_BLOCK, CRK5_EXL_BLOCK_SIZE, "REAP", "Read from special file" },
	{ /* 149 */ CRK5_EXL_ARG_BLOCK, CRK5_EXL_BLOCK_SIZE, "WRIP", "Write to process special file" },
	{ /* 150 */ CRK5_EXL_ARG_BLOCK, CRK5_EXL_BLOCK_SIZE, "READ", "Read block" },
	{ /* 151 */ CRK5_EXL_ARG_BLOCK, CRK5_EXL_BLOCK_SIZE, "WRIT", "Write block" },
	{ /* 152 */ CRK5_EXL_ARG_ERR, CRK5_EXL_ERR_SIZE, "OES", "Set own alarm handler" },
	{ /* 153 */ CRK5_EXL_ARG_NONE, 0, "ERR", "Handle last alarm" },
	{ /* 154 */ CRK5_EXL_ARG_R4, 0, "CORE", "Allocate memory" },
	{ /* 155 */ CRK5_EXL_ARG_FIL, CRK5_EXL_FIL_SIZE, "CPRF", "Create special file" },
	{ /* 156 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "JUMP", "Move process to a separate block" },
	{ /* 157 */ CRK5_EXL_ARG_DIR, CRK5_EXL_DIR_SIZE, "SDIR", "Set directory parameters" },
	{ /* 158 */ CRK5_EXL_ARG_DIR, CRK5_EXL_DIR_SIZE, "TDIR", "Get directory parameters" },
	{ /* 159 */ CRK5_EXL_ARG_DIR, CRK5_EXL_DIR_SIZE, "CDIR", "Change directory parameters" },
	{ /* 160 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "DEFP", "Define child process" },
	{ /* 161 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "DELP", "Remove child process" },
	{ /* 162 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "SREG", "Set child process registers" },
	{ /* 163 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "TREG", "Get child process registers" },
	{ /* 164 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "RUNP", "Start child process" },
	{ /* 165 */ CRK5_EXL_ARG_PROC, CRK5_EXL_PROC_SIZE, "HANG", "Stop child process" },
	{ /* 166 */ CRK5_EXL_ARG_R4, 0, "TERR", "Check children alarm list" },
	{ /* 167 */ CRK5_EXL_ARG_R4, 0, "WAIT", "Wait N quants", },
	{ /* 168 */ CRK5_EXL_ARG_R4, 0, "STOP", "Stop and wait N quants", },
	{ /* 169 */ CRK5_EXL_ARG_NONE, 0, "RELD", "Release character devices" },
	{ /* 170 */ CRK5_EXL_ARG_DAT, CRK5_EXL_DAT_SIZE, "DATE", "Get date" },
	{ /* 171 */ CRK5_EXL_ARG_TIM, CRK5_EXL_TIM_SIZE, "TIME", "Get time" },
	{ /* 172 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 173 */ CRK5_EXL_ARG_R4, 0, "CHPI", "Change priority",  },
	{ /* 174 */ CRK5_EXL_ARG_R4, 0, "WAIS", "Semaphore wait",  },
	{ /* 175 */ CRK5_EXL_ARG_R4, 0, "SIGN", "Semaphore signal",  },
	{ /* 176 */ CRK5_EXL_ARG_MET, CRK5_EXL_MET_SIZE, "TLAB", "Get disk label" },
	{ /* 177 */ CRK5_EXL_ARG_PINF, CRK5_EXL_PINF_SIZE, "PINF", "Get process info" },
	{ /* 178 */ CRK5_EXL_ARG_R4, 0, "CSUM", "Check OS control sum",  },
	{ /* 179 */ CRK5_EXL_ARG_R4, 0, "CSYS", "Change system" },
	{ /* 180 */ CRK5_EXL_ARG_R4, 0, "UNL", "Unloaddisk area" },
	{ /* 181 */ CRK5_EXL_ARG_R4, 0, "LOD", "Load disk area" },
	{ /* 182 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "TAKS", "Take stream semaphore" },
	{ /* 183 */ CRK5_EXL_ARG_STR, CRK5_EXL_STR_SIZE, "RELS", "Release stream semaphore" },
	{ /* 184 */ CRK5_EXL_ARG_R4, 0, "GMEM", "Add memory segmets" },
	{ /* 185 */ CRK5_EXL_ARG_R4, 0, "RMEM", "Free memory segments" },
	{ /* 186 */ CRK5_EXL_ARG_TABL, 0, "LRAM", "Get RAM file parameters" },
	{ /* 187 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 188 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 189 */ CRK5_EXL_ARG_PI, CRK5_EXL_PI_SIZE, "OPPI", "PI operation" },
	{ /* 190 */ CRK5_EXL_ARG_PI, CRK5_EXL_PI_SIZE, "WFPI", "Get PI interrupt" },
	{ /* 191 */ CRK5_EXL_ARG_CAM, CRK5_EXL_CAM_SIZE, "CAMAC", "CAMAC operation" },
	{ /* 192 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "RWMT", "Rewind tape to the beginning" },
	{ /* 193 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "FBMT", "Rewind tape by one file" },
	{ /* 194 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "FFMT", "Forward tape by one file" },
	{ /* 195 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "BBMT", "Rewind tape one block" },
	{ /* 196 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "BFMT", "Forward tape one block" },
	{ /* 197 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "FMMT", "Write file mark" },
	{ /* 198 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "REMT", "Read block from tape" },
	{ /* 199 */ CRK5_EXL_ARG_MT, CRK5_EXL_MT_SIZE, "WRMT", "Write block to tape" },
	{ /* 200 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 201 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 202 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 203 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 204 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 205 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 206 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 207 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 208 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 209 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 210 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 211 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 212 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 213 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 214 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 215 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 216 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 217 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 218 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 219 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 220 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 221 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 222 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 223 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 224 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 225 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 226 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 227 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 228 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 229 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 230 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 231 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 232 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 233 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 234 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 235 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 236 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 237 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 238 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 239 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 240 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 241 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 242 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 243 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 244 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 245 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 246 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 247 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 248 */ CRK5_EXL_ARG_NONE, 0, NULL, NULL },
	{ /* 249 */ CRK5_EXL_ARG_REJE, 0, "SCON", "Set XOSL state word bits" },
	{ /* 250 */ CRK5_EXL_ARG_REJE, 0, "TCON", "Get XOSL state word" },
	{ /* 251 */ CRK5_EXL_ARG_R4, 0, "END", "End program", },
	{ /* 252 */ CRK5_EXL_ARG_NONE, 0, "BACK", "Run in background" },
	{ /* 253 */ CRK5_EXL_ARG_R4, 0, "ABO", "Abort program" },
	{ /* 254 */ CRK5_EXL_ARG_R4, 0, "KILL", "Kill program" },
	{ /* 255 */ CRK5_EXL_ARG_R4, 0, "EOSL", "End program, output message" }
};

// -----------------------------------------------------------------------
const struct crk5_exl * crk5_get_exl(int num)
{
	if ((num < CRK5_EXL_MIN) || (num > CRK5_EXL_MAX)) return &crk5_exl_invalid;

	if (num == 172) return &crk5_exl_invalid;
	if ((num == 187) || (num == 188))  return &crk5_exl_invalid;
	if ((num > 199) && (num < 249)) return &crk5_exl_invalid;

	return crk5_exl_list + (num - CRK5_EXL_MIN);
}

// -----------------------------------------------------------------------
struct crk5_exl_fil * crk5_exl_fil_unpack(uint16_t *data)
{
	struct crk5_exl_fil *exl = malloc(sizeof(struct crk5_exl_fil));
	if (!exl) return NULL;

	exl->err = data[0];
	exl->stream_id = data[1];
	exl->type = data[2];
	exl->len = data[3];
	exl->param[0] = data[4];
	exl->param[1] = data[5];
	exl->rights = (data[6] & 0b1111110000000000) >> 10;
	exl->attributes = (data[6] & 0b0000001111110000) >> 4;
	exl->mem = (data[6] & 0b0000000000001111) >> 0;
	exl->obj = crk5_obj_unpack(data+7, &(exl->obj_type));

	return exl;
};

// -----------------------------------------------------------------------
void crk5_exl_fil_delete(struct crk5_exl_fil *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_tmem * crk5_exl_tmem_unpack(uint16_t *data)
{
	struct crk5_exl_tmem *exl = malloc(sizeof(struct crk5_exl_tmem));
	if (!exl) return NULL;

	exl->err = data[0];
	exl->stream_id = data[1];
	exl->addr = data[2];
	// data[3] - unused
	exl->seg = data[4];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_tmem_delete(struct crk5_exl_tmem *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_dat * crk5_exl_dat_unpack(uint16_t *data)
{
	struct crk5_exl_dat *exl = malloc(sizeof(struct crk5_exl_dat));
	if (!exl) return NULL;

	exl->year = data[0];
	exl->month = data[1];
	exl->day = data[2];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_dat_delete(struct crk5_exl_dat *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_tim * crk5_exl_tim_unpack(uint16_t *data)
{
	struct crk5_exl_tim *exl = malloc(sizeof(struct crk5_exl_tim));
	if (!exl) return NULL;

	exl->hour = data[0];
	exl->minute = data[1];
	exl->second = data[2];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_tim_delete(struct crk5_exl_tim *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_proc * crk5_exl_proc_unpack(uint16_t *data)
{
	struct crk5_exl_proc *exl = malloc(sizeof(struct crk5_exl_proc));
	if (!exl) return NULL;

	exl->err = data[0];
	exl->num = data[1];
	exl->ic = data[2];
	exl->r0 = data[3];
	exl->prio_sr = data[4];
	for (int i=0 ; i<7 ; i++) {
		exl->uregs[i] = data[i+5];
	}

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_proc_delete(struct crk5_exl_proc *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_pinf *crk5_exl_pinf_unpack(uint16_t *data)
{
	struct crk5_exl_pinf *exl = malloc(sizeof(struct crk5_exl_pinf));
	if (!exl) return NULL;

	exl->sys_ver_maj = (data[0] & 0b0000000001111111);
	exl->sys_ver_min = (data[0] & 0b1111111110000000) >> 7;
	exl->seg_total = data[1] >> 8;
	exl->user_rights = data[1] & 0xff;
	exl->prio = data[2];
	exl->spec_file_len = data[3];
	exl->entry_addr = data[4];
	exl->words_used = data[5];
	exl->attr = data[6];
	exl->space_name = r40_to_ascii(data+7, 1, NULL);
	exl->user_name = r40_to_ascii(data+8, 2, NULL);
	exl->proc_name = r40_to_ascii(data+10, 2, NULL);

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_pinf_delete(struct crk5_exl_pinf *exl)
{
	if (!exl) return;
	free(exl->space_name);
	free(exl->user_name);
	free(exl->proc_name);
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_err * crk5_exl_err_unpack(uint16_t *data)
{
	struct crk5_exl_err *exl = malloc(sizeof(struct crk5_exl_err));
	if (!exl) return NULL;

	exl->proc_addr = data[0];
	exl->alarm_addr = data[1];
	exl->alarm_nr = data[2] & 0xff;

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_err_delete(struct crk5_exl_err *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_str * crk5_exl_str_unpack(uint16_t *data)
{
	struct crk5_exl_str *exl = malloc(sizeof(struct crk5_exl_str));
	if (!exl) return NULL;

	exl->err = data[0];
	exl->stream_id = data[1];

	return exl;
}
// -----------------------------------------------------------------------
void crk5_exl_str_delete(struct crk5_exl_str *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_dir * crk5_exl_dir_unpack(uint16_t *data)
{
	struct crk5_exl_dir *exl = malloc(sizeof(struct crk5_exl_dir));
	if (!exl) return NULL;

	exl->count = data[0];

	if (exl->count > 16) {
		exl->user_id = NULL;
		exl->dir_id = NULL;
	} else {
		exl->user_id = malloc(sizeof(uint16_t) * exl->count);
		exl->dir_id = malloc(sizeof(uint16_t) * exl->count);
		for (int i=0 ; i<exl->count ; i++) {
			exl->user_id[i] = data[1+i*2+0];
			exl->dir_id[i] = data[1+i*2+1];
		}
	}

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_dir_delete(struct crk5_exl_dir *exl)
{
	if (!exl) return;
	free(exl->user_id);
	free(exl->dir_id);
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_met * crk5_exl_met_unpack(uint16_t *data)
{
	struct crk5_exl_met *exl = malloc(sizeof(struct crk5_exl_met));
	if (!exl) return NULL;

	exl->disk_id = data[0];
	exl->disk_name = r40_to_ascii(data, 1, NULL);
	exl->dicdic = data[1];
	exl->fildic = data[2];
	exl->map = data[3];
	exl->len = data[4];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_met_delete(struct crk5_exl_met *exl)
{
	if (!exl) return;
	free(exl->disk_name);
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_rec * crk5_exl_rec_unpack(uint16_t *data)
{
	struct crk5_exl_rec *exl = malloc(sizeof(struct crk5_exl_rec));
	if (!exl) return NULL;

	exl->position = data[0];
	exl->stream_id = data[1];
	exl->buf_addr = data[2];
	exl->end_char = data[3] >> 8;
	exl->max_len = data[3] & 0xff;
	exl->prechars[0] = data[4] >> 8;
	exl->prechars[1] = data[4] & 0xff;
	exl->prechars[2] = 0;

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_rec_delete(struct crk5_exl_rec *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_block * crk5_exl_block_unpack(uint16_t *data)
{
	struct crk5_exl_block *exl = malloc(sizeof(struct crk5_exl_block));
	if (!exl) return NULL;

	exl->transmitted = data[0];
	exl->stream_id = data[1];
	exl->addr = data[2];
	exl->count = data[3];
	exl->start_sector = data[4];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_block_delete(struct crk5_exl_block *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_mt * crk5_exl_mt_unpack(uint16_t *data)
{
	struct crk5_exl_mt *exl = malloc(sizeof(struct crk5_exl_mt));
	if (!exl) return NULL;

	exl->tape_end = (data[0] >> 15) & 1;
	exl->tape_rev_start = (data[0] >> 14) & 1;
	exl->file_mark = (data[0] >> 13) & 1;
	exl->transmitted = data[0] & 0b0001111111111111;
	exl->unit_num = data[1];
	exl->addr = data[2];
	exl->len = data[3];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_mt_delete(struct crk5_exl_mt *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_pi * crk5_exl_pi_unpack(uint16_t *data)
{
	struct crk5_exl_pi *exl = malloc(sizeof(struct crk5_exl_pi));
	if (!exl) return NULL;

	exl->comm = data[0];
	exl->o = (data[1] & 0b1100000000000000) >> 14;
	exl->f = (data[1] & 0b0011100000000000) >> 11;
	exl->chan_num = (data[1] & 0b0000011100000000) >> 8;
	if (exl->f == 0) {
		exl->fc = (data[1] & 0b0000000000011100) >> 2;
	} else {
		exl->addr_kas = (data[1] & 0b0000000011110000) >> 4;
		exl->addr_pak = (data[1] & 0b0000000000001111);
	}

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_pi_delete(struct crk5_exl_pi *exl)
{
	if (!exl) return;
	free(exl);
}

// -----------------------------------------------------------------------
struct crk5_exl_cam * crk5_exl_cam_unpack(uint16_t *data)
{
	struct crk5_exl_cam *exl = malloc(sizeof(struct crk5_exl_cam));
	if (!exl) return NULL;

	exl->request = (data[0] & 0b0000111100000000) >> 8;
	exl->response = (data[0] & 0b1110000000000000) >> 13;
	exl->transmitted = (data[0] & 0b0001111111111111);
	exl->c = (data[1] & 0b1100000000000000) >> 14;
	exl->f = (data[1] & 0b0011111000000000) >> 9;
	exl->a = (data[1] & 0b0000000111100000) >> 5;
	exl->n = (data[1] & 0b0000000000011111);
	exl->addr = data[2];
	exl->count = data[3];

	return exl;
}

// -----------------------------------------------------------------------
void crk5_exl_cam_delete(struct crk5_exl_cam *exl)
{
	if (!exl) return;
	free(exl);
}

// vim: tabstop=4 shiftwidth=4 autoindent
