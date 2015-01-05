#include <inttypes.h>
#include <stdlib.h>

#define CRK5_SPACE_STR 0x79f2
#define CRK5_SPACE_DEV 0x19de
#define CRK5_SPAVE_MES 0x521b
#define CRK5_SPAVE_RAM 0x70b5

enum crk5_obj_types {
	CRK5_OBJ_INVALID,
	CRK5_OBJ_STREAM,
	CRK5_OBJ_DISK,
	CRK5_OBJ_FILE,
	CRK5_OBJ_DEVICE,
	CRK5_OBJ_MESSAGE,
	CRK5_OBJ_MEM,
};

struct crk5_obj_stream {
	uint16_t id;
};

struct crk5_obj_disk {
	uint16_t space;
	char *space_name;
	uint16_t start;
	uint16_t end;
};

struct crk5_obj_file {
	char *space_name;
	char *dir_name;
	char *file_name;
};

struct crk5_obj_dev {
	int chan;
	int unit;
};

struct crk5_obj_mes {
	uint16_t addr;
	int len;
	uint16_t receiver;
};

struct crk5_obj_mem {
	char *name;
};

void * crk5_obj_unpack(uint16_t *data, int *type);

struct crk5_obj_stream * crk5_obj_stream_unpack(uint16_t *data);
void crk5_obj_stream_delete(struct crk5_obj_stream *obj);
struct crk5_obj_disk * crk5_obj_disk_unpack(uint16_t *data);
void crk5_obj_disk_delete(struct crk5_obj_disk *obj);
struct crk5_obj_file * crk5_obj_file_unpack(uint16_t *data);
void crk5_obj_file_delete(struct crk5_obj_file *obj);
struct crk5_obj_dev * crk5_obj_dev_unpack(uint16_t *data);
void crk5_obj_dev_delete(struct crk5_obj_dev *obj);
struct crk5_obj_mes * crk5_obj_mes_unpack(uint16_t *data);
void crk5_obj_mes_delete(struct crk5_obj_mes *obj);
struct crk5_obj_mem * crk5_obj_mem_unpack(uint16_t *data);
void crk5_obj_mem_delete(struct crk5_obj_mem *obj);

// vim: tabstop=4 shiftwidth=4 autoindent
