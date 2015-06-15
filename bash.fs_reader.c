#include "bash.fs_reader.h"
#include "mkfs_struct.h"
#include "bitmap_reader.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int write_to_file(int child_nbr, inode inode, uint16_t newblock_nbr, uint8_t block[], superblock spb, FILE * ufs)
{
	if (child_nbr >= 1024)
	{
		printf("Inode full\n");
		return -1;
	}
	int blocksize = spb.magic_number;
	int initial_pos = ftell(ufs);
	if (child_nbr < BLK_PER_IND-4) //No refered position
	{
		inode.blocks[child_nbr] = newblock_nbr;
		inode_write(inode.id, ufs, blocksize, spb.root_inode, inode);
		fseek(ufs, spb.root_dir * blocksize, SEEK_SET);
		fseek(ufs, newblock_nbr * blocksize, SEEK_CUR);
		fwrite(block, sizeof(uint8_t), blocksize, ufs);
	}
	else //Refered position
	{
		// printf("entrou aqui\n");
		int newblock;
		int block_in_vec = (BLK_PER_IND-4) + ((child_nbr - (BLK_PER_IND-4))/(blocksize/2));
		int jump_inside = 0;
		if ((child_nbr - (BLK_PER_IND-4))%(blocksize/2) == 0)//First position, verify is block is already there
		{
			if (inode.blocks[block_in_vec] == 0)//Alloc the block
			{
				// printf("Seraaqui?\n");
				newblock = frst_free_block(ufs, spb.magic_number, spb.root_dir);
				// printf("%d<>%d\n",newblock, block_in_vec);
				inode.blocks[block_in_vec] = newblock;
				inode_write(inode.id, ufs, blocksize, spb.root_inode, inode);
			}
			else newblock = inode.blocks[block_in_vec];
		}
		else 
		{
			newblock = inode.blocks[block_in_vec];
			jump_inside = (child_nbr - (BLK_PER_IND-4))%(blocksize/2) * sizeof(uint16_t);
		}
		fseek(ufs, spb.root_dir * blocksize, SEEK_SET);
		fseek(ufs, newblock * blocksize, SEEK_CUR);
		if (jump_inside) fseek(ufs, jump_inside, SEEK_CUR);
		fwrite(&newblock_nbr, sizeof(uint16_t), 1, ufs);
		fseek(ufs, spb.root_dir * blocksize, SEEK_SET);
		fseek(ufs, newblock_nbr * blocksize, SEEK_CUR);
		fwrite(block, sizeof(uint8_t), blocksize, ufs);
	}
	fseek(ufs, initial_pos, SEEK_SET);
	return 0;
}

void remove_from_dir(int inode_id, inode parent, FILE *ufs, superblock spb)
{
	uint16_t children[1024] = {0};
	inode realoc;
	int i;
	int position = -1;
	int blocksize = spb.magic_number;
	int last = first_free_child(parent, ufs, spb, blocksize, children);
	for (i = 0; i < last; i++)
	{
		if (children[i] == inode_id)
		{
			position = i;
		}
	}
	if (last == -1) last = 1023;
	else last -= 1;
	if (position == -1) 
	{
		printf("Error\n");
		return;
	}
	// printf("Position %d last %d children %d\n", position, last, children[last]);
	write_to_dir(position, parent, children[last], spb, ufs);
	inode_read(parent.id, ufs, spb.magic_number, spb.root_inode, &parent);
	// printf("last one is %d\n",last);
	//inode_read(last, ufs, blocksize, spb.root_inode, &realoc);
	// inode_write(inode_id, ufs, blocksize, uint16_t first_inode_blk, realoc);

	// write_to_dir(inode_id, parent, last, spb, ufs);
	write_to_dir(last, parent, 0, spb, ufs);
	if ((last - (BLK_PER_IND-4))%(blocksize/2) == 0)
	{
		// printf("caiu aqui\n");
		int block_in_vec = (BLK_PER_IND-4) + ((last - (BLK_PER_IND-4))/(blocksize/2));
		inode_read(parent.id, ufs, spb.magic_number, spb.root_inode, &parent);
		parent.blocks[block_in_vec] = 0;
		inode_write(parent.id, ufs, blocksize, spb.root_inode, parent);
	}
}

int write_to_dir(int child_nbr, inode inode, uint16_t new_inode_id, superblock spb, FILE * ufs)
{
	if (child_nbr >= 1024)
	{
		printf("Inode full\n");
		return -1;
	}
	int blocksize = spb.magic_number;
	int initial_pos = ftell(ufs);
	if (child_nbr < BLK_PER_IND-4) //No refered position
	{
		inode.blocks[child_nbr] = new_inode_id;
		inode_write(inode.id, ufs, blocksize, spb.root_inode, inode);
	}
	else //Refered position
	{
		// printf("entrou aqui\n");
		int newblock;
		int block_in_vec = (BLK_PER_IND-4) + ((child_nbr - (BLK_PER_IND-4))/(blocksize/2));
		int jump_inside = 0;
		if ((child_nbr - (BLK_PER_IND-4))%(blocksize/2) == 0)//First position, verify is block is already there
		{
			if (inode.blocks[block_in_vec] == 0)//Alloc the block
			{
				// printf("Seraaqui?\n");
				newblock = frst_free_block(ufs, spb.magic_number, spb.root_dir);
				// printf("%d<>%d\n",newblock, block_in_vec);
				inode.blocks[block_in_vec] = newblock;
				inode_write(inode.id, ufs, blocksize, spb.root_inode, inode);
			}
			else newblock = inode.blocks[block_in_vec];
		}
		else 
		{
			newblock = inode.blocks[block_in_vec];
			jump_inside = (child_nbr - (BLK_PER_IND-4))%(blocksize/2) * sizeof(uint16_t);
		}
		fseek(ufs, spb.root_dir * blocksize, SEEK_SET);
		fseek(ufs, newblock * blocksize, SEEK_CUR);
		if (jump_inside) fseek(ufs, jump_inside, SEEK_CUR);
		fwrite(&new_inode_id, sizeof(uint16_t), 1, ufs);
	}
	fseek(ufs, initial_pos, SEEK_SET);
	return 0;
	// else if ((child_nbr-10)%(blocksize/2) == 0)//must alloc 2 times
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
	if (curr_inode.blocks[BLK_PER_IND-4] == 0 && !found_free)
	{
		// printf("\nBloco de fronteira livre>%d<\n\n", curr_inode.blocks[BLK_PER_IND-4]);
		found_free = true;
		frst_fr_blk = current_blk;
	}
	if (!found_free)
	{
		// printf("looking refrd\n");
		for (j = BLK_PER_IND - 4; j < BLK_PER_IND; j++)
		{
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
				if (current_blk >= 1024) break;
				children_list[current_blk] = block[0];
				current_blk++;
			}
		}
	}
	fseek(ufs, initial_pos, SEEK_SET);
	// printf("RETURNING \n");
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
	if (inode_per_block >=2)
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