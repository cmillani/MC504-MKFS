#include "ls.h"
#include "../bash.fs_reader.h"
#include "../mkfs_struct.h"
#include <time.h>
#include <string.h>


void ls_bash(inode curr_inode, FILE * ufs, superblock spb, char extra[])
{
	uint16_t children[1024] = {0};
	uint16_t aux[1024] = {0};
	int toprint = first_free_child(curr_inode, ufs, spb, spb.magic_number, children);
	// printf("will ls %d children\n", toprint);
	inode to_ls;
	int i;
	if (extra && !strcmp(extra, "-l"))
	{
		printf("NAME:\t\tSize:\tPermissions:\tTimeStamp:\n");
		for (i = 0; i < toprint; i++)
		{
			inode_read(children[i], ufs, spb.magic_number, spb.root_inode, &to_ls);
			printf("%s\t\t",to_ls.metadata.name);
			
			//PRINTA SIZE
			int children_qtd = first_free_child(to_ls, ufs, spb, spb.magic_number, aux);
			printf("%d\t", children_qtd * spb.magic_number);
			
			if (to_ls.metadata.permissions & (1 << READ_PERMISSION)) printf("R");
			if (to_ls.metadata.permissions & (1 << WRITE_PERMISSION)) printf("W");
			if (to_ls.metadata.permissions & (1 << EXEC_PERMISSION)) printf("E");
			printf("\t\t");
			printf("%d\n", to_ls.metadata.unix_time);
		}
	}
	else if (extra[0] == '\0')
	{
		for (i = 0; i < toprint; i++)
		{
			inode_read(children[i], ufs, spb.magic_number, spb.root_inode, &to_ls);
			printf("%s\t\t",to_ls.metadata.name);
			if ((i+1)%6 == 0) printf("\n");
		}
	}
	else printf("Unknown command\n");
	// printf(">%s<",extra);
	printf("\n");
}