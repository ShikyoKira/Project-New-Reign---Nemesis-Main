#ifndef READTEXTFILE_H_
#define READTEXTFILE_H_

struct TextFile
{
	FILE* file;

	TextFile(std::string filename)
	{
		fopen_s(&file, filename.c_str(), "r");
	}

	~TextFile()
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
