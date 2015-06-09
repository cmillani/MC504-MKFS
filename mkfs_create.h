#ifndef MKFS_CREATE
#define MKFS_CREATE

FILE* mkfs_create(char filename[]);

int mkfs_set_fs(FILE * output, int blocksize);

int mkfs_wr_superblk(FILE * output, int blocksize);

int mkfs_wr_inode_bmp(FILE * output, int blocksize);
	
int mkfs_wr_block_bmp(FILE * output, int blocksize);

int mkfs_wr_inodes(FILE * output, int blocksize);

#endif