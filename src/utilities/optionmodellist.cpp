#include <regex>

#include "utilities/optionmodel.h"
#include "utilities/templateclass.h"
#include "utilities/optionmodellist.h"
#include "utilities/stringextension.h"

using pType = nemesis::OptionModelList::Parser;
using pFuncPtr = void(nemesis::OptionModelList::Parser::*)(const nemesis::Line&);

const std::unordered_map<std::string, pFuncPtr> nemesis::OptionModelList::Parser::mapfunc =
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

    uint pos = str.find("'");

    if (pos == NOT_FOUND) return nemesis::Line(str, line.GetLineNumber());

    return nemesis::Line(str.substr(0, pos), line.GetLineNumber());
}

std::string nemesis::OptionModelList::Parser::GetTemplateClass() const
{
    return host.templateclass.GetName();
}

std::filesystem::path nemesis::OptionModelList::Parser::GetFilePath() const
{
    return host.filepath;
}

void nemesis::OptionModelList::Parser::ErrorTemplate(int errcode, int linenum)
{
    ErrorMessage(errcode, GetTemplateClass(), GetFilePath(), linenum);
}

void nemesis::OptionModelList::Parser::AddEvent(const nemesis::Line& line)
{
    const std::string evntsyntax = "event ";
    host.events.emplace_back(line.substr(evntsyntax.length()));
}

void nemesis::OptionModelList::Parser::AddVariable(const nemesis::Line& line)
{
    const std::string evntsyntax = "variable ";
    host.variables.emplace_back(line.substr(evntsyntax.length()));
}

void nemesis::OptionModelList::Parser::AddAnimObject(const nemesis::Line& line)
{
    VecStr animinfo;
    StringSplit(line, animinfo);

    if (animinfo.size() != 2 || !isOnlyNumber(animinfo.back()))
    {
        ErrorMessage(1175, GetTemplateClass(), GetFilePath(), line.GetLineNumber(), line.ToString());
    }

    host.iAnimObj = std::make_unique<uint>(stoi(animinfo.back()));
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

    host.iState = std::make_unique<uint>(std::stoi(number));
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

    host.iMin = std::make_unique<uint>(std::stoi(num));
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
    else if (!cachecompulsory.empty())
    {
        ErrorTemplate(1045, line.GetLineNumber());
    }

    cachecompulsory = StringSplit(split.back(), ',');
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

void nemesis::OptionModelList::Parser::AddOption(const nemesis::Line& line)
{
    auto shptr       = std::make_shared<nemesis::OptionModel>(line, GetFilePath(), host.templateclass);
    std::string name = shptr->GetName();

    if (host.Contains(name))
    {
        ErrorMessage(1177, GetTemplateClass(), GetFilePath(), line.GetLineNumber(), name);
    }

    host.optionsmap.insert(std::make_pair(name.length(), shptr));
    host.options.insert(shptr);
}

void nemesis::OptionModelList::Parser::LinkedOption()
{
    for (auto& eachlist : cachelink)
    {
        std::vector<std::shared_ptr<nemesis::OptionModel>> modellist;

        for (auto& each : eachlist)
        {
            auto& ptr = host.GetModel(each);

            if (ptr == nullptr) continue;

            modellist.emplace_back(ptr);
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

void nemesis::OptionModelList::Parser::VariableAddition()
{
    for (auto& line : cacheadditionline)
    {
        VecStr split = StringSplit(line);

        if (split.size() != 4) ErrorTemplate(1200, line.GetLineNumber());

        std::string key = "<" + split[2] + ">";

        if (split.back().find(key) == NOT_FOUND)
        {
            ErrorMessage(1006, GetTemplateClass(), GetFilePath(), line.GetLineNumber(), line);
        }

        if (split.back() == key) ErrorTemplate(1005, line.GetLineNumber());

        auto& model = host.GetModel(split[1]);

        if (model == nullptr) continue;

        model->AddVarBlock(split);
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

    AddOption(str);
}

int nemesis::OptionModelList::Parser::TryAddMultiRoot(const nemesis::Line& line)
{
    std::smatch match;

    if (!std::regex_match(line.ToString(), match, multiroottestregex)) return -1;

    if (!std::regex_match(line.ToString(), match, multirootregex)) return 1069;

    if (match.str(1) == "0") return 1076;

    if (match.str(3)[0] != '#' || !isOnlyNumber(match.str(3).substr(1))) return 1071;

    auto& itr = host.multistates.find(match.str(2));

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
    GetFunctionLines(GetFilePath(), storeline);

    for (auto& line : storeline)
    {
        ParseLine(line);
    }

    LinkedOption();
    VariableAddition();
}

nemesis::OptionModelList::Rules::Rules(std::shared_ptr<nemesis::OptionModel> begin,
                                     std::shared_ptr<nemesis::OptionModel> end)
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

std::shared_ptr<nemesis::OptionModel> nemesis::OptionModelList::Rules::GetBegin()
{
    return begin;
}

std::shared_ptr<nemesis::OptionModel> nemesis::OptionModelList::Rules::GetEnd()
{
    return end;
}

void nemesis::OptionModelList::ReadOptionListFile()
{
    Parser parser(*this);
    parser.Run();

}

nemesis::OptionModelList::OptionModelList(const std::filesystem::path& filepath,
                                          const nemesis::TemplateClass& _templateclass) noexcept
    : templateclass(_templateclass)
{
    this->filepath = filepath;
    ReadOptionListFile();
}

uint nemesis::OptionModelList::GetAnimObjectCount() const
{
    if (!iAnimObj) ErrorMessage(1017, templateclass.GetName(), filepath);

    return *iAnimObj;
}

bool nemesis::OptionModelList::Contains(std::string name) const noexcept
{
    for (auto& option : options)
    {
        if (option->GetName() == name) return true;
    }

    return false;
}

std::shared_ptr<nemesis::OptionModel> nemesis::OptionModelList::GetModel(std::string name) const noexcept
{
    for (auto& option : options)
    {
        if (option->GetName() == name) return option;
    }

    return nullptr;
}

void nemesis::OptionModelList::AddOptionModel(const std::string& modelinfo)
{
    options.insert(std::make_shared<OptionModel>(modelinfo, filepath, templateclass));
}

std::unique_ptr<nemesis::Option>
nemesis::OptionModelList::CreateOption(const std::string& query) const noexcept
{
    for (auto& each : optionsmap)
    {
        if (!StringStartWith(query, each.second->GetName())) continue;

        return std::move(each.second->CreateOption(query));
    }

    return nullptr;
}
