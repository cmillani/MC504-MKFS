#ifndef BITMAP_READER
#define BITMAP_READER

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int frst_free_inode(FILE * ufs, int blocksize);
int frst_free_block(FILE * ufs, int blocksize, int frst_data_blk);

#endif