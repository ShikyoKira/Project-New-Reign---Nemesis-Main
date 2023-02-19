#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
    namespace animsetdata
    {
        struct Anim
        {
        private:
            SPtr<nemesis::LinkedPreprocessLine> name;
            SPtr<nemesis::LinkedPreprocessLine> unknown;
            Vec<nemesis::LinkedPreprocessLine> attacklist;

        public:
            Anim(const VecNstr& lines);
            Anim(const Anim& anim) noexcept;

            VecNstr GetLines() const;
            VecNstr GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const;
        };
    }
}
