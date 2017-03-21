#include<string.h>


struct Data{
	int value;
	int count;
	struct Data* next;

};

struct Data* head;

void  Print(struct Data*r)
{
    r=head;
    if(r==NULL)
    {
	   return;
    }
    while(r!=NULL)
    {
	printf("%d ",r->value);
	printf("%d ",r->count);
	r=r->next;
    }
    printf("\n");
}


void Insert( int num )
{
    struct Data *temp;
    temp=(struct Data*)malloc(sizeof(struct Data));
    temp->value=num;
    temp->count=1;
    if (head== NULL)
    {
	   head=temp;
	   head->next=NULL;
    }
    else
    {
    temp->next=head;
    head=temp;
    }
}
int  Check(struct Data*r,int num)
{
    int sum;
    r=head;
    if(r==NULL)
    {
	   return;
    }
    while(r!=NULL)
    {   if(r->value==num)
	{
	    sum=r->count;
	    r->count=sum+1;
		return 1;
	}
	r=r->next;
    }
    return 0;
}

//void Create()
//{

//}