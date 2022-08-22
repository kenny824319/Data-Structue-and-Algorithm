#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Tree_node
{
	int key;
	struct Tree_node* left;
	struct Tree_node* right;
	
}node;

int count = 0;

void search(node* tree, int lower, int upper)
{
	if(tree == NULL)
		return;
	if(lower > upper)
		return;
	if((lower < tree->key) && (tree->key < upper))
		count++;
    int new_upper = upper;    
	if(tree->key<upper)
		new_upper = tree->key;
	search(tree->left, lower, new_upper);
	int new_lower = lower;
    if(lower<tree->key)
		new_lower = tree->key;
	search(tree->right, new_lower, upper);
}

int main()
{
	int n;
	scanf("%d", &n);
	
	node* tree = (node *)malloc(n * sizeof(node));//construct tree
	
	int i;
	
	int key;
	int left_id;
	int right_id;
	for(i = 0;i < n;i++)
	{
		scanf("%d%d%d", &key, &left_id, &right_id);
		node* ptr = &tree[i];
		node* temp;
		ptr->key = key;
		
		if(left_id == -1)
			ptr->left = NULL;
		else
			ptr->left = &tree[left_id-1];
		if(right_id == -1)
			ptr->right = NULL;
		else
			ptr->right = &tree[right_id-1];
	}
	search(tree, -1, 2147483647);
	printf("%d", count);
}
