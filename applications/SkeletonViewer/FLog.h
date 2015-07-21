#pragma once
#include <fstream>

class FLog
{
	std::ofstream out;
	std::string filename;
public:
	FLog(void);
	FLog(const std::string& filename);
	~FLog(void);
	std::ofstream& stream()
	{
		return out;
	}

};

static FLog flog;
static FLog flog1("log1.log");
static FLog flog2("log2.log");
static FLog flog3("log3.log");

#define FLOG  flog.stream()
#define FLOG1  flog1.stream()
#define FLOG2  flog2.stream()
#define FLOG3  flog3.stream()

