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
	uint16_t param1;
	uint16_t param2;
	int rights;
	int attributes;
	int mem;
	char *disk;
	char *dir;
	char *file;
};

struct crk5_exl_tmem {
	uint16_t err;
	uint16_t stream_id;
	uint16_t addr;
	uint16_t unused;
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




// vim: tabstop=4 shiftwidth=4 autoindent
