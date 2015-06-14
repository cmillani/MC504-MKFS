#include "chdir.h"
#include <string.h>


//void mkdir_bash(inode curr_inode, const char dir_name[], FILE * ufs, superblock spb) //TODO verificar permissoes

void chdir_bash(inode curr_inode, const char dir_name[], node **head, FILE * ufs, superblock spb)
{
	int blocksize = spb.magic_number;
	uint16_t children_list[1024] = {0};
	int i, done = 0;
	inode inode_before, the_one;
	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	if (!strcmp(dir_name, ".."))
	{
		if (curr_inode.id != 0)
		{
			Pop(head);
		}
		return;
	}
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &inode_before);
		if (!strcmp(dir_name, (char *)&inode_before.metadata.name[0]))
		{
			done = 1;
			the_one = inode_before;
			break;
		}
	}
	if (the_one.metadata.type != DIR_TYPE)
	{
		printf("Not a directory\n");
		return;
	}
	if (!done) printf("Directory not found\n");
	else
	{
		Push(&the_one, head);
	}
}