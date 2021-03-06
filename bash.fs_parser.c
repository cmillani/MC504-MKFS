#include <stdbool.h>
#include <stdio.h>
#include "bash.fs_parser.h"
#include "mkfs_error.h"
#include "bash.fs_bsmode.h"
#include "mkfs_struct.h"
#include <string.h>
#include "bash_apps/package.h"
#include "Stack/stack.h"
#include "bash.fs_reader.h"
#include "bash_outside.h"

int parse_fs_command(char * command_list[], int arg_pos, int vec_sz) 
{
	int ret_val = 0;
	if (!strcmp(command_list[arg_pos], "-i"))
	{
		if (vec_sz - arg_pos < 4) parse_error(INVALID_ARGS_ERR);
			FILE * input = fopen(command_list[arg_pos + 1], "rb");
		FILE * ufs = fopen(command_list[arg_pos + 3], "rb+");
		minus_i(input, command_list[arg_pos + 2], ufs);
		ret_val = 3;
	}
	else if (!strcmp(command_list[arg_pos], "-o"))
	{
		if (vec_sz - arg_pos < 4) parse_error(INVALID_ARGS_ERR);
		FILE * output = fopen(command_list[arg_pos + 2], "wb");
		FILE * ufs = fopen(command_list[arg_pos + 3], "rb+");
		minus_o(output, command_list[arg_pos + 1], ufs);
		ret_val = 3;
	}
	else if (!strcmp(command_list[arg_pos], "-b"))
	{
		if (vec_sz - arg_pos < 2) parse_error(INVALID_ARGS_ERR);
		//printf("Should -b\n");
		//printf("%s\n",command_list[arg_pos+1]);
		enable_bash_mode(command_list[arg_pos + 1]);
		ret_val = 2;
	}
	else if (!strcmp(command_list[arg_pos], "-d"))
	{
		if (vec_sz - arg_pos < 2) parse_error(INVALID_ARGS_ERR);
		// printf("Vai\n");
		FILE * ufs = fopen(command_list[arg_pos + 1], "rb");
		minus_d(ufs);
		ret_val = 2;
	}
	else
	{
		parse_error(INVALID_ARGS_ERR);
	}
	return ret_val;
}
int parse_bash_command(char * command_list[], int vec_sz, superblock spb, node **head, FILE* ufs, char buffer[])
{
	int ret_val = 0;
	//printf("QTD>%d\n", vec_sz);
	int i = 0;
	//for(i = 0; i < vec_sz; i++) printf("%s\n", command_list[i])s;
	inode reloaded;
	inode_read(ViewLast(head)->id, ufs, spb.magic_number,spb.root_inode, &reloaded);
	
	if (!strcmp(command_list[0], "ls"))
	{
		if (vec_sz<=2)
		{
			ls_bash(reloaded, ufs, spb, command_list[1]);
		}
		else printf("Unknown command\n");
		// printf("Should ls\n");
	}
	else if (!strcmp(command_list[0], "chmod"))
	{
		if (vec_sz == 3)
		{
			bash_chmod(reloaded, command_list[1], command_list[2], spb, ufs);
		}
		else printf("Unknown command\n");
	}
	else if (!strcmp(command_list[0], "mkdir"))
	{
		// printf("Should mkdir\n");
		if (vec_sz == 2) 
		{
			mkdir_bash(reloaded,command_list[1],ufs,spb);
		}
		else printf("Unknown command\n");
		//printf("DEPOIS%s\n",command_list[1]);
	}
	else if (!strcmp(command_list[0], "chdir"))
	{
		if (vec_sz == 2)
		{
			chdir_bash(reloaded, command_list[1], head, ufs, spb);
		}
		else printf("Unknown command\n");
		// printf("Should chdir\n");
	}
	else if (!strcmp(command_list[0], "rm"))
	{
		rm_bash(reloaded, command_list[1], spb, ufs);
		// printf("Should rm\n");
	}
	else if (!strcmp(command_list[0], "echo"))
	{
		if (vec_sz == 3)
		{
			echo_bash(reloaded, ufs, spb, buffer);
		}
		else printf("Unknown command\n");
		// printf("Should echo\n");
	}
	else if (!strcmp(command_list[0], "cat"))
	{
		if (vec_sz == 2)
		{
			cat_bash(reloaded, command_list[1], ufs, spb);
		}
		else printf("Unknown command\n");
		// printf("Should cat\n");
	}
	else if (!strcmp(command_list[0], "exit"))
	{
		//printf("Should exit\n");
		exit_bash();
	}
	else
	{
		printf("Unknown command\n");
	}
	return ret_val;
}
