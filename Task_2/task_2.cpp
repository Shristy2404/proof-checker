#include <iostream>
#include <string>
#include <vector>

using namespace std;

class proof_validator
{
public:
	std::vector<std::string> str;
	int k;
	void remove_brackets( string rb);
	proof_validator( int l);
	void removeSpaces();
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

void removeSpaces(){
	for(int i=0;i<k;i++){
		int p=0;
		for(int j=0;j<str[i].length();j++){
			if(str[i][j]!=' '){
				str[i][p++]=str[i][j];
			}
		}
		str[i].resize(p);

	}
}

int main()
{
	proof_validator obj(10);
	return 0;
}
