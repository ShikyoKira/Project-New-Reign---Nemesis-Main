#include "update/animsetdata/animsetdataprojectcond.h"

using namespace std;

ProjectAnimSetData::ProjectAnimSetData(const ProjectName& _name, const VecNstr& storeline)
{
    name = _name;

    for (auto& line : storeline)
    {
        addAnimSetData(line);
    }
}

nemesis::LinkedVar<AnimSetData>& ProjectAnimSetData::addAnimSetData(const nemesis::Line& line)
{
    setIndexMap[nemesis::to_lower_copy(line)] = animSetData.size();
    animSetData.push_back(AnimSetData(line, line.linenum));
    return animSetData.back();
}

nemesis::LinkedVar<AnimSetData>& ProjectAnimSetData::addAnimSetData(const nemesis::Line& line,
                                                                    const nemesis::CondDetails& condition)
{
    setIndexMap[nemesis::to_lower_copy(line)] = animSetData.size();
    animSetData.push_back(nemesis::CondVar(AnimSetData(line, line.linenum), condition));
    return animSetData.back().backCond().rawlist.back();
}

void ProjectAnimSetData::importAnimSetData(filesystem::path filepath, const VecNstr& _namelist, size_t& index)
{
    for (; index < _namelist.size(); ++index)
    {
        auto& line = _namelist[index];

        if (line == "V3") return;

        addAnimSetData(line);
    }
}

void ProjectAnimSetData::importAnimSetData(filesystem::path filepath,
                                           const VecNstr& _namelist,
                                           size_t& index,
                                           nemesis::CondCheckFunc condfunc,
                                           const ModCode& format)
{
    bool original = false;
    deque<nemesis::Line>* edits = nullptr;
    vector<nemesis::CondDetails> condtype;

    for (; index < _namelist.size(); ++index)
    {
        auto& line = _namelist[index];

        if (line == "V3") return;

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
                addAnimSetData(edit, condtype.back());
            }

            original = false;
            condtype.pop_back();
            edits = condtype.size() > 0 ? &condtype.back().contents : nullptr;
        }
        else if (original)
        {
            nemesis::Line vline = nemesis::CondDetails::getOriginalLine(edits);
            addAnimSetData(vline, condtype.back());
        }
        else if (edits)
        {
            edits->push_back(line);
        }
    }
}

const vector<nemesis::LinkedVar<AnimSetData>>& ProjectAnimSetData::getAnimSetData() const
{
    return animSetData;
}

shared_ptr<AnimSetData> ProjectAnimSetData::find(const ProjectName& _name) const
{
    return animSetData[getIndex(_name)].raw;
}

shared_ptr<AnimSetData> ProjectAnimSetData::find(const ProjectName& _name, const ModCode& modcode) const
{
    auto& curASD = animSetData[getIndex(_name)];

    if (!curASD.raw)
    {
        for (auto& cond : curASD.nestedcond)
        {
            if (cond.conditions != modcode) continue;

            auto& animData = cond.rawlist[0];

            if (!animData.raw) continue;

            return animData.raw;
        }
    }

    return curASD.raw;
}

void ProjectAnimSetData::getlines(VecStr& storeline) const
{
    VecStr tempstore;
    storeline.push_back(to_string(animSetData.size()));
    multimap<string, pair<VecStr, pair<VecStr, const nemesis::LinkedVar<AnimSetData>*>>, alphanum_less> orderedASD;

    for (auto& data : animSetData)
    {
        VecStr temp;
        VecStr nametemp;
        getLinkedLines(data, temp, nametemp);

        for (auto& name : nametemp)
        {
            if (name.find("<!--") == NOT_FOUND)
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

uint ProjectAnimSetData::getIndex(const ProjectName& _name) const
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
    vector<pair<const string*, const nemesis::CondVar<ProjectAnimSetData>*>> modcodelist;

    for (auto& cond : linkedpack.nestedcond)
    {
        switch (cond.conditionType)
        {
            case nemesis::MOD_CODE:
            {
                modcodelist.push_back(make_pair<const string*, const nemesis::CondVar<ProjectAnimSetData>*>(
                    &cond.conditions, &cond));
                break;
            }
            case nemesis::FOREACH:
            {
                storeline.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");
                namelines.push_back("<!-- FOREACH ^" + cond.conditions + "^ -->");

                for (auto& each : cond.rawlist)
                {
                    getLinkedLines(each, storeline, namelines);
                }

                storeline.push_back("<!-- CLOSE -->");
                namelines.push_back("<!-- CLOSE -->");
                break;
            }
        }
    }

    if (modcodelist.size() > 0)
    {
        if (linkedpack.raw)
        {
            vector<pair<string, VecStr>> list;
            vector<pair<string, VecStr>> namelist;

            for (auto& modcode : modcodelist)
            {
                list.push_back(pair<string, VecStr>());
                list.back().first = *modcode.first;
                namelist.push_back(pair<string, VecStr>());
                namelist.back().first = *modcode.first;
                getLinkedLines(modcode.second->rawlist[0], list.back().second, namelist.back().second);
            }

            for (auto& each : list)
            {
                storeline.push_back("<!-- NEW *" + each.first + "* -->");
                storeline.insert(storeline.end(), each.second.begin(), each.second.end());
            }

            for (auto& each : namelist)
            {
                namelines.push_back("<!-- NEW *" + each.first + "* -->");
                namelines.insert(namelines.end(), each.second.begin(), each.second.end());
            }

            if (list.size() > 0)
            {
                storeline.push_back("<!-- ORIGINAL -->");
                linkedpack.raw->getlines(storeline);
                storeline.push_back("<!-- CLOSE -->");

                namelines.push_back("<!-- ORIGINAL -->");
                linkedpack.raw->getname(namelines);
                namelines.push_back("<!-- CLOSE -->");
            }
            else
            {
                linkedpack.raw->getlines(storeline);
                linkedpack.raw->getname(namelines);
            }
        }
        else
        {
            unordered_map<string, vector<const nemesis::CondVar<ProjectAnimSetData>*>> mappedLink;

            for (auto& modcode : modcodelist)
            {
                mappedLink[*modcode.first].push_back(modcode.second);
            }

            for (auto& asdprojlist : mappedLink)
            {
                storeline.push_back("<!-- NEW *" + asdprojlist.first + "* -->");
                namelines.push_back("<!-- NEW *" + asdprojlist.first + "* -->");

                for (auto& asd : asdprojlist.second)
                {
                    getLinkedLines(asd->rawlist[0], storeline, namelines);
                }

                storeline.push_back("<!-- CLOSE -->");
                namelines.push_back("<!-- CLOSE -->");
            }
        }
    }
    else if (linkedpack.raw)
    {
        linkedpack.raw->getlines(storeline);
        linkedpack.raw->getname(namelines);
    }
}
