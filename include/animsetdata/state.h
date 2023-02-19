#pragma once

#include "core/linkedpreprocessline.h"

#include "animsetdata/anim.h"
#include "animsetdata/type.h"
#include "animsetdata/animsetdata-crc32.h"

#include "animsetdata/linked/linkeddata.h"

namespace nemesis
{
    namespace animsetdata
    {
        struct State
        {
        private:
            std::string name;

            Vec<nemesis::LinkedPreprocessLine> equiplist;
            Vec<nemesis::animsetdata::LinkedData<nemesis::animsetdata::Type>> typelist;
            Vec < nemesis::animsetdata::LinkedData<nemesis::animsetdata::Anim>> animlist;
            Vec<nemesis::animsetdata::LinkedData<nemesis::animsetdata::CRC32>> crc32list;

        public:
            State(const VecNstr& lines);
            State(const State& state) noexcept;

            void SetName(const std::string& name);
            std::string_view GetName() const;

            VecNstr GetLines() const;
            VecNstr GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const;
        };
    }
}
