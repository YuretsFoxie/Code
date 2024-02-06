#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void split(const string& s)
{
	istringstream stream(s); 
	string word;
	
	while (stream >> word)
		cout << word << "\n";
}

int main()
{
	string s = "test the test \nperetest  ds\rddf";
	cout << s << endl << endl;
	
	split(s);
	
	cin.get();
	return 0;
}
