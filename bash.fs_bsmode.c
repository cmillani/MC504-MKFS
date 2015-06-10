#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "bash.fs_bsmode.h"
#include "bash.fs_parser.h"
#include "mkfs_struct.h"

void enable_bash_mode(char fs_name[])
{
	bool halted = false;
	char buffer[3072] = {0};
	char * parameters[3];
	FILE * ufs;
	int read = 0;
	ufs = fopen(fs_name, "rb");
	printf("Went bash mode\n");
	superblock  spb[1];
	inode inode[1];
	fread(spb, sizeof(superblock), 1, ufs);
	printf("%d\n",spb->magic_number);
	
	//fseek(ufs, SEEK_SET, spb->magic_number*1);
	
	//fread(inode, sizeof(inode), 1, ufs);
	printf("Position Dir: %d Ind: %d\n", spb->root_dir*spb->magic_number, spb->root_inode*spb->magic_number);
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