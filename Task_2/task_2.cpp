/*!
 * \author Vishwangi Shah, Shravanti, Shristy Kapoor
 * \version 1.0
 * \date 24-10-2018
 * \copyright GNU Public License
 * \mainpage Logic in Computer Science Project: A simple proof-checker
 * \section intro_sec Introduction
 * This code was developed as a part of as assignment for Logic in Computer Science. It is a simple proof-checker which takes
 * in a proof and outputs if the proof is valid or not.
 * \section detail_sec Details
 * This code takes in the number of lines in your proof and the lines as inputs, and returns if the proof is valid or not.
 * \subsection Rules Checked
 * Rules allowed in the proof are :
 * 1) Premise
 * 2) And Introduction
 * 3) And Elimination
 * 4) Or Introduction
 * 5) Implies Elimination
 * 6) Modus Tollens
 * \section comp_sec Compilation
 * You can compile this code by `g++ task_2.cpp` and execute it by `./a.out`
 */

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
	int k;         //! Number of lines in proof
    //number of lines in proof
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
	bool and_elimination(int line_1, string line, int rule_index);
	bool or_introduction(int line_1, string line, int rule_index);
	bool implies_elimination(int a,int b,int current);
	bool modus_tollens(int a,int b,int current);
	int check_rules();
};

/*!
 * \brief Constructor to assign l to k, i.e., initialize proof length value
 * \param l number of lines in the proof
 */
proof_validator:: proof_validator(int l)
{
	k=l;
	flag=0;
}


/*!
 * \brief Function to remove first and last bracket from the string passed
 * \param string rb string from which you need to remove the first and last bracket
 * \details It checks if the first and last characters of the string are '(' and ')' respectively. If yes, it returns
 * the substring between these two positions after resizing, else, returns the same string which was received as input.
 */

//function to remove first and last bracket from the string passed
string proof_validator::remove_brackets(string rb)
{
	int len = rb.length(); /*!< Length of string */
	if(rb[0]=='(' && rb[len-1]==')')
	{
	string temp = rb.substr(1,len-2); /*!< String without bracket*/
	rb = temp;
	rb.resize(len-2);
	}
	return rb;
}

/*!
 * \brief Function to return an integer value corresponding to the rule which is to be checked
 * \param string rule string which is the rule that has been implemented in that particular line
 * \details It compares the received string with the permitted rules and returns the matching macro value for the
 * obtained rule, if no option matches, it returns -1.
 */
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

/*!
 * \brief Function to remove spaces from a string
 * \param String in which spaces are to be removed
 * \details The function takes in the string, stores the characters except ' ' in an array and returns the number of non-space characters in the string.
 */
int removeSpaces(string &a)
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
 * \brief Function to return the actual statement before the rule specifications in the proof line
 * \param string line string which is the line whose concluded or claimed statement we want
 * \details It extracts the index of the first '/' character and returns the subtring upto that index
 */
string proof_validator::only_statement(string line)
{
	string statement; /*!< String storing only statement */
	int index = line.find('/'); /*!< Index of first '/' occuring in the string */
	statement = line.substr(0, index);
	return statement;
}

/*!
 * \brief Function which returns the root operator of a given expression; i.e. root of parse tree of expression
 * \param string line string which is the line whose root operator we need
 * \details The function checks different possible scenarios for the expression which can be received. If the expression still
 * has parantheses after removing the outermost parantheses, it counts upto that index in the string where the number of opening
 * and closing brackets become equal and returns that index. Else it returns 1 or 2 depending on whether the first atom has a
 * negation or not.
 */
int proof_validator::return_rule_operator_index(string line)
{
	string rule_left;
	int index=0;
	int index_1 = line.find('/');
	int index_2 = line.find('/', index_1+1);
	string rule_right = line.substr(index_1+1,1);
	string rule_statement = remove_brackets(only_statement(line));
	int opening_brackets=0; int closing_brackets =0;
	if(rule_statement[0]!='(')
	{
		rule_left = rule_statement[1];
		index=0;
	}
	if(rule_statement[0]=='~' && rule_statement[1]!='(')
	{
		rule_left = rule_statement[2];
		index=1;
	}
	else if(rule_statement[0]=='(' || (rule_statement[0]=='~' && rule_statement[1]=='('))
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
	if(line.substr(index_1+1).compare("P"))
	{
		if(rule_right.compare(rule_left))
		{
		flag =1;
		return -1;
		}
	}
	return (index+1);
}

/*!
 * \brief Main driver function to check the proof line by line, if any line is wrong, it returns -1
 * \details The function extracts the rule index and calls the appropriate rule_checking function to check that line. All the
 * required functions are called corresponding to whatever rule index is received for that particular line.
 */
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
			int line_1 = 0; //!< The index of the line on which and-elimination is performed.
			stringstream var1(temp.substr(index_2+1));
			var1 >> line_1;
			cout << line_1 << endl;
			if(!(and_elimination(line_1-1, temp, rule_index)))
			{
				flag = 1;
				return -1;
			}
		}

		if(rule_index == 4 || rule_index == 5)
		{
			int line_1=0; //!< The index of the line on which or-introduction is performed.
			stringstream var1(temp.substr(index_2+1));
			var1 >> line_1;
			cout << line_1 << endl;
			if(!(or_introduction(line_1-1, temp, rule_index)))
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
            if(!(implies_elimination(line_1-1,line_2-1,this_line-1)))
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
            if(!(modus_tollens(line_1-1,line_2-1,this_line-1)))
            {
                flag=1;
                return -1;
            }
		}
    }
    cout << "line" << this_line << "correct" << endl;
	return 0;
}

/*!
 * \brief Function to check if given line of proof is a premise or not
 * \param string prem Takes the string after first '/' for each line
 * \details The function checks if the passed string is 'P' or not i.e if the given line is a premise or not since that
 * is a special case.
 */
bool proof_validator::check_premise(string prem)
{
	if(!(prem.compare("P")))
		return true;
	return false;
}

/*!
 * \brief Function to check AND Introduction rule
 * \param
 * 1. line_1: The line from where first statement of AND will come
 * 2. line_2: The line from where second statement of AND will come
 * 3. line: Line where AND Introduction has been concluded
 * \details
 * 1. This function extracts the AND Statement.
 * 2. Then it extracts the two statements from which it is claimed to be concluded.
 * 3. Then it concatenates them, and checks if they are equal or not.
 */
bool proof_validator::and_introduction(int line_1, int line_2, string line)
{
    string statement_1 = remove_brackets(only_statement(line));
	string comp_1 = only_statement(str[line_1]) + "^" + only_statement(str[line_2]);
	string comp_2 = only_statement(str[line_2]) + "^" + only_statement(str[line_1]);
	if( (!(statement_1.compare(comp_1))) || (!(statement_1.compare(comp_2))))
		return true;
	return false;
}

/*!
* \brief Function to check AND Elimination rule
* \param
* 1. line_1: The line on which or introduction is performed
* 2. line : The or-introduction line
* 3. rule_index : The index where '^' is present
* \details
* 1. For AND elimination-1 we take the string before '^'
* 2. We compare this string with the string on which AND elimination is performed
* 3. If it matches then we continue, else return invalid input.
* 4. For AND elimination-2 we take the string after '^'
* 5. We compare this string with the string on which AND elimination is performed
* 6. If it matches then we continue, else return invalid input.
*/
bool proof_validator::and_elimination(int line_1, string line, int rule_index)
{
	string and_statement; //!< The line on which AND elimination is performed.
	and_statement = remove_brackets(only_statement(str[line_1]));
	int rule_operator_pos; //!< The index of '^'.
	rule_operator_pos = return_rule_operator_index(str[line_1]);
	string eliminated; //!< The string which is eliminated.
	eliminated = only_statement(line);
	if(rule_index == 2)
	{
		string first_formula; //!< The substring in and-statement till '^'.
 		first_formula = and_statement.substr(0,rule_operator_pos);
		if(!(first_formula.compare(eliminated)))
			return true;
	}
	if(rule_index == 3)
	{
		string second_formula; //!< The substring in and-statement from '^' to last.
		second_formula = and_statement.substr(rule_operator_pos+1);
		if(!(second_formula.compare(eliminated)))
			return true;
	}
	return false;
}

/*!
* \brief Function to check OR Introduction rule
* \param
* 1. line_1: The line on which or introduction is performed
* 2. line : The or-introduction line
* 3. rule_index : The index where 'V' is present
* \details
* 1. For OR introduction-1 we take the string before 'V'
* 2. We compare this string with the string on which OR introduction is performed
* 3. If it matches then we continue, else return invalid input.
* 4. For OR introduction-2 we take the string after 'V'
* 5. We compare this string with the string on which OR introduction is performed
* 6. If it matches then we continue, else return invalid input.
*/
bool proof_validator::or_introduction(int line_1, string line, int rule_index)
{
	string or_beginner = only_statement(str[line_1]); /*!< The initial expression(line) on which we use or introduction rules */
	string or_statement = remove_brackets(only_statement(line)); /*!< Or introduction statement line */
    //cout << or_beginner << endl;
	//cout << or_statement << endl;
	int rule_operator_pos = return_rule_operator_index(line);
	if(rule_index == 4)
	{
		string first_formula;	/*!< The substring from beginning to "V" */
		first_formula = or_statement.substr(0,rule_operator_pos);
		if(!(first_formula.compare(or_beginner)))
			return true;
	}
	if(rule_index == 5)
	{
		string second_formula;	/*!< The substring from "V" till the end */
		second_formula = or_statement.substr(rule_operator_pos+1);
		if(!(second_formula.compare(or_beginner)))
			return true;
	}
	return false;
}

/*!
 * \brief A function to check if the implies elimination rule is used correctly.
 * \param 1st parameter- Index of the line in which the implies is eliminated 2nd parameter- Index of the line which leads to implies elimination
 * 3rd parameter- Index of the line which is the result of implies elimination.
 * \detail From the line having the implies statement, the function gets the strings before and after the '>' symbol and compares it with the strings
 * given by parameter 2 and parameter 3 respectively. If each pair of strings are equal, the function returns 'true' else 'false'.
 */
bool proof_validator::implies_elimination(int a,int b,int current)
{
    string x = remove_brackets(only_statement(str[a]));
    int rule_operator_pos=return_rule_operator_index(str[a]);
	string y = only_statement(str[b]);
	string x1 = x.substr(0,rule_operator_pos);
	string x2 = x.substr(rule_operator_pos+1);
	string currentline = str[current];
    string implies_statement = remove_brackets(only_statement(str[a])); /*!< Implies statement */
    //cout<<x<<endl;
    int rule_operator_pos=return_rule_operator_index(str[a]); /*!< Index of '>' in the implies statement. */
    //cout<<rule_operator_pos<<endl;
	string statement_1 = only_statement(str[b]); /*!< Propositional formula in the line at parameter 2 */
	//cout<<y<<endl;
	string before_implies = implies_statement.substr(0,rule_operator_pos); /*!< String before '>' in the implies statement */
	//cout<<x1<<endl;
	string after_implies = implies_statement.substr(rule_operator_pos+1);  /*!< String after '>' in the implies statement */
	//cout<<x2<<endl;
	string currentline = str[current]; /*!< Current line */
	currentline=only_statement(currentline);
	if(before_implies.compare(statement_1)!=0||after_implies.compare(currentline)!=0)
		return false;
	else
		return true;
}

/*!
 * \brief A function to check if the Modus Tollens rule is used correctly.
 * \param 1st parameter- Index of the line on which the MT rule is used. 2nd parameter- Index of the line which leads to the applying MT
 * 3rd parameter- Index of the line which is the result of MT.
 * \detail From the line having the implies statement, the function gets the strings before and after the '>' symbol and introduces a '~'
 * symbol before these strings. These strings are then compared with the strings given by parameter 3 and parameter 2 respectively. If
 * each pair of strings are equal, the function returns true else false.
 */
bool proof_validator::modus_tollens(int a,int b,int current)
{
	int rule_operator_pos=return_rule_operator_index(str[a]);
	string y = only_statement(str[b]);
	cout<<y<<endl;
    string x = remove_brackets(only_statement(str[a]));
	string x1=x.substr(0,rule_operator_pos);
	string x2=x.substr(rule_operator_pos+1);
	string currentline=str[current];
	currentline=only_statement(currentline);
	string negation("~");
	x1.insert(0,negation);
	x2.insert(0,negation);
	if((x2.compare(y)!=0)||(x1.compare(currentline)!=0))
	int rule_operator_pos=return_rule_operator_index(str[a]); /*!< Index of '>' in the implies statement */
	//cout<<rule_operator_pos<<endl;
	//cout << str[b] << endl;
	string statement_1 = only_statement(str[b]); /*!< Propositional formula in the line at parameter 2 */
	//cout<<y<<endl;
    string implies_statement = remove_brackets(only_statement(str[a])); /*!< Implies statement */
	string before_implies=implies_statement.substr(0,rule_operator_pos); /*!< String before '>' in implies statement */
	string after_implies=implies_statement.substr(rule_operator_pos+1); /*!< String after '>' in implies statement */
	string currentline=str[current]; /*!< Current line */
	currentline=only_statement(currentline);
	//cout<<currentline<<endl;
	string negation("~"); /*!< character ~ */
	before_implies.insert(0,negation);
	after_implies.insert(0,negation);
	//cout<<x1<<endl;
	//cout<<x2<<endl;
	//cout<<x1<<endl;
	//cout<<x2<<endl;
	//string p="~"+x1;
	//string q="~"+x2;
	if((after_implies.compare(statement_1)!=0)||(before_implies.compare(currentline)!=0))
        return false;
    else
        return true;
}

/*!
* \brief main() function where execution begins
* \details The function accepts the number of lines first and adds them to the vector defined as class variable. Then a
* iterator object iterates over the vector and removes spaces from all the lines. Then the function just calls check_rules()
* function. If '-1' is received, it is an invalid proof, else a valid proof.
*/
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
    if(result == -1)
    	cout << "Invalid proof" << endl;
    else if(result == 0)
    	cout << "Valid proof" << endl;
	return 0;
}
