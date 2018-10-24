#include <bits/stdc++.h>
#include <iostream> 
#include <string.h> 
using namespace std;

// sravanthi's part
string s;
string output;
int precedence(char c)
{
	char ch=c;
	if(ch=='~')
		return 4;
	else if(ch=='^')
		return 3;
	else if(ch=='V')
		return 2;
	else if(ch=='>')
		return 1;
	else return -1;
	
}
void infixToPostfix(string s)
{
	std::stack<char> myStack;
	myStack.push('\0');
	
	
	int l=s.length();
	int j=0;
	for(int i=0;i<l;i++) 
	{
	    //cout<<prec(s[i]);
		if((s[i]>='a' && s[i]<='z')/*||(s[i]>='A' && s[i]<='Z')*/)  { //cout<<s[i];
			output+=s[i];}
		else if(s[i]=='(')
			myStack.push('(');
		else if(s[i]==')')
		{
			while(myStack.top()!='(' && myStack.top()!='\0') {
			char c = myStack.top();
			myStack.pop();
			//cout<<c;
			output+=c; }
			if(myStack.top()=='(') 
			myStack.pop();
		}
		else {
			if(precedence(s[i])>precedence(myStack.top())|myStack.top()=='\0')
				myStack.push(s[i]);
			else //if(precendenceOrder(s[i])<=precendenceOrder(myStack.top())
			{
				while(precedence(s[i])<=precedence(myStack.top())&myStack.top()!='\0'){
					char c = myStack.top();
					myStack.pop();
					//cout<<c;
					output+=c;
				}
				myStack.push(s[i]);
			}
		}
	} 
	
	while(myStack.top()!='\0') { 
	    output+=myStack.top();
	    //cout<< myStack.top();
	    myStack.pop();
	    }
	cout << output <<endl;
}
// shristy's part
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

node* construct_tree(string str)
{
	stack<node *> tree_stack;
	node *t, *temp_right, *temp_left;

	for(int i=0; i<str.length(); i++)
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
	return t;
}

//vishu's part
void inorder(node *t)
{ 
    if(t)
    {
        if(t->left)
        	cout << "(";

        inorder(t->left);
        cout << t->val;

        inorder(t->right);
        if(t->right)
            cout << ")";
    }
}

int main() 
{	
	cin.ignore();
	string expression;
	getline(cin,expression);
	infixToPostfix(expression);
	node *temp;
	temp=construct_tree(output);
	inorder(temp);
	return 0;
}


