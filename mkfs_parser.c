#include "mkfs_parser.h"
#include "mkfs_error.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int block_size = -1;

bool blk_sz_isdef = false;

int parse_command(char * command_list[], int arg_pos, int vec_sz) //Parses the command and all its arguments and returns the number of arguments it needed during the process
{
	int ret_val = 0;
	if (!strcmp(command_list[arg_pos], "-b"))
	{
		if (arg_pos + 1 >= vec_sz) parse_error(MISSING_PAR_ERR);
		int pre_block_size;
		char data_type[3];
		if (sscanf(command_list[arg_pos+1],"%d%s", &pre_block_size, data_type) != 2) //TODO verificar se leitura foi ok
		{
			parse_error(INVALID_ARGS_ERR);
		}
		if (!strcmp(data_type, "KB"))
		{
			block_size = pre_block_size * 1024;
			blk_sz_isdef = true;
		}
		else if (!strcmp(data_type, "B"))
		{
			block_size = pre_block_size;
			blk_sz_isdef = true;
		}
		else
		{
			parse_error(INVALID_ARGS_ERR);
		}
		if (block_size < 512 || block_size > 4*1024) 
		{
			parse_error(BLOCK_SZ_ERR);
		}
		ret_val = 1;
	}
	return ret_val;
}

int get_block_size(void)
{
	if (blk_sz_isdef) return block_size;
	else parse_error(NO_BLOCK_SIZE_ERR);
	return -1;
}
