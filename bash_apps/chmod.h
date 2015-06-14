#ifndef BASH_CHMOD
#define BASH_CHMOD

#include "../mkfs_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include "../bash.fs_reader.h"

void bash_chmod(inode curr_inode, char target[], char mode[], superblock spb, FILE * ufs);

#endif