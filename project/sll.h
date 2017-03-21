#ifndef _SLL_H_
#define _SLL_H_
#include <stdio.h>
#include <stdlib.h>
/* declaration of node in Singly linked list */

typedef struct node{
  int value;
  int valcount;
  struct node * next;
}NODE;

/* declaration of head (pointer to node) */
extern NODE * head;

/* declaration for functions working on sll */
/*creating a list, freeing previously used memory */
void create_list(NODE ** head);

/* adding node at the end of sll */
int add_at_end(NODE **another, int val, int vcount);

/*printing all nodes in list one after another */
void print_list(NODE * another);

/* counting nodes in list */
int count_nodes(NODE * another);

/* locating value in node in sll */
NODE * locate(NODE * another, int val);

#endif