#ifndef BASH_CAT
#define BASH_CAT

#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include "../bash.fs_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cat_bash(inode curr_inode, const char target_name[], FILE * ufs, superblock spb);

#endif