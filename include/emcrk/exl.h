#include <stdlib.h>

enum crk5_exl_types {
	CRK5_EXL_INVALID,
	CRK5_EXL_NONE,
	CRK5_EXL_FIL,
	CRK5_EXL_TMEM,
	CRK5_EXL_STR,
	CRK5_EXL_REC,
	CRK5_EXL_BLOCK,
	CRK5_EXL_ERR,
	CRK5_EXL_R4,
	CRK5_EXL_PROC,
	CRK5_EXL_DIR,
	CRK5_EXL_DAT,
	CRK5_EXL_TIM,
	CRK5_EXL_MET,
	CRK5_EXL_PINF,
	CRK5_EXL_TABL,
	CRK5_EXL_PI,
	CRK5_EXL_CAM,
	CRK5_EXL_MT,
	CRK5_EXL_REJE,
};

struct crk5_exl {
	int type;
	char *name;
	char *desc;
};

struct crk5_exl_fil {
	uint16_t err;
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
	uint16_t err;
	uint16_t stream_id;
	uint16_t addr;
	uint16_t block_num;
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
	uint16_t err;
	uint16_t num;
	uint16_t ic;
	uint16_t r0;
	uint16_t prio_state;
	uint16_t uregs[7];
};

struct crk5_exl_pinf {
	uint16_t gen;
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
	uint16_t err;
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
	uint16_t dicdic_addr;
	uint16_t fildic_addr;
	uint16_t mapa_addr;
	uint16_t len;
};

struct crk5_exl_rec {
	uint16_t char_n;
	uint16_t stream_id;
	uint16_t buf_addr;
	uint8_t end_char;
	uint8_t len;
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
void crk4_exl_str_delete(struct crk5_exl_str *exl);
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


// vim: tabstop=4 shiftwidth=4 autoindent
