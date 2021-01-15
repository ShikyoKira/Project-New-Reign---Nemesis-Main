#ifndef READTEXTFILE_H_
#define READTEXTFILE_H_

#include <filesystem>

#include <QtCore\QFile.h>
#include <QtCore\QTextStream.h>

struct FileReader
{
    QFile file;
    QTextStream stream;

    FileReader(const char* filename)
    {
        file.setFileName(QString::fromStdString(filename));
    }

    FileReader(const std::filesystem::path& filename)
    {
        file.setFileName(QString::fromStdWString(filename.wstring()));
        stream.setDevice(&file);
    }

    std::filesystem::path GetFilePath() const
    {
        return std::filesystem::path(file.fileName().toStdWString());
    }

    bool GetFile()
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        stream.setDevice(&file);
        stream.setCodec("UTF-8");
        return true;
    }

    inline bool GetLines(std::string& line)
    {
        if (file.atEnd()) return false;

        line = file.readLine().toStdString();

        while (line.length() > 0 && (line.back() == '\n' || line.back() == '\r'))
        {
            line.pop_back();
        }

        return true;
    }

    inline bool GetLines(std::wstring& line)
    {
        if (file.atEnd()) return false;

        QString qline = file.readLine();
        line          = qline.toStdWString();

        while (line.length() > 0 && (line.back() == L'\n' || line.back() == L'\r'))
        {
            line.pop_back();
        }

        return true;
    }
};

#endif
