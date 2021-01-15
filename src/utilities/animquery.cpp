#include "utilities/animquery.h"
#include "utilities/animquerylist.h"
#include "utilities/templateclass.h"
#include "utilities/stringextension.h"

bool nemesis::AnimQuery::OptionValidation(const std::unique_ptr<nemesis::Option>& option)
{
    if (!option) return false;

    if (option->GetModel().IsArray()) return true;

    for (const auto& each : options)
    {
        if (option->GetName() == each->GetName()) return false;
    }

    return true;
}

nemesis::AnimQuery::AnimQuery(const nemesis::Line& query,
                              const nemesis::AnimQueryList& querylist,
                              std::shared_ptr<const nemesis::TemplateClass> animclass)
    : querylist(querylist)
{
    this->animclass = animclass;
    this->fullquery = query;
}

void nemesis::AnimQuery::AddAnimObject(const std::string& animobject)
{
    if (animobjects.size() >= animclass->GetOptionModels().GetAnimObjectCount())
    {
        ErrorMessage(1144, querylist.GetFilePath(), fullquery.GetLineNumber());
    }

    animobjects.emplace_back(animobject);
}

void nemesis::AnimQuery::AddQueryToGroup(const std::weak_ptr<const nemesis::AnimQuery>& query) const
{
    group.emplace_back(std::move(query));
}

void nemesis::AnimQuery::AddOptions(const std::string& optionquery)
{
    if (optionquery.empty()) return;

    VecStr optionlist = StringSplit(optionquery.substr(1), ',');
    auto& model       = animclass->GetOptionModels();

    for (auto& each : optionlist)
    {
        auto optptr = model.CreateOption(each);

        if (!OptionValidation(optptr))
        {
            ErrorMessage(
                1178, animclass->GetName(), querylist.GetFilePath(), fullquery.GetLineNumber(), fullquery);
        }

        options.emplace_back(std::move(optptr));
    }
}

void nemesis::AnimQuery::AddAnimName(const std::string& animname) noexcept
{
    this->animname = animname;
}

void nemesis::AnimQuery::AddAnimFile(const std::string& animfile) noexcept
{
    this->animfile = animfile;
}

std::shared_ptr<const nemesis::TemplateClass> nemesis::AnimQuery::GetAnimClass() const noexcept
{
    return animclass;
}
