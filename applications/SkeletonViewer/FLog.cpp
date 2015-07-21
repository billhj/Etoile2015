#include "FLog.h"
#include <time.h>
#include <string>
#include <sstream>

FLog::FLog(void)
{
	filename = "Log.log";
	time_t now = time(0);
   // convert now to string form
	char* dt = ctime(&now);
	//out.open("Log.log");
	out.open(filename);
}

FLog::FLog(const std::string& _filename):filename(_filename)
{
	time_t now = time(0);
   // convert now to string form
	char* dt = ctime(&now);
	//out.open("Log.log");
	out.open(filename);
}


FLog::~FLog(void)
{
	out.close();
}
