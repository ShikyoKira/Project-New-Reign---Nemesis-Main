#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

#include "utilities/atomiclock.h"

class FileWriter
{
private:
	FILE* file;
	boost::atomic_flag filelock = BOOST_ATOMIC_FLAG_INIT;

public:
	FileWriter(const char* filename)
	{
		fopen_s(&file, filename, "w");
	}

	FileWriter(std::string filename)
	{
		fopen_s(&file, filename.c_str(), "w");
	}

	FileWriter(std::filesystem::path filename)
	{
		fopen_s(&file, filename.string().c_str(), "w");
	}

	~FileWriter()
	{
		if (file)
		{
			fflush(file);
			fclose(file);
		}
	}

    bool is_open() { return file; }

    template<typename T>
    FileWriter &operator<<(const T &input)
    {
        std::stringstream sstream;
		sstream << input;
		Lockless lock(filelock);
		fprintf(file, "%s", sstream.str().c_str());
		return *this;
    }

    template<typename T>
    FileWriter &operator<<(const T *input)
    {
		std::stringstream sstream;
        sstream << input;
        Lockless lock(filelock);
        fprintf(file, "%s", sstream.str().c_str());
        return *this;
    }

    void LockFreeWrite(std::string line) { fprintf(file, "%s", (line + "\n").c_str()); }

    void LockFreeWrite(char *line) { fprintf(file, "%s", line + '\n'); }
};

#endif
