#pragma once

#include <deque>
#include <vector>
#include <filesystem>

#include "utilities/condtype.h"

namespace nemesis
{
    struct Line;

    struct CondDetails
    {
        using CondDetailsList = Vec<CondDetails>;

        CondType type;
        std::string condition;
        std::deque<nemesis::Line> contents;

        CondDetails() = default;
        CondDetails(CondType _type) noexcept;
        CondDetails(CondType _type, const std::string& _condition) noexcept;

        static bool templateCheck(const std::filesystem::path& path,
                                  const std::string& format,
                                  const nemesis::Line& line,
                                  CondDetailsList& condtype);

        static bool templateCheckRev(const std::filesystem::path& path,
                                     const std::string& format,
                                     const nemesis::Line& line,
                                     CondDetailsList& condtype);

        static bool modCheck(const std::filesystem::path& path,
                             const std::string& format,
                             const nemesis::Line& line,
                             CondDetailsList& condtype);

        static bool modCheckRev(const std::filesystem::path& path,
                                const std::string& format,
                                const nemesis::Line& line,
                                CondDetailsList& condtype);

        static bool originalScope(const std::filesystem::path& path,
                                  const std::string& format,
                                  const nemesis::Line& line,
                                  CondDetailsList& condtype);

        static bool originalScopeRev(const std::filesystem::path& path,
                                     const std::string& format,
                                     const nemesis::Line& line,
                                     CondDetailsList& condtype);

        static bool closeScope(const std::filesystem::path& path,
                               const std::string& format,
                               const nemesis::Line& line,
                               CondDetailsList& condtype);

        static bool closeScopeRev(const nemesis::Line& line, CondDetailsList& condtype);

        static nemesis::Line getOriginalLine(std::deque<Line>* edits);

        class CondDetailsProcess
        {
            const std::filesystem::path& path;
            const std::string& format;
            const nemesis::Line& line;

        public:
            CondDetailsProcess(std::filesystem::path _path,
                               const std::string& _format,
                               const nemesis::Line& _line);

            void IfValidation(CondDetailsList& condtype) const noexcept;
            void IfValidationRev(CondDetailsList& condtype) const noexcept;
            void EndIfValidationRev(CondDetailsList& condtype) const noexcept;
            void ForEachValidationRev(CondDetailsList& condtype) const noexcept;
        };
    };

    using CondCheckFunc = bool (*)(const std::filesystem::path&,
                                   const std::string&,
                                   const nemesis::Line&,
                                   std::vector<CondDetails>&);
}
