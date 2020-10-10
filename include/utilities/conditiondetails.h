#pragma once

#include <deque>
#include <vector>
#include <filesystem>

#include "utilities/line.h"

#define NEM_FOREACH "<!-- FOREACH ^"
#define NEM_IF "<!-- IF ^"
#define NEM_ELSEIF "<!-- ELSEIF ^"
#define NEM_ENDIF "<!-- ENDIF -->"
#define NEM_CLOSE "<!-- CLOSE -->"
#define NEM_MODCODE "<!-- MOD_CODE ~"
#define NEM_ORIGINAL "<!-- ORIGINAL -->"
#define NEM_ENDFUNC "^ -->"

namespace nemesis
{
    enum CondType
    {
        NONE,
        FOREACH,
        NEW,
        NEW_ORDER,
        CONDITION_START,
        CONDITION,
        CONDITION_DEFAULT,
        MOD_CODE,
        ORIGINAL,
        CLOSE,
        CONDITION_END,
    };

    struct CondDetails
    {
        nemesis::CondType type;
        std::string condition;
        std::deque<nemesis::Line> contents;

        CondDetails() {}
        CondDetails(nemesis::CondType _type);
        CondDetails(nemesis::CondType _type, std::string _condition);

        static bool templateCheck(std::filesystem::path path,
                                  const std::string& format,
                                  const nemesis::Line& line,
                                  std::vector<nemesis::CondDetails>& condtype);

        static bool templateCheckRev(std::filesystem::path path,
                                     const std::string& format,
                                     const nemesis::Line& line,
                                     std::vector<nemesis::CondDetails>& condtype);

        static bool modCheck(std::filesystem::path path,
                             const std::string& format,
                             const nemesis::Line& line,
                             std::vector<nemesis::CondDetails>& condtype);

        static bool modCheckRev(std::filesystem::path path,
                                const std::string& format,
                                const nemesis::Line& line,
                                std::vector<nemesis::CondDetails>& condtype);

        static bool originalScope(std::filesystem::path path,
                                  const std::string& format,
                                  const nemesis::Line& line,
                                  std::vector<nemesis::CondDetails>& condtype);

        static bool originalScopeRev(std::filesystem::path path,
                                     const std::string& format,
                                     const nemesis::Line& line,
                                     std::vector<nemesis::CondDetails>& condtype);

        static bool closeScope(std::filesystem::path path,
                               const std::string& format,
                               const nemesis::Line& line,
                               std::vector<nemesis::CondDetails>& condtype);

        static bool closeScopeRev(const nemesis::Line& line, std::vector<nemesis::CondDetails>& condtype);

        static nemesis::Line getOriginalLine(std::deque<nemesis::Line>* edits);
    };

    using CondCheckFunc = bool (*)(std::filesystem::path,
                                   const std::string&,
                                   const nemesis::Line&,
                                   std::vector<nemesis::CondDetails>&);
}
