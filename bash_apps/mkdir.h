#ifndef BASH_MKDIR
#define BASH_MKDIR

#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include <stdio.h>
#include <stdlib.h>

void mkdir_bash(inode curr_inode, const char dir_name[], FILE * ufs, superblock spb);

#endif