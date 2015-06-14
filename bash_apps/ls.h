#ifndef BASH_LS
#define BASH_LS

#include "../mkfs_struct.h"
#include <stdio.h>
#include <stdlib.h>

void ls_bash(inode curr_inode, FILE * ufs, superblock spb, char extra[]);

#endif