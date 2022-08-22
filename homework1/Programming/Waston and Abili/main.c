#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct XOR_linked_list
{
	int data;
	struct XOR_linked_list* addr_1;
	struct XOR_linked_list* addr_2;
	int type;
	
}node;

node*  xor(node* a,node* b)
{
    return (node*) ((unsigned int) (a) ^ (unsigned int) (b));
}

void push(node** head, node** tail, int data)
{
	node* cur = (node *)malloc(sizeof(node));
	assert(cur != NULL);
	
	cur->data = data;
	cur->addr_1 = xor(*tail, NULL);
	cur->addr_2 = NULL;
	cur->type = 0;
	
	if(*head == NULL)//the first node
	{
		*head = cur;
		*tail = cur;
	}
	else
	{
		if((*tail)->type == 0)
		{
			node* prev = xor((*tail)->addr_1, NULL);
			(*tail)->addr_1 = xor(prev, cur);
			*tail = cur;
		}
		else if((*tail)->type == 1)
		{
			node* prev = xor((*tail)->addr_2, NULL);
			(*tail)->addr_2 = xor(prev, cur);
			*tail = cur;
		}
		
	}
	
	
	return;
}

void pop(node** head, node** tail)
{
	if((*head) == (*tail))
	{
		(*head) = NULL;
		(*tail) = NULL;
	}
	else
	{
		node* prev;
		node* p_prev;
		if((*tail)->type == 0)
		{
			prev = xor((*tail)->addr_1, NULL);
			if(prev->type == 0)
			{
				p_prev = xor((*tail), prev->addr_1);
				prev->addr_1 = xor(p_prev, NULL);
			}
			else if(prev->type == 1)
			{
				p_prev = xor((*tail), prev->addr_2);
				prev->addr_2 = xor(p_prev, NULL);
			}
		}
		else if((*tail)->type == 1)
		{
			prev = xor((*tail)->addr_2, NULL);
			if(prev->type == 0)
			{
				p_prev = xor((*tail), prev->addr_1);
				prev->addr_1 = xor(p_prev, NULL);
			}
			else if(prev->type == 1)
			{
				p_prev = xor((*tail), prev->addr_2);
				prev->addr_2 = xor(p_prev, NULL);
			}
		}
		
		*tail = prev;
	}
	
	return;
}



void printList(node** head)
{
    node* cur = *head;
    node* prev = NULL;
    node* next;
 
 
    while (cur != NULL)
    {
        printf ("%d ", cur->data);
		if(cur->type == 0)
		{
			next = xor(prev, cur->addr_1);
		}
		else if(cur->type == 1)
		{
			next = xor(prev, cur->addr_2);
		}
        prev = cur;
        cur = next;
    }
	return;
}

int main()
{
	int rail_num, command_num;
	scanf("%d%d", &rail_num, &command_num);
	node** head;
	node** tail;
	head = (node* *)malloc(rail_num * sizeof(node*));
	tail = (node* *)malloc(rail_num * sizeof(node*));
	
	int i;
	for(i = 0;i < rail_num;i++)
	{
		head[i] = NULL;
		tail[i] = NULL;
	}
	
	char command[8];
	int num1, num2;
	
	for(i = 0;i < command_num;i++)
	{
		scanf("%s", command);
		if(strcmp(command, "enter") == 0)
		{
			scanf("%d%d", &num1, &num2);
			push(&head[num1], &tail[num1], num2);
		}
		else if(strcmp(command, "leave") == 0)
		{
			scanf("%d", &num1);
			pop(&head[num1], &tail[num1]);
		}
		else if(strcmp(command, "migrate") == 0)
		{
			scanf("%d%d", &num1, &num2);
			if(tail[num1] != NULL)
			{
				
				if(tail[num1]->type == 1)
				{
					node* num1_prev = xor(tail[num1]->addr_2, NULL);
					tail[num1]->type = 0;
					tail[num1]->addr_1 = xor(tail[num2], num1_prev);
					node* num2_prev;
					if(tail[num2] == NULL)
					{
						head[num2] = tail[num1];
						tail[num2] = head[num1];
					}
					else
					{
						if(tail[num2]->type == 1)
						{
							num2_prev = xor(tail[num2]->addr_2, NULL);
							tail[num2]->type = 0;
							tail[num2]->addr_1 = xor(num2_prev, tail[num1]);
							tail[num2] = head[num1];
						}
						else if(tail[num2]->type == 0)
						{
							num2_prev = xor(tail[num2]->addr_1, NULL);
							tail[num2]->type = 1;
							tail[num2]->addr_2 = xor(num2_prev, tail[num1]);
							tail[num2] = head[num1];
						}
					}
					
					tail[num1] = NULL;
					head[num1] = NULL;
				}
				else if(tail[num1]->type == 0)
				{
					node* num1_prev = xor(tail[num1]->addr_1, NULL);
					tail[num1]->type = 1;
					tail[num1]->addr_2 = xor(tail[num2], num1_prev);
					node* num2_prev;
					if(tail[num2] == NULL)
					{
						head[num2] = tail[num1];
						tail[num2] = head[num1];
					}
					else
					{
						if(tail[num2]->type == 1)
						{
							num2_prev = xor(tail[num2]->addr_2, NULL);
							tail[num2]->type = 0;
							tail[num2]->addr_1 = xor(num2_prev, tail[num1]);
							tail[num2] = head[num1];
						}
						else if(tail[num2]->type == 0)
						{
							num2_prev = xor(tail[num2]->addr_1, NULL);
							tail[num2]->type = 1;
							tail[num2]->addr_2 = xor(num2_prev, tail[num1]);
							tail[num2] = head[num1];
						}
					}
				
					tail[num1] = NULL;
					head[num1] = NULL;
				}
			}
		}
	}
	for(i = 0;i < rail_num;i++)
	{
		printList(&head[i]);
		if(i != rail_num-1)
			printf("\n");
	}
	return 0;
}