#pragma once

#include "animsetdata/linked/linkedstate.h"

namespace nemesis
{
    namespace animsetdata
    {
        struct Project
        {
        private:
            std::string name;
            Vec<nemesis::animsetdata::LinkedState> states;

        public:
            Project(const VecNstr& lines);
            Project(const Project& project) noexcept;

            void SetName(const std::string& name);
            std::string_view GetName() const;

            VecNstr GetLines() const;
            VecNstr GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const;
        };
    }
}
