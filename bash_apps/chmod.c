#include "chmod.h"
#include <string.h>

void bash_chmod(inode curr_inode, char target[], char mode[], superblock spb, FILE * ufs)
{
	int newmode = atoi(mode);
	
	int blocksize = spb.magic_number;
	uint16_t children_list[1024] = {0};
	int i, done = 0;
	inode inode_before, the_one;
	
	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &inode_before);
		if (!strcmp(target, (char *)&inode_before.metadata.name[0]))
		{
			done = 1;
			the_one = inode_before;
			break;
		}
	}
	if (!done) printf("File not found\n");
	else
	{
		the_one.metadata.permissions = (newmode%10 << EXEC_PERMISSION)|((newmode/10)%10 << WRITE_PERMISSION)|((newmode/100)%10 << READ_PERMISSION);
		inode_write(the_one.id, ufs, spb.magic_number, spb.root_inode, the_one);
	}
}