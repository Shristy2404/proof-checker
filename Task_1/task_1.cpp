#include <bits/stdc++.h>
#include <iostream> 
#include <string.h> 

using namespace std;

string input; //!< The string containing the infix expression(given)
string output; //!< The string containing the final postfix expression
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

/*!
 * \brief Function gives the postfix expression when given an infix expression
 * \param string s takes the input infix expression
 * \details
 * 1. Scan the infix expression from left to right.
 * 2. If the scanned character is an operand, stire in output string.
 * 3. Else,
 * …..3.1 If the precedence of the scanned operator is greater than the precedence of the operator in the stack(or the stack is empty), push it.
 * …..3.2 Else, Pop the operator from the stack until the precedence of the scanned operator is less-equal to the precedence of the operator  *  residing on the top of the stack. Push the scanned operator to the stack.
 * 4. If the scanned character is an ‘(‘, push it to the stack.
 * 5. If the scanned character is an ‘)’, pop and store in output string from the stack until an ‘(‘ is encountered.
 * 6. Repeat steps 2-6 until infix expression is scanned.
 * 7. Pop and store in the output string from the stack until it is not empty.
 * 8. Print the output string.
*/
void infixToPostfix(string input)
{
	std::stack<char> expression_stack; //!< Creating a character stack
	expression_stack.push('\0');
	
	int len = input.length(); //!< Length of input string
	int j=0;
	for(int i=0;i<len;i++) 
	{
		if((input[i]>='a' && input[i]<='z'))  { 
			output += input[i];}
		else if(input[i]=='(')
			expression_stack.push('(');
		else if(input[i]==')')
		{
			while(expression_stack.top()!='(' && expression_stack.top()!='\0') {
			char c = expression_stack.top();
			expression_stack.pop();
			output+=c; }
			if(expression_stack.top()=='(') 
			expression_stack.pop();
		}
		else {
			if(precedence(input[i])>precedence(expression_stack.top())|expression_stack.top()=='\0')
				expression_stack.push(input[i]);
			else 
			{
				while(precedence(input[i])<=precedence(expression_stack.top())&expression_stack.top()!='\0'){
					char c = expression_stack.top();
					expression_stack.pop();
					
					output+=c;
				}
				expression_stack.push(input[i]);
			}
		}
	} 
	
	while(expression_stack.top()!='\0') { 
	    output+=expression_stack.top();
	    
	    expression_stack.pop();
	    }
	cout << output <<endl;
}
/*!
* \brief Create node with value and 2 pointers left and right
*/
struct node
{
	char val;
	node *left, *right;
};

/*!
* \brief A function to return a node pointer
* \param char ch : It is the value of the pointer taken from output string
*/
node* create_node(char ch)
{
	node *temp = new node;
	temp->left = NULL;
	temp->right = NULL;
	temp->val = ch;
	return temp;
}

/*!
* \brief A function to check if the scanned character is an operator or not
* \param char ch: Takes a character from string output
*/
bool is_operator(char ch)
{
	if(ch == '^' || ch == 'V' || ch == '>' || ch == '~')
		return true;
	return false;
}
/*!
 * \brief Function generates the tree from a given postfix expression
 * \param string s takes the input postfix expression
 * \details
 * 1. Scan the postfix expression from left to right.
 * 2. If the character found is not an operator, we push it in the stack. 
 * 3. If the character found is an operator, we create a new node instance and pop top two elements from stack and make them 
 * as children of the new node. 
 * 4. Then we push this new node in the stack so that the whole instance can be popped the next time a node is encountered.
 * 5. Thus the tree is constructed in the stack. 
 * 6. We return the pointer to the root node, which will be used for further traversal of the tree. 
*/
node* construct_tree(string str)
{
	stack<node *> tree_stack; //!< Creating a pointer stack
	node *t;//!< Creating a node pointer
	*temp_right;//!< Creating a pointer that points to the right of the node
	temp_left;//!< Creating a pointer that points to the left of the node

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
 

/*! 
* \brief Functon to print infix expression
* \param node *t : Node of the tree
* \details
* 1. We traverse from the root.
* 2. Go to the left subtree and go to it's left subtree again if it's exist.
* 3. Finally reach the left most leaf print it and go to it's root.
* 4. Print the root and repeat 2 and 3.
*/
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
	string expression;//!< The input expression (infix)
	getline(cin,expression);
	infixToPostfix(expression);
	node *temp;
	temp=construct_tree(output);
	inorder(temp);
	return 0;
}


