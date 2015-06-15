#include "cat.h"

void cat_bash(inode curr_inode, const char target_name[], FILE * ufs, superblock spb)
{
	int blocksize = spb.magic_number;
	int initial_pos = ftell(ufs);
	uint16_t children_list[1024] = {0};
	int i, done = 0;
	inode inode_before, the_one;
	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &inode_before);
		if (!strcmp(target_name, (char *)&inode_before.metadata.name[0]))
		{
			done = 1;
			the_one = inode_before;
			break;
		}
	}
	// printf("Will cat %s\n", the_one.metadata.name);
	if (the_one.metadata.type == DIR_TYPE)
	{
		printf("Is a directory\n");
		return;
	}
	if (!done) printf("File not found\n");
	else
	{
		int print_lim = first_free_child(the_one, ufs, spb, blocksize, children_list);
		// printf("Numb of blocks = %d\n", print_lim);
		uint8_t block[blocksize];
		// int j;
		for (i = 0; i < print_lim; i++)
		{
			// printf("will print stuff\n");
			fseek(ufs, spb.root_dir * blocksize, SEEK_SET);
			fseek(ufs, children_list[i] * blocksize, SEEK_CUR);
			// printf(">>>%d<<<\n", ftell(ufs));
			fread(block, sizeof(uint8_t), blocksize, ufs);
			// for (j = 0; j < blocksize; j++) printf("'%c'", block[j]);
			printf("%s",block);
		}
	}
	printf("\n");
}