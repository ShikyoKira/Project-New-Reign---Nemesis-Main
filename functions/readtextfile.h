#ifndef READTEXTFILE_H_
#define READTEXTFILE_H_

#include <QtCore\QFile.h>

struct FileReader
{
	QFile file;

	FileReader(std::string filename)
	{
		file.setFileName(QString::fromStdString(filename));
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
};

#endif
