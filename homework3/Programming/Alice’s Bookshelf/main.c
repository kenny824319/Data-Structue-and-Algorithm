#include <stdio.h>
#include <stdlib.h>
#define find_max(a,b) ((a>b)?a:b)
//reference:treap相關資料皆是來自 http://gg.gg/merge_split_treap 及他的blog其它技術文章
//The concept of the treap is all from 日月掛長,and other technical articles from his blog.
//collaborator:與B06303047陳昱嘉同學主要一起研究關於operation 6之操作及其他操作之部分deubg
//We focus on the discussion of operation 6 and debug for other operations.

typedef struct Treap_Node
{
	struct Treap_Node* left;
	struct Treap_Node* right;
	int key, priority, size;
	int max, rev, add;//lazy tag
}node;

node* new(int key)//new出一個新的node
{
	node* cur = (node *)malloc(sizeof(node));
	cur->left = NULL;
	cur->right = NULL;
	cur->size = 1;
	cur->key = key;
	cur->max = key;
	cur->priority = rand();
	cur->rev = 0;
	cur->add = 0;
	return cur;
}
int get_size(node* n)
{
	return n? n->size : 0;
}

int get_max(node* n)
{
	return n? n->max : -2147483648;
}

void down(node* n)//將lazy tag向下傳遞更新
{
	if(!n)
		return;
	else
	{
		if(n->rev)
		{
			node* temp = n->left;
			n->left = n->right;
			n->right = temp;
			if(n->left)
				(n->left)->rev ^= 1;
			if(n->right)
				(n->right)->rev ^= 1;
			n->rev = 0;
		}
		
		if(n->add != 0)
		{
			n->key += n->add;
			n->max += n->add;
			if(n->left)
				(n->left)->add += n->add;
				
			if(n->right)
				(n->right)->add += n->add;
			n->add = 0;
		}
		return;
	}
}
 
void update(node* n)//更新size, max
{
	n->size = 1;
	n->max = n->key;
	if(n->left)
	{
		down(n->left);
		n->size += get_size(n->left);
		n->max = find_max((n->left)->max, n->max);
	}
		
	if(n->right)
	{
		down(n->right);
		n->size += get_size(n->right);
		n->max = find_max((n->right)->max, n->max);
	}
		
	return;
}

node* merge(node* a, node* b)
{
	if(!a||!b)
		return a?a:b;
	if(a->priority > b->priority)
	{
		down(a);
		a->right = merge(a->right, b);
		update(a);
		return a;
	}
	else
	{
		down(b);
		b->left = merge(a, b->left);
		update(b);
		return b;
	}
}

void split_by_size(node* n, node** a, node** b, int size)//切出大小為size的treap給a,剩餘給b
{
	if(!n)
	{
		*a = NULL;
		*b = NULL;
		return;
	}
	else
	{
		down(n);
		if(size >= get_size(n->left)+1)
		{
			*a = n;
			int residual_size = size-(get_size(n->left)+1);
			split_by_size(n->right, &((*a)->right), b, residual_size);
		}
		else
		{
			*b = n;
			split_by_size(n->left, a, &((*b)->left), size);
		}
		update(n);
	}
}


void insert(node** root, int key, int pos)//將點於pos插入
{
	node* a = NULL;
	node* b = NULL;
	split_by_size(*root, &a, &b, pos);
	b = merge(new(key), b);
		
	*root = merge(a, b);
	return;
}

void del(node** root, int k)//刪第k個點
{
	node* a = NULL;
	node* b = NULL;
	node* c = NULL;
	split_by_size(*root, &a, &c, k);
	split_by_size(a, &a, &b, k-1);
	down(b);//將lazy tag往下傳
	*root = merge(a, c);
	return;
}

void reverse(node** root, int l, int r)//序列區間反轉
{
	if(l == r)
		return;
	node* a = NULL;
	node* b = NULL;
	node* c = NULL;
	split_by_size(*root, &a, &b, l-1);
	split_by_size(b, &b, &c, r-l+1);
	b->rev ^= 1;
	*root = merge(a, merge(b, c));
	return;
	
}

void add_range(node** root, int l, int r, int num)//序列區間加值
{
	node* a = NULL;
	node* b = NULL;
	node* c = NULL;
	split_by_size(*root, &a, &b, l-1);
	split_by_size(b, &b, &c, r-l+1);
	b->add += num;
	*root = merge(a, merge(b, c));
	return;
}

int query_max(node** root, int l, int r)//序列區間求最大值
{
	node* a = NULL;
	node* b = NULL;
	node* c = NULL;
	split_by_size(*root, &a, &b, l-1);
	split_by_size(b, &b, &c, r-l+1);
	int ans = b->max;
	*root = merge(a, merge(b, c));
	return ans;
}

void del_max(node** root)//將整條序列最大值刪除,若有複數相同的最大值則刪序列最左者
{
	int pos = 1;
	node* cur = *root;
	down(cur);
	int max = cur->max;
	while(1)
	{
		down(cur->left);
		down(cur->right);
		if(get_max(cur->left) != max && cur->key == max)
			break;
		if(get_max(cur->left) == max)
			cur = cur->left;
		else
		{
			pos += (get_size(cur->left) + 1);
			cur = cur->right;
		}
	}
	
	pos += get_size(cur->left);
	del(root, pos);
	return;
}

int main()
{
	int n;
	int i;
	int priority;
	srand(423);
	node* root = NULL;
	scanf("%d", &n);
	int q, k, l, r;
	scanf("%d", &q);
	for(i = 0;i < n;i++)
	{
		scanf("%d", &priority);
		insert(&root, priority, i);
	}
	for(i = 0;i < q;i++)
	{
		int operation_id;
		scanf("%d", &operation_id);
		if(operation_id == 1)//Insert a book with priority p after k-th
		{
			scanf("%d%d", &priority, &k);
			insert(&root, priority, k);
		}
		if(operation_id == 2)// Delete the k-th book.
		{
			scanf("%d", &k);
			del(&root, k);
		}
		if(operation_id == 3)//Increase the priorities of the books between l and r
		{
			scanf("%d%d%d", &l, &r, &priority);
			add_range(&root, l, r, priority);
		}
		if(operation_id == 4)//Query the max between position l and r
		{
			scanf("%d%d", &l, &r);
			int ans = query_max(&root, l, r);
			printf("%d\n", ans);
		}
		if(operation_id == 5)//Reverse position between l and r
		{
			scanf("%d%d", &l, &r);
			reverse(&root, l, r);
		}
		if(operation_id == 6)//Remove the book with the largest priority
			del_max(&root);
	}
}