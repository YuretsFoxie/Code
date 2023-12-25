#include "dataModel.h"
#include "file.h"

#include <windows.h>

// Public Functions

void DataModel::save()
{
	if (!data.empty())
		File(generateName()).save(data);
}

void DataModel::load(const string& name)
{
	data = File(name).load();
}

void DataModel::clear()
{
	data.clear();
}

void DataModel::push(int value)
{
	data.emplace_back(value);
}

const vector<int>& DataModel::getData() const
{
	return data;
}

// Private Functions

string DataModel::generateName()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	
	string day		= (time.wDay	< 10 ? "0" : "") + to_string(time.wDay);
	string month	= (time.wMonth	< 10 ? "0" : "") + to_string(time.wMonth);
	string hour		= (time.wHour	< 10 ? "0" : "") + to_string(time.wHour);
	string minute	= (time.wMinute	< 10 ? "0" : "") + to_string(time.wMinute);
	string second	= (time.wSecond < 10 ? "0" : "") + to_string(time.wSecond);
	
	string year = to_string(time.wYear);
	
	return "data/" + day + "." + month + "." + year + " " + hour + "." + minute + "." + second + ".txt";
}
