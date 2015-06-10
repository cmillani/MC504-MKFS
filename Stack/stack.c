//
//  stack.c
//  Stack
//
//  Created by Letícia Gonçalves on 6/10/15.
//  Copyright (c) 2015 LazyFox. All rights reserved.
//

#include "stack.h"

void Display(node **head) {
    node *temp;
    temp = *head;
    if (StackIsEmpty(temp))
        printf("\nThe stack is empty!");
    else {
        while (temp != NULL) {
            printf("%d\n", temp->data);
            temp = temp->next;
        }
    }
}

void Push(int Item, node **head) {
    node *New;
    node *temp;
    New = create_node(Item);
    temp = *head;
    if (StackIsEmpty(temp)){
        temp = (struct node*) malloc (sizeof (struct node));
        temp = temp->next;
    }
    while (temp != NULL) {
        temp = temp->next;
    }
    temp = New;
}

node * create_node(int item) {
    node * temp;
    temp = (node *) malloc(sizeof(node));
    if (temp == NULL)
        printf("\nMemory Cannot be allocated");
    temp->data = item;
    temp->next = NULL;
    return (temp);
}

int StackIsEmpty(node *temp) {
    if (temp == NULL)
        return 1;
    else
        return 0;
}

int Pop(node **head) {
    int item;
    node *temp;
    node *oldNode = NULL;
    temp = *head;
    if (StackIsEmpty(temp)){
        printf("\nThe stack is empty!");
        return -1;
    }
    else {
        do {
            oldNode = temp;
            temp = temp->next;
        }while (temp != NULL);
    }
    item = (oldNode)->data;
    free(oldNode);
    
    return (item);
}



