#pragma once

#include "base/file.h"

#include "utilities/regexsearch.h"
#include "utilities/lexersearch.h"

namespace nemesis
{
    struct ProcessParser;
    struct LexerSearch;
    struct RegexSearch;

    struct ImporterBase
    {
    protected:
        UPtr<nemesis::regex> stateid_rgx = nullptr;
        UPtr<nemesis::regex> animobj_rgx = nullptr;
        Vec<nemesis::RegexSearch> animidrgx_list;
        Vec<nemesis::RegexSearch> groupidrgx_list;
        Vec<nemesis::RegexSearch> masteridrgx_list;

        Vec<UPtr<nemesis::LexerSearch>> lexersearch_list;

        Vec<Pair<std::string_view, nemesis::AnimVarPtr::Lexer>> varlexer_list;

        bool bCountElement = false;
        bool bEnding       = false;
        int iElementRange  = false;

        VecNstr filelines;
        nemesis::File& file;

    public:
        ImporterBase(nemesis::File& file)
            : file(file)
        {
        }

        virtual ~ImporterBase() {}

        nemesis::File& GetFile()
        {
            return file;
        }

        friend ProcessParser;
    };
}
