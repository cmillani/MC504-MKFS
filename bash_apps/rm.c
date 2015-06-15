#include "rm.h"

int rm_bash(inode parent, char to_remove_name[], superblock spb, FILE * ufs)
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
		return 0;
	}
	
	if (!(to_remove.metadata.permissions & (1 << WRITE_PERMISSION))) 
	{
		printf("You don`t have the permission to write to the file \"%s\"\n", to_remove.metadata.name);
		return 0;
	}
	
	uint16_t children[1024] = {0};
	int count = first_free_child(to_remove, ufs, spb, blocksize, children);
	if (to_remove.metadata.type == ARQ_TYPE)
	{
		for (i = 0; i < count; i++)
		{
			clear_block(ufs, blocksize, spb.root_dir, children[i]);
		}
	}
	else // Should remove children too
	{
		spb.dir_inode--;
		update_spb(spb, ufs);
		
		int try = ftell(ufs);
		
		fseek(ufs, 0, SEEK_SET);
		fread(&spb, sizeof(superblock), 1, ufs);//Reloads superblock
		
		fseek(ufs, try, SEEK_SET);
		
		inode recursive;
		int fail = 0;
		for (i = 0; i < count; i++)
		{
			inode_read(children[i], ufs, blocksize, spb.root_inode, &recursive);
			// printf("%s<<\n",recursive.metadata.name);
			if (!rm_bash(to_remove, (char *)&recursive.metadata.name[0], spb, ufs)) fail = 1;
		}
		if (fail) return 0;
	}
	clear_inode(ufs, blocksize, to_remove.id);
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
	
	return 1;
}