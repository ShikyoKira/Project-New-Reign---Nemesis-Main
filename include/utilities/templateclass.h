#pragma once

#include <filesystem>

#include "utilities/template.h"
#include "utilities/optionmodellist.h"

namespace nemesis
{
    struct Template;
    struct HkxBehavior;
    struct AnimTemplate;

	struct TemplateClass : public std::enable_shared_from_this<TemplateClass>
    {
    private:
        const nemesis::AnimTemplate* animtemplate;
        nemesis::Line name;
        VecSPtr<const nemesis::Template> templatelist;
        nemesis::OptionModelList optionmodels;

        bool group = false;
        bool master = false;

        void AddTemplate(const std::filesystem::path& behaviorfolder);

    public:
        TemplateClass(const std::filesystem::path& optionlistfile);

        void SetAnimTemplate(const nemesis::AnimTemplate* animtemplt);
        void AddBehaviorTemplate(const std::filesystem::path& behaviorfolder);
        void TryLinkToBehavior(nemesis::HkxBehavior& behavior) const;
        void LinkToBehaviorList(const VecSPtr<nemesis::HkxBehavior>& behaviorlist) const;

        const nemesis::AnimTemplate& GetAnimTemplate() const noexcept;
        const nemesis::Line& GetName() const noexcept;
        const nemesis::OptionModelList& GetOptionModels() const noexcept;
        const VecSPtr<const nemesis::Template>& GetTemplateList() const noexcept;

        bool HasGroup() const noexcept;
        bool HasMaster() const noexcept;

        static bool HasOptionList(const std::filesystem::path& classdir);
        static SPtr<nemesis::TemplateClass> CreateTemplate(const std::filesystem::path& classdir);

    private:
        static bool IsValidTemplateFormat(const std::filesystem::path& file);
        static bool IsNameValid(const std::string& name);

        static const char* invalidnames[];
    };
}
