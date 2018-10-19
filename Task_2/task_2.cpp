#include <iostream>

using namespace std;

class proof_validator
{
public:
	string str[];
	int k;
	void remove_brackets();
	proof_validator( int l);
}; 

proof_validator:: proof_validator(int l)
{
	k=l;
	str[k];
	cout << k << endl;
}

void remove_brackets()
{
  cout << "Working";
}

int main()
{
	proof_validator obj(10);
	//cout << str.length() << endl;
	remove_brackets();
	return 0;
}