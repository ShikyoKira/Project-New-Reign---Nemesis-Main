#pragma once

#include "core/linkedpreprocessline.h"

namespace nemesis
{
	namespace animdata
    {
        struct SingleFileFactory;

        struct AnimProject
        {
        private:
            std::string name;

            Vec<SPtr<nemesis::LinkedPreprocessLine>> hkx_files;
			Vec<SPtr<nemesis::LinkedPreprocessLine>> animdata;
			Vec<SPtr<nemesis::LinkedPreprocessLine>> movedata;

        public:
            AnimProject() = default;
            AnimProject(const VecNstr& lines);

            void SetName(const std::string& name);
            std::string_view GetName() const;

            void AddHkxFile(const nemesis::Line& hkx_file);
            void AddAnimationData(const nemesis::Line& hkx_file);
            void AddMovementData(const nemesis::Line& hkx_file);

            VecNstr GetLines() const;
            VecNstr GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const;

            friend nemesis::animdata::SingleFileFactory;
        };
    }
}
