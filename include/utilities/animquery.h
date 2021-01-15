#pragma once

#include "utilities/option.h"

namespace nemesis
{
    struct TemplateClass;
    struct AnimQueryList;

	struct AnimQuery : public std::enable_shared_from_this<AnimQuery>
    {
    private:
        const nemesis::AnimQueryList& querylist;
        std::shared_ptr<const nemesis::TemplateClass> animclass;
        std::vector<std::unique_ptr<nemesis::Option>> options;
        mutable std::vector<std::weak_ptr<const nemesis::AnimQuery>> group;

        nemesis::Line fullquery;
        std::string animname;
        std::string animfile;
        VecStr animobjects;

        bool OptionValidation(const std::unique_ptr<nemesis::Option>& option);
    public:
        AnimQuery(const nemesis::Line& query,
                  const nemesis::AnimQueryList& querylist,
                  std::shared_ptr<const nemesis::TemplateClass> animclass);

        void AddAnimObject(const std::string& animobject);
        void AddQueryToGroup(const std::weak_ptr<const nemesis::AnimQuery>& query) const;
        void AddOptions(const std::string& optionquery);
        void AddAnimName(const std::string& animname) noexcept;
        void AddAnimFile(const std::string& animfile) noexcept;

        std::shared_ptr<const nemesis::TemplateClass> GetAnimClass() const noexcept;
    };
}
