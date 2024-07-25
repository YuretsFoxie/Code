#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

class File
{
public:
    File(string name): name(name) {}
	
    void save(const vector<int>& content);
    vector<int> load();

private:
    string name;
};

#endif // FILE_H_INCLUDED
