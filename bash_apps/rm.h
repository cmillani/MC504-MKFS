#ifndef BASH_RM
#define BASH_RM

#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include "../bash.fs_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rm_bash(inode parent, char to_remove_name[], superblock spb, FILE * ufs);

#endif