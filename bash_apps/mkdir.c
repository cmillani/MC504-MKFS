#include "mkdir.h"
#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include "../bash.fs_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void mkdir_bash(inode curr_inode, const char dir_name[], FILE * ufs, superblock spb) //TODO verificar permissoes
{
	if (!(curr_inode.metadata.permissions & (1 << WRITE_PERMISSION)))
	{
		printf("Not enough permissions\n");
		return;
	}
	uint16_t children_list[1024] = {0};
	int i;
	int blocksize = spb.magic_number;
	inode comparer;
	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &comparer);
		if (!strcmp((char *)&comparer.metadata.name[0], dir_name))
		{
			printf("A directory with this name already exists\n");
			return;
		}
	}
	//printf("%s\n",dir_name);
	spb.dir_inode++;
	update_spb(spb, ufs);
	
	inode oldinode;
	
	int offset = frst_free_inode(ufs, blocksize);
	
	if(offset == -1 || offset > 1023) //Selected inode is not within the valid range
	{
		printf("No free inodes in UFS. Remove some files in order to execute this command\n");
		return;
	}
	int pos_saver = ftell(ufs);
	
	// fseek(ufs, spb.root_inode * blocksize, SEEK_SET);
	// fseek(ufs, offset, SEEK_CUR);
	// int temp = ftell(ufs);
	// fread(oldinode, sizeof(inode), 1, ufs);
	inode_read(offset, ufs, blocksize, spb.root_inode, &oldinode);
	
	
	// printf("OFF:%d\t%d<>%d\n", offset, oldinode.id, curr_inode.id);
	// printf("OFF:%d\t%s<>%s\n", offset, oldinode.metadata.name, curr_inode.metadata.name);
	
	strcpy((char *)&(oldinode.metadata.name[0]), dir_name);
	
	oldinode.metadata.type = DIR_TYPE;
	oldinode.metadata.unix_time = (uint32_t)time(NULL);
	oldinode.metadata.permissions = (1 << READ_PERMISSION) | (1 << WRITE_PERMISSION);
	oldinode.metadata.parent = curr_inode.id;
	
	//printf("%s\n",oldinode.metadata.name);
	
	inode_write(offset, ufs, blocksize, spb.root_inode, oldinode);
	
	
	// uint16_t children_list[1024] = {0};
// 	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	// printf("FREE_IND_BLK = %d\n",freeblk);
	write_to_dir(freeblk, curr_inode, oldinode.id, spb, ufs);
	// printf("FREE:%d\n",freeblk);
	
	fseek(ufs, pos_saver, SEEK_SET);
}