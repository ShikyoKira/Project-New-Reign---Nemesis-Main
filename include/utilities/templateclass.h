#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include "utilities/template.h"
#include "utilities/animquery.h"
#include "utilities/optionmodellist.h"

namespace nemesis
{
	struct TemplateClass : public std::enable_shared_from_this<TemplateClass>
    {
    private:
        nemesis::Line name;
        VecSPtr<nemesis::Template> templatelist;
        nemesis::OptionModelList optionmodels;

        void AddTemplate(const std::filesystem::path& behaviorfolder);
    public:
        TemplateClass(const std::filesystem::path& optionlistfile);

        void AddBehaviorTemplate(const std::filesystem::path& behaviorfolder);
        const nemesis::Line& GetName() const noexcept;
        const nemesis::OptionModelList& GetOptionModels() const;
        const VecSPtr<nemesis::Template>& GetTemplateList() const;

        static bool HasOptionList(const std::filesystem::path& classdir);
        static std::shared_ptr<nemesis::TemplateClass> CreateTemplate(const std::filesystem::path& classdir);

    private:
        static bool IsValidTemplateFormat(const std::filesystem::path& file);
        static bool IsNameValid(const std::string& name);

        static const char* invalidnames[];
    };
}
