/*!
 * \author Vishwangi Shah, Sravanthi, Shristy Kapoor
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
#include <time.h>
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
    //constructor
	proof_validator(int l);

	//utility functions
	string remove_brackets(string rb);
	int return_rule_index(string rule);
	int remove_spaces();
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
	k = l;
}

/*!
 * \brief Function to remove first and last bracket from the string passed
 * \param string rb string from which you need to remove the first and last bracket
 * \details It checks if the first and last characters of the string are '(' and ')' respectively. If yes, it returns
 * the substring between these two positions after resizing, else, returns the same string which was received as input.
 */
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
	string rule_left; /*!< Operator in the propositional formula */
	int index=0; /*!< Index where '(' equals ')' */
	int index_1 = line.find('/'); /*!< First occurence of '/' */
	int index_2 = line.find('/', index_1+1); /*!< Second occurence of '/' */
	string rule_right = line.substr(index_1+1,1); /*!< Operator used in the rule */
	string rule_statement = remove_brackets(only_statement(line)); /*!< Propositional formula in the line */
	int opening_brackets=0; /*!< Number of '(' */
	int closing_brackets =0; /*!< Number of ')' */
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
	vector<string>::iterator ptr; /*!< Defining an iterator object for the vector */
	int this_line=0; /*!< Index of current line */

	for( ptr = str.begin(); ptr!= str.end(); ptr++)
	{
	    ++this_line;
		string temp = *ptr; /*!< Current line */
		int index_1 = temp.find('/'); /*!< Index of first '/' */
		int index_2 = temp.find('/', index_1+1); /*!< Index of second '/' */
		string temp_rule = temp.substr(index_1+1, (index_2-index_1-1)); /*!< Rule used in the line */
		int rule_index = return_rule_index(temp_rule); /*!< Index assigned to that rule */
		string prem = temp.substr(index_1+1,1); /*!< First char after first '/' */
		int rule_operator_pos = return_rule_operator_index(temp); /*!< Index of the operator in the line */

		if(check_premise(prem))
		{
			continue;
		}

		if(rule_index == 1 || rule_index == 4 || rule_index == 5)
		{
			if(rule_operator_pos==-1)
			{
			return -1;
			}
		}

		if(rule_index == 1)
		{
			int line_1=0; /*!< First line number used in the rule */
			int line_2=0; /*!< Second line number used in the rule */
			int index_3 = temp.find('/', index_2+1); /*!< Third occurence of '/' */
			stringstream var1(temp.substr(index_2+1, index_3-index_2)); /*!< First line number used in the rule in string format */
			var1 >> line_1;
			stringstream var2(temp.substr(index_3+1)); /*!< Second line number used in the rule in string formatm*/
			var2 >> line_2;
			if(!(and_introduction(line_1-1, line_2-1, temp)))
			{
				return -1;
			}
		}
		if(rule_index == 2 || rule_index == 3)
		{
			int line_1 = 0; //!< The index of the line on which and-elimination is performed.
			stringstream var1(temp.substr(index_2+1));
			var1 >> line_1;
			if(!(and_elimination(line_1-1, temp, rule_index)))
			{1;
				return -1;
			}
		}

		if(rule_index == 4 || rule_index == 5)
		{
			int line_1=0; //!< The index of the line on which or-introduction is performed.
			stringstream var1(temp.substr(index_2+1));
			var1 >> line_1;
			if(!(or_introduction(line_1-1, temp, rule_index)))
			{
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
           
                return -1;
            }
		}
    }
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
    string statement_1 = remove_brackets(only_statement(line)); /*!< Line in which and is introduced */
	string comp_1 = only_statement(str[line_1]) + "^" + only_statement(str[line_2]); /*!< Introducing and on line 1 and line 2 */
	if((!(statement_1.compare(comp_1))))
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
    string implies_statement = remove_brackets(only_statement(str[a])); /*!< Implies statement */
    int rule_operator_pos=return_rule_operator_index(str[a]); /*!< Index of '>' in the implies statement. */
	string statement_1 = only_statement(str[b]); /*!< Propositional formula in the line at parameter 2 */
	string before_implies = implies_statement.substr(0,rule_operator_pos); /*!< String before '>' in the implies statement */
	string after_implies = implies_statement.substr(rule_operator_pos+1);  /*!< String after '>' in the implies statement */
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
	int rule_operator_pos=return_rule_operator_index(str[a]); /*!< Index of '>' in the implies statement */
	string statement_1 = only_statement(str[b]); /*!< Propositional formula in the line at parameter 2 */
    string implies_statement = remove_brackets(only_statement(str[a])); /*!< Implies statement */
	string before_implies=implies_statement.substr(0,rule_operator_pos); /*!< String before '>' in implies statement */
	string after_implies=implies_statement.substr(rule_operator_pos+1); /*!< String after '>' in implies statement */
	string currentline=str[current]; /*!< Current line */
	currentline=only_statement(currentline);
	string negation("~"); /*!< character ~ */
	before_implies.insert(0,negation);
	after_implies.insert(0,negation);
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
	clock_t start,end;
	int lines; /*!< Number of lines in the proof */
	int v=0; /*!< Current index */
	cin >> lines;
	cin.ignore();
	proof_validator obj(lines); /*!< Object of class proof_validator */
	for( int i=0; i<obj.k; i++)
	{
		string temp; /*!< Storing the inputted string */
		getline(cin, temp);
		obj.str.push_back(temp);
	}
	start = clock();

	vector<string>::iterator ptr; /*!< Iterator object for the vector */

	for(ptr=obj.str.begin();ptr!=obj.str.end();ptr++)
    {
        string temp=*ptr; /*!< String pointed at by the iterator object */
        int x=remove_spaces(temp); /*!< Number of elements in line with no space */
        temp.resize(x);
        obj.str.at(v++)=temp;
    }
    int result = obj.check_rules(); /*!< Result after checking rules */
    end = clock();
    if(result == -1)
    	cout << "Invalid proof" << endl;
    else if(result == 0)
    	cout << "Valid proof" << endl;

    cout << "Time required for execution: "
	<< (double)(end-start)/CLOCKS_PER_SEC
	<< " seconds." << "\n\n";

	return 0;
}
