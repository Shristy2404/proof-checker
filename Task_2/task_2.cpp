#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>

#define AND_INTRODUCTION 1
#define AND_ELIMINATION_1 2
#define AND_ELIMINATION_2 3
#define OR_INTRODUCTION_1 4
#define OR_INTRODUCTION_2 5
#define IMPLIES_ELIMINATION 6
#define MODUS_TOLLENS 7

using namespace std;

class proof_validator
{
public:
	std::vector<std::string> str;
	int k;
	void remove_brackets( string rb);
	int return_rule(string rule);
	proof_validator( int l);
}; 

proof_validator:: proof_validator(int l)
{
	k=l;
}

string remove_brackets(string rb)
{
	int len = rb.length();
	string temp = rb.substr(1,len-2);
	rb = temp;
	rb.resize(len-2);
	return rb;
}

int return_rule(string rule)
{
	if(!(rule.compare("^i")))
		return AND_INTRODUCTION;
	else if(!(rule.compare("^e1")))
		return AND_ELIMINATION_1;
	else if(!(rule.compare("^e2")))
		return AND_ELIMINATION_2;
	else if(!(rule.compare("Vi1")))
		return OR_INTRODUCTION_1;
	else if(!(rule.compare("Vi2")))
		return OR_INTRODUCTION_2;
	else if(!(rule.compare(">e")))
		return IMPLIES_ELIMINATION;
	else if(!(rule.compare("MT")))
		return MODUS_TOLLENS;
	return -1;
}

int main()
{	
	int lines;
	cin >> lines;
	proof_validator obj(lines);
	for( int i=0; i<obj.k; i++)
	{
		string temp;
		cin >> temp;
		obj.str.push_back(temp);
	}
	vector<string>::iterator ptr;

	for( ptr = obj.str.begin(); ptr!= obj.str.end(); ptr++)
	{
		string temp = *ptr;
		int index_1 = temp.find('/');
		int index_2 = temp.find('/', index_1+1);
		//cout << index_1 << " " << index_2 << endl;
		string temp_rule = temp.substr(index_1+1, (index_2-index_1-1));
		//cout << temp_rule;
		int rule_index = return_rule(temp_rule);
		cout << rule_index;
	} 
	return 0;
}