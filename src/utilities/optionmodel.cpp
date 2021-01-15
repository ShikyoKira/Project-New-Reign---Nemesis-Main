#include "utilities/line.h"
#include "utilities/option.h"
#include "utilities/optionmodel.h"
#include "utilities/templateclass.h"
#include "utilities/stringextension.h"

void nemesis::OptionModel::SetName(const nemesis::Line& modelinfo)
{
    uint pos = modelinfo.find("<");

    if (pos == NOT_FOUND && bArray)
    {
        pos = modelinfo.rfind("[]");
    }

    name = modelinfo.substr(0, pos).ToString();

    if (name.length() > 4)
    {
        ErrorMessage(1011, templateclass.GetName(), filepath, modelinfo.GetLineNumber(), name);
    }
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
                                  const nemesis::TemplateClass& _templateclass)
    : templateclass(_templateclass)
{
    filepath = _filepath;
    full     = modelinfo;
    bArray   = HasArraySyntax(modelinfo);
    SetName(modelinfo);
    TryAddVariables(modelinfo);
}

void nemesis::OptionModel::TryAddVariables(const nemesis::Line& modelinfo)
{
    if (modelinfo.find("<") == NOT_FOUND || modelinfo.find(">") == NOT_FOUND) return;

    auto opening  = count(modelinfo.begin(), modelinfo.end(), '<');
    auto closing  = count(modelinfo.begin(), modelinfo.end(), '>');
    auto throwerr = [&]() {
        ErrorMessage(1014, templateclass.GetName(), filepath, modelinfo.GetLineNumber());
    };

    if (opening != closing) throwerr();

    novarline.clear();
    varpoints.clear();
    varnameset.clear();
    variables.clear();

    uint size = modelinfo.length() - 2 * bArray;
    uint i;
    uint open;
    auto& tryclose = [&]() {
        variables.emplace_back("");
        std::string& varname = variables.back();

        for (++i; i < size; ++i)
        {
            auto& ch = modelinfo[i];

            switch (ch)
            {
                case '<':
                    throwerr();
                    break;
                case '>':
                    varnameset.insert(varname);
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
                varpoints.insert(novarline.length());
                tryclose();
                break;
            case '>':
                throwerr();
                break;
            default:
                novarline.push_back(ch);
                break;
        }
    }
}

void nemesis::OptionModel::AddLink(std::shared_ptr<nemesis::OptionModel> modelptr)
{
    linkedmodel.emplace_back(modelptr);
}

std::string nemesis::OptionModel::GetName() const noexcept
{
    return name;
}

std::string nemesis::OptionModel::GetFullName() const noexcept
{
    return full;
}

bool nemesis::OptionModel::IsArray() const noexcept
{
    return bArray;
}

bool nemesis::OptionModel::HasVariable(const std::string& var) const noexcept
{
    return varnameset.find(var) != varnameset.end();
}

std::unique_ptr<nemesis::Option> nemesis::OptionModel::CreateOption(const std::string& query) const noexcept
{
    return std::make_unique<nemesis::Option>(query, *this);
}

bool nemesis::OptionModel::HasArraySyntax(const std::string& modelinfo) noexcept
{
    return StringEndWith(modelinfo, "[]");
}
