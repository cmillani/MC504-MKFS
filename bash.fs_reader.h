#ifndef BASH_READER
#define BASH_READER

#include "mkfs_struct.h"
#include <stdio.h>
#include <stdlib.h>

int inode_read(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk, inode* read_one);
int inode_write(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk, inode to_write);
int first_free_child(inode curr_inode, FILE * ufs, superblock spb, int blocksize, uint16_t *children_list);
int write_to_dir(int child_nbr, inode inode, uint16_t new_inode_id, superblock spb, FILE * ufs);
int write_to_file(int child_nbr, inode inode, uint16_t newblock_nbr, uint8_t block[], superblock spb, FILE * ufs);

#endif