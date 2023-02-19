#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
    namespace animsetdata
    {
        struct CRC32
        {
        private:
            SPtr<nemesis::LinkedPreprocessLine> filepath;
            SPtr<nemesis::LinkedPreprocessLine> filename;
            SPtr<nemesis::LinkedPreprocessLine> fileformat;

        public:
            CRC32(const VecNstr& lines);
            CRC32(const CRC32& crc32) noexcept;

            VecNstr GetLines() const;
            VecNstr GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const;
        };
    }
}
