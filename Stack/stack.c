//
//  stack.c
//  Stack
//
//  Created by Letícia Gonçalves on 6/10/15.
//  Copyright (c) 2015 LazyFox. All rights reserved.
//

#include "stack.h"
#include "../mkfs_struct.h"
#include <string.h>

void Display(node *head) {
    node *temp;
    temp = head;
    if (StackIsEmpty(head))
        printf("\nThe stack is empty!\n");
    else {
		printf("%s", temp->inode->metadata.name);
		temp = temp->next;
        while (temp != NULL) {
            printf("%s", temp->inode->metadata.name);
			if (temp->next != NULL) printf("/");
            temp = temp->next;
        }
    }
	printf(" # ");
}

void Push(inode *Item, node **head) {
    node *New;
    node *temp;
    New = create_node(Item);
    temp = *head;
    if (StackIsEmpty(temp)){
        *head = New;
    }
	else
	{
	    while (temp->next != NULL) {
	        temp = temp->next;
	    }
    	temp->next = New;
	}
}

node * create_node(inode *item) {
		/*typedef struct metadata
	{
		uint32_t unix_time;
		uint8_t permissions;
		uint8_t name[256];
		uint16_t parent; //The id of the parent inode
		uint8_t type;
	}metadata;

	typedef struct inode
	{
		uint16_t id;
		uint16_t blocks[BLK_PER_IND];
		metadata metadata;	
	}inode;*/
    inode *new = (inode *)malloc(sizeof(inode));
	int i;
	new->id = item->id;
	new->metadata = item->metadata;
	for (i = 0; i < BLK_PER_IND; i++)
	{
		new->blocks[i] = item->blocks[i];
	}
	new->metadata.unix_time = item->metadata.unix_time;
	new->metadata.permissions = item->metadata.permissions;
	new->metadata.parent = item->metadata.parent;
	new->metadata.type = item->metadata.type;
	strcpy((char *)&new->metadata.name[0], (char *)&item->metadata.name[0]);
	
    node * temp;
    temp = (node *) malloc(sizeof(node));
    if (temp == NULL)
        printf("\nMemory Cannot be allocated");
    temp->inode = new;
    temp->next = NULL;
    return (temp);
}

int StackIsEmpty(node *temp) {
    if (temp == NULL)
        return 1;
    else
        return 0;
}

inode * Pop(node **head) {
    inode *item;
    node *temp;
    node *oldNode = NULL;
    temp = *head;
    if (StackIsEmpty(temp)){
        return NULL;
    }
    else {
		while (temp->next != NULL)
        {
            oldNode = temp;
            temp = temp->next;
        }
    }
    item = temp->inode;
    free(temp);
	if (oldNode != NULL) oldNode->next = NULL;
	else *head = NULL;
    
    return item;
}
inode * ViewLast(node **head)
{
    inode *item;
    node *temp;
    temp = *head;
    if (StackIsEmpty(temp)){
        return NULL;
    }
    else {
		while (temp->next != NULL)
        {
            temp = temp->next;
        }
    }
    item = temp->inode;
    
    return item;
}


