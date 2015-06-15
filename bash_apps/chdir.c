#include "chdir.h"
#include <string.h>


//void mkdir_bash(inode curr_inode, const char dir_name[], FILE * ufs, superblock spb) //TODO verificar permissoes

int chdir_bash(inode curr_inode, const char dir_name[], node **head, FILE * ufs, superblock spb)
{
	int blocksize = spb.magic_number;
	uint16_t children_list[1024] = {0};
	int i, done = 0;
	inode inode_before, the_one;
	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	// printf("NOW %s\n",curr_inode.metadata.name);
	if (!strcmp(dir_name, ".."))
	{
		if (curr_inode.id != 0)
		{
			Pop(head);
		}
		return 1;
	}
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &inode_before);
		// printf("%s<>%s\n", dir_name, (char *)&inode_before.metadata.name[0]);
		if (!strcmp(dir_name, (char *)&inode_before.metadata.name[0]))
		{
			done = 1;
			the_one = inode_before;
			break;
		}
	}
	if (done && the_one.metadata.type != DIR_TYPE)
	{
		printf("Not a directory\n");
		// printf("theone->%s\n",the_one.metadata.name);
		return 0;
	}
	if (!done) 
	{
		printf("Directory not found\n");
		return 0;
	}
	else
	{
		Push(&the_one, head);
		return 1;
	}
	return 1;
}