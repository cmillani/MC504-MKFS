#include <stdio.h>
#include <stdint.h>
#include "mkfs_struct.h"
#include "mkfs_create.h"
#include <time.h>
#include <math.h>

FILE* mkfs_create(char filename[])
{
	FILE * retval; //File to be created
	int i;
	uint8_t zeroes[1024*1024] = {0}; //1MB of zeroes
	retval = fopen(filename, "wb");//Create the file
	for (i = 0; i < 24; i++) //Populate the file with 24MB of zeroes
	{
		fwrite(zeroes,sizeof(uint8_t),sizeof(zeroes), retval);
	}
	return retval;
}
int mkfs_set_fs(FILE * output, int blocksize)//Write the control structure of the file system
{
	mkfs_wr_superblk(output, blocksize);//Writes the superblock
	mkfs_wr_inode_bmp(output, blocksize);//Writes the inode bitmap
	mkfs_wr_block_bmp(output, blocksize);//Writes the block bitmap
	mkfs_wr_inodes(output, blocksize);//Writes the inodes region
	return 0;
}
int mkfs_wr_superblk(FILE * output, int blocksize)
{
	char zero[] = {0};
	int i;
	superblock fs_superblock; //Creates a superblock
	uint16_t inodes_blocks = ((double)blocksize/sizeof(inode))>1?(1024./((uint16_t)floor((double)blocksize/sizeof(inode)))):((uint16_t)ceil(sizeof(inode)/(double)blocksize)*(1024.)); //The size in blocks of the inodes section
	uint16_t blkbmp_sz = (uint16_t)(ceil((((24.*1024.*1024.)/(blocksize)) - 2 - inodes_blocks)/(1 + 8*blocksize)));//Calculates the space used by the block bitmap
	uint16_t root_addr = 2 + blkbmp_sz + inodes_blocks;
	uint16_t inode_addr = 2 + blkbmp_sz;
	// printf("BKSZ%d\n",blocksize); 
	fs_superblock.magic_number = blocksize;//The magic number holds the block size
	fs_superblock.root_inode = inode_addr; //Byte position of the first inode
	fs_superblock.root_dir = root_addr; //Byte position of the root dir
	// printf("IN>%d<ROOT>%d\n", inode_addr*blocksize, root_addr*blocksize);
	fs_superblock.dir_inode = 1;
	fs_superblock.arq_inode = 0;
	fwrite(&fs_superblock, sizeof(superblock), 1, output);//Writes the superblock to the file
	for (i = 0; i < blocksize-sizeof(superblock); i++)//Complete the block with zeroes
	{
		fwrite(zero, sizeof(uint8_t), 1, output);
	}
	// printf("SPB%lu\n",ftell(output));
	return 0;
}
int mkfs_wr_inode_bmp(FILE * output, int blocksize)
{
	uint8_t inode_bmp[128] = {0}; //1024 cleared bits
	inode_bmp[0] = 1 << 7; //First inode is the root inode
	int i = 0;
	uint8_t zero[] = {0};
	fwrite(inode_bmp, sizeof(uint8_t), sizeof(inode_bmp), output);//Writes the inode bitmap region
	for (i = 0; i < blocksize-128; i++)//Completes the block with zeroes
	{
		fwrite(zero, sizeof(uint8_t), 1, output);
	}
	// printf("INBMP%lu\n",ftell(output));
	return 0;
}
int mkfs_wr_block_bmp(FILE * output, int blocksize)
{
	uint16_t inodes_blocks = (blocksize/sizeof(inode))>1?(1024./((uint16_t)floor((double)blocksize/sizeof(inode)))):((uint16_t)ceil(sizeof(inode)/(double)blocksize)*(1024.)); //The size in blocks of the inodes section
	//uint8_t blkbmp_sz = (uint8_t)ceil((24.*1024.*1024.)/(8.*(blocksize*blocksize)));//Calculates the space used by the block bitmap
	uint16_t blkbmp_sz = (uint16_t)(ceil((((24.*1024.*1024.)/(blocksize)) - 2 - inodes_blocks)/(1 + 8*blocksize)));//Calculates the space used by the block bitmap
	uint8_t block_bmp[blkbmp_sz*blocksize];
	//printf(">>>>>>%d\n", blkbmp_sz);
	int i;
	for (i = 0; i < blkbmp_sz * blocksize; i++)//Writes zeroes to all positions, all are free
	{
		block_bmp[i] = 0;
	}
	block_bmp[0] = 0 | (1 << 7); //First block is root already
	fwrite(block_bmp, sizeof(uint8_t), sizeof(block_bmp), output);//Writes the blocks
	// printf("BLKBMP%lu\n",ftell(output));
	return 0;
}
int mkfs_wr_inodes(FILE * output, int blocksize) 
{
	inode inodes[1024]; //Creates the 1024 inodes to be written in the file
	int i, j, k;
	char zero[] = {0};
	double inode_per_block = (double)blocksize/sizeof(inode); //Number of inodes that a block requires
	double block_per_inode = sizeof(inode)/(double)blocksize; //Number of blocks that an inode can hold
	// printf("%f, %f\n", inode_per_block, block_per_inode);
	inodes[0].id = 0;//The root dir, being initialized
	for (j = 0; j < BLK_PER_IND; j++) inodes[0].blocks[j] = 0;
	inodes[0].metadata.unix_time = (uint32_t)time(NULL);
	inodes[0].metadata.permissions |= (1 << READ_PERMISSION) | (1 << WRITE_PERMISSION);
	inodes[0].metadata.name[0] = '/';//ROOT!
	inodes[0].metadata.parent = 0;//NULL parent
	inodes[0].metadata.type = DIR_TYPE;//A dir
	// printf("ROOT>>%s\n", inodes[0].metadata.name);
	for (i = 1; i < 1024; i++)//Initializes all the other Inodes
	{
		inodes[i].id = i;
		for (j = 0; j < BLK_PER_IND; j++) inodes[i].blocks[j] = 0;
		inodes[i].metadata.unix_time = (uint32_t)time(NULL);
		inodes[i].metadata.permissions = 111;
		inodes[i].metadata.name[0] = '\0';
		inodes[i].metadata.parent = 1;
		inodes[i].metadata.type = DIR_TYPE;
	}
	if (inode_per_block < 1) //The case where an inode needs more than a block
	{
		for (i = 0; i < 1024; i++)
		{
			fwrite(&inodes[i], sizeof(inode), 1, output);//Writes the inodes to the file
			//printf("%f, %d",(block_per_inode), (int)sizeof(inode));
			for (j = 0; j < ((int)ceil(block_per_inode)*blocksize - (int)sizeof(inode)); j++)//Completes with zeroes
			{
				//printf("%d\n",(int)sizeof(inode));
				fwrite(zero, sizeof(uint8_t), 1, output);
			}
			//printf(">>%lu\n",ftell(output));
		}
	}
	else //Where a block of more can fit inside the same block
	{
		k = 0;
		for (i = 0; i < 1024; i++)
		{
			fwrite(&inodes[i], sizeof(inode), 1, output);
			k++;
			if (k == floor(inode_per_block))//Have filled a block already
			{
				k = 0;
				for (j = 0; j < blocksize - (int)floor(inode_per_block)*sizeof(inode); j++)//Completes the block with zeroes and then goes to the next block
				{
					fwrite(zero, sizeof(uint8_t), 1, output);
				}
			}
			// printf(">>%lu\n",ftell(output));
		}
		while(ftell(output)%1024 != 0)//Completes with zeroes
		{
			fwrite(zero, sizeof(uint8_t), 1, output);
		}
	}
	
	// printf("INDS%lu\n",ftell(output));
	return 0;
}