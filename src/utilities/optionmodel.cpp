#include "utilities/line.h"
#include "utilities/option.h"
#include "utilities/optionmodel.h"
#include "utilities/templatecategory.h"
#include "utilities/stringextension.h"

const nemesis::regex nemesis::OptionModel::nameexclusion_rgx("AnimObject/[0-9]+");

void nemesis::OptionModel::Variable::AddName(const std::string& name)
{
    this->name = name;

    while (!this->name.empty())
    {
        if (this->name.back() == ' ')
        {
            this->name.pop_back();
            continue;
        }
        else if (this->name.front() == ' ')
        {
            this->name.erase(0);
            continue;
        }

        break;
    }
}

void nemesis::OptionModel::Variable::AddDefaultValue(const std::string& value)
{
    defaultvalue = std::make_unique<std::string>(value);
}

nemesis::OptionModel::Variable::Variable(std::string variable)
{
    size_t pos = variable.rfind("=");

    if (pos != NOT_FOUND)
    {
        auto value = variable.substr(pos + 1);
        AddDefaultValue(value);
        variable = variable.substr(0, pos);
    }

    AddName(variable);
}

std::string_view nemesis::OptionModel::Variable::GetName() const
{
    return name;
}

std::string_view nemesis::OptionModel::Variable::GetDefaultValue() const
{
    if (!HasDefault()) return "";

    return *defaultvalue;
}

bool nemesis::OptionModel::Variable::HasDefault() const
{
    return defaultvalue.operator bool();
}

void nemesis::OptionModel::AddModelInfoNoCheck(const nemesis::Line& modelinfo)
{
    try
    {
        full   = modelinfo;
        bArray = HasArraySyntax(modelinfo);
        TryAddVariables(modelinfo);
        SetNameNoCheck(modelinfo);
    }
    catch (const std::exception& ex)
    {
        ErrorMessage(6002, "None", ex.what());
    }
    catch (const nemesis::exception&)
    {
    }
}

void nemesis::OptionModel::SetName(const nemesis::Line& modelinfo)
{
    SetNameNoCheck(modelinfo);

    if (name.length() <= 4) return;

    if (nemesis::regex_match(name, nameexclusion_rgx)) return;

    ErrorMessage(1011, templateclass->GetName(), filepath, modelinfo.GetLineNumber(), name);
}

void nemesis::OptionModel::SetNameNoCheck(const nemesis::Line& modelinfo)
{
    size_t pos = modelinfo.find("<");

    if (pos == NOT_FOUND && bArray)
    {
        pos = modelinfo.rfind("[]");
    }

    name = modelinfo.substr(0, pos).ToString();
}

void nemesis::OptionModel::AddVarBlock(const VecStr& additioninfo)
{
    std::string key = "<" + additioninfo[2] + ">";
    auto& curblocks = varblocks[additioninfo[2]];
    curblocks.clear();

    auto templine = additioninfo.back();
    auto pos      = templine.find(key);

    while (pos != NOT_FOUND)
    {
        curblocks.emplace_back(templine.substr(0, pos));
        curblocks.emplace_back("");
        templine.erase(0, pos + key.length());
        pos = templine.find(key);
    }

    if (!templine.empty()) curblocks.emplace_back(templine);
}

nemesis::OptionModel::OptionModel(const nemesis::Line& modelinfo,
                                  std::filesystem::path& _filepath,
                                  const nemesis::TemplateCategory& _templateclass)
    : templateclass(&_templateclass)
{
    filepath = _filepath;
    AddModelInfo(modelinfo);
}

nemesis::OptionModel::OptionModel(std::filesystem::path& _filepath,
                                  const nemesis::TemplateCategory& _templateclass)
    : templateclass(&_templateclass)
{
    filepath = _filepath;
}

nemesis::OptionModel::OptionModel(const nemesis::Line& modelinfo,
                                  std::filesystem::path& _filepath,
                                  const nemesis::TemplateClass& template_class)
    : TemplateClass(&template_class)
{
}

void nemesis::OptionModel::TryAddVariables(const nemesis::Line& modelinfo)
{
    if (modelinfo.find("<") == NOT_FOUND && modelinfo.find(">") == NOT_FOUND) return;

    auto opening  = count(modelinfo.begin(), modelinfo.end(), '<');
    auto closing  = count(modelinfo.begin(), modelinfo.end(), '>');
    auto throwerr = [&]() {
        ErrorMessage(1014, templateclass->GetName(), filepath, modelinfo.GetLineNumber());
    };

    if (opening != closing) throwerr();

    std::string rgxstr = "^";
    variables.clear();

    size_t size = modelinfo.length() - 2 * bArray;
    size_t i;
    auto tryclose = [&]() {
        std::string varname;

        for (++i; i < size; ++i)
        {
            auto& ch = modelinfo[i];

            switch (ch)
            {
                case '<':
                    throwerr();
                    break;
                case '>':
                    variables.emplace_back(varname);
                    return;
                default:
                    varname.push_back(ch);
                    break;
            }
        }

        throwerr();
    };

    for (i = 0; i < size; ++i)
    {
        auto& ch = modelinfo[i];

        switch (ch)
        {
            case '<':
                rgxstr.append("(.*?)");
                tryclose();
                break;
            case '>':
                throwerr();
                break;
            default:
                if (!isalnum(ch))
                {
                    rgxstr.push_back('\\');
                }

                rgxstr.push_back(ch);
                break;
        }
    }

    rgxstr.push_back('$');
    rgx = rgxstr;
}

void nemesis::OptionModel::AddLink(nemesis::OptionModel* modelptr)
{
    linkedmodel.emplace_back(modelptr);
}

std::string_view nemesis::OptionModel::GetName() const noexcept
{
    return name;
}

std::string_view nemesis::OptionModel::GetFullName() const noexcept
{
    return full;
}

bool nemesis::OptionModel::IsArray() const noexcept
{
    return bArray;
}

bool nemesis::OptionModel::HasVariable() const noexcept
{
    return !variables.empty();
}

bool nemesis::OptionModel::HasVariable(const std::string& name) const noexcept
{
    for (auto& var : variables)
    {
        if (nemesis::iequals(std::string(var.GetName()), name)) return true;
    }

    return false;
}

const nemesis::OptionModel::Variable*
nemesis::OptionModel::GetVariablePtr(const std::string name) const noexcept
{
    for (auto& var : variables)
    {
        if (nemesis::iequals(std::string(var.GetName()), name)) return &var;
    }

    return nullptr;
}

Vec<const nemesis::OptionModel::Variable*> nemesis::OptionModel::GetVariablesList() const
{
    Vec<const nemesis::OptionModel::Variable*> HkxVariableList;

    for (auto& var : variables)
    {
        HkxVariableList.emplace_back(&var);
    }

    return HkxVariableList;
}

void nemesis::OptionModel::AddModelInfo(const nemesis::Line& modelinfo)
{
    try
    {
        full   = modelinfo;
        bArray = HasArraySyntax(modelinfo);
        TryAddVariables(modelinfo);
        SetName(modelinfo);
    }
    catch (const std::exception& ex)
    {
        ErrorMessage(6002, "None", ex.what());
    }
    catch (const nemesis::exception&)
    {
    }
}

UMap<std::string, std::string> nemesis::OptionModel::ParseVariables(const std::string& query) const noexcept
{
    UMap<std::string, std::string> varmap;

    if (!HasVariable()) return varmap;

    nemesis::smatch mtch;

    if (!regex_match(query, mtch, rgx)) return varmap;

    for (size_t i = 1; i < mtch.size(); ++i)
    {
        std::string tempstr = mtch[i];
        auto& curvar        = variables[i - 1];

        if (tempstr.empty()) tempstr = curvar.GetDefaultValue();

        varmap[std::string(curvar.GetName())] = tempstr;
    }

    return varmap;
}

UPtr<nemesis::Option> nemesis::OptionModel::CreateOption(const std::string& query,
                                                         const nemesis::AnimQuery& animquery) const noexcept
{
    return std::make_unique<nemesis::Option>(query, *this, animquery);
}

bool nemesis::OptionModel::HasArraySyntax(const std::string& modelinfo) noexcept
{
    return StringEndWith(modelinfo, "[]");
}
