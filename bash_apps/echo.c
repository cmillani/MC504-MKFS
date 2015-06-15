#include "echo.h"
#include "rm.h"

void echo_bash(inode curr_inode, FILE * ufs, superblock spb, char buffer[])
{
	if (!(curr_inode.metadata.permissions & (1 << WRITE_PERMISSION)))
	{
		printf("Not enough permissions\n");
		return;
	}
	//Reads and initializes the new inode
	int blocksize = spb.magic_number;
	inode oldinode;
	uint16_t children_list[1024] = {0}; //To use in order to get the first free child in the parent inode
	uint8_t block[4096] = {0}; //Buffer
	uint8_t the_name[256] = {0}; //To read the file name
	int overflow = 0;

	int offset = frst_free_inode(ufs, blocksize);

	if(offset == -1 || offset > 1023) //Selected inode is not within the valid range
	{
		printf("No free inodes in UFS. Remove some files in order to execute this command\n");
		return;
	}
	int pos_saver = ftell(ufs);

	inode_read(offset, ufs, blocksize, spb.root_inode, &oldinode);

	oldinode.metadata.type = ARQ_TYPE;
	oldinode.metadata.unix_time = (uint32_t)time(NULL);
	oldinode.metadata.permissions = (1 << READ_PERMISSION) | (1 << WRITE_PERMISSION);
	oldinode.metadata.parent = curr_inode.id;
	int pos_in_buffer = 5; //5 = "echo " size
	int start_writing = 0;
	// printf(">>%c\n",buffer[pos_in_buffer]);
	if (buffer[pos_in_buffer] != '\"') //No "
	{
		sscanf(&buffer[pos_in_buffer],"%s",&block[0]);
		// int j = 0;
		// for(j = 0; j < blocksize; j++)printf("%c",block[j]);
		pos_in_buffer += strlen((char *)&block[0]) + 1;
		// printf(">>%c\n",buffer[strlen((char *)&block[0]) + 5]);
		if (buffer[strlen((char *)&block[0]) + 5] != ' ') overflow = 1; 
		do
		{
			int newblock = frst_free_block(ufs, blocksize, spb.root_dir);
			int child = first_free_child(oldinode, ufs, spb, blocksize, children_list);

			write_to_file(child, oldinode, newblock, block, spb, ufs);

			strcpy((char *)&block[0], (char *)&block[blocksize]);//Sends the extra part the was not written to the start of the buffer again
		}
		while (strlen((char *)&block[0]) > blocksize);
		
		if(overflow) //TODO tratar o caso de cair aqui
		{
			// printf("MAAAALS\n");
			start_writing = strlen((char *)&block[0]); //So we dont erease what was last read
		}
		else	
		{
			// printf("AMEM\n");
			sscanf(&buffer[pos_in_buffer],"%s",&the_name[0]);
		}
	}
	else //It has " so we must read until its closed
	{
		sscanf(&buffer[pos_in_buffer],"\"%[^\"]\"",block);
		pos_in_buffer += strlen((char *)&block[0]) + 1;
		// printf(">>%c\n",buffer[strlen((char *)&block[0]) + 5 + 1]);
		// printf("%s\n",block);
		if (buffer[strlen((char *)&block[0]) + 5 + 1] != '\"') overflow = 1; 
		do
		{
			int newblock = frst_free_block(ufs, blocksize, spb.root_dir);
			int child = first_free_child(oldinode, ufs, spb, blocksize, children_list);

			write_to_file(child, oldinode, newblock, block, spb, ufs);
		
			strcpy((char *)&block[0], (char *)&block[blocksize]);//Sends the extra part the was not written to the start of the buffer again
		}
		while (strlen((char *)&block[0]) > blocksize);
		
		if(overflow) //TODO tratar o caso de cair aqui
		{
			// printf("Ruinnmnmnm\n");
			start_writing = strlen((char *)&block[0]); //So we dont erease what was last read
		}
		else	
		{
			// printf("AMEMDOOIX\n");
			sscanf(&buffer[pos_in_buffer],"\"%s",&the_name[0]);
		}
	}
	inode_read(oldinode.id, ufs, spb.magic_number,spb.root_inode, &oldinode);
	strcpy((char *)&(oldinode.metadata.name[0]), (char *)&the_name[0]); //There is no ", then the 2 parameter is the target
	inode_write(offset, ufs, blocksize, spb.root_inode, oldinode);


	//Writes to the parent directory, this inode as a child
	int i;
	inode comparer;
	int found = 0;
	int freeblk = first_free_child(curr_inode, ufs, spb, blocksize, children_list);
	for (i = 0; i < freeblk; i++)
	{
		inode_read(children_list[i], ufs, blocksize, spb.root_inode, &comparer);
		if (!strcmp((char *)&comparer.metadata.name[0], (char *)&the_name[0]))
		{
			// printf("Found");
			found = 1;
			if (comparer.metadata.type == DIR_TYPE)
			{
				write_to_dir(freeblk, curr_inode, oldinode.id, spb, ufs);
				inode_read(curr_inode.id, ufs, blocksize, spb.root_inode, &curr_inode);
				rm_bash(curr_inode, (char *)&the_name[0], spb, ufs);
				
			}
			else
			{
				// printf("Foundya\n");
				write_to_dir(freeblk, curr_inode, oldinode.id, spb, ufs);
				inode_read(curr_inode.id, ufs, blocksize, spb.root_inode, &curr_inode);
				rm_bash(curr_inode, (char *)&the_name[0], spb, ufs);
				// inode_write(offset, ufs, blocksize, spb.root_inode, oldinode);
			}
		}
	}
	if (!found)
	{
		inode_read(curr_inode.id, ufs, blocksize, spb.root_inode, &curr_inode);
		write_to_dir(freeblk, curr_inode, oldinode.id, spb, ufs);
	}

	fseek(ufs, pos_saver, SEEK_SET);
}