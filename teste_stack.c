#include "Stack/stack.h"
#include "mkfs_struct.h"
#include <string.h>
	
int main()
{
	inode ind, ind2;
	inode * hue;
	node *nd;
	Display(nd);
	strcpy((char*)&ind.metadata.name[0], "Teste1");
	strcpy((char*)&ind2.metadata.name[0], "Teste2");
	Push(&ind, &nd);
	Display(nd);
	strcpy((char*)&ind.metadata.name[0], "TesteHUE");
	Push(&ind2, &nd);
	Display(nd);
	hue = Pop(&nd);
	printf("<><>%s\n",hue->metadata.name);
	Display(nd);
	hue = Pop(&nd);
	printf("<><>%s\n",hue->metadata.name);
	Display(nd);
}