#pragma once

/*

THE DESIGN PHILOSOPHY OF ANIM TEMPLATE
- CONTAINS ALL TEMPLATE CLASSES
- SHOULD BE USED AS A SINGLETON BUT NOT (1 object per generation)
- MUST BE CONST

*/

#include "utilities/template.h"
#include "utilities/templateclass.h"
#include "utilities/animqueryfile.h"

namespace nemesis
{
    struct Template;
    struct TemplateClass;
    struct HkxBehavior;

	struct AnimTemplate
    {
        using AnimQueryPtr = SPtr<nemesis::AnimQuery>;

        class Parser
        {
            AnimQueryPtr* lastptr;
            const nemesis::AnimTemplate& host;
            std::filesystem::path filepath;
            SPtr<nemesis::AnimQueryFile> querylist;

            void ParseQuery(const nemesis::Line& query);
            nemesis::AnimTemplate::AnimQueryPtr
            CreateQuery(const nemesis::Line& query, const std::string& classname, const nemesis::smatch& matches);

            static std::string GetTranslation(const nemesis::smatch& matches);

        public:
            Parser(const nemesis::AnimTemplate& host, const std::filesystem::path& filepath);

            void ParseLines();

            SPtr<const nemesis::AnimQueryFile> GetResult() const noexcept;

        private:
            static const std::string animation_dir;
        };

    private:
        VecSPtr<const nemesis::TemplateClass> classlist;
        VecSPtr<const nemesis::Template> imports;

        mutable std::atomic<size_t> behaviorindex = 0;

        // group 1 = template class
        // group 2 = options
        // group 3 = animation name
        // group 4 = animation filename
        // group 5 = animation objects
        nemesis::regex queryrgx;

        void TryAddTemplate(const std::filesystem::path& classdir);
        void AddImportTemplate(const std::filesystem::path& filepath);
        void UpdateRegex();

    public:
        AnimTemplate(const std::filesystem::path& templatedir);

        const nemesis::TemplateClass* GetClass(const std::string& name) const;
        void LinkToBehaviorList(const VecSPtr<nemesis::HkxBehavior>& behaviorlist);
        SPtr<const nemesis::AnimQueryFile> ReadListFile(const std::filesystem::path& path) const;
        SPtr<const nemesis::Template> GetImport(const std::string importname) const;
        VecSPtr<const nemesis::Template> GetBehaviorTemplateList(const std::wstring behaviorname) const;
    };
}
