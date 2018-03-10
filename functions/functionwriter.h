#ifndef FUNCTIONWRITER_H_
#define FUNCTIONWRITER_H_

#include <mutex>
#include <fstream>
#include <boost\filesystem.hpp>

class FunctionWriter
{
private:
	std::ofstream* file;
	std::mutex mtx;

public:
	FunctionWriter(std::ofstream* file) : file(file){}

	template<typename T>
	FunctionWriter &operator<<(const T& input)
	{
		std::lock_guard<std::mutex> filelocker(mtx);
		(*file) << input;
		return *this;
	}
};

#endif