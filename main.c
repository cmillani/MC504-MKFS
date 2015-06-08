#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

char few_args[] = "Error: block size must be provided\n"; //TODO implementar -h
#define FEW_ARGS_ERR 1
char invalid_args[] = "Error: invalid command\n"; //TODO implementar -h
#define INVALID_ARGS_ERR 2

char * filename;
int block_size;

int parse_command(char * command_list[], int arg_pos) //Parses the command and all its arguments and returns the number of arguments it needed during the process
{
	int ret_val = 0;
	if (!strcmp(command_list[arg_pos], "-b"))
	{
		int pre_block_size;
		char data_type[3];
		sscanf(command_list[arg_pos+1],"%d%s", &pre_block_size, data_type);
		if (!strcmp(data_type, "KB"))
		{
			block_size = pre_block_size * 1024;
		}
		else if (!strcmp(data_type, "B"))
		{
			block_size = pre_block_size;
		}
		else
		{
			fprintf(stderr, "%s", invalid_args);
			exit(INVALID_ARGS_ERR);
		}
		ret_val = 1;
	}
	return ret_val;
}
void parse_error(int error_type)
{
	fprintf(stderr, "%s", invalid_args);
}

int main (int argc, char* argv[])
{
	bool out_name_defined = false;
	int i;
	if (argc < 4)//Is there enough arguments to create the UFS?
	{
		fprintf(stderr, "%s", few_args);
		exit(FEW_ARGS_ERR);
	}
	
	for  (i = 1; i < argc; i++) //Reads and interprets the arguments
	{
		if (argv[i][0] == '-') 
		{
			i += parse_command(argv, i);
		}
		else if (!out_name_defined)
		{
			filename = argv[i];
		}
		else
		{
			fprintf(stderr, "%s", invalid_args);
			exit(INVALID_ARGS_ERR);
		}
	}
}