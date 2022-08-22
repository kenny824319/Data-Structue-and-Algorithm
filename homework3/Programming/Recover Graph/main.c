#include <stdio.h>
#include <stdlib.h>

int count = 0;
typedef struct Node
{
	int x;
	int y;
	struct Node* next;
}node;

typedef struct Queue
{
	node increase[1000001];
	node decrease[1000001];
	int front;
	int end;
	int size;
}queue;

void insert(node** head, node** tail, int x, int y)
{
	node* cur = (node *)malloc(sizeof(node));
	
	cur->x = x;
	cur->y = y;
	cur->next = NULL;
	
	if(*head == NULL)
	{
		*head = cur;
		*tail = cur;
		return;
	}
	else
	{
		(*tail)->next = cur;
		*tail = cur;
		return;
	}
}

queue* queue_init()
{
	queue* result = (queue *)malloc(sizeof(queue));
	result->front = 0;
	result->end = 0;
	result->size = 0;
	return result;
}

void enqueue(queue* q, node** head1, node** head2)//head1壓進increase[],head2壓進decrease
{
	count++;
	q->size++;
	q->end++;
	q->increase[q->end] = *(*head1);
	q->decrease[q->end] = *(*head2);
	
	return;
}

int dequeue(queue* q, node** head)
{
	q->front++;
	q->size--;
	node* ins = &(q->increase[q->front]);
	node* des = &(q->decrease[q->front]);
	
	head[ins->x] = head[ins->x]->next;
	head[des->x] = head[des->x]->next;
	
	if(head[ins->x] && head[head[ins->x]->y] && head[ins->x]->x == head[head[ins->x]->y]->y)
	{
		if(head[ins->x]->x < head[ins->x]->y)
			enqueue(q, &head[head[ins->x]->x], &head[head[ins->x]->y]);
		else
			enqueue(q, &head[head[ins->x]->y], &head[head[ins->x]->x]);
	}
	if(head[des->x] && head[head[des->x]->y] && head[des->x]->x == head[head[des->x]->y]->y)
	{
		if(head[des->x]->x < head[des->x]->y)
			enqueue(q, &head[head[des->x]->x], &head[head[des->x]->y]);
		else
			enqueue(q, &head[head[des->x]->y], &head[head[des->x]->x]);
	}
	return q->size;
}

int main()
{
	int n;
	scanf("%d", &n);
	node** adjacency_list_head = (node* *)malloc((n+1) * sizeof(node*));
	node** adjacency_list_tail = (node* *)malloc((n+1) * sizeof(node*));
	int i, j;
	int total_vertex_num = 0;
	
	for(i = 1;i <= n;i++)
	{
		adjacency_list_head[i] = NULL;
		adjacency_list_tail[i] = NULL;
	}
	
	for(i = 1;i <= n;i++)
	{
		int num;
		scanf("%d", &num);
		if(num == 0)
			continue;
		for(j = 0;j < num;j++)
		{
			int neighbor_vertex;
			scanf("%d", &neighbor_vertex);
			if(neighbor_vertex > i)
				total_vertex_num++;
			insert(&adjacency_list_head[i], &adjacency_list_tail[i], i, neighbor_vertex);
		}
	}
	
	queue* double_queue = queue_init();
	for(i = 1;i <= n;i++)
	{
		node* ptr = adjacency_list_head[i];
		if(!ptr)
			continue;
		if(i < ptr->y)
		{
			node* down = adjacency_list_head[ptr->y];
			if(i == down->y)//遇到重複
				enqueue(double_queue, &adjacency_list_head[i], &adjacency_list_head[ptr->y]);
		}
	}
	int size = double_queue->size;
	while(size != 0)
	{
		size = dequeue(double_queue, adjacency_list_head);
		if(count == total_vertex_num)
			break;
	}
	
	if(count == total_vertex_num)
	{
		printf("Yes\n");
		for(i = 1;i <= count;i++)
		{
			node* ptr = &(double_queue->increase[i]);
			printf("%d %d\n", ptr->x, ptr->y);
		}
	}
	else
		printf("No\n");
}