#include "bash_outside.h"
#include "bash_apps/package.h"

void minus_i(FILE * input, char path_inside[], FILE * ufs)
{

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