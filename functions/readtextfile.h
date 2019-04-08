#ifndef READTEXTFILE_H_
#define READTEXTFILE_H_

struct FileReader
{
	FILE* file;

	FileReader(std::string filename)
	{
		fopen_s(&file, filename.c_str(), "r");
	}

	~FileReader()
	{
		if (file)
		{
			fclose(file);
		}
	}

	FILE* GetFile()
	{
		return file;
	}
};

#endif
