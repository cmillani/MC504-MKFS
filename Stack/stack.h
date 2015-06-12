//
//  stack.h
//  Stack
//
//  Created by Letícia Gonçalves on 6/10/15.
//  Copyright (c) 2015 LazyFox. All rights reserved.
//

#ifndef __Stack__stack__
#define __Stack__stack__

#include <stdio.h>
#include "../mkfs_struct.h"
#include <stdlib.h>

typedef struct node{
	inode * inode;
    int data;
    struct node *next;
} node;

typedef struct stack{
    int size;
    struct node *top;
    struct node *first;
} *stack;

void Push(inode *Item, node **head);
node * create_node(inode *item);
void Display(node *head);
int StackIsEmpty(node *temp);
inode * Pop(node **head);
inode * ViewLast(node **head);


#endif /* defined(__Stack__stack__) */
