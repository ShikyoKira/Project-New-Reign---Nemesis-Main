#include <functional>

#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"

#include "update/animsetdata/projectanimsetdata.h"

using namespace std;
namespace ns = nemesis::syntax;

ProjectAnimSetData::Parser::Parser(ProjectAnimSetData& base, const VecNstr& namelist) noexcept
    : host(base)
    , namelist(namelist)
{
}

void ProjectAnimSetData::Parser::SetFormat(const string& format) noexcept
{
    this->format = format;
}

void ProjectAnimSetData::Parser::SetPath(std::filesystem::path path) noexcept
{
    this->path = path;
}

void ProjectAnimSetData::Parser::SetStartIndex(const size_t& startindex) noexcept
{
    index = startindex;
}

void ProjectAnimSetData::Parser::AddAnimSetDataList()
{
    for (; index < namelist.size(); ++index)
    {
        auto& line = namelist[index];

        if (line == "V3") return;

        host.AddAnimSetData(line);
    }
}

void ProjectAnimSetData::Parser::ImportAnimSetDataList()
{
    //bool original = false;
    //deque<nemesis::Line>* edits = nullptr;
    //vector<nemesis::CondDetails> condtype;

    tobedeleted = nullptr;
    nemesis::ConditionScope cscope(format, path);

    for (; index < namelist.size(); ++index)
    {
        auto& line = namelist[index];

        if (line == "V3") return;

        auto conditioninfo = cscope.TryGetConditionInfo(line);

        if (!conditioninfo)
        {
            if (!cscope.Empty() && cscope.Back().GetType() == nemesis::CondType::ORIGINAL)
            {
                PointingOriAnimSetData();
            }

            continue;
        }

        switch (conditioninfo->GetType())
        {
            case nemesis::CondType::ORIGINAL:
            {
                tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                break;
            }
            case nemesis::CondType::CLOSE:
            {
                if (!tobedeleted)
                {
                    tobedeleted = cscope.GetToBeDeleted().shared_from_this();
                }

                CloseAnimSetData();
                tobedeleted = nullptr;
                break;
            }
        }

        /*
        if (condfunc(filepath, format, line, condtype))
        {
            edits = &condtype.back().contents;
        }
        else if (nemesis::CondDetails::originalScope(filepath, format, line, condtype))
        {
            original = true;
        }
        else if (nemesis::CondDetails::closeScope(filepath, format, line, condtype))
        {
            for (auto& edit : *edits)
            {
                host.AddAnimSetData(edit, condtype.back());
            }

            original = false;
            condtype.pop_back();
            edits = !condtype.empty() ? &condtype.back().contents : nullptr;
        }
        else if (original)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            host.AddAnimSetData(vline, condtype.back());
        }
        else if (edits)
        {
            edits->emplace_back(line);
        }
        */
    }
}

size_t ProjectAnimSetData::Parser::GetIndex() const noexcept
{
    return index;
}

void ProjectAnimSetData::Parser::PointingOriAnimSetData()
{
    nemesis::Line vline = nemesis::CondDetails::getOriginalLine(&tobedeleted->GetRefContents());
    host.AddAnimSetData(vline, *tobedeleted);
}

void ProjectAnimSetData::Parser::CloseAnimSetData()
{
    for (auto& edit : tobedeleted->GetContents())
    {
        host.AddAnimSetData(edit, *tobedeleted);
    }
}

ProjectAnimSetData::ProjectAnimSetData(const ProjectName& _name, const VecNstr& storeline)
{
    name = _name;

    for (auto& line : storeline)
    {
        AddAnimSetData(line);
    }
}

nemesis::LinkedVar<AnimSetData>& ProjectAnimSetData::AddAnimSetData(const nemesis::Line& line)
{
    setIndexMap[nemesis::to_lower_copy(line).ToString()] = animSetData.size();
    animSetData.emplace_back(AnimSetData(line, line.GetLineNumber()));
    return animSetData.back();
}

nemesis::LinkedVar<AnimSetData>& ProjectAnimSetData::AddAnimSetData(const nemesis::Line& line,
                                                                    const nemesis::CondDetails& condition)
{
    setIndexMap[nemesis::to_lower_copy(line).ToString()] = animSetData.size();
    animSetData.emplace_back(nemesis::CondVar(AnimSetData(line, line.GetLineNumber()), condition));
    return animSetData.back().backCond().rawlist.back();
}

nemesis::LinkedVar<AnimSetData>&
ProjectAnimSetData::AddAnimSetData(const nemesis::Line& line, const nemesis::ConditionInfo& conditioninfo)
{
    setIndexMap[nemesis::to_lower_copy(line).ToString()] = animSetData.size();
    animSetData.emplace_back(nemesis::CondVar(AnimSetData(line, line.GetLineNumber()), conditioninfo));
    return animSetData.back().backCond().rawlist.back();
}

void ProjectAnimSetData::ImportData(std::filesystem::path path, size_t& num, const VecNstr& storeline)
{
    Parser parser(*this, storeline);
    parser.SetPath(path);
    parser.SetStartIndex(num);

    parser.AddAnimSetDataList();
    num = parser.GetIndex();
}

void ProjectAnimSetData::ImportData(std::filesystem::path path,
                                    size_t& num,
                                    const VecNstr& storeline,
                                    const ModCode format)
{
    Parser parser(*this, storeline);
    parser.SetPath(path);
    parser.SetStartIndex(num);
    parser.SetFormat(format);

    parser.ImportAnimSetDataList();
    num = parser.GetIndex();
}

const vector<nemesis::LinkedVar<AnimSetData>>& ProjectAnimSetData::getAnimSetData() const
{
    return animSetData;
}

SPtr<AnimSetData> ProjectAnimSetData::find(const ProjectName& _name) const
{
    return animSetData[getIndex(_name)].raw;
}

SPtr<AnimSetData> ProjectAnimSetData::find(const ProjectName& _name, const ModCode& modcode) const
{
    auto& curASD = animSetData[getIndex(_name)];

    if (curASD.raw) return curASD.raw;

    for (auto& cond : curASD.nestedcond)
    {
        if (cond.conditions != modcode) continue;

        auto& animData = cond.rawlist[0];

        if (!animData.raw) continue;

        return animData.raw;
    }

    return nullptr;
}

void ProjectAnimSetData::getlines(VecStr& storeline) const
{
    VecStr tempstore;
    storeline.emplace_back(to_string(animSetData.size()));
    multimap<string, pair<VecStr, pair<VecStr, const nemesis::LinkedVar<AnimSetData>*>>, alphanum_less> orderedASD;

    for (auto& data : animSetData)
    {
        VecStr temp;
        VecStr nametemp;
        getLinkedLines(data, temp, nametemp);

        for (auto& name : nametemp)
        {
            if (name.find(ns::OpenComment()) == NOT_FOUND)
            {
                orderedASD.insert(make_pair(name, make_pair(temp, make_pair(nametemp, &data))));
                break;
            }
        }
    }

    string last;
    string lastname;

    for (auto& asd : orderedASD)
    {
        combineLines(last, asd.second.first, tempstore);
        combineLines(lastname, asd.second.second.first, storeline);
    }

    storeline.insert(storeline.end(), tempstore.begin(), tempstore.end());
}

void ProjectAnimSetData::getname(VecStr& storeline) const
{
    getLinkedLines(name, storeline);
}

size_t ProjectAnimSetData::getIndex(const ProjectName& _name) const
{
    auto itr = setIndexMap.find(nemesis::to_lower_copy(_name));

    if (itr == setIndexMap.end()) ErrorMessage(5026, _name);

    return itr->second;
}

bool ProjectAnimSetData::contains(const ProjectName& _name) const
{
    return setIndexMap.find(nemesis::to_lower_copy(_name)) != setIndexMap.end();
}

void getLinkedLines(const nemesis::LinkedVar<ProjectAnimSetData>& linkedpack,
                    VecStr& storeline,
                    VecStr& namelines)
{
    VecStr tempstore;
    VecStr tempnames;
    unordered_map<string, pair<VecStr, VecStr>> mappedproject;
    function<void(const string&, const nemesis::CondVar<ProjectAnimSetData>&)> modcodelambda;

    if (linkedpack.raw)
    {
        modcodelambda = [&](const string& condition, const nemesis::CondVar<ProjectAnimSetData>& project)
        {
            string modcondition = ns::ModCode(condition);
            tempstore.emplace_back(modcondition);
            tempnames.emplace_back(modcondition);
            getLinkedLines(project.rawlist[0], tempstore, tempnames);
        };
    }
    else
    {
        modcodelambda = [&](const string& condition, const nemesis::CondVar<ProjectAnimSetData>& project)
        {
            auto& mproject = mappedproject[condition];
            getLinkedLines(project.rawlist[0], mproject.first, mproject.second);
        };
    }

    auto addToBoth = [&](const string& input)
    {
        storeline.emplace_back(input);
        namelines.emplace_back(input);
    };

    for (auto& cond : linkedpack.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::CondType::MOD_CODE:
            {
                modcodelambda(cond.conditions, cond);
                break;
            }
            case nemesis::CondType::FOREACH:
            {
                addToBoth(ns::ForEach(cond.conditions));

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline, namelines);
                }

                addToBoth(ns::Close());
                break;
            }
        }
    }

    if (linkedpack.raw)
    {
        if (!tempstore.empty())
        {
            storeline.insert(storeline.end(), tempstore.begin(), tempstore.end());
            namelines.insert(namelines.end(), tempnames.begin(), tempnames.end());

            addToBoth(ns::ModOriginal());
            linkedpack.raw->getlines(storeline);
            linkedpack.raw->getname(namelines);
            addToBoth(ns::Close());
        }
        else
        {
            linkedpack.raw->getlines(storeline);
            linkedpack.raw->getname(namelines);
        }

        return;
    }

    for (auto& asdprojlist : mappedproject)
    {
        addToBoth(ns::ModCode(asdprojlist.first));
        auto& projlines = asdprojlist.second.first;
        auto& projnames = asdprojlist.second.second;
        storeline.insert(storeline.end(), projlines.begin(), projlines.end());
        namelines.insert(namelines.end(), projnames.begin(), projnames.end());
        addToBoth(ns::Close());
    }
}
