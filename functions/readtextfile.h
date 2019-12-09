#ifndef READTEXTFILE_H_
#define READTEXTFILE_H_

#include <QtCore\QFile.h>

struct FileReader
{
	QFile file;

	FileReader(const char* filename)
	{
		file.setFileName(QString::fromStdString(filename));
	}

	FileReader(boost::filesystem::path filename)
	{
		file.setFileName(QString::fromStdWString(filename.wstring()));
	}

	~FileReader()
	{
		if (GetFile())
		{
			file.close();
		}
	}

	bool GetFile()
	{
		return file.open(QIODevice::ReadOnly);
	}

	bool GetLines(std::string& line)
	{
		while (!file.atEnd())
		{
			QString qline = file.readLine();
			line = qline.toStdString();

			if (line.length() > 0)
			{
				if (line.back() == '\n' || line.back() == '\r') line.pop_back();

				if (line.length() > 0)
				{
					if (line.back() == '\r' || line.back() == '\n') line.pop_back();
				}
			}

			return true;
		}

		return false;
	}

	bool GetLines(std::wstring& line)
	{
		while (!file.atEnd())
		{
			QString qline = file.readLine();
			line = qline.toStdWString();

			if (line.length() > 0)
			{
				if (line.back() == '\n' || line.back() == '\r') line.pop_back();

				if (line.length() > 0)
				{
					if (line.back() == '\r' || line.back() == '\n') line.pop_back();
				}
			}

			return true;
		}

		return false;
	}
};

#endif
