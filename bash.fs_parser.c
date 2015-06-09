#include <stdbool.h>
#include <stdio.h>
#include "bash.fs_parser.h"
#include "mkfs_error.h"
#include "bash.fs_bsmode.h"
#include <string.h>

int parse_fs_command(char * command_list[], int arg_pos, int vec_sz) 
{
	int ret_val = 0;
	if (!strcmp(command_list[arg_pos], "-i"))
	{
		if (vec_sz - arg_pos < 4) parse_error(INVALID_ARGS_ERR);
		printf("Should -i\n");
		ret_val = 3;
	}
	else if (!strcmp(command_list[arg_pos], "-o"))
	{
		if (vec_sz - arg_pos < 4) parse_error(INVALID_ARGS_ERR);
		printf("Should -o\n");
		ret_val = 3;
	}
	else if (!strcmp(command_list[arg_pos], "-b"))
	{
		if (vec_sz - arg_pos < 2) parse_error(INVALID_ARGS_ERR);
		printf("Should -b\n");
		enable_bash_mode(command_list[1]);
		ret_val = 2;
	}
	else if (!strcmp(command_list[arg_pos], "-d"))
	{
		if (vec_sz - arg_pos < 2) parse_error(INVALID_ARGS_ERR);
		printf("Should -d\n");
		ret_val = 2;
	}
	else
	{
		parse_error(INVALID_ARGS_ERR);
	}
	return ret_val;
}
int parse_bash_command(char * command_list[], int vec_sz)
{
	int ret_val = 0;
	//printf("QTD>%d\n", vec_sz);
	int i = 0;
	//for(i = 0; i < vec_sz; i++) printf("%s\n", command_list[i])s;
	if (!strcmp(command_list[0], "ls"))
	{
		printf("Should ls\n");
	}
	else if (!strcmp(command_list[0], "chmod"))
	{
		printf("Should chmod\n");
	}
	else if (!strcmp(command_list[0], "mkdir"))
	{
		printf("Should mkdir\n");
	}
	else if (!strcmp(command_list[0], "chdir"))
	{
		printf("Should chdir\n");
	}
	else if (!strcmp(command_list[0], "rm"))
	{
		printf("Should rm\n");
	}
	else if (!strcmp(command_list[0], "echo"))
	{
		printf("Should echo\n");
	}
	else if (!strcmp(command_list[0], "cat"))
	{
		printf("Should cat\n");
	}
	else if (!strcmp(command_list[0], "exit"))
	{
		printf("Should exit\n");
	}
	else
	{
		parse_error(INVALID_ARGS_ERR);
	}
	return ret_val;
}
