#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <string>
#include <boost/atomic.hpp>
#include "atomiclock.h"

class FileWriter
{
private:
	FILE* file;
	boost::atomic_flag filelock = BOOST_ATOMIC_FLAG_INIT;

public:
	FileWriter(std::string filename)
	{
		fopen_s(&file, filename.c_str(), "w");
	}

	~FileWriter()
	{
		if (file)
		{
			fflush(file);
			fclose(file);
		}
	}

	bool is_open()
	{
		return file;
	}

	template<typename T>
	FileWriter& operator<<(T& input)
	{
		std::stringstream sstream;
		sstream << input;
		innerLock lock(filelock);
		fprintf(file, "%s", sstream.str().c_str());
		return *this;
	}

	void LockFreeWrite(std::string line)
	{
		fprintf(file, "%s", (line + "\n").c_str());
	}

	void LockFreeWrite(char* line)
	{
		fprintf(file, "%s", line + '\n');
	}
};

#endif