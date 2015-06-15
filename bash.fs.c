/**********************************************************
RA 145654 Carlos Eduardo Millani
RA 146906 Letícia da Silva Gonçalves
**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mkfs_struct.h"
#include "mkfs_error.h"
#include "bash.fs_parser.h"

int main(int argc, char * argv[])
{
	int i;
	if (argc < 3) //The command with the less parameters requires 3 arguments
	{
		parse_error(INVALID_ARGS_ERR);
	}
	for  (i = 1; i < argc; i++) //Reads and interprets the arguments
	{
		if (argv[i][0] == '-') 
		{
			i += parse_fs_command(argv, i, argc);
		}
		else
		{
			parse_error(INVALID_ARGS_ERR);
		}
	}
	return 0;
}