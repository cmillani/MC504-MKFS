#ifndef MKFS_STRUCT
#define MKFS_STRUCT

#include <stdint.h>

void setbit(uint8_t byte, uint8_t bit);
void clearbit(uint8_t byte, uint8_t bit);

#define DIR_TYPE 0
#define ARQ_TYPE 1
#define READ_PERMISSION 2
#define WRITE_PERMISSION 1
#define EXEC_PERMISSION 0

#define BLK_PER_IND 16

typedef uint8_t byte;

typedef struct superblock
{
	uint16_t magic_number;
	uint16_t root_inode; //block number
	uint16_t root_dir; //block number
	uint16_t dir_inode;
	uint16_t arq_inode;
}superblock;
typedef struct metadata
{
	uint32_t unix_time;
	uint8_t permissions;
	uint8_t name[256];
	uint16_t parent; //The id of the parent inode
	uint8_t type;
}metadata;

typedef struct inode
{
	uint16_t id;
	uint16_t blocks[BLK_PER_IND];
	metadata metadata;	
}inode;

#endif