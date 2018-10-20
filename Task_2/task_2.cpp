//all header files 
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream> 

//defining macros for rule names 
#define AND_INTRODUCTION 1
#define AND_ELIMINATION_1 2
#define AND_ELIMINATION_2 3
#define OR_INTRODUCTION_1 4
#define OR_INTRODUCTION_2 5
#define IMPLIES_ELIMINATION 6
#define MODUS_TOLLENS 7

using namespace std;

//declaring class proof_validator
class proof_validator
{
	//class has a string vector and length of the proof as class variables 
public:
	std::vector<std::string> str;
	int k;
	int flag; 
	string remove_brackets( string rb);
	int return_rule(string rule);
	proof_validator( int l);
	//void removeSpaces();
	bool check_premise(string prem);
	bool and_introduction( int line_1, int line_2, string line);
	bool and_elimination(int line_1, string line, int rule_index);
	string only_statement(string line);
	void check_rules();
};

//constructor to assign l to k; initialize proof length value 
proof_validator:: proof_validator(int l)
{
	k=l;
	flag=0;
}

//function to remove first and last bracket from the string passed 
string proof_validator::remove_brackets(string rb)
{
	int len = rb.length();
	string temp = rb.substr(1,len-2);
	rb = temp;
	rb.resize(len-2);
	return rb;
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

//function to return the actual statement before the rule specifications in the proof line	
string proof_validator::only_statement(string line)
{
	string statement;
	int index = line.find('/');
	statement = line.substr(0, index);
	return statement;
}

//function to return an integer value corresponding to the rule which is to be checked 
int proof_validator::return_rule(string rule)
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

void proof_validator::check_rules()
{
	vector<string>::iterator ptr;

	for( ptr = str.begin(); ptr!= str.end(); ptr++)
	{
		string temp = *ptr;
		int index_1 = temp.find('/');
		string prem = temp.substr(index_1+1,1);
		if(check_premise(prem))
		{
			continue;
		}
		int index_2 = temp.find('/', index_1+1);
		string temp_rule = temp.substr(index_1+1, (index_2-index_1-1));
		int rule_index = return_rule(temp_rule);	

		if(rule_index == 1)
		{
			int line_1, line_2 = 0;
			int index_3 = temp.find('/', index_2+1);
			stringstream var1(temp.substr(index_2+1, index_3-index_2));
			var1 >> line_1;
			stringstream var2(temp.substr(index_3+1));
			var2 >> line_2;
			if(!(and_introduction(line_1-1, line_2-1, temp)))
			{
				flag=1;
				break;
			}
		}
		if(rule_index == 2 || rule_index == 3)
		{
			//cout << "and elimination begins" << endl;
			int line_1 = 0;
			stringstream var1(temp.substr(index_2+1));
			var1 >> line_1;
			//cout << line_1 << endl;
			if(!(and_elimination(line_1-1, temp, rule_index)))
			{
				flag = 1;
				break;
			}
			else
				cout << "and elim valid" << endl;
		}

	}
}

//function to check if given line of proof is a premise or not 
bool proof_validator::check_premise(string prem)
{
	if(!(prem.compare("P")))
		return true;
	return false;
}

//function to check and_introduction rule 
bool proof_validator::and_introduction(int line_1, int line_2, string line)
{
    string statement_1 = remove_brackets(only_statement(line));
	string comp_1 = only_statement(str[line_1]) + "^" + only_statement(str[line_2]);
	string comp_2 = only_statement(str[line_2]) + "^" + only_statement(str[line_1]);
	if( (!(statement_1.compare(comp_1))) || (!(statement_1.compare(comp_2))))
		return true;
	return false;
}

//function to check and_elimination rule 
bool proof_validator::and_elimination(int line_1, string line, int rule_index)
{
	string and_statement = remove_brackets(only_statement(str[line_1]));
	//cout << and_statement << endl;
	string eliminated = only_statement(line);
	//cout << eliminated << endl;
	int len = eliminated.length();
	if(rule_index == 2)
	{
		string first_formula = and_statement.substr(0,len);
		//cout << first_formula << endl;
		if(!(first_formula.compare(eliminated)))
			return true;
	}
	if(rule_index == 3)
	{
		string second_formula = and_statement.substr(and_statement.length()-len);
		//cout << second_formula << endl;
		if(!(second_formula.compare(eliminated)))
			return true;
	}	
	return false;
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
	obj.check_rules();

	// for( ptr=obj.str.begin();ptr!=obj.str.end();ptr++)
 //    {
 //        string temp=*ptr;
 //        int x=removeSpaces(temp,temp.length());
 //        temp.resize(x);
 //        obj.str.at(v++)=temp;


 //    }

	return 0;
}

