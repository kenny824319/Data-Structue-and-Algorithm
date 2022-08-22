#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char email[1000000];
char ans[1000000] = "\0";
int letter[128] = {0};
int alpha[128];
char garbled_text[1000000];
int condition = 0;
int cur_condition = 0;
int min_len = 2147483647;
int head = -1;
int tail = 0;
int final_head = -1;
int final_tail = -1;


void construct_letter(char* garbled_text)
{
	int len = strlen(garbled_text);
	for(int i = 0;i < len;i++)
	{
		letter[(int)garbled_text[i]]++;
		if(letter[(int)garbled_text[i]] == 1)
			condition++;
	}
}

void search_head(char* email)
{
	
	for(int i = head+1;i < tail+1;i++)
	{
		if(letter[(int)email[i]] != 0)
		{
			alpha[(int)email[i]]++;
			if(alpha[(int)email[i]] == 1)
			{
				head = i;
				cur_condition--;
				break;
			}
		}
	}
	
}

void search(char* email)
{
	int len = strlen(email);
	int cur_substring_len;
	for(int i = 0;i < len;i++)
	{
		if(letter[(int)email[i]] != 0)
		{
			alpha[(int)email[i]]--;
			if(alpha[(int)email[i]] == 0)
				cur_condition++;
			if(cur_condition == condition)
			{
				tail = i;
				search_head(email);
				cur_substring_len = tail-head+1;
				if(cur_substring_len < min_len)
				{
					final_head = head;
					final_tail = tail;
					min_len = cur_substring_len;
				}
			}
		}
	}
}

void del(char* email, int start)
{
	int i;
	int len = strlen(email)-min_len-start;
	for(i = 0;i < len;i++)
		email[start+i] = email[start+min_len+i];
	email[start+i] = '\0';
}

int char_to_int(char ch)
{
	int num = ch - 'A';
	if(num >= 32)
		num -= 6;
	return num;
}

int power(int* pow)
{
	int q = 2147483647;
	pow[0] = 1;
	for(int i = 1;i < 100000;i++)
		pow[i] = (pow[i-1]*52)%q;
}

int my_strcmp(char* email, int left_lower_bound, int left_idx, int right_idx, int right_upper_bound)
{
	int flag = 1;
	int len = left_idx-left_lower_bound+1;
	int left = left_lower_bound;
	int right = right_idx;
	while(len > 0)
	{
		if(email[left] == email[right])
		{
			left++;
			right++;
		}
		else
		{
			flag = 0;
			break;
		}
		len--;
	}
	return flag;
}

int modify(char* email, int* pow)
{
	int len = strlen(email);//注意奇偶!!
	int left_idx = 0;
	int gong_idx = 0;
	int right_idx = len-1;
	int right_hash_idx = 0;
	int mid;
	int right_num = 52;
	int q = 2147483647;
	int left_hash_val = 0;
	int right_hash_val = 0;
	int gong_table[100000] = {0};
	int total_gong = 0;
	int flag = 0;
	int final_len;
	int enter_flag = 0;
	int left_lower_bound = 0;
	int right_upper_bound = len-1;
	if(len % 2 == 0)
		mid = len/2-1;
	else
		mid = len/2;
	while(1)
	{
		if(len == 1)
			break;
		left_hash_val = (52*left_hash_val + char_to_int(email[left_idx])) % q;
		right_hash_val = (pow[right_hash_idx] * char_to_int(email[right_idx]) + right_hash_val) % q;
		if(left_hash_val == right_hash_val)
		{
			int equal_flag = my_strcmp(email, left_lower_bound, left_idx, right_idx, right_upper_bound);
			if(equal_flag)
			{
				left_lower_bound = left_idx+1;
				right_upper_bound = right_idx-1;
				enter_flag = 1;
				gong_idx++;
				gong_table[total_gong] = gong_idx;
				total_gong++;
				right_hash_idx = -1;
				left_hash_val = 0;
				right_hash_val = 0;
				if(left_idx == mid && len % 2 == 0)
					flag = 1;
				else if(left_idx == right_idx)
					total_gong--;
					
			}
		}
		if(left_idx == mid)
		{
			final_len = len + 2*total_gong;
			if(flag)
				final_len--;
			break;
		}
			
		left_idx++;
		gong_idx++;
		right_idx--;
		right_hash_idx++;
	}
	if(enter_flag == 0)
		return enter_flag;
	else
	{
		int current_idx = 0;
		int email_idx = 0;
		for(int i = 0;i < total_gong;i++)
		{
			for(int j = current_idx, k = 0;j < gong_table[i];j++, k++)
			{
				ans[j] = email[email_idx];
				ans[final_len-gong_table[i]+k] = email[email_idx];
				email_idx++;
			}
			ans[gong_table[i]] = '|';
			ans[final_len-gong_table[i]-1] = '|';
			current_idx = gong_table[i]+1;
		}
		ans[final_len] = '\0';
		int residual_len = final_len-gong_table[total_gong-1]-1;
		for(int i = gong_table[total_gong-1]+1;i < residual_len;i++)
		{
			ans[i] = email[email_idx];
			email_idx++;
		}
		
		return enter_flag;	
	}
}

int main()
{
	int t;
	int pow[100000];
	power(pow);
	scanf("%d", &t);
	while(t > 0)
	{
		int i;
		int enter_flag;
		scanf("%s", email);
		scanf("%s", garbled_text);
		construct_letter(garbled_text);
		for(i = 0;i < 128;i++)
			alpha[i] = letter[i];
		search(email);
		if(final_head != -1 && final_tail != -1)
			del(email, final_head);
		enter_flag = modify(email, pow);
		if(enter_flag)
			printf("%s\n", ans);
		else
			printf("%s\n", email);
		
		condition = 0;
		cur_condition = 0;
		head = -1;
		tail = 0;
		final_head = -1;
		final_tail = -1;
		min_len = 2147483647;
		memset(ans,'\0',sizeof(ans)); 
		for(i = 0;i < 128;i++)
			letter[i] = 0;
		t--;
	}
}