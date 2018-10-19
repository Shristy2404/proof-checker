#include <bits/stdc++.h>
#include <iostream> 
#include <string> 
using namespace std;
string s;
//int i;
int prec(char c)
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
	string output;
	
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
			if(prec(s[i])>prec(myStack.top())|myStack.top()=='\0')
				myStack.push(s[i]);
			else //if(precendenceOrder(s[i])<=precendenceOrder(myStack.top())
			{
				while(prec(s[i])<=prec(myStack.top())&myStack.top()!='\0'){
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
int main() 
{	
	//string s;
	getline(cin,s);
	infixToPostfix(s);
return 0;}
