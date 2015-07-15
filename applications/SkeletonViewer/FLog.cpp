#include "FLog.h"
#include <time.h>
#include <string>
#include <sstream>

FLog::FLog(void)
{
	time_t now = time(0);
   // convert now to string form
	char* dt = ctime(&now);
	//out.open("Log.log");
	out.open("Log.log");
}


FLog::~FLog(void)
{
	out.close();
}
