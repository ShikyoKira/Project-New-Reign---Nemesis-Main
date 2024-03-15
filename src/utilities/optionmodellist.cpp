#include <regex>

#include "utilities/constants.h"
#include "utilities/optionmodel.h"
#include "utilities/templatecategory.h"
#include "utilities/optionmodellist.h"
#include "utilities/stringextension.h"

using pType = nemesis::OptionModelList::Parser;
using pFuncPtr = void(nemesis::OptionModelList::Parser::*)(const nemesis::Line&);

const UMap<std::string, pFuncPtr> nemesis::OptionModelList::Parser::mapfunc =
{
    {"event", &pType::AddEvent},
    {"event_group", &pType::AddRules},
    {"variable", &pType::AddVariable},
    {"variable_group", &pType::AddVariable},
    {"animobject", &pType::AddAnimObject},
    {"rules", &pType::AddRules},
    {"state", &pType::AddStartState},
    {"minimum", &pType::AddMinimum},
    {"ignore_group", &pType::FlagIgnoreGroup},
    {"compulsory", &pType::AddCompulsory},
    {"core", &pType::AddCore},
    {"link", &pType::AddOptionLink},
    {"add", &pType::AddAddition}
};

const VecStr nemesis::OptionModelList::Parser::fixedoptions =
{
    "k",
    "bsa"
};

const std::regex multiroottestregex("^S([0-9]+)?\\s(.+?)\\s(.+?)\\s*$");
const std::regex multirootregex("^S([0-9]+)?\\s([^\\s]+?)\\s([^\\s]+?)\\s*$");

nemesis::Line nemesis::OptionModelList::Parser::TryRemoveComment(const nemesis::Line& line)
{
    auto& str = line.ToString();

    if (str.length() == 0) return nemesis::Line(str, line.GetLineNumber());

    size_t pos = str.find("'");

    if (pos == NOT_FOUND) return nemesis::Line(str, line.GetLineNumber());

    return nemesis::Line(str.substr(0, pos), line.GetLineNumber());
}

std::string nemesis::OptionModelList::Parser::GetTemplateCategory() const
{
    return host.templatecategory->GetName();
}

std::filesystem::path nemesis::OptionModelList::Parser::GetFilePath() const
{
    return host.filepath;
}

void nemesis::OptionModelList::Parser::ErrorTemplate(int errcode, int linenum)
{
    ErrorMessage(errcode, GetTemplateCategory(), GetFilePath(), linenum);
}

void nemesis::OptionModelList::Parser::AddEvent(const nemesis::Line& line)
{
    constexpr std::string_view evntsyntax = "event ";
    host.events.emplace_back(line.substr(evntsyntax.length()));
}

void nemesis::OptionModelList::Parser::AddVariable(const nemesis::Line& line)
{
    constexpr std::string_view evntsyntax = "variable ";
    host.variables.emplace_back(line.substr(evntsyntax.length()));
}

void nemesis::OptionModelList::Parser::AddAnimObject(const nemesis::Line& line)
{
    VecStr animinfo;
    StringSplit(line, animinfo);

    if (animinfo.size() != 2 || !isOnlyNumber(animinfo.back()))
    {
        ErrorMessage(1175, GetTemplateCategory(), GetFilePath(), line.GetLineNumber(), line.ToString());
    }

    host.iAnimObj = std::make_unique<size_t>(stoi(animinfo.back()));

    //for (size_t i = 1; i <= *host.iAnimObj; ++i)
    //{
    //    nemesis::Line line(animobj_prefix + std::to_string(i), line.GetLineNumber());
    //    AddOptionModel(line);
    //}
}

void nemesis::OptionModelList::Parser::AddOptionLink(const nemesis::Line& line)
{
    VecStr split = StringSplit(line);
    split.erase(split.begin());

    if (split.size() < 2) return;

    split.erase(split.begin());
    cachelink.emplace_back(split);
}

void nemesis::OptionModelList::Parser::AddRules(const nemesis::Line& line)
{
    VecStr split = StringSplit(line);

    if (split.size() > 3)
    {
        ErrorTemplate(1029, line.GetLineNumber());
    }
    else if (host.rules != nullptr)
    {
        ErrorTemplate(1040, line.GetLineNumber());
    }

    cacherules = std::make_pair(split[1], split[2]);
}

void nemesis::OptionModelList::Parser::AddStartState(const nemesis::Line& line)
{
    VecStr split = StringSplit(line);

    if (split.size() != 2)
    {
        ErrorTemplate(1013, line.GetLineNumber());
    }
    else if (host.iState)
    {
        ErrorTemplate(1041, line.GetLineNumber());
    }

    std::string number = split.back();

    if (!isOnlyNumber(number))
    {
        ErrorTemplate(1013, line.GetLineNumber());
    }

    host.iState = std::make_unique<size_t>(std::stoi(number));
}

void nemesis::OptionModelList::Parser::AddMinimum(const nemesis::Line& line)
{
    VecStr split = StringSplit(line);

    if (split.size() != 2)
    {
        ErrorTemplate(1194, line.GetLineNumber());
    }
    else if (host.iMin)
    {
        ErrorTemplate(1042, line.GetLineNumber());
    }

    std::string num = split.back();

    if (!isOnlyNumber(num))
    {
        ErrorTemplate(1194, line.GetLineNumber());
    }

    host.iMin = std::make_unique<size_t>(std::stoi(num));
}

void nemesis::OptionModelList::Parser::FlagIgnoreGroup(const nemesis::Line& line)
{
    host.ignore_group = true;
}

void nemesis::OptionModelList::Parser::AddCompulsory(const nemesis::Line& line)
{
    VecStr split    = StringSplit(line);

    if (split.size() != 2)
    {
        ErrorTemplate(1199, line.GetLineNumber());
    }
    else if (!host.compulsory.empty())
    {
        ErrorTemplate(1045, line.GetLineNumber());
    }

    VecStr compulsories = StringSplit(split.back(), ',');

    for (auto& compulsory : compulsories)
    {
        cachecompulsory.emplace_back(nemesis::Line(compulsory, line.GetLineNumber()));
    }
}

void nemesis::OptionModelList::Parser::AddCore(const nemesis::Line& line)
{
    const std::string coresyntax = "core ";

    if (nemesis::to_lower_copy(line).find(coresyntax) == NOT_FOUND)
    {
        ErrorTemplate(1176, line.GetLineNumber());
    }

    host.corebehavior = std::make_unique<std::string>(line.substr(coresyntax.length()));
}

void nemesis::OptionModelList::Parser::AddAddition(const nemesis::Line& line)
{
    cacheadditionline.emplace_back(line);
}

void nemesis::OptionModelList::Parser::AddOptionModel(const nemesis::Line& line)
{
    host.AddOptionModel(line);
}

void nemesis::OptionModelList::Parser::Commit()
{
    CommitLinkedOption();
    CommitVariableAddition();
    CommitCompulsoryVariable();
}

void nemesis::OptionModelList::Parser::CommitLinkedOption()
{
    for (auto& eachlist : cachelink)
    {
        Vec<nemesis::OptionModel*> modellist;

        for (auto& each : eachlist)
        {
            auto ptr = host.GetModel(each);

            if (ptr == nullptr) continue;

            modellist.push_back(ptr);
        }

        for (auto& model : modellist)
        {
            for (auto& mod : modellist)
            {
                if (mod->GetFullName() == model->GetFullName()) continue;

                model->AddLink(mod);
            }
        }
    }
}

void nemesis::OptionModelList::Parser::CommitVariableAddition()
{
    for (auto& line : cacheadditionline)
    {
        VecStr split = StringSplit(line);

        if (split.size() != 4) ErrorTemplate(1200, line.GetLineNumber());

        std::string key = "<" + split[2] + ">";

        if (split.back().find(key) == NOT_FOUND)
        {
            ErrorMessage(1006, GetTemplateCategory(), GetFilePath(), line.GetLineNumber(), line);
        }

        if (split.back() == key) ErrorTemplate(1005, line.GetLineNumber());

        auto model = host.GetModel(split[1]);

        if (model == nullptr) continue;

        model->AddVarBlock(split);
    }
}

void nemesis::OptionModelList::Parser::CommitCompulsoryVariable()
{
    for (auto& option : cachecompulsory)
    {
        if (host.Contains(option))
        {
            host.compulsory.insert(option.ToString());
            continue;
        }

        auto& templtclass = host.templatecategory;
        ErrorMessage(1221, templtclass->GetName(), host.filepath, option.GetLineNumber());
    }
}

void nemesis::OptionModelList::Parser::AddFixedOptions()
{
    for (auto& opt : fixedoptions)
    {
        ParseLine(opt);
    }
}

void nemesis::OptionModelList::Parser::ParseLine(const nemesis::Line& line)
{
    auto str = TryRemoveComment(line);

    if (str.length() == 0 && nemesis::to_lower_copy(str) == nemesis::to_upper_copy(str)) return;

    VecStr split = StringSplit(str);
    auto itr     = mapfunc.find(nemesis::to_lower_copy(split.front()));

    if (itr != mapfunc.end())
    {
        (this->*itr->second)(line);
        return;
    }

    int rtn = TryAddMultiRoot(line);

    if (rtn == 0) return;

    if (rtn > 0) ErrorTemplate(rtn, line.GetLineNumber());

    AddOptionModel(str);
}

int nemesis::OptionModelList::Parser::TryAddMultiRoot(const nemesis::Line& line)
{
    std::smatch match;

    if (!std::regex_match(line.ToString(), match, multiroottestregex)) return -1;

    if (!std::regex_match(line.ToString(), match, multirootregex)) return 1069;

    if (match.str(1) == "0") return 1076;

    if (match.str(3)[0] != '#' || !isOnlyNumber(match.str(3).substr(1))) return 1071;

    auto itr = host.multistates.find(match.str(2));

    if (itr != host.multistates.end())
    {
        if (itr->second.find(stoi(match.str(1))) != itr->second.end()) return 1070;
    }

    host.multistates[match.str(2)][stoi(match.str(1))] = match.str(3);
    return 0;
}

nemesis::OptionModelList::Parser::Parser(OptionModelList& host)
    : host(host)
{
}

void nemesis::OptionModelList::Parser::Run()
{
    AddFixedOptions();
    GetFileLines(GetFilePath(), storeline);

    for (auto& line : storeline)
    {
        ParseLine(line);
    }

    Commit();
}

nemesis::OptionModelList::Rules::Rules(SPtr<nemesis::OptionModel> begin,
                                     SPtr<nemesis::OptionModel> end)
{
    this->begin = begin;
    this->end   = end;
}

bool nemesis::OptionModelList::Rules::HasBegin()
{
    return begin != nullptr;
}

bool nemesis::OptionModelList::Rules::HasEnd()
{
    return end != nullptr;
}

SPtr<nemesis::OptionModel> nemesis::OptionModelList::Rules::GetBegin()
{
    return begin;
}

SPtr<nemesis::OptionModel> nemesis::OptionModelList::Rules::GetEnd()
{
    return end;
}

void nemesis::OptionModelList::ReadOptionListFile()
{
    Parser parser(*this);
    parser.Run();
}

void nemesis::OptionModelList::AddOptionModel(const nemesis::Line& option)
{
    auto shptr = std::make_shared<nemesis::OptionModel>(option, filepath, *templatecategory);
    auto name  = std::string(shptr->GetName());

    if (Contains(name)) ErrorMessage(1177, templatecategory->GetName(), filepath, option.GetLineNumber(), name);

    optionsmap.insert(std::make_pair(name.length(), shptr));
    options.insert(name);
}

void nemesis::OptionModelList::SortOptionsByLength() {}

nemesis::OptionModel* nemesis::OptionModelList::GetInnerModel(const std::string modelname) const noexcept
{
    for (auto& option : optionsmap)
    {
        if (option.second->GetName() == modelname) return option.second.get();
    }

    return nullptr;
}

nemesis::OptionModelList::OptionModelList(const nemesis::TemplateCategory& _templateclass) noexcept
    : templatecategory(&_templateclass)
{
}

nemesis::OptionModelList::OptionModelList(const std::filesystem::path& filepath,
                                          const nemesis::TemplateCategory& _templateclass) noexcept
    : templatecategory(&_templateclass)
{
    try
    {
        this->filepath = filepath;
        InjectOptionModel(motion_str);
        InjectOptionModel(rotation_str);
        AddOptionModel(duration::fullname);
        ReadOptionListFile();
    }
    catch (const std::exception& ex)
    {
        ErrorMessage(6002, "None", ex.what());
    }
    catch (const nemesis::exception&)
    {
    }
}

size_t nemesis::OptionModelList::GetAnimObjectCount() const
{
    if (!iAnimObj) ErrorMessage(1017, templatecategory->GetName(), filepath);

    return *iAnimObj;
}

bool nemesis::OptionModelList::Contains(const std::string& name) const
{
    return options.find(name) != options.end();
}

nemesis::OptionModel* nemesis::OptionModelList::GetModel(const std::string& modelname) noexcept
{
    return GetInnerModel(modelname);
}

const nemesis::OptionModel* nemesis::OptionModelList::GetModel(const std::string& modelname) const noexcept
{
    return GetInnerModel(modelname);
}

bool nemesis::OptionModelList::IsCompulsoryMet(const VecStr& optionlist) const noexcept
{
    auto contains_option = [&](const std::string& name) {
        for (auto& option : optionlist)
        {
            if (option == name) return true;
        }

        return false;
    };

    for (auto& name : compulsory)
    {
        if (!contains_option(name)) return false;
    }

    return true;
}

Vec<const nemesis::OptionModel*> nemesis::OptionModelList::GetOptionList() const
{
    Vec<const OptionModel*> optionmodellist;

    for (auto& each : optionsmap)
    {
        optionmodellist.emplace_back(each.second.get());
    }

    return optionmodellist;
}

void nemesis::OptionModelList::InjectOptionModel(const std::string& modelinfo)
{
    auto optptr = std::make_shared<OptionModel>(filepath, *templatecategory);
    optptr->AddModelInfoNoCheck(modelinfo);
    auto name = optptr->GetName();
    optionsmap.insert(std::make_pair(name.length(), optptr));
    options.insert(std::string(name));
}

UPtr<nemesis::Option>
nemesis::OptionModelList::CreateOption(const std::string& name,
                                       const nemesis::AnimQuery& animquery) const noexcept
{
    auto model = GetModel(name);

    if (model != nullptr) return std::move(model->CreateOption(name, animquery));

    for (const auto& each : optionsmap)
    {
        model = each.second.get();

        if (!model->HasVariable()) continue;

        if (!StringStartWith(name, model->GetName())) continue;

        return std::move(model->CreateOption(name, animquery));
    }

    return nullptr;
}
