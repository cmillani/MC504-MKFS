#ifndef BLOCK_READER
#define BLOCK_READER

#include "mkfs_struct.h"
#include <stdio.h>
#include <stdlib.h>

inode * dir_read(inode curr_inode, FILE* ufs, int count, int blocksize, uint16_t first_inode_blk);

int inode_read(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk, inode* read_one);
int inode_write(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk, inode to_write);
int first_free_child(inode curr_inode, FILE * ufs, superblock spb, int blocksize, uint16_t *children_list);

#endif