#pragma once
#include <fstream>

class FLog
{
	std::ofstream out;
public:
	FLog(void);
	~FLog(void);
	std::ofstream& stream()
	{
		return out;
	}

};

static FLog flog;

#define FLOG  flog.stream()

