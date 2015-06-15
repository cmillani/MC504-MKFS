#include "bash_outside.h"
#include "bash_apps/package.h"
#include <math.h>

void minus_d(FILE * ufs)
{
	// printf("Aqui")
	uint8_t byte[] = {0};
	superblock  spb[1];
	fread(spb, sizeof(superblock), 1, ufs);
	int blocksize = spb->magic_number;
	int inodes = 0, i, j, blocks = 0;
	fseek(ufs, blocksize, SEEK_SET);
	for (i = 0; i < 128; i++) //1024 inodes, 1 inode per bit = 128 bytes (128*8 = 1024)
	{
		fread(byte, sizeof(uint8_t), 1, ufs);
		// printf("%d/",byte[0]);
		for (j = 7; j > -1; j--)
		{
			// printf(">%d/",j);
			if ((byte[0] & 1 << j))//If the bit is clear the inode is so
			{
				inodes++;
			}
		}
	}
	
	fseek(ufs, 2*blocksize, SEEK_SET);
	int blocks_number = ((1024*1024*24)-(spb->root_dir*blocksize))/blocksize;
	// printf(">%d, %d\n",blocks_number, spb->root_dir);
	for (i = 0; i < (int)ceil(blocks_number/8); i++)
	{
		fread(byte, sizeof(uint8_t), 1, ufs);
		// printf("%d/",byte[0]);
		for (j = 7; j > -1; j--)
		{
			if ((byte[0] & 1 << j))//If the bit is clear the inode is so
			{
				blocks++;
			}
		}
	}
	
	printf("Used Inodes: %i \nUsed Directories: %i \nUsed Data Blocks: %i\n", inodes, spb->dir_inode, blocks-1);
}

void minus_i(FILE * input, char path_inside[], FILE * ufs)
{
	superblock  spb[1];
	inode inode_one[1];
	node *current_dir = NULL;
	char * tok;
	int count = 0, i = 0;
	char buffer[strlen(path_inside)];
	fread(spb, sizeof(superblock), 1, ufs);
	// fwrite(frase, sizeof(char), sizeof(frase), output);
	fseek(ufs, spb->root_inode*spb->magic_number, SEEK_SET);
	fread(inode_one, sizeof(inode), 1, ufs);
	Push(&inode_one[0], &current_dir);
	
	// printf("%d<>%s\n",spb->magic_number, inode_one->metadata.name);
	
	strcpy(buffer, path_inside);
	
	inode reloaded;
	// inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
	
	tok = strtok(buffer, "/");
	while (tok != NULL)
	{
		count++;
		tok = strtok(NULL, "/");
	}
	// printf("%d\n",count);
	tok = strtok(path_inside, "/");
	for (i = 0; i < count-1; i++)
	{
		// printf(">%s<\n", tok);
		inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
		chdir_bash(reloaded, tok, &current_dir, ufs, *spb);
		// Display(current_dir);
		tok = strtok(NULL, "/");
	}
	// printf("last %s\n",tok);
	
	inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
	int blocksize = spb->magic_number;
	uint16_t children_list[1024] = {0};
	int done = 0;
	inode oldinode;
	
	
	uint8_t translate_buffer[blocksize];
	int k;
	for (k = 0; k < blocksize; k++)
	{
		translate_buffer[k] = 0;
	}
	int offset = frst_free_inode(ufs, blocksize);
	if(offset == -1 || offset > 1023) //Selected inode is not within the valid range
	{
		printf("No free inodes in UFS. Remove some files in order to execute this command\n");
		return;
	}

	inode_read(offset, ufs, blocksize, spb->root_inode, &oldinode);

	oldinode.metadata.type = ARQ_TYPE;
	oldinode.metadata.unix_time = (uint32_t)time(NULL);
	oldinode.metadata.permissions = (1 << READ_PERMISSION) | (1 << WRITE_PERMISSION);
	oldinode.metadata.parent = reloaded.id;
	strcpy((char *)&(oldinode.metadata.name[0]), tok); //There is no ", then the 2 parameter is the target
	inode_write(offset, ufs, blocksize, spb->root_inode, oldinode);
	int j = 0;
	while (!feof(input) && !ferror(input))
	{
		while (j < blocksize && !feof(input))
		{
			fread(&translate_buffer[j], sizeof(uint8_t), 1, input);
			j++;
		}
		
		// printf("+%s",translate_buffer);
		int newblock = frst_free_block(ufs, blocksize, spb->root_dir);
		int child = first_free_child(oldinode, ufs, *spb, blocksize, children_list);

		write_to_file(child, oldinode, newblock, translate_buffer, *spb, ufs);
		j = 0;
	}
	
	inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
	inode_read(oldinode.id, ufs, spb->magic_number, spb->root_inode, &oldinode);
	int freeblk = first_free_child(reloaded, ufs, *spb, blocksize, children_list);
	write_to_dir(freeblk, reloaded, oldinode.id, *spb, ufs);
}

void minus_o(FILE * output, char path_inside[], FILE * ufs)
{
	superblock  spb[1];
	inode inode_one[1];
	node *current_dir = NULL;
	char * tok;
	int count = 0, i = 0;
	char buffer[strlen(path_inside)];
	fread(spb, sizeof(superblock), 1, ufs);
	// fwrite(frase, sizeof(char), sizeof(frase), output);
	fseek(ufs, spb->root_inode*spb->magic_number, SEEK_SET);
	fread(inode_one, sizeof(inode), 1, ufs);
	Push(&inode_one[0], &current_dir);
	
	// printf("%d<>%s\n",spb->magic_number, inode_one->metadata.name);
	
	strcpy(buffer, path_inside);
	
	inode reloaded;
	// inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
	
	tok = strtok(buffer, "/");
	while (tok != NULL)
	{
		count++;
		tok = strtok(NULL, "/");
	}
	// printf("%d\n",count);
	tok = strtok(path_inside, "/");
	for (i = 0; i < count-1; i++)
	{
		// printf(">%s<\n", tok);
		inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
		chdir_bash(reloaded, tok, &current_dir, ufs, *spb);
		// Display(current_dir);
		tok = strtok(NULL, "/");
	}
	// printf("last %s\n",tok);
	
	inode_read(ViewLast(&current_dir)->id, ufs, spb->magic_number, spb->root_inode, &reloaded);
	int blocksize = spb->magic_number;
	uint16_t children_list[1024] = {0};
	int done = 0;
	inode inode_before, the_one;
	int freeblk = first_free_child(reloaded, ufs, *spb, blocksize, children_list);
	// printf("NOW %s\n",curr_inode.metadata.name);
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb->root_inode, &inode_before);
		// printf("%s<>%s\n", tok, (char *)&inode_before.metadata.name[0]);
		if (!strcmp(tok, (char *)&inode_before.metadata.name[0]))
		{
			done = 1;
			the_one = inode_before;
			break;
		}
	}
	if (done && the_one.metadata.type != ARQ_TYPE)
	{
		printf("Not a file\n");
		// printf("theone->%s\n",the_one.metadata.name);
		return;
	}
	else if (!done)
	{
		printf("File not found\n");
		return;
	}
	// printf("theoneFOUND->%s\n",the_one.metadata.name);
	freeblk = first_free_child(the_one, ufs, *spb, blocksize, children_list);
	uint8_t translate[blocksize];
	for (i = 0; i < blocksize; i++)
	{
		translate[i] = 0;
		// printf("'%c'",translate[i]);
	}
	for (i = 0; i < freeblk; i++)
	{
		fseek(ufs, spb->root_dir * blocksize, SEEK_SET);
		fseek(ufs, children_list[i] * blocksize, SEEK_CUR);
		// printf(">>>%d<<<\n", ftell(ufs));
		fread(translate, sizeof(uint8_t), blocksize, ufs);
		fwrite(translate, sizeof(uint8_t), blocksize, output);
	}
}