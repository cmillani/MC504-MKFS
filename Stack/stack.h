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
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
} node;

typedef struct stack{
    int size;
    struct node *top;
    struct node *first;
} *stack;

void StackPush();
void StackPop();
void StackDisplay(node **head);
void StackCreate();
void StackDestroy();
int StackIsEmpty();
node * create_node(int);


#endif /* defined(__Stack__stack__) */
