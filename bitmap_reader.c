#include <stdint.h>

int frst_free_inode(FILE * ufs, int blocksize)
{
	int saved_position = ftell(ufs);
	int i, j, k = 0;
	int selected = -1;
	uint8_t byte[] = {0};
	fseek(ufs, blocksize, SEEK_SET);
	for (i = 0; i < 128; i++) //1024 inodes, 1 inode per bit = 128 bytes (128*8 = 1024)
	{
		fread(byte, sizeof(uint8_t), 1, ufs);
		for (j = 7; j > 0; j--)
		{
			if (!(byte[0] & 1 << j))//If the bit is clear the inode is so
			{
				selected = k; 
				break;
			}
			k++;
		}
	}
	if (selected != -1) return selected*blocksize;
	else return -1;b
}