#include "bash.fs_reader.h"
#include "mkfs_struct.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


int write_to_dir(int child_nbr, inode inode, int new_inode_id, superblock spb, FILE * ufs)
{
	if (child_nbr >= 1024)
	{
		printf("Inode full\n");
		return -1;
	}
	int blocksize = spb.magic_number;
	int initial_pos = ftell(ufs);
	if (child_nbr < 12-4) //No refered position
	{
		inode.blocks[child_nbr] = new_inode_id;
		inode_write(inode.id, ufs, blocksize, spb.root_inode, inode);
	}
	else //Refered position //TODO TERMINAR
	{
		if ((child_nbr - (12-4))%(blocksize/2) == 0)//First position, verify is block is already there
		{
			int block_in_vec = (12-4) + ((child_nbr - (12-4))/(blocksize/2));
			if (inode.blocks[block_in_vec] == 0)//Alloc the block
			{
				
			}
		}
	}
	fseek(ufs, initial_pos, SEEK_SET);
	return 0;
	// else if ((child_nbr-10)%(blocksize/2) == 0)//must alloc 2 times
}

int read_blk_16(int blk_id, uint16_t block[], int firstblk, FILE *ufs)
{
	
}

int first_free_child(inode curr_inode, FILE * ufs, superblock spb, int blocksize, uint16_t *children_list)//Returns a children list via the children_list pointer and the first free block number via the function return
{
	bool found_free = false;
	int initial_pos = ftell(ufs);
	int pos_saver;
	int frst_fr_blk = 0;
	int current_blk = 0;
	int i = 0, j = 0;
	uint16_t block[1];

	for (j = 0; j < BLK_PER_IND-4; j++)
	{
		if (curr_inode.blocks[j] == 0 && !found_free) 
		{
			found_free = true;
			frst_fr_blk = current_blk;
			break;
		}
		children_list[current_blk] = curr_inode.blocks[j];
		current_blk++;
	}
	if (curr_inode.blocks[BLK_PER_IND-2] == 0 && !found_free)
	{
		found_free = true;
		frst_fr_blk = current_blk;
	}
	if (!found_free)
	{
		for (j = BLK_PER_IND - 4; j < BLK_PER_IND; j++)
		fseek(ufs, spb.root_dir * blocksize, SEEK_SET);
		fseek(ufs, curr_inode.blocks[j]*blocksize, SEEK_CUR);
		for (i = 0; i < blocksize/2; i++)
		{
			fread(block, sizeof(uint16_t), 1, ufs);
			if (block[0] == 0 && !found_free)
			{
				found_free = true;
				frst_fr_blk = current_blk;
				break;
			}
			children_list[current_blk] = curr_inode.blocks[j];
			current_blk++;
		}
	}
	fseek(ufs, initial_pos, SEEK_SET);
	
	return found_free?frst_fr_blk:-1;
}



int inode_read(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk, inode* read_one)
{
	uint32_t status_zero;
	uint8_t block_per_inode = (uint8_t)ceil(sizeof(inode)/(double)blocksize);
	uint8_t inode_per_block = ((double)blocksize/sizeof(inode));
	int i;
	// printf("ShouldRead>%d, calc = %d\n",inode_numb, inode_per_block);
	// printf("<><><>%d\n",sizeof(inode));
	status_zero = ftell(ufs);
	if ( inode_per_block >=2)
	{
		fseek(ufs, first_inode_blk * blocksize, SEEK_SET);
		fseek(ufs, (inode_numb/inode_per_block)*blocksize, SEEK_CUR);//Jumps to the start of the corresponding block
		for (i = 0; i <= inode_numb%inode_per_block; i++)
		{
			fread(read_one, sizeof(inode), 1, ufs);//More than one inode per block, so reads until the wanted one is read
		}
	} else {
		fseek(ufs, first_inode_blk * blocksize, SEEK_SET);
		fseek(ufs, inode_numb*blocksize*block_per_inode, SEEK_CUR);
		fread(read_one, sizeof(inode), 1, ufs);
	}
	fseek(ufs, status_zero, SEEK_SET);
	return 1;
}

int inode_write(uint16_t inode_numb, FILE * ufs, int blocksize, uint16_t first_inode_blk, inode to_write)
{
	uint32_t status_zero;
	uint8_t block_per_inode = (uint8_t)ceil(sizeof(inode)/(double)blocksize);
	uint8_t inode_per_block = ((double)blocksize/sizeof(inode));
	int i;
	// printf("ShouldRead>%d, calc = %d\n",inode_numb, inode_per_block);
	// printf("<><><>%d\n",sizeof(inode));
	status_zero = ftell(ufs);
	if ( inode_per_block >=2)
	{
		fseek(ufs, first_inode_blk * blocksize, SEEK_SET);
		fseek(ufs, (inode_numb/inode_per_block)*blocksize, SEEK_CUR);//Jumps to the start of the corresponding block
		fseek(ufs, sizeof(inode)*(inode_numb%inode_per_block), SEEK_CUR);//Jumps to the start of the corresponding inode
		fwrite(&to_write, sizeof(inode), 1, ufs);

	} else {
		fseek(ufs, first_inode_blk * blocksize, SEEK_SET);
		fseek(ufs, inode_numb*blocksize*block_per_inode, SEEK_CUR);
		fwrite(&to_write, sizeof(inode), 1, ufs);
	}
	fseek(ufs, status_zero, SEEK_SET);
	return 1;
}