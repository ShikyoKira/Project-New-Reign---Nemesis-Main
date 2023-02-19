#include "utilities/template.h"
#include "utilities/animquery.h"
#include "utilities/animqueryfile.h"
#include "utilities/templateclass.h"
#include "utilities/stringextension.h"

nemesis::regex nemesis::AnimQuery::animobj_rgx = "^(.+?)(?=$|\\/[0-9]+$)\\/?([0-9]*)$";

bool nemesis::AnimQuery::OptionValidation(const UPtr<nemesis::Option>& option)
{
    if (!option) return false;

    if (option->GetModel().IsArray()) return true;

    return !HasOption(option);
}

nemesis::AnimQuery::AnimQuery(const nemesis::Line& query,
                              const nemesis::AnimQueryFile& querylist,
                              const nemesis::TemplateClass& animclass)
    : querylist(querylist)
    , animclass(animclass)
{
    this->fullquery = query;
}

void nemesis::AnimQuery::AddIndex(size_t index) noexcept
{
    this->index = index;
}

void nemesis::AnimQuery::AddBehaviorIndex(size_t index) noexcept
{
    behaviorindex = index;
}

void nemesis::AnimQuery::AddAnimObjects(const std::string& animobject)
{
    VecStr list = StringSplit(animobject);

    for (auto each : list)
    {
        nemesis::smatch match;
        nemesis::regex_match(each, match, animobj_rgx);

        if (match.empty()) continue;

        auto numstr = match.str(2);
        int num     = numstr.empty() ? 1 : stoi(numstr);

        animobjects[num].emplace_back(each);

        if (animobjects.size() <= animclass.GetOptionModels().GetAnimObjectCount()) continue;

        ErrorMessage(1144, querylist.GetFilePath(), fullquery.GetLineNumber());
    }
}

void nemesis::AnimQuery::AddQueryToGroup(nemesis::AnimQuery& query)
{
    query.index = array->back()->index;
    sharedarray.emplace_back(query.shared_from_this());
    query.arrayindex = array->size();
    array->emplace_back(&query);
    query.array = array;
}

void nemesis::AnimQuery::AddOptions(const std::string& optionquery)
{
    if (optionquery.empty()) return;

    VecStr optionlist = StringSplit(optionquery.substr(1), ',');

    for (auto& each : optionlist)
    {
        AddOption(each);
    }
}

void nemesis::AnimQuery::AddOption(const std::string& option)
{
    auto optptr = animclass.GetOptionModels().CreateOption(option, *this);

    if (!OptionValidation(optptr))
    {
        auto& name = animclass.GetName();
        auto filename = querylist.GetFilePath();
        ErrorMessage(1178, name, filename, fullquery.GetLineNumber(), option);
    }

    if (!optptr->IsSuccess())
    {
        auto filename = querylist.GetFilePath();
        WarningMessage(1026, filename, fullquery.GetLineNumber(), option);
        return;
    }

    if (option == "k" || option == "bsa")
    {
        known = true;
    }

    optionsmap[optptr->GetName()].emplace_back(optptr.get());
    options.emplace_back(std::move(optptr));
}

void nemesis::AnimQuery::AddAnimName(const std::string& animname) noexcept
{
    this->animname = animname;
}

void nemesis::AnimQuery::AddAnimPath(const std::filesystem::path& animpath) noexcept
{
    this->animpath = animpath;
}

void nemesis::AnimQuery::AddMotion(const std::string& translation)
{
    VecStr list = StringSplit(translation);

    if (list.size() != 4) ErrorMessage(1089, querylist.GetFilePath(), fullquery.GetLineNumber());

    for (auto each : list)
    {
        if (!isOnlyNumber(each)) ErrorMessage(1091, querylist.GetFilePath(), fullquery.GetLineNumber());
    }

    const std::string motion = "motion";

    if (!HasOption(motion)) AddOption(motion);
}

void nemesis::AnimQuery::AddRotation(const std::string& translation)
{
    VecStr list = StringSplit(translation);

    if (list.size() != 5) ErrorMessage(1088, querylist.GetFilePath(), fullquery.GetLineNumber());

    for (auto each : list)
    {
        if (!isOnlyNumber(each)) ErrorMessage(1088, querylist.GetFilePath(), fullquery.GetLineNumber());
    }

    const std::string rotation = "rotation";

    if (!HasOption(rotation)) AddOption(rotation);
}

void nemesis::AnimQuery::AddSelfToArray() noexcept
{
    nemesis::AnimQuery* self = this;
    array->emplace_back(self);
}

bool nemesis::AnimQuery::HasOption(const UPtr<nemesis::Option>& option) const noexcept
{
    if (optionsmap.find(option->GetName()) == optionsmap.end()) return false;

    for (const auto& each : optionsmap.at(option->GetName()))
    {
        if (option->GetName() == each->GetName()) return true;
    }

    return false;
}

bool nemesis::AnimQuery::HasOption(const std::string& optionname) const noexcept
{
    return optionsmap.find(optionname) != optionsmap.end();
}

const Vec<const nemesis::Option*>* nemesis::AnimQuery::GetOptionListPtr(const std::string& optionname) const
{
    auto itr = optionsmap.find(optionname);

    if (itr == optionsmap.end()) return nullptr;

    return &itr->second;
}

const nemesis::Option* nemesis::AnimQuery::GetOptionPtr(const std::string& optionname, size_t offset) const
{
    auto listptr = GetOptionListPtr(optionname);

    if (!listptr) return nullptr;

    for (const auto& each : *listptr)
    {
        if (offset == 0) return each;

        offset--;
    }

    return nullptr;
}

const nemesis::Template*
nemesis::AnimQuery::MatchTemplate(USet<const nemesis::Template*>& templateset) const noexcept
{
    for (auto& each : animclass.GetTemplateList())
    {
        auto* rawptr = each.get();

        if (templateset.find(rawptr) != templateset.end()) return rawptr;
    }

    return nullptr;
}

const VecStr& nemesis::AnimQuery::GetMotion() const
{
    return motiondata;
}

const VecStr& nemesis::AnimQuery::GetRotation() const
{
    return rotationdata;
}

const std::filesystem::path& nemesis::AnimQuery::GetAnimPath() const
{
    return animpath;
}

std::string_view nemesis::AnimQuery::GetAnimationName() const
{
    return animname;
}

const VecStr& nemesis::AnimQuery::GetAnimObject(size_t num) const
{
    auto itr = animobjects.find(num);

    if (itr == animobjects.end()) ErrorMessage(1222, querylist.GetFilePath(), fullquery.GetLineNumber());

    return itr->second;
}

const std::string& nemesis::AnimQuery::GetAnimObject(size_t num, size_t index) const
{
    auto& list = GetAnimObject(num);

    if (index >= list.size()) ErrorMessage(1222, querylist.GetFilePath(), fullquery.GetLineNumber());

    return list.at(index);
}

size_t nemesis::AnimQuery::GetArrayNum() const noexcept
{
    for (size_t i = 0; i < array->size(); ++i)
    {
        auto& each = array->at(i);

        if (this == each) return i;
    }

    return 0;
}

const nemesis::AnimQuery& nemesis::AnimQuery::GetNextInArray() const noexcept
{
    return IsLast() ? *this : *array->at(GetArrayNum() + 1);
}

const nemesis::AnimQuery& nemesis::AnimQuery::GetBackInArray() const noexcept
{
    return IsFirst() ? *this : *array->at(GetArrayNum() - 1);
}

const nemesis::AnimQuery& nemesis::AnimQuery::GetLastInArray() const noexcept
{
    return *array->back();
}

const nemesis::AnimQuery& nemesis::AnimQuery::GetArrayQuery(size_t index) const
{
    return *array->at(index);
}

const Vec<const nemesis::AnimQuery*>& nemesis::AnimQuery::GetArray() const
{
    return *array;
}

size_t nemesis::AnimQuery::GetArraySize() const noexcept
{
    return array->size();
}

size_t nemesis::AnimQuery::GetIndex() const noexcept
{
    return index;
}

size_t nemesis::AnimQuery::GetArrayIndex() const noexcept
{
    return arrayindex;
}

size_t nemesis::AnimQuery::GetBehaviorIndex() const noexcept
{
    return behaviorindex;
}

const nemesis::TemplateClass& nemesis::AnimQuery::GetAnimClass() const noexcept
{
    return animclass;
}

const nemesis::AnimQueryFile& nemesis::AnimQuery::GetQueryList() const noexcept
{
    return querylist;
}

bool nemesis::AnimQuery::IsKnown() const noexcept
{
    return known;
}

bool nemesis::AnimQuery::IsLast() const noexcept
{
    if (array->empty()) return false;

    return this == array->back();
}

bool nemesis::AnimQuery::IsFirst() const noexcept
{
    if (array->empty()) return false;

    return this == array->front();
}
