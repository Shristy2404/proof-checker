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
	string remove_brackets( string rb);
	int return_rule(string rule);
	proof_validator( int l);
	//void removeSpaces();
	bool check_premise(string prem);
	bool and_introduction( int line_1, int line_2, string line);
	string only_statement(string line);
};

proof_validator:: proof_validator(int l)
{
	k=l;
}

string proof_validator::remove_brackets(string rb)
{
	cout << rb << endl;
	int len = rb.length();
	cout << len << endl;
	string temp = rb.substr(1,len-2);
	rb = temp;
	rb.resize(len-2);
	cout << rb << endl;
	return rb;
	//(a^(b^c))
}

int removeSpaces(string &a)
{
    int count=0;
	for(int i=0;i<a.length();i++)
	{
		if(a[i]!=' '){
            a[count++]=a[i];
		}
	}
	return count;
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

bool proof_validator::check_premise(string prem)
{
	if(!(prem.compare("P")))
		return 1;
	return 0;
}

bool proof_validator::and_introduction( int line_1, int line_2, string line)
{
	string and_intro_1 = only_statement(str[line_1]);
	string and_intro_2 = only_statement(str[line_2]);
	string statement_3 = only_statement(line);
    string statement_4 = remove_brackets(statement_3);
	string comp_1 = and_intro_1 + "^" + and_intro_2;
	string comp_2 = and_intro_2 + "^" + and_intro_1;
	cout << comp_1 << " " << comp_2 << endl;
	cout << statement_4 << endl;
	if( (!(statement_4.compare(comp_1))) || (!(statement_4.compare(comp_2))))
		return true;
	return false;
	
}
	

string proof_validator::only_statement(string line)
{
	string statement;
	int index = line.find('/');
	statement = line.substr(0, index);
	return statement;
}

int main()
{
	int lines;
	int v=0;
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
			int line_1, line_2 = 0;
			int index_3 = (temp.find('/',index_2+1));
			stringstream var1(temp.substr(index_2+1, index_3-index_2));
			var1 >> line_1;
			//cout << line_1 << endl;
			stringstream var2(temp.substr(index_3+1));
			var2 >> line_2;
			//cout << line_2 << endl;
			if(!(obj.and_introduction( line_1-1, line_2-1, temp)))
				break;

		}
	} 

	// for( ptr=obj.str.begin();ptr!=obj.str.end();ptr++)
 //    {
 //        string temp=*ptr;
 //        int x=removeSpaces(temp,temp.length());
 //        temp.resize(x);
 //        obj.str.at(v++)=temp;


 //    }

	return 0;
}

