#pragma once

#include <filesystem>
#include <string>

#include "base/sharablewrapper.h"

#include "utilities/readtextfile.h"

namespace nemesis
{
    struct File
    {
        enum class FileType
        {
            NONE,
            SINGLE,
            GROUP,
            MASTER,
            BASE,
            IMPORT,
            FNIS_LIST,
            NEMESIS_LIST,
            NEMESIS_TEMPLATE,
        };

    protected:
        std::string classname;
        std::filesystem::path filepath;
        FileType type;

    private:
        static const VecStr condend;
        static const VecWstr wcondend;
        static const VecNstr n_condend;
        static const VecNwstr wn_condend;

    protected:
        template <typename LineType>
        SPtr<FileReader> CreateReader(const std::filesystem::path& filename,
                                      Vec<LineType>& functionlines) const
        {
            if (sf::is_directory(filename)) ErrorMessage(3001, filename);

            SPtr<FileReader> filereader = std::make_shared<FileReader>(filename.wstring());

            if (!filereader->TryGetFile()) ErrorMessage(3002, filename);

            functionlines = Vec<LineType>();
            functionlines.reserve(100000);
            return filereader;
        }

        template <typename LineType>
        bool ValidateEndLine(bool emptylast, Vec<LineType>& lines, const Vec<LineType>& exclusions) const
        {
            if (lines.empty()) return false;

            if (emptylast)
            {
                if (lines.back().empty()) return true;

                for (auto& each : exclusions)
                {
                    if (lines.back().find(each) == NOT_FOUND) continue;

                    return true;
                }

                LineType line;
                lines.emplace_back(line);
                return true;
            }

            if (!lines.back().empty()) return true;

            lines.pop_back();
            return true;
        }

        template <typename LineType>
        bool ValidateEndLine(bool emptylast,
                             Vec<LineType>& lines,
                             const Vec<LineType>& exclusions,
                             size_t linenum) const
        {
            if (lines.empty()) return false;

            if (emptylast)
            {
                if (lines.back().empty()) return true;

                for (auto& each : exclusions)
                {
                    if (lines.back().find(each) == NOT_FOUND) continue;

                    return true;
                }

                LineType line(linenum);
                lines.emplace_back(line);
            }

            if (!lines.back().empty()) return true;

            lines.pop_back();
            return true;
        }

        template <typename ContainerType, typename BaseType>
        bool TryGetLines(Vec<ContainerType>& lines,
                         std::function<bool(BaseType&)> predicament,
                         bool emptylast,
                         const Vec<ContainerType>& condition_end) const
        {
            return TryGetLines<ContainerType, BaseType>(
                lines,
                predicament,
                [](const BaseType& line) -> ContainerType { return line; },
                emptylast,
                condition_end);
        }

        template <typename ContainerType, typename BaseType>
        bool TryGetLines(Vec<ContainerType>& lines,
                         std::function<bool(BaseType&)> predicament,
                         std::function<ContainerType(const BaseType&)> selector,
                         bool emptylast,
                         const Vec<ContainerType>& condition_end) const
        {
            SPtr<FileReader> BehaviorFormat = CreateReader(filepath, lines);
            BaseType line;

            while (BehaviorFormat->TryGetLines(line))
            {
                if (error) throw nemesis::exception();

                if (!predicament(line)) continue;

                lines.emplace_back(selector(line));
            }

            auto rtn = ValidateEndLine(emptylast, lines, condition_end);
            lines.shrink_to_fit();
            return rtn;
        }

    public:
        File() = default;
        File(const std::filesystem::path& filepath);

        FileType GetType() const noexcept;

        std::wstring GetLastModified() const;

        virtual void SetFilePath(const std::filesystem::path& path);
        virtual std::string_view GetFileClassName() const;
        virtual std::filesystem::path GetFilePath() const;

        VecNstr GetLines(bool emptylast = false) const;
        VecNstr GetLines(std::function<const std::string(const std::string&)> selector,
                         bool emptylast = false) const;
        VecNstr GetLines(std::function<bool(std::string&)> predicament, bool emptylast = false) const;

        bool TryGetLines(VecNstr& lines, bool emptylast = false) const;
        bool TryGetLines(VecNstr& lines,
                         std::function<const std::string(const std::string&)> selector,
                         bool emptylast = false) const;
        bool TryGetLines(VecNstr& lines,
                         std::function<bool(std::string&)> predicament,
                         bool emptylast = false) const;

        bool TryGetLines(VecNwstr& lines, bool emptylast = false) const;
        bool TryGetLines(VecNwstr& lines,
                         std::function<const std::wstring(const std::wstring&)> selector,
                         bool emptylast = false) const;
        bool TryGetLines(VecNwstr& lines,
                         std::function<bool(std::wstring&)> predicament,
                         bool emptylast = false) const;

        bool TryGetLines(VecStr& lines, bool emptylast = false) const;
        bool TryGetLines(VecStr& lines,
                         std::function<const std::string(const std::string&)> selector,
                         bool emptylast = false) const;
        bool TryGetLines(VecStr& lines,
                         std::function<bool(std::string&)> predicament,
                         bool emptylast = false) const;

        bool TryGetLines(VecWstr& lines, bool emptylast = false) const;
        bool TryGetLines(VecWstr& lines,
                         std::function<const std::wstring(const std::wstring&)> selector,
                         bool emptylast = false) const;
        bool TryGetLines(VecWstr& lines,
                         std::function<bool(std::wstring&)> predicament,
                         bool emptylast = false) const;

        static nemesis::File::FileType GetFileType(const std::filesystem::path& path);
    };
}
