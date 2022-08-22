#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char expression[1000000];

struct double_stack {
  double dynamic_array[1000000];
  int top;
};

struct char_stack {
  char dynamic_array[1000000];
  int top;
};


struct double_stack* create_double_stack() {
  struct double_stack* result;
  result = malloc(sizeof(struct double_stack));
  result->top = 0;
  
  return result;
}

struct char_stack* create_char_stack() {
  struct char_stack* result;
  result = malloc(sizeof(struct char_stack));
  result->top = 0;
  
  return result;
}

void char_stack_push(struct char_stack* this, char operator)
{
	this->dynamic_array[this->top] = operator;
	this->top++;
}

char char_stack_pop(struct char_stack* this)
{
	this->top--;
	char operator = this->dynamic_array[this->top];
	return operator;
}

char char_stack_peek(struct char_stack* this)
{
	int index = this->top;
	if(index == 0)
		return '%';
	return this->dynamic_array[index-1];
}


int char_stack_is_empty(struct char_stack* this)
{
	if(this->top == 0)
		return 1;
	return 0;
}

void double_stack_push(struct double_stack* this, double digit)
{
	this->dynamic_array[this->top] = digit;
	this->top++;
}

double double_stack_pop(struct double_stack* this)
{
	this->top--;
	double digit = this->dynamic_array[this->top];
	return digit;
}

int double_stack_is_empty(struct double_stack* this)
{
	if(this->top == 0)
		return 1;
	return 0;
}

int is_digit(char input)
{
	if(input >= '0' && input <= '9')
		return 1;//is digit
	return 0;//is operator
}


int operator_weight(char operator)
{
	int weight;
	switch(operator)
	{
		case '+': 
		case '-':
			weight = 1;
			break;
		
		case '*': 
		case '/':
			
			weight = 2;
			break;
		
		case '(':
			weight = 0;
			break;
		case '%':
			weight = -1;
			break;
	}
	
	return weight;
}

void evaluate(struct char_stack* char_stack_addr, struct double_stack* double_stack_addr)
{
	char operator = char_stack_pop(char_stack_addr);
	double second_num = double_stack_pop(double_stack_addr);
	double first_num = double_stack_pop(double_stack_addr);
	double ans;
	switch(operator)
	{
		case '+':
		{
			ans = first_num+second_num;
			break;
		}
		case '-':
		{
			ans = first_num-second_num;
			break;
		}
		case '*':
		{
			ans = first_num*second_num;
			break;
		}
		case '/':
		{
			ans = first_num/second_num;
			break;
		}
	}
	double_stack_push(double_stack_addr, ans);
	
	return;
}

double sol(char* exp)
{
	int i = 0;
	int digit_num = 0;
	char digit[20] = "\0";
	struct double_stack* double_stack_addr = create_double_stack();
	struct char_stack* char_stack_addr = create_char_stack();
	while(1)
	{
		if(is_digit(exp[i]) == 1)
		{
			digit[digit_num] = exp[i];
			digit_num++;
		}
		
		if(is_digit(exp[i]) == 0)
		{
			if(digit[0] != '\0')
			{
				double_stack_push(double_stack_addr, (double)atoi(digit));//push digit
			}
			if(exp[i] != '\0')
			{
				if(char_stack_is_empty(char_stack_addr) == 1)
				{
					char_stack_push(char_stack_addr, exp[i]);
				}
				else
				{
					if(exp[i] == '(')
						char_stack_push(char_stack_addr , exp[i]);
					else
					{
						if(exp[i] == ')')
						{
							while(1){
								char operator = char_stack_pop(char_stack_addr);
								if(operator == '(')
									break;
								double second_num = double_stack_pop(double_stack_addr);
								double first_num = double_stack_pop(double_stack_addr);
								double ans;
								switch(operator)
								{
									case '+':
									{
										ans = first_num+second_num;
										break;
									}
									case '-':
									{
										ans = first_num-second_num;
										break;
									}
									case '*':
									{
										ans = first_num*second_num;
										break;
									}
									case '/':
									{
										ans = first_num/second_num;
										break;
									}
								}
								double_stack_push(double_stack_addr, ans);
							}
						}
						else if(operator_weight(exp[i]) > operator_weight(char_stack_peek(char_stack_addr))){
							char_stack_push(char_stack_addr, exp[i]);
							
						}
						else if(operator_weight(exp[i]) <= operator_weight(char_stack_peek(char_stack_addr)))
						{
							while(operator_weight(exp[i]) <= operator_weight(char_stack_peek(char_stack_addr)))
								evaluate(char_stack_addr, double_stack_addr);
							char_stack_push(char_stack_addr, exp[i]);
						}
					}
				}
			}
			for(int index = 0;index < 20;index++)
				digit[index] = '\0';
			digit_num = 0;
		}
		if(exp[i] == '\0')
			break;
		i++;
	}
	while(char_stack_is_empty(char_stack_addr) == 0)//char stack is not empty
	{
		evaluate(char_stack_addr, double_stack_addr);
	}
	double final_ans = double_stack_pop(double_stack_addr);
	free(double_stack_addr);
	free(char_stack_addr);
	return final_ans;
}

int main()
{
	while(scanf("%s", expression) != EOF)
	{
		printf("%.15f\n", sol(expression));
	}
	
	return 0;
}