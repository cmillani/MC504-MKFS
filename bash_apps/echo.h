#ifndef BASH_ECHO
#define BASH_ECHO

#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include "../bash.fs_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void echo_bash(inode curr_inode, FILE * ufs, superblock spb, char buffer[]);

#endif