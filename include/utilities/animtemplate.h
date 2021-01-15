#pragma once

#include <regex>

#include "utilities/template.h"
#include "utilities/templateclass.h"
#include "utilities/animquerylist.h"

namespace nemesis
{
    struct Template;
    struct TemplateClass;

	struct AnimTemplate
    {
        using AnimQueryPtr = SPtr<nemesis::AnimQuery>;

        class Parser
        {
            AnimQueryPtr* lastptr;
            const nemesis::AnimTemplate& host;
            std::filesystem::path filepath;
            SPtr<nemesis::AnimQueryList> querylist;

            void ParseQuery(const nemesis::Line& query);
            nemesis::AnimTemplate::AnimQueryPtr
            CreateQuery(const nemesis::Line& query, const std::string& classname, const std::smatch& matches);

        public:
            Parser(const AnimTemplate& host, const std::filesystem::path& filepath);

            void ParseLines();

            SPtr<nemesis::AnimQueryList> GetResult() const noexcept;
        };

    private:
        VecSPtr<const nemesis::TemplateClass> classlist;
        VecSPtr<nemesis::Template> imports;

        // group 1 = template class
        // group 2 = options
        // group 3 = animation name
        // group 4 = animation filename
        // group 5 = animation objects
        std::regex queryrgx;

        void TryAddTemplate(const std::filesystem::path& classdir);
        void UpdateRegex();
        SPtr<const nemesis::TemplateClass> GetClass(std::string name) const;

    public:
        AnimTemplate(const std::filesystem::path& templatedir);

        SPtr<nemesis::AnimQueryList> ReadListFile(const std::filesystem::path& listfile) const;
        const SPtr<nemesis::Template>& GetImport(const std::string importname) const;
        VecSPtr<nemesis::Template> GetBehaviorTemplateList(const std::wstring behaviorname) const;
    };
}
