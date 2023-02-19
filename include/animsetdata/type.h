#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
    namespace animsetdata
    {
        struct Type
        {
        private:
            SPtr<nemesis::LinkedPreprocessLine> name;
            SPtr<nemesis::LinkedPreprocessLine> equiptype1;
            SPtr<nemesis::LinkedPreprocessLine> equiptype2;

        public:
            Type(const VecNstr& lines);
            Type(const Type& type) noexcept;

            VecNstr GetLines() const;
            VecNstr GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const;
        };
    }
}
