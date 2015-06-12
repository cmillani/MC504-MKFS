#include "chmod.h"
void bash_chmod(inode *inode, int mode)
{
	inode->metadata.permissions = (mode%10 << EXEC_PERMISSION)|((mode/10)%10 << WRITE_PERMISSION)|((mode/100)%10 << READ_PERMISSION);
}