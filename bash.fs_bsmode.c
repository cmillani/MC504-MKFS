#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "bash.fs_bsmode.h"
#include "bash.fs_parser.h"

void enable_bash_mode(char fs_name[])
{
	bool halted = false;
	char buffer[3072] = {0};
	char * parameters[3];
	FILE * ufs;
	int read = 0;
	//fopen ()
	
	while (!halted)
	{
		parameters[0] = (char *) malloc (256*sizeof(char));
		parameters[1] = (char *) malloc (256*sizeof(char));
		parameters[2] = (char *) malloc (256*sizeof(char));
		if (fgets(buffer, 3072, stdin)) //TODO verificar se nao ultrapassa limite do vetor
		{
			printf("%s\n", buffer);
			read = sscanf(buffer,"%s%s%s",parameters[0], parameters[1], parameters[2]);
			printf ("Read>%d\n", read);
			int i;
			for (i = 0; i < read; i++)
			{
				printf("%s\n",parameters[i]);
			}
			parse_bash_command(parameters, read);
		}
	}
}