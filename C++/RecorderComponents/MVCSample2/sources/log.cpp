#include <windows.h>
#include "log.h"

Log& Log::shared()
{
	static Log log;
	return log;
}

void Log::save(const string& message)
{
	file << getTime() + message << '\n';
}

Log::Log(): file(name.c_str(), ios::app)
{
	file << "========== started: " + getDate() + getTime() + "==========\n";
}

Log::~Log()
{
	file << "========== ended:   " + getDate() + getTime() + "==========\n\n";
	file.close();
}

string Log::getTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	
	string hour		= (time.wHour	< 10 ? "0" : "") + to_string(time.wHour);
	string minute	= (time.wMinute	< 10 ? "0" : "") + to_string(time.wMinute);
	string second	= (time.wSecond < 10 ? "0" : "") + to_string(time.wSecond);
	
	return hour + ":" + minute + ":" + second + " ";
}

string Log::getDate()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	
	string day		= (time.wDay	< 10 ? "0" : "") + to_string(time.wDay);
	string month	= (time.wMonth	< 10 ? "0" : "") + to_string(time.wMonth);
	string year		= to_string(time.wYear);
	
	return day + "." + month + "." + year + " ";
}
