#ifndef BLOCK_READER
#define BLOCK_READER

#include "mkfs_struct.h"
#include <stdio.h>
#include <stdlib.h>

inode * dir_read(inode curr_inode, FILE* ufs, int count, int blocksize, uint16_t first_inode_blk);
uint16_t * arq_read(inode curr_inode, FILE* ufs, int count, int blocksize);

#endif