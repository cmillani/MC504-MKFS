#ifndef BASH_FS_PARSER
#define BASH_FS_PARSER

int parse_fs_command(char * command_list[], int arg_pos, int vec_sz);
int parse_bash_command(char * command_list[], int vec_sz);

#endif