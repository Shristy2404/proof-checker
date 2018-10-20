#include <iostream>
#include <string>
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
	string remove_brackets( string rb);
	int return_rule(string rule);
	proof_validator( int l);
	void removeSpaces();
	bool implies_elimination(int a,int b,int current);



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

string onlyFormula(string &a){
	string arr;
	int count=0,count1=0;
	for(int i=0;i<a.length();i++){
	    //cout<<a[i]<<endl;
		if(a[i]!='/'){
		    //cout<<a[i]<<endl;
		    arr.resize(++count1);
		    arr[count++]=a[i];
		    //cout<<arr<<endl;
		}
        else
            break;
	}
	return arr;
}
bool proof_validator::implies_elimination(int a,int b,int current){
	string a = onlyFormula(str[a-1]);
	string y = onlyFormula(str[b-1]);
    string x=proof_validator::remove_brackets(a);
	int z=x.find(">");
	string x1=x.substr(z+1);
	string x2=x.substr(z+1,x.length()-(z+1));
	string currentline=str[current-1];
	currentline=onlyFormula(currentline);
	if(x1.compare(y)!=0||x2.compare(currentline)!=0)
		return false;
	else
		return true;
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
		int index_2 = temp.find('/', index_1+1);
		//cout << index_1 << " " << index_2 << endl;
		string temp_rule = temp.substr(index_1+1, (index_2-index_1-1));
		//cout << temp_rule;
		int rule_index = return_rule(temp_rule);
		cout << rule_index;
	}
	for( ptr=obj.str.begin();ptr!=obj.str.end();ptr++)
    {
        string temp=*ptr;
        int x=removeSpaces(temp);
        temp.resize(x);
        obj.str.at(v++)=temp;


    }
    bool tf=obj.implies_elimination(1,2,3);

	return 0;
}

