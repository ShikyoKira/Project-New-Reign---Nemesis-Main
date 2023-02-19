#pragma once

#include "base/file.h"

namespace nemesis
{
	struct MemoryFile
    {
    private:
        VecNstr contents;
        std::filesystem::path filepath;
        SPtr<nemesis::SharableWrapper<std::filesystem::path>> filepath_ptr;

    public:
        MemoryFile(const std::filesystem::path& filepath);

        void Read();
        VecNstr GetLines() const noexcept;
        
        void Clear();

        void Write(const VecStr& lines);
        void Write(const VecNstr& lines);
        void Write(const std::string& line);
        void Write(const std::string_view& vline);
        void Write(const nemesis::Line& line);

        void Append(const VecStr& lines);
        void Append(const VecNstr& lines);
        void Append(const std::string& line);
        void Append(const std::string_view& vline);
        void Append(const nemesis::Line& line);

        //void InsertAt(size_t pos, const VecStr& lines);
        //void InsertAt(size_t pos, const VecNstr& lines);
        //void InsertAt(size_t pos, const std::string& line);
        //void InsertAt(size_t pos, const std::string_view& line);
        //void InsertAt(size_t pos, const nemesis::Line& line);

        void Save() const;
    };
}
