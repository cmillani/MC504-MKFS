#include <stdio.h>
#include <stdint.h>
#include "mkfs_struct.h"
#include "mkfs_create.h"
#include <time.h>
#include <math.h>

FILE* mkfs_create(char filename[])
{
	FILE * retval;
	int i;
	uint8_t zeroes[1024*1024] = {0}; //1MB of zeroes
	retval = fopen(filename, "wb");
	for (i = 0; i < 24; i++)
	{
		fwrite(zeroes,sizeof(uint8_t),sizeof(zeroes), retval);
	}
	return retval;
}
int mkfs_set_fs(FILE * output, int blocksize)
{
	mkfs_wr_superblk(output, blocksize);
	mkfs_wr_inode_bmp(output, blocksize);
	mkfs_wr_block_bmp(output, blocksize);
	mkfs_wr_inodes(output, blocksize);
	return 0;
}
int mkfs_wr_superblk(FILE * output, int blocksize)
{
	char zero[] = {0};
	int i;
	superblock fs_superblock;
	printf("BKSZ%d\n",blocksize);
	fs_superblock.magic_number = blocksize;
	fs_superblock.root_inode = 0;//TODO calcular saqui
	fs_superblock.root_dir = 0;//TODO calcular saqui
	fs_superblock.dir_inode = 0;
	fs_superblock.arq_inode = 0;
	fwrite(&fs_superblock, sizeof(superblock), 1, output);
	for (i = 0; i < blocksize-sizeof(superblock); i++)
	{
		fwrite(zero, sizeof(uint8_t), 1, output);
	}
	printf("SPB%lu\n",ftell(output));
	return 0;
}
int mkfs_wr_inode_bmp(FILE * output, int blocksize)
{
	uint8_t inode_bmp[128] = {0}; //1024 cleared bits
	int i = 0;
	char zero[] = {0};
	fwrite(inode_bmp, sizeof(uint8_t), sizeof(inode_bmp), output);
	for (i = 0; i < blocksize-128; i++)
	{
		fwrite(zero, sizeof(uint8_t), 1, output);
	}
	printf("INBMP%lu\n",ftell(output));
	return 0;
}
int mkfs_wr_block_bmp(FILE * output, int blocksize)
{
	uint8_t blkbmp_sz = (uint8_t)ceil((24.*1024.*1024.)/(8.*(blocksize*blocksize)));
	uint8_t block_bmp[blkbmp_sz*blocksize];
	printf(">>>>>>%d\n", blkbmp_sz);
	int i;
	for (i = 0; i < blkbmp_sz; i++)
	{
		block_bmp[i] = 0;
	}
	fwrite(block_bmp, sizeof(uint8_t), sizeof(block_bmp), output);
	printf("BLKBMP%lu\n",ftell(output));
	return 0;
}
int mkfs_wr_inodes(FILE * output, int blocksize)
{
	inode inodes[1024];
	int i, j, k;
	char zero[] = {0};
	double inode_per_block = (double)blocksize/sizeof(inode);
	double block_per_inode = sizeof(inode)/(double)blocksize;
	
	inodes[0].id = 1;
	for (j = 0; j < 1024; j++) inodes[0].blocks[j] = 0;
	inodes[0].metadata.unix_time = (uint32_t)time(NULL);
	inodes[0].metadata.permissions |= (1 << READ_PERMISSION) | (1 << WRITE_PERMISSION);
	inodes[0].metadata.name[0] = '/';
	inodes[0].metadata.parent = 0;
	inodes[0].metadata.type = DIR_TYPE;
	for (i = 1; i < 1024; i++)
	{
		inodes[i].id = i;
		for (j = 0; j < 1024; j++) inodes[i].blocks[j] = 0;
		inodes[i].metadata.unix_time = (uint32_t)time(NULL);
		inodes[i].metadata.permissions = 0;
		inodes[i].metadata.name[0] = '\0';
		inodes[i].metadata.parent = 1;
		inodes[i].metadata.type = DIR_TYPE;
	}
	if (inode_per_block < 1)
	{
		for (i = 0; i < 1024; i++)
		{
			fwrite(&inodes[i], sizeof(inode), 1, output);
			//printf("%f, %d",(block_per_inode), (int)sizeof(inode));
			for (j = 0; j < ((int)ceil(block_per_inode)*blocksize - (int)sizeof(inode)); j++)
			{
				//printf("%d\n",(int)sizeof(inode));
				fwrite(zero, sizeof(uint8_t), 1, output);
			}
			//printf(">>%lu\n",ftell(output));
		}
	}
	else
	{
		k = 0;
		for (i = 0; i < 1024; i++)
		{
			fwrite(&inodes[i], sizeof(inode), 1, output);
			k++;
			if (k == floor(inode_per_block))
			{
				k = 0;
				for (j = 0; j < blocksize - (int)floor(inode_per_block)*sizeof(inode); j++)
				{
					fwrite(zero, sizeof(uint8_t), 1, output);
				}
			}
			//printf(">>%lu\n",ftell(output));
		}
	}
	
	printf("INDS%lu\n",ftell(output));
	return 0;
}