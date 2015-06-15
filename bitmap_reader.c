#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitmap_reader.h"
#include <math.h>



void clear_inode(FILE * ufs, int blocksize, int inode_id)
{
	int saved_position = ftell(ufs);
	uint8_t byte[] = {0};
	
	fseek(ufs, blocksize, SEEK_SET);
	fseek(ufs, sizeof(uint8_t)*(inode_id/8), SEEK_CUR);
	
	fread(byte, sizeof(uint8_t), 1, ufs);
	
	// printf("Ant%d\n", byte[0]);
	byte[0] &= ~(1 << (7 - inode_id%8));
	// printf("Dep%d\n", byte[0]);
	
	fseek (ufs, -sizeof(uint8_t),SEEK_CUR);
	fwrite(byte, sizeof(uint8_t), 1, ufs);
	
	fseek(ufs, saved_position, SEEK_SET);
}

void clear_block(FILE * ufs, int blocksize, int frst_data_blo, int block_nbr)
{
	int saved_position = ftell(ufs);
	uint8_t byte[] = {0};
	
	fseek(ufs, 2*blocksize, SEEK_SET);
	fseek(ufs, sizeof(uint8_t)*(block_nbr/8), SEEK_CUR);
	
	fread(byte, sizeof(uint8_t), 1, ufs);
	
	// printf("Ant%d\n", byte[0]);
	byte[0] &= ~(1 << (7 - block_nbr%8));
	// printf("Dep%d\n", byte[0]);
	
	fseek (ufs, -sizeof(uint8_t),SEEK_CUR);
	fwrite(byte, sizeof(uint8_t), 1, ufs);
	
	fseek(ufs, saved_position, SEEK_SET);
}

int frst_free_inode(FILE * ufs, int blocksize) //Return the number of the first free inode and marks it as not free anymore
{
	int saved_position = ftell(ufs);
	int i, j, k = 0;
	int selected = -1;
	uint8_t byte[] = {0};
	fseek(ufs, blocksize, SEEK_SET);
	for (i = 0; i < 128; i++) //1024 inodes, 1 inode per bit = 128 bytes (128*8 = 1024)
	{
		fread(byte, sizeof(uint8_t), 1, ufs);
		// printf("POS: %ld\n",ftell(ufs));
		// printf("%d\n",byte[0]);
		for (j = 7; j > -1; j--)
		{
			if (!(byte[0] & 1 << j))//If the bit is clear the inode is so
			{
				selected = k; 
				byte[0] |= (1 << j);
				// printf(">%d\n",byte[0]);
				fseek (ufs, -sizeof(uint8_t),SEEK_CUR);
				// printf("POSAFT: %ld\n",ftell(ufs));
				fwrite(byte, sizeof(uint8_t), 1, ufs);
				
				// fflush(ufs);
				break;
			}
			k++;
		}
		if (selected != -1) break;
	}
	fseek(ufs, saved_position, SEEK_SET);
	if (selected != -1) return selected;//Not util
	else return -1;
}
int frst_free_block(FILE * ufs, int blocksize, int frst_data_blk) //Return the number of the first free data block and marks it as not free anymore
{
	int saved_position = ftell(ufs);
	int i, j, k = 0;
	int selected = -1;
	uint8_t byte[] = {0};
	fseek(ufs, 2*blocksize, SEEK_SET);
	int blocks_number = ((1024*1024*24)-(frst_data_blk*blocksize))/blocksize;
	for (i = 0; i < (int)ceil(blocks_number/8); i++) //1024 inodes, 1 inode per bit = 128 bytes (128*8 = 1024)
	{
		fread(byte, sizeof(uint8_t), 1, ufs);
		// printf("POS: %ld\n",ftell(ufs));
		// printf("%d\n",byte[0]);
		for (j = 7; j > -1; j--)
		{
			if (!(byte[0] & 1 << j))//If the bit is clear the inode is so
			{
				selected = k; 
				byte[0] |= (1 << j);
				// printf(">%d\n",byte[0]);
				fseek (ufs, -sizeof(uint8_t),SEEK_CUR);
				// printf("POSAFT: %ld\n",ftell(ufs));
				fwrite(byte, sizeof(uint8_t), 1, ufs);
				
				// fflush(ufs);
				break;
			}
			k++;
		}
		if (selected != -1) break;
	}
	fseek(ufs, saved_position, SEEK_SET);
	if (selected != -1) return selected;//Not util
	else return -1;
}