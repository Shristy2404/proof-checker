#include <bits/stdc++.h> 
#include <string.h>

using namespace std:

struct node
{
	char val;
	node *left, *right;
};

node* create_node(char ch)
{
	node *temp = new node;
	temp->left = NULL;
	temp->right = NULL;
	temp->val = ch;
	return temp;
}

bool is_operator(char ch)
{
	if(ch == '^' || ch == 'V' || ch == '>' || ch == '~')
		return true;
	return false;
}

void construct_tree(string str)
{
	stack<node *> tree_stack;
	node *t, *temp_right, *temp_left;

	for(int i=0; i<str.length(), i++)
	{
		if(!(is_operator(str[i])))
		{
			t=create_node(str[i]);
			tree_stack.push(t);
		}
		else
		{
			t=create_node(str[i]);

			temp_right = tree_stack.top();
			tree_stack.pop();
			temp_left = tree_stack.top();
			tree_stack.pop();

			t->right = temp_right;
			t->left = temp_left;

			tree_stack.push(t);
		}
	}
}

int main()
{
	string postfix;
	cin << postfix;
	construct_tree(postfix);
	return 0;
}