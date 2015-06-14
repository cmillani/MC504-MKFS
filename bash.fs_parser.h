#ifndef BASH_FS_PARSER
#define BASH_FS_PARSER

#include "Stack/stack.h"
#include "mkfs_struct.h"

int parse_fs_command(char * command_list[], int arg_pos, int vec_sz);
int parse_bash_command(char * command_list[], int vec_sz, superblock spb, node **head, FILE* ufs, char buffer[]);

#endif