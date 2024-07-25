#include "file.h"

#include <fstream>

// Public Methods

void File::save(const vector<int>& content)
{
    ofstream file(name.c_str(), ios::out);
	
    if(!file) return;
	
    for(int i = 0; i < content.size(); i++)
        file << content[i] << endl;
	
    file.close();
}

vector<int> File::load()
{
    ifstream file(name.c_str(), ios::in);
	
	vector<int> content;
	int number = 0;
	
    if(!file) return content;
	
	while (file >> number)
		content.push_back(number);
	
    file.close();
    return content;
}
