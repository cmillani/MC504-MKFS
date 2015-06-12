//
//  stack.c
//  Stack
//
//  Created by Letícia Gonçalves on 6/10/15.
//  Copyright (c) 2015 LazyFox. All rights reserved.
//

#include "stack.h"
#include "../mkfs_struct.h"

void Display(node *head) {
    node *temp;
    temp = head;
    if (StackIsEmpty(head))
        printf("\nThe stack is empty!\n");
    else {
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
    node * temp;
    temp = (node *) malloc(sizeof(node));
    if (temp == NULL)
        printf("\nMemory Cannot be allocated");
    temp->inode = item;
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


