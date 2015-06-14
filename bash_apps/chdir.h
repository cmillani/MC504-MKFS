#ifndef BASH_CHDIR
#define BASH_CHDIR

#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include "../bash.fs_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Stack/stack.h"

void chdir_bash(inode curr_inode, const char dir_name[], node **head, FILE * ufs, superblock spb);

#endif