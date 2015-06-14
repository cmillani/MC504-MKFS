#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "bash.fs_bsmode.h"
#include "bash.fs_parser.h"
#include "mkfs_struct.h"
#include "Stack/stack.h"

//#include "bash.fs_reader.h"

void enable_bash_mode(char fs_name[])
{
	bool halted = false;
	char buffer[3072] = {0};
	char * parameters[3];
	FILE * ufs;
	int read = 0;
	
	node *current_dir = NULL;
	
	ufs = fopen(fs_name, "rb+");
	
	//printf("Went bash mode\n");
	
	superblock  spb[1];
	inode inode[1];
	
	fread(spb, sizeof(superblock), 1, ufs);//Superblock read
	// printf("%d\n",spb->magic_number);
	//(inode curr_inode, FILE* ufs, int count, int blocksize)
	// int variable = (int)spb->root_inode*(int)spb->magic_number;
	printf("+++++++++++++++++++++++++++++\n");
	printf("Welcome to the UFS bash mode.\nFile active:\t%s\nBlock Size:\t%d\n",fs_name, spb->magic_number);
	printf("+++++++++++++++++++++++++++++\n");
	fseek(ufs, spb->root_inode*spb->magic_number, SEEK_SET);
	// printf("Pos: %d, should: %ld\n", spb->root_inode*spb->magic_number, ftell(ufs));
	fread(inode, sizeof(inode), 1, ufs);
	Push(&inode[0], &current_dir);
	// printf(">>>%s\n",inode->metadata.name);
	//arq_read(inode[0], ufs, 7, spb->magic_number);
	
	//fseek(ufs, SEEK_SET, spb->magic_number*1);
	
	//fread(inode, sizeof(inode), 1, ufs);
	// printf("Position Dir: %d Ind: %d\n", spb->root_dir*spb->magic_number, spb->root_inode*spb->magic_number);
	while (!halted)
	{
		Display(current_dir);
		parameters[0] = (char *) malloc (256*sizeof(char));
		parameters[1] = (char *) malloc (256*sizeof(char));
		parameters[2] = (char *) malloc (256*sizeof(char));
		if (fgets(buffer, 3072, stdin)) //TODO verificar se nao ultrapassa limite do vetor
		{
			//printf("%s\n", buffer);
			read = sscanf(buffer,"%s%s%s",parameters[0], parameters[1], parameters[2]);
			//printf ("Read>%d\n", read);
			// int i;
			// for (i = 0; i < read; i++)
			// {
				// printf("%s\n",parameters[i]);
			// }
			parse_bash_command(parameters, read, spb[0], &current_dir, ufs, buffer);
		}
	}
}