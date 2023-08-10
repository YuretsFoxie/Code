#include <fstream>
#include "file.h"

// Public Methods

void File::save(const vector<string>& content)
{
    ofstream file(name.c_str(), ios::out);

    if(!file) return;

    for(unsigned int i = 0; i < content.size(); i++)
        file << content[i] << endl;

    file.close();
}

vector<string> File::load()
{
    ifstream file(name.c_str(), ios::in);
    vector<string> content;

    if(!file) return content;
    for(string str; getline(file, str); content.push_back(str));

    file.close();
    return content;
}
