#include "mkdir.h"
#include "../mkfs_struct.h"
#include "../bitmap_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void mkdir_bash(inode curr_inode, char * name, FILE * ufs, superblock spb) //TODO verificar permissoes
{
	int blocksize = spb.magic_number;
	int frst_fr_blk = -1;
	inode oldinode[0];
	int i = 0, j = 0;
	bool found_free = false;
	uint16_t block[1];
	
	int offset = frst_free_inode(ufs, blocksize);
	
	if(offset == -1)
	{
		printf("No free inodes in UFS. Remove some files in order to execute this command\n");
		return;
	}
	int pos_saver = ftell(ufs);
	
	fseek(ufs, spb.root_inode * blocksize, SEEK_SET);
	fseek(ufs, offset, SEEK_CUR);
	int temp = ftell(ufs);
	fread(oldinode, sizeof(inode), 1, ufs);
	fseek(ufs, temp, SEEK_SET);
	strcpy((char *)&(oldinode[0].metadata.name), name);
	oldinode->metadata.type = DIR_TYPE;
	oldinode->metadata.unix_time = (uint32_t)time(NULL);
	oldinode->metadata.permissions = (1 << READ_PERMISSION) | (1 << WRITE_PERMISSION);
	oldinode->metadata.parent = curr_inode.id;
	
	for (j = 0; j < BLK_PER_IND-2; j++)
	{
		if (curr_inode.blocks[j] == 0) 
		{
			found_free = true;
			frst_fr_blk = j;
			break;
		}
	}
	if (!frst_fr_blk)
	{
		fseek(ufs, spb.root_dir, SEEK_SET);
		fseek(ufs, curr_inode.blocks[BLK_PER_IND - 2], SEEK_CUR);
		for (i = 0; i < blocksize/16; i++)
		{
			fread(block, sizeof(uint16_t), 1, ufs);
			if (block[0] == 0)
			{
				found_free = true;
				frst_fr_blk = BLK_PER_IND - 2 + i;
				break;
			}
		}
	}
	if (!frst_fr_blk)
	{
		
	}
	
	//TODO ESCREVER O BLOCO NOVO NO INODE ATUAL, COLOCANDO COMO FILHO
	
}