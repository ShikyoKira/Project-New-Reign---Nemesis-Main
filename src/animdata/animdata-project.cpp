#include "animdata/animdata-project.h"

nemesis::animdata::AnimProject::AnimProject(const VecNstr& lines)
{
}

void nemesis::animdata::AnimProject::SetName(const std::string& name)
{
    this->name = name;
}

std::string_view nemesis::animdata::AnimProject::GetName() const
{
    return name;
}

void nemesis::animdata::AnimProject::AddHkxFile(const nemesis::Line& line)
{
    auto linkedline = std::make_shared<LinkedPreprocessLine>(line);
    hkx_files.emplace_back(linkedline);
}

void nemesis::animdata::AnimProject::AddAnimationData(const nemesis::Line& line)
{
    auto linkedline = std::make_shared<LinkedPreprocessLine>(line);
    animdata.emplace_back(linkedline);
}

void nemesis::animdata::AnimProject::AddMovementData(const nemesis::Line& line)
{
    auto linkedline = std::make_shared<LinkedPreprocessLine>(line);
    movedata.emplace_back(linkedline);
}

VecNstr nemesis::animdata::AnimProject::GetLines() const
{
    VecNstr lines;
    VecNstr hkxlines;
    VecNstr animlines;
    VecNstr mdlines;

    for (auto& hkx_file : hkx_files)
    {
        hkx_file->GetRawData(hkxlines);
    }

    for (auto& data : animdata)
    {
        data->GetRawData(animlines);
    }

    for (auto& data : movedata)
    {
        data->GetRawData(mdlines);
    }

    lines.emplace_back(std::to_string(hkxlines.size() + animlines.size() + 3));
    lines.emplace_back(hkxlines.empty() && animlines.empty() && mdlines.empty() ? "0" : "1");
    lines.emplace_back(std::to_string(hkxlines.size()));
    lines.insert(lines.end(), hkxlines.begin(), hkxlines.end());

    lines.emplace_back(animlines.empty() ? "0" : "1");
    lines.emplace_back(std::to_string(animlines.size()));
    lines.insert(lines.end(), animlines.begin(), animlines.end());

    lines.emplace_back(std::to_string(mdlines.size()));
    lines.insert(lines.end(), mdlines.begin(), mdlines.end());
    return lines;
}

VecNstr nemesis::animdata::AnimProject::GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines;
    VecNstr hkxlines;
    VecNstr animlines;
    VecNstr mdlines;
    
    for (auto& hkx_file : hkx_files)
    {
        auto compiledlines = hkx_file->GetProcessedLines(scopeinfo);
        hkxlines.insert(hkxlines.end(), compiledlines.begin(), compiledlines.end());
    }
    
    for (auto& data : animdata)
    {
        auto compiledlines = data->GetProcessedLines(scopeinfo);
        animlines.insert(animlines.end(), compiledlines.begin(), compiledlines.end());
    }
    
    for (auto& data : movedata)
    {
        auto compiledlines = data->GetProcessedLines(scopeinfo);
        mdlines.insert(mdlines.end(), compiledlines.begin(), compiledlines.end());
    }

    lines.emplace_back(std::to_string(hkxlines.size() + animlines.size() + 3));
    lines.emplace_back(hkxlines.empty() && animlines.empty() && mdlines.empty() ? "0" : "1");
    lines.emplace_back(std::to_string(hkxlines.size()));
    lines.insert(lines.end(), hkxlines.begin(), hkxlines.end());

    lines.emplace_back(animlines.empty() ? "0" : "1");
    lines.emplace_back(std::to_string(animlines.size()));
    lines.insert(lines.end(), animlines.begin(), animlines.end());
    
    lines.emplace_back(std::to_string(mdlines.size()));
    lines.insert(lines.end(), mdlines.begin(), mdlines.end());
    return lines;
}
