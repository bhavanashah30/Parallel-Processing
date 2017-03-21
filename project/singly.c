#include "sll.h"

/* returns 1 if node is created, 0 if not */

int add_at_end(NODE **another, int val, int vcount)
{
  while(*another!=NULL)
    another=&(*another)->next;
  *another=malloc(sizeof(NODE));
  if(*another==NULL)
    return 0;
  (*another)->value=val;
  (*another)->valcount=vcount;
  (*another)->next=NULL;
  return 1;
}


/* counds nodes in the list */
int count_nodes(NODE * another)
{
  int count=0;
  while(another!=NULL)
  {
    count++;
    another=another->next; 
  }

  return count;
}

/* sets head of list to NULL, frees memory if there was previous list */
void create_sll(NODE **node)
{
  NODE * prev;
  NODE * nxt;
  nxt=*node;
  while(nxt!=NULL)
  {
    prev=nxt;
    nxt=prev->next;
    printf("Removing node with val: %d\n\n", prev->value);
    free(prev);
  }
  *node=NULL;
}

/* checks if nodes is in the list, returns pointer to the node */
NODE * locate(NODE * another, int val)
{
    while(another!=NULL)
    {
      if(another->value==val)
    return another;
      another=another->next;
    }

    return another;
}

/* prints out all nodes in the list */
void print_list(NODE * another)
{
  if(another!=NULL)
  {
    while(another!=NULL)
    {
      printf("node val: %d\n", another->value);
      another=another->next;
    }
    putchar('\n');
  }
  else
    printf("List is currently empty.\n\n");
}
