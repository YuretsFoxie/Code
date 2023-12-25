#ifndef DATAMODEL_H_INCLUDED
#define DATAMODEL_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

class DataModel
{
public:
    static DataModel& shared()
    {
        static DataModel d;
        return d;
    }
	
	void save();
	void load(const string& name);
	void clear();
	void push(int value);
	
	const vector<int>& getData() const;
	
private:
	vector<int> data;
	
	DataModel()
	{
		data.reserve(100000);
	}
	
	string generateName();
};

#endif // DATAMODEL_H_INCLUDED
