#include "bash.fs_reader.h"
#include "mkfs_struct.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

inode inode_read(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk); //Private function

inode * dir_read(inode curr_inode, FILE* ufs, int count, int blocksize, uint16_t first_inode_blk)
{
	uint32_t status_zero;
	int i = -1;
	int inod_blklst_sz = 0;
	bool done = false;
	uint16_t addr[] = {0};
	
	if (curr_inode.metadata.type != DIR_TYPE) return 0; //Verify if it s a dir
	
	status_zero = ftell(ufs);
	
	for (i = 0; i < BLK_PER_IND - 2; i++)
	{
		fread(addr, sizeof(uint16_t), 1, ufs);
		//list[i] = inode_read(addr[0],ufs, blocksize, first_inode_blk);
	}
	
	return 0;
}

uint16_t * arq_read(inode curr_inode, FILE* ufs, int count, int blocksize)
{
	uint32_t status_zero;
	int i, j, k;
	bool done = false;
	status_zero = ftell(ufs);
	int inod_blklst_sz = 0;
	uint16_t read[] = {0};
	uint16_t addr[] = {0};
	uint16_t * retval;
	int last_block = 0;
	int last_addr = 0;
	for (i = 0; i < BLK_PER_IND - 2; i++)
	{
		fread(addr, sizeof(uint16_t), 1, ufs);
		if (addr[0] != 0) inod_blklst_sz++;
		else break;
	}
	fread(addr, sizeof(uint16_t), 1, ufs);
	last_block = ftell(ufs);
	if (addr[0] != 0)
	{
		fseek(ufs, addr[0]*blocksize, SEEK_SET);
		for (i = 0; i < blocksize; i ++)
		{
			fread(addr, sizeof(uint16_t), 1, ufs);
			if (addr[0] != 0) inod_blklst_sz++;
			else break;
		}
	}
	fseek(ufs, last_block, SEEK_SET);
	fread(addr, sizeof(uint16_t), 1, ufs);
	if (addr[0] != 0)
	{
		fseek(ufs, addr[0]*blocksize, SEEK_SET);
		for (i = 0; i < blocksize; i++)
		{
			fread(addr, sizeof(uint16_t), 1, ufs);
			last_addr = ftell(ufs);
			if (addr[0] != 0)
			{
				fseek(ufs, addr[0]*blocksize, SEEK_SET);
				for (j = 0; j < blocksize; j ++)
				{
					fread(addr, sizeof(uint16_t), 1, ufs);
					if (addr[0] != 0) inod_blklst_sz++;
					else break;
				}
			}
			if (addr[0] == 0) break;
			fseek (ufs, last_addr, SEEK_SET);
		}
	}
	printf(">>THIS_SZ>>%d\n",inod_blklst_sz);
	retval = (uint16_t *)malloc((blocksize*inod_blklst_sz*sizeof(uint16_t))/2 + 1);
	fseek(ufs, status_zero, SEEK_SET);
	////////////////////////////////////////////////////////////////////////////////
	int pos_now = 0;
	for (i = 0; i < BLK_PER_IND - 2; i++)
	{
		fread(addr, sizeof(uint16_t), 1, ufs);
		if (addr[0] != 0)
		{
			//for (k = 0; k < )
		}
		else break;
	}
	fread(addr, sizeof(uint16_t), 1, ufs);
	last_block = ftell(ufs);
	if (addr[0] != 0)
	{
		fseek(ufs, addr[0]*blocksize, SEEK_SET);
		for (i = 0; i < blocksize; i ++)
		{
			fread(addr, sizeof(uint16_t), 1, ufs);
			if (addr[0] != 0) inod_blklst_sz++;
			else break;
		}
	}
	fseek(ufs, last_block, SEEK_SET);
	fread(addr, sizeof(uint16_t), 1, ufs);
	if (addr[0] != 0)
	{
		fseek(ufs, addr[0]*blocksize, SEEK_SET);
		for (i = 0; i < blocksize; i++)
		{
			fread(addr, sizeof(uint16_t), 1, ufs);
			last_addr = ftell(ufs);
			if (addr[0] != 0)
			{
				fseek(ufs, addr[0]*blocksize, SEEK_SET);
				for (j = 0; j < blocksize; j ++)
				{
					fread(addr, sizeof(uint16_t), 1, ufs);
					if (addr[0] != 0) inod_blklst_sz++;
					else break;
				}
			}
			if (addr[0] == 0) break;
			fseek (ufs, last_addr, SEEK_SET);
		}
	}	
	return retval;
}

inode inode_read(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk)
{
	uint32_t status_zero;
	uint8_t inode_per_block = (sizeof(inode)/(double)blocksize);
	inode inode_n[1];
	status_zero = ftell(ufs);
	if ( inode_per_block >=2)
	{
		fseek(ufs, first_inode_blk * blocksize, SEEK_SET);
		fseek(ufs, (inode_numb/inode_per_block)*blocksize, SEEK_CUR);
		fread(inode_n, sizeof(inode_n), 1, ufs);
	} else {
		fseek(ufs, first_inode_blk * blocksize, SEEK_SET);
		fseek(ufs, inode_numb*blocksize, SEEK_CUR);
		fread(inode_n, sizeof(inode_n), 1, ufs);
	}
	fseek(ufs, status_zero, SEEK_SET);
	return inode_n[0];
}