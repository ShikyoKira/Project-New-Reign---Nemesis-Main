#ifndef READTEXTFILE_H_
#define READTEXTFILE_H_

#include <filesystem>

#include <QtCore\QFile.h>
#include <QtCore\QTextStream.h>

struct FileReader
{
    QFile file;
    QTextStream stream;
    size_t linenum = 0;

    FileReader(const char* filename)
    {
        file.setFileName(QString::fromStdString(filename));
    }

    FileReader(const std::filesystem::path& filename)
    {
        file.setFileName(QString::fromStdWString(filename.wstring()));
        stream.setDevice(&file);
    }

    ~FileReader()
    {
        file.close();
    }

    std::filesystem::path GetFilePath() const
    {
        return std::filesystem::path(file.fileName().toStdWString());
    }

    bool TryGetFile()
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        linenum = 0;
        stream.setDevice(&file);
        stream.setCodec("UTF-8");
        return true;
    }

    inline bool TryGetLines(std::string& line)
    {
        ++linenum;

        if (file.atEnd()) return false;

        line = file.readLine().toStdString();

        if (line.empty()) return true;

        while (!line.empty() && (line.back() == L'\n' || line.back() == L'\r'))
        {
            line.pop_back();
        }

        return true;
    }

    inline bool TryGetLines(std::wstring& line)
    {
        ++linenum;

        if (file.atEnd()) return false;

        QString qline = file.readLine();
        line          = qline.toStdWString();

        if (line.empty()) return true;

        while (!line.empty() && (line.back() == L'\n' || line.back() == L'\r'))
        {
            line.pop_back();
        }

        return true;
    }
    
    inline bool TryGetLines(QString& line)
    {
        ++linenum;

        if (file.atEnd()) return false;

        line = file.readLine();

        if (line.isEmpty()) return true;

        while (!line.isEmpty() && (line.back() == L'\n' || line.back() == L'\r'))
        {
            line.chop(1);
        }

        return true;
    }
    
    inline bool TryGetLines(nemesis::Line& line)
    {
        ++linenum;

        if (file.atEnd()) return false;

        QString qline = file.readLine();
        line          = nemesis::Line(qline.toStdString(), linenum);

        if (line.empty()) return true;

        while (!line.empty() && (line.back() == L'\n' || line.back() == L'\r'))
        {
            line.pop_back();
        }

        return true;
    }
    
    inline bool TryGetLines(nemesis::Line& line, nemesis::SharableWrapper<std::filesystem::path>* path_ptr)
    {
        ++linenum;

        if (file.atEnd()) return false;

        QString qline = file.readLine();
        line          = nemesis::Line(qline.toStdString(), linenum, path_ptr);

        if (line.empty()) return false;

        while (!line.empty() && (line.back() == L'\n' || line.back() == L'\r'))
        {
            line.pop_back();
        }

        return true;
    }

    std::string ErrorMessage() const
    {
        return file.errorString().toStdString();
    }
};

#endif
