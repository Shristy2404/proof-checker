#include <iostream>
#include <vector>

using namespace std;

class proof_validator
{
public:
	std::vector<std::string> str;
	int k;
	void remove_brackets( string rb);
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

int main()
{
	proof_validator obj(10);
	return 0;
}