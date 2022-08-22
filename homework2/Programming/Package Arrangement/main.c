#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//reference:https://www.geeksforgeeks.org/leftist-tree-leftist-heap/

typedef struct leftist_heap_node
{
	int key;
	int dist;
	int is_del_flag;
	struct leftist_heap_node* left;//maintain relation of leftist heap
	struct leftist_heap_node* right;//maintain relation of leftist heap
	struct leftist_heap_node* prev;//maintain relation of queue
	struct leftist_heap_node* next;//maintain relation of queue
}node;

int pop_k = 0;//current position of pop_permutation

node* merge(node* h1, node* h2);

node* Merge1(node* h1, node* h2)
{
    if (h1->left == NULL)
        h1->left = h2;
    else
    {
        h1->right = merge(h1->right, h2);
        if (h1->left->dist < h1->right->dist)
		{
			node* temp = h1->left;
			h1->left = h1->right;
			h1->right = temp;
		}
        h1->dist = (h1->right)->dist + 1;
    }
    return h1;
}

node* merge(node* h1, node* h2)
{
	/* if(h1 == NULL)
		return h2;
	if(h2 == NULL)
		return h1; */
	if(!h1 || !h2)
		return (h1 == NULL) ? h2 : h1;
	if (h1->key > h2->key)
	{
		return Merge1(h1, h2);
	}
    else
        return Merge1(h2, h1);
}

void insert(node** root, node** start, node** end, int package_num)
{
	node* cur = (node *)malloc(sizeof(node));
	
	cur->key = package_num;
	cur->left = NULL;
	cur->right = NULL;
	cur->prev = NULL;
	cur->next = NULL;
	cur->dist = 0;
	cur->is_del_flag = 0;
	
	if(*root == NULL)
	{
		*start = cur;
		*end = cur;
	}
	else
	{
		(*end)->next = cur;
		cur->prev = *end;
		*end = cur;
	}

	*root = merge(cur, *root);//construct leftist heap
	
}

int pop(node** root, node** start, node** end, int package_id)
{
	if(*root == NULL)
		return 0;
	
	int del_flag = 0;
	int start_flag = 0;
	int end_flag = 0;
	int max_flag = 0;
	
	if((*root)->key == package_id)//pop max
	{
		(*root)->is_del_flag = 1;
		max_flag = 1;
		del_flag = 1;
	}
	if((*start)->key == package_id)//pop start
	{
		(*start)->is_del_flag = 1;
		start_flag = 1;
		del_flag = 1;
	}
	if((*end)->key == package_id)//pop end
	{
		(*end)->is_del_flag = 1;
		end_flag = 1;
		del_flag = 1;
	}
	if(del_flag == 1)
		pop_k++;
	if(start_flag && end_flag)
	{
		*start = NULL;
		*end = NULL;
	}
	else if(start_flag == 1)
	{
		(*start) = (*start)->next;
		(*start)->prev = NULL;
	}
	else if(end_flag == 1)
	{
		(*end) = (*end)->prev;
		(*end)->next = NULL;
	}
	else if(max_flag == 1)
	{
		node* previous = (*root)->prev;
		node* next = (*root)->next;
		previous->next = next;
		next->prev = previous;
	}
	
	while((*root)->is_del_flag == 1)
	{
		*root = merge((*root)->left, (*root)->right);
		if(*root == NULL)
			break;
	}

	return del_flag;
	
}

int find(int i, int* production_line)//path compression
{
	if(i != production_line[i])
	{
		production_line[i] = find(production_line[i], production_line);
		return production_line[i];
	}
	else
		return i;
}

void Union(int i, int j, int* production_line)
{
	production_line[find(i, production_line)] = production_line[find(j, production_line)];
}

void merge_heap(node** root, node** start, node** end, int source, int target)
{
	if(!root[source] && !root[target])
		return;
	else if(!root[source])
		return;
	else
	{
		if(root[target] != NULL)
		{
			start[source]->prev = end[target];
			end[target]->next = start[source];
			start[source] = NULL;
			end[target] = end[source];
		}
		else
		{
			start[target] = start[source];
			end[target]= end[source];
			start[source] = NULL;
			end[source] = NULL;
		}
		root[target] = merge(root[source], root[target]);
		root[source] = NULL;
	}
	
}

int main()
{
	int t;
	scanf("%d", &t);
	while(t > 0)
	{
		int i;
		int n;//num of package
		int o;//num of operation
		int l;//num of line
		scanf("%d%d%d", &n, &o, &l);
		char command[o][8];
		int source[o];
		int target[o];
		int pop_permutation[n];
		node* root[l];
		node* start[l];
		node* end[l];
		
		for(i = 0;i < l;i++)
		{
			root[i] = NULL;
			start[i] = NULL;
			end[i] = NULL;
		}
		
		int package_info[n+1];
		int production_line[l];//disjoint set of production_line
		
		for(i = 0;i < l;i++)
			production_line[i] = i;
		
		int cur_heap_id;
		int del_flag = 0;
		
		for(i = 0;i < o;i++)
			scanf("%s%d%d", &command[i], &source[i], &target[i]);
		
		for(i = 0;i < n;i++)
			scanf("%d", &pop_permutation[i]);
		
		for(i = 0;i < o;i++)
			if(strcmp(command[i], "push") == 0)
				package_info[source[i]] = target[i];
		
		for(i = 0;i < o;i++)
		{
			if(strcmp(command[i], "push") == 0)
				insert(&root[target[i]], &start[target[i]], &end[target[i]], source[i]);
			else if(strcmp(command[i], "merge") == 0)
			{
				Union(source[i], target[i], production_line);
				merge_heap(root, start, end, source[i], target[i]);
			}
			cur_heap_id = find(package_info[pop_permutation[pop_k]], production_line);
			del_flag = pop(&root[cur_heap_id], &start[cur_heap_id], &end[cur_heap_id], pop_permutation[pop_k]);
			
			while((pop_k < n) && (del_flag == 1))
			{
				cur_heap_id = find(package_info[pop_permutation[pop_k]], production_line);
				del_flag = pop(&root[cur_heap_id], &start[cur_heap_id], &end[cur_heap_id], pop_permutation[pop_k]);
			}
			if(pop_k >= n)
				break;
		}
		if(pop_k == n)
			printf("possible\n");
		else
			printf("impossible\n");
		t--;
		pop_k = 0;
	}
}
