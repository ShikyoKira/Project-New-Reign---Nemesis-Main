#include "animsetdata/animsetdata-project.h"

nemesis::animsetdata::Project::Project(const VecNstr& lines) {}

nemesis::animsetdata::Project::Project(const Project& project) noexcept 
    : name(project.name)
    , states(project.states)
{
}

void nemesis::animsetdata::Project::SetName(const std::string& name)
{
    this->name = name;
}

std::string_view nemesis::animsetdata::Project::GetName() const
{
    return name;
}

VecNstr nemesis::animsetdata::Project::GetLines() const
{
    VecNstr lines;
    lines.emplace_back(std::to_string(states.size()));

    for (auto& state : states)
    {
        state.GetRawName(lines);
    }
    
    for (auto& state : states)
    {
        state.GetRawData(lines);
    }

    return lines;
}

VecNstr nemesis::animsetdata::Project::GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines;
    Vec<const nemesis::animsetdata::State*> statelist;

    for (auto& state : states)
    {
        auto compiledstates = state.GetCompiledData(scopeinfo);
        statelist.insert(statelist.end(), compiledstates.begin(), compiledstates.end());
    }

    std::sort(statelist.begin(),
              statelist.end(),
              [](const nemesis::animsetdata::State* state1, const nemesis::animsetdata::State* state2)
              { return alphanum_less()(state1->GetName(), state2->GetName()); });

    lines.emplace_back(std::to_string(statelist.size()));

    for (auto& state : statelist)
    {
        auto statelines = state->GetLines();
        std::move(statelines.begin(), statelines.end(), std::back_inserter(lines));
    }

    return lines;
}
