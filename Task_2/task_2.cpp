#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream> 

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
	//void removeSpaces();
	bool check_premise(string prem);
	bool and_introduction( int line_1, int line_2);
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


// void removeSpaces()
// {
// 	for(int i=0;i<k;i++)
// 	{
// 		int p=0;
// 		for(int j=0;j<str[i].length();j++)
// 		{
// 			if(str[i][j]!=' ')
// 			{
// 				str[i][p++]=str[i][j];
// 			}
// 		}
// 		str[i].resize(p);
// 	}
// }

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

bool proof_validator::check_premise(string prem)
{
	if(!(prem.compare("P")))
		return 1;
	return 0;
}

bool proof_validator::and_introduction( int line_1, int line_2)
{
	string and_intro_1 = str[line_1];
	string and_intro_2 = str[line_2];
	cout << and_intro_1 << " " << and_intro_2 << endl;
	
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
		string prem = temp.substr(index_1+1,1);
		if(obj.check_premise(prem))
		{
			//cout << "premise found" << endl;
			continue;
		}
		int index_2 = temp.find('/', index_1+1);
		//cout << index_1 << " " << index_2 << endl;
		string temp_rule = temp.substr(index_1+1, (index_2-index_1-1));
		//cout << temp_rule;
		int rule_index = return_rule(temp_rule);
		if(rule_index == 1)
		{
			//(a^b)/^i/1/2
			int index_3 = (temp.find('/',index_2+1));
			stringstream var1(temp.substr(index_2+1, index_3-index_2));
			int line_1 = 0;
			var1 >> line_1;
			cout << line_1 << endl;
			stringstream var2(temp.substr(index_3+1));
			int line_2 =0;
			var2 >> line_2;
			cout << line_2 << endl;
			obj.and_introduction( line_1-1, line_2-1);


		}
	} 
	return 0;
}
