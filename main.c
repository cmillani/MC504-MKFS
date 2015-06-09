#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mkfs_error.h"
#include "mkfs_parser.h"
#include "mkfs_create.h"

char * filename;


int main (int argc, char* argv[])
{
	bool out_name_defined = false;
	int i, block_size;
	FILE * ufs;

	for  (i = 1; i < argc; i++) //Reads and interprets the arguments
	{
		if (argv[i][0] == '-') 
		{
			i += parse_command(argv, i, argc);
		}
		else if (!out_name_defined)
		{
			filename = argv[i];
			out_name_defined = true;
		}
		else
		{
			parse_error(INVALID_ARGS_ERR);
		}
	}
	if (!out_name_defined)
	{
		parse_error(NO_NAME_ERR);
	}
	if (get_block_size() == -1)
	{
		parse_error(NO_BLOCK_SIZE_ERR);
	}
	
	block_size = get_block_size();
	
	ufs = mkfs_create(filename);
	
	fseek(ufs, 0, SEEK_SET);
	mkfs_set_fs(ufs, block_size);
	
	printf("%lf\n", ((ftell(ufs))/((24.*1024*1024))*100));
	
	fclose(ufs);
	
	return 0;
}