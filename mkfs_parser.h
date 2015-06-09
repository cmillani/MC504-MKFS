#ifndef MKFS_PARSER
#define MKFS_PARSER

int parse_command(char * command_list[], int arg_pos, int vec_sz); //Parses the command and all its arguments and returns the number of arguments it needed during the process
int get_block_size(void);

#endif