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
	//class variables
	
	//number of lines in proof
	int k;
	//variable to signify is proof is valid or not 
	int flag;

	//constructor
	proof_validator(int l);

	//utility functions
	string remove_brackets(string rb);
	int return_rule_index(string rule);
	int removeSpaces();
	string only_statement(string line);
	int return_rule_operator_index(string line);

	//functions to check rules
	bool check_premise(string prem);
	bool and_introduction( int line_1, int line_2, string line);
	bool and_elimination(int line_1, string line, int rule_index, int rule_operator_pos);
	bool or_introduction(int line_1, string line, int rule_index, int rule_operator_pos);
	bool implies_elimination(int a,int b,int current, int rule_operator_pos);	
	bool modus_tollens(int a,int b,int current, int rule_operator_pos);
	int check_rules();	
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
	if(rb[0]=='(' && rb[len-1]==')')
	{
	string temp = rb.substr(1,len-2);
	rb = temp;
	rb.resize(len-2);
	}
	return rb;
}

//function to return an integer value corresponding to the rule which is to be checked
int proof_validator::return_rule_index(string rule)
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

//function to remove all spaces from a given passed string
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

//function which returns the root operator of a given expression; i.e. root of parse tree of expression
int proof_validator::return_rule_operator_index(string line)
{
	string rule_left; int index=0;
	int index_1 = line.find('/');
	int index_2 = line.find('/', index_1+1);
	string rule_right = line.substr(index_1+1,1);
	string rule_statement = remove_brackets(only_statement(line));
	int opening_brackets=0; int closing_brackets =0;
	if(rule_statement[0]!='(')
	{
		rule_left = rule_statement[1];
		index=1;
	}
	else
	{
		for(int i=0; i<rule_statement.length(); i++)
		{
			char ch = rule_statement[i];
			if(ch=='(')
				opening_brackets++;
			else if(ch==')')
				closing_brackets++;
			if(opening_brackets==closing_brackets)
			{
				index = i;
				break;
			}
		}
		rule_left = rule_statement[index+1];
	}
	if(rule_right.compare(rule_left))
	{
		flag =1;
		return -1;
	}
	return index;
}

//
int proof_validator::check_rules()
{
	vector<string>::iterator ptr;
	int this_line=0;

	for( ptr = str.begin(); ptr!= str.end(); ptr++)
	{
	    ++this_line;
		string temp = *ptr;
		int index_1 = temp.find('/');
		int index_2 = temp.find('/', index_1+1);
		string temp_rule = temp.substr(index_1+1, (index_2-index_1-1));
		int rule_index = return_rule_index(temp_rule);
		string prem = temp.substr(index_1+1,1);
		int rule_operator_pos = return_rule_operator_index(temp);

		if(check_premise(prem))
		{
			continue;
		}

		if(rule_index == 1 || rule_index == 4 || rule_index == 5)
		{
			if(rule_operator_pos==-1)
			{
			flag = 1;
			return -1;
			}
		}

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
				return -1;
			}
		}
		if(rule_index == 2 || rule_index == 3)
		{
			//cout << "and elimination begins" << endl;
			int line_1 = 0;
			stringstream var1(temp.substr(index_2+1));
			var1 >> line_1;
			//cout << line_1 << endl;
			if(!(and_elimination(line_1-1, temp, rule_index, rule_operator_pos)))
			{
				flag = 1;
				return -1;
			}
		}

		if(rule_index == 4 || rule_index == 5)
		{
			int line_1=0;
			stringstream var1(temp.substr(index_2+1));
			var1>>line_1;
			if(!(or_introduction(line_1-1, temp, rule_index, rule_operator_pos)))
			{
				flag=1;
				return -1;
			}
		}

		if(rule_index==6)
		{
            int line_1,line_2;
            int index_3=temp.find('/',index_2+1);
            stringstream var1(temp.substr(index_2+1,index_3-index_2-1));
            var1>>line_1;
            stringstream var2(temp.substr(index_3+1,temp.length()-index_3-1));
            var2>>line_2;
            if(!(implies_elimination(line_1-1,line_2-1,this_line-1, rule_operator_pos)))
            {
                flag=1;
                return -1;
            }
		}

		if(rule_index==7)
		{
            int line_1,line_2;
            int index_3=temp.find('/',index_2+1);
            stringstream var1(temp.substr(index_2+1,index_3-index_2-1));
            var1>>line_1;
            stringstream var2(temp.substr(index_3+1,temp.length()-index_3-1));
            var2>>line_2;
            if(!(modus_tollens(line_1-1,line_2-1,this_line-1, rule_operator_pos)))
            {
                flag=1;
                return -1;
            }
		}
	}
	return 0;
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
bool proof_validator::and_elimination(int line_1, string line, int rule_index, int rule_operator_pos)
{
	string and_statement = remove_brackets(only_statement(str[line_1]));
	string eliminated = only_statement(line);
	int len = eliminated.length();
	if(rule_index == 2)
	{
		string first_formula = and_statement.substr(0,rule_operator_pos-1);
		if(!(first_formula.compare(eliminated)))
			return true;
	}
	if(rule_index == 3)
	{
		string second_formula = and_statement.substr(rule_operator_pos+1);
		if(!(second_formula.compare(eliminated)))
			return true;
	}
	return false;
}

//function to check or_Introduction rules
bool proof_validator::or_introduction(int line_1, string line, int rule_index, int rule_operator_pos)
{  
	string or_beginner; 	/*!< The initial expression(line) on which we use or introduction rules  */ 
    or_beginner = only_statement(str[line_1]);

	string or_statement;  	/*!< Or introduction statement line */
	or_statement = remove_brackets(only_statement(line));
	int index = rule_operator_pos;
	if(rule_index == 4)
	{   
		string first_formula;	/*!< The substring from beginning to "V" */
		first_formula = or_statement.substr(0,index-1);
		if(!(first_formula.compare(or_beginner)))
			return true;
	}
	if(rule_index == 5)
	{  
		string second_formula;	/*!< The substring from "V" till the end */
		second_formula = or_statement.substr(index+1);

		if((second_formula.compare(or_beginner)))
			return true; 
	}	
	return false;
}

//function to check implies elimination rule
bool proof_validator::implies_elimination(int a,int b,int current, int rule_operator_pos)
{
	string y = only_statement(str[b]);
    string x = remove_brackets(only_statement(str[a]));
	string x1 = x.substr(0,rule_operator_pos);
	string x2 = x.substr(rule_operator_pos+1);
	string currentline = str[current];
	currentline=only_statement(currentline);
	if(x1.compare(y)!=0||x2.compare(currentline)!=0)
		return false;
	else
		return true;
}

//function to check Modus Tollens rule
bool proof_validator::modus_tollens(int a,int b,int current, int rule_operator_pos)
{
	string y = only_statement(str[b]);
    string x = remove_brackets(only_statement(str[a]));
	string x1=x.substr(0,rule_operator_pos);
	string x2=x.substr(rule_operator_pos+1);
	string currentline=str[current];
	currentline=only_statement(currentline);
	string negation("~");
	x1.insert(0,negation);
	x2.insert(0,negation);
	if((x2.compare(y)!=0)||(x1.compare(currentline)!=0))
        return false;
    else
        return true;
}


int main()
{
	int lines;
	int v=0;
	cin >> lines;
	cin.ignore();
	proof_validator obj(lines);
	for( int i=0; i<obj.k; i++)
	{
		string temp;
		getline(cin, temp);
		obj.str.push_back(temp);
	}

	vector<string>::iterator ptr;

	for(ptr=obj.str.begin();ptr!=obj.str.end();ptr++)
    {
        string temp=*ptr;
        int x=removeSpaces(temp);
        temp.resize(x);
        obj.str.at(v++)=temp;
    }
    int result = obj.check_rules();
    if(result == -1 && obj.flag == 1)
    	cout << "Invalid proof" << endl;
    else if(result == 0 && obj.flag == 0)
    	cout << "Valid proof" << endl;
	return 0;
}
