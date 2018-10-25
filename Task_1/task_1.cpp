/*!
 * \author Vishwangi Shah, Sravanthi, Shristy Kapoor
 * \version 1.0
 * \date 24-10-2018
 * \copyright GNU Public License
 * \mainpage Logic in Computer Science Project: Parse Tree Generator
 * \section intro_sec Introduction
 * This code was developed as a part of as assignment for Logic in Computer Science. It is a simple program which takes in an 
 * an infix expression and generates the parse tree and then prints it's inorder traversal back.
 * \section detail_sec Details
 * This code takes in the infix expression. 
 * \section comp_sec Compilation
 * You can compile this code by `g++ task_1.cpp` and execute it by `./a.out`
 */

// all required header files
#include <bits/stdc++.h>
#include <iostream> 
#include <string.h> 

using namespace std;

string input; //!< The string containing the infix expression(given)
string output; //!< The string containing the final postfix expression

/*!
* \brief Create node with value and two pointers, left and right
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
* \brief Return the precedence of operators
*/
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
 * \brief Function to remove spaces from a string
 * \param String in which spaces are to be removed
 * \details The function takes in the string, stores the characters except ' ' in an array and returns the number of 
 * non-space characters in the string.
 */
int remove_spaces(string &a)
{
    int count=0; /*!< Number of non-space characters */
	for(int i=0;i<a.length();i++)
	{
		if(a[i]!=' ')
        	a[count++]=a[i];
	}
	return count;
}

/*!
 * \brief Function gives the postfix expression when given an infix expression
 * \param string s takes the input infix expression
 * \details
 * 1. Scan the infix expression from left to right.
 * 2. If the scanned character is an operand, stire in output string.
 * 3. Else,
 * …..3.1 If the precedence of the scanned operator is greater than the precedence of the operator in the stack(or the stack
 *  is empty), push it.
 * …..3.2 Else, Pop the operator from the stack until the precedence of the scanned operator is less-equal to the precedence 
 * of the operator  *  residing on the top of the stack. Push the scanned operator to the stack.
 * 4. If the scanned character is an ‘(‘, push it to the stack.
 * 5. If the scanned character is an ‘)’, pop and store in output string from the stack until an ‘(‘ is encountered.
 * 6. Repeat steps 2-6 until infix expression is scanned.
 * 7. Pop and store in the output string from the stack until it is not empty.
 * 8. Print the output string.
*/
void infix_to_postfix(string input)
{
	std::stack<char> expression_stack; //!< Creating a character stack
	expression_stack.push('\0');
	
	int len = input.length(); //!< Length of input string
	int j=0;
	for(int i=0;i<len;i++) 
	{
		if((input[i]>='a' && input[i]<='z'))
		{ 
			output += input[i];
		}
		else if(input[i]=='(')
			expression_stack.push('(');
		else if(input[i]==')')
		{
			while(expression_stack.top()!='(' && expression_stack.top()!='\0')
			{
			char c = expression_stack.top();
			expression_stack.pop();
			output+=c; 
			}
			if(expression_stack.top()=='(') 
			expression_stack.pop();
		}
		else 
		{
			if(precedence(input[i])>precedence(expression_stack.top())|expression_stack.top()=='\0')
				expression_stack.push(input[i]);
			else 
			{
				while(precedence(input[i])<=precedence(expression_stack.top())&expression_stack.top()!='\0')
				{
					char c = expression_stack.top();
					expression_stack.pop();
					
					output+=c;
				}
				expression_stack.push(input[i]);
			}
		}
	} 
	
	while(expression_stack.top()!='\0')
	{ 
	    output+=expression_stack.top();
	    
	    expression_stack.pop();
	}
	cout << "The postfix expression is: " << output <<endl;
}

/*!
* \brief A function to construct the expression tree from given postfix expression 
* \param string str the given postfix expression 
* \details 
* 1. It traverses through the string and extracts each character.
* 2. If that character is not an operator, it pushes it to the stack. 
* 3. If it is an operator, then we pop two elements from the stack, assign them as child nodes of the operator obtained, and 
* pushes this entire structure instance as a new node into the stack so that it can be popped as a child of some operator later.
* 4. It returns a pointer to the root node of the constructed tree after the iterations are done.
*/
node* construct_tree(string str)
{
	stack<node *> tree_stack; //!< Creating a pointer stack
	node *t; //!< Creating a node pointer
	node *temp_right; //!< Creating a pointer that points to the right of the node
	node *temp_left; //!< Creating a pointer that points to the left of the node

	for(int i=0; i<str.length(); i++)
	{
		if(!(is_operator(str[i])))
		{
			t=create_node(str[i]);
			tree_stack.push(t);
		}
		else if(str[i]=='~')
		{
			t=create_node(str[i]);
			temp_right = tree_stack.top();
			tree_stack.pop();
			t->right = temp_right;
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
	string expression;//!< The input expression (infix)
	getline(cin,expression);
	int x= remove_spaces(expression);
	expression.resize(x);
	infix_to_postfix(expression);
	node *temp;
	temp=construct_tree(output);
	inorder(temp);
	cout << endl;
	return 0;
}
