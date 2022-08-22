#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include <assert.h>
//reference:https://www.geeksforgeeks.org/merge-sort/

long long int ans = 0;

typedef struct Triangle_node
{
	int top;
	int left;
	int right;
	
}node;

void demo(node* array)
{
	printf("triangle top:%d\n", array->top);
	printf("triangle left:%d\n", array->left);
	printf("triangle right:%d\n", array->right);
	printf("\n");
}
node L[3000000], R[3000000];

void merge(node* arr , int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
	{
		node* ptr_left = &L[i];
		node* ptr_right = &R[j];
        if ((ptr_left->top) < (ptr_right->top)) 
		{
            arr[k] = L[i];
            i++;
        }
		else if((ptr_left->top) == (ptr_right->top))//若頂點相同 右邊點較大者則先放入
		{
			if((ptr_left->right) > (ptr_right->right))
			{
				arr[k] = L[i];
				i++;
			}
			else
			{
				arr[k] = R[j];
				j++;
			}
		}
        else 
		{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) 
	{
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) 
	{
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(node* arr, int l, int r)
{
    if (l < r) 
	{
        // Same as (l+r)/2, but avoids overflow for larger l and r
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

node left_L[3000000], left_R[3000000];
node right_L[3000000], right_R[3000000];

void merge_query(node* left, node* right, int l, int m, int r)
{
	int i, j;
    int n1 = m - l + 1;
    int n2 = r - m;
 

	for (i = 0; i < n1; i++)
	{
        left_L[i] = left[l + i];
		right_L[i] = right[l + i];
	}
    for (j = 0; j < n2; j++)
	{
		left_R[j] = left[m+1+j];
		right_R[j] = right[m+1+j];
	}
	
	
	int left_i = 0;
	int left_j = 0;
	int right_i = 0;
	int right_j = 0;
	
	int left_k = l;
	int right_k = l;
	int flag = 0;
	
	//calculate the number of overlap triangle
	for(left_j = 0;left_j < n2;left_j++)
	{
		while((&left_R[left_j])->left > (&right_L[right_i])->right)
		{
			right_i++;
			if(right_i == n1)
			{
				flag = 1;
				break;
			}
		}
		long long int temp = (long long int)(n1-right_i);
		ans += temp;
		if(flag == 1)
			break;
	}
	
	left_j = 0;
	right_i = 0;
    
	while ((left_i < n1 && left_j < n2) || (right_i < n1 && right_j < n2))
	{
		node* left_array_left = &left_L[left_i];
		node* left_array_right = &left_R[left_j];
		node* right_array_left = &right_L[right_i];
		node* right_array_right = &right_R[right_j];
		
		if(left_i < n1 && left_j < n2)
		{
			if(left_array_left->left <= left_array_right->left)
			{
				left[left_k] = left_L[left_i];
				left_i++;
			}
			else
			{
				left[left_k] = left_R[left_j];
				left_j++;
			}
			
			left_k++;	
		}
		if(right_i < n1 && right_j < n2)
		{
			if(right_array_left->right <= right_array_right->right)
			{
				right[right_k] = right_L[right_i];
				right_i++;
			}
			else
			{
				right[right_k] = right_R[right_j];
				right_j++;
				
			}
			
			right_k++;
		}
    }
 

    while (left_i < n1) 
	{
        left[left_k] = left_L[left_i];
        left_i++;
        left_k++;
    }
 

    while (left_j < n2) 
	{
        left[left_k] = left_R[left_j];
        left_j++;
        left_k++;
    }
	
	while (right_i < n1) 
	{
        right[right_k] = right_L[right_i];
        right_i++;
        right_k++;
    }

    while (right_j < n2) 
	{
        right[right_k] = right_R[right_j];
        right_j++;
        right_k++;
    }
}


void merge_sort_2array(node* left, node* right, int l, int r)
{
    if (l < r) 
	{
        // Same as (l+r)/2, but avoids overflow for large l and r
        int m = l + (r - l) / 2;
        merge_sort_2array(left, right, l, m);
        merge_sort_2array(left, right, m + 1, r);
        merge_query(left, right, l, m, r);
    }
}

int main()
{
	generator.init();
	int t = generator.getT();
	while (t--)
	{
		int n, *p, *q, *r;		
		generator.getData(&n, &p, &q, &r);
		node* array = (node *)malloc(n * sizeof(node));//construct triangle node array
		node* left = (node *)malloc(n * sizeof(node));//construct triangle node array
		node* right = (node *)malloc(n * sizeof(node));//construct triangle node array
		int i, j;
		for(i = 0;i < n;i++)
		{
			node* ptr = &array[i];
			ptr->top = p[i];
			if(r[i] < q[i])
			{
				int temp;
				temp = q[i];
				q[i] = r[i];
				r[i] = temp;
			}
			ptr->left = q[i];
			ptr->right = r[i];
		}
		mergeSort(array, 0, n-1);//sort by top
		
		for(i = 0;i < n;i++)
		{
			left[i] = array[i];
			right[i] = array[i];
		}
		merge_sort_2array(left, right, 0, n-1);
		
		
		printf("%lld\n", ans);
		ans = 0;
	}
}