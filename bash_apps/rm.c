#include "rm.h"

void rm_bash(inode parent, char to_remove_name[], superblock spb, FILE * ufs)
{
	int blocksize = spb.magic_number;
	
	uint16_t children_list[1024] = {0};
	int i, done = 0;
	inode inode_before, to_remove;
	int freeblk = first_free_child(parent, ufs, spb, blocksize, children_list);
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &inode_before);
		if (!strcmp(to_remove_name, (char *)&inode_before.metadata.name[0]))
		{
			done = 1;
			to_remove = inode_before;
			break;
		}
	}
	if (!done) 
	{
		printf("Target not found\n");
		return;
	}
	
	uint16_t children[1024] = {0};
	clear_inode(ufs, blocksize, to_remove.id);
	int count = first_free_child(to_remove, ufs, spb, blocksize, children);
	if (to_remove.metadata.type == ARQ_TYPE)
	{
		for (i = 0; i < count; i++)
		{
			clear_block(ufs, blocksize, spb.root_dir, children[i]);
		}
	}
	if (count > BLK_PER_IND-4)
	{
		for (i = BLK_PER_IND-4; i < count; i++)
		{
			clear_block(ufs, blocksize, spb.root_dir, to_remove.blocks[i]);
		}
	}
	for (i = 0; i < BLK_PER_IND; i++)
	{
		to_remove.blocks[i] = 0;
	}
	inode_write(to_remove.id, ufs, blocksize, spb.root_inode, to_remove);
	remove_from_dir(to_remove.id, parent, ufs, spb);
}