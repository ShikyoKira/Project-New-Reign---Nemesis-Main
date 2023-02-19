#include "animsetdata/state.h"

nemesis::animsetdata::State::State(const VecNstr& lines) {}

nemesis::animsetdata::State::State(const State& state) noexcept
    : name(state.name)
    , equiplist(state.equiplist)
    , typelist(state.typelist)
    , animlist(state.animlist)
    , crc32list(state.crc32list)
{
}

void nemesis::animsetdata::State::SetName(const std::string& name) 
{
    this->name = name;
}

std::string_view nemesis::animsetdata::State::GetName() const
{
    return name;
}

VecNstr nemesis::animsetdata::State::GetLines() const
{
    VecNstr lines = {"V3", std::to_string(equiplist.size())};

    for (auto& equip : equiplist)
    {
        equip.GetRawData(lines);
    }

    lines.emplace_back(std::to_string(typelist.size()));

    nemesis::animsetdata::Type typee(lines);
    nemesis::animsetdata::Type type2(typee);

    for (auto& type : typelist)
    {
        type.GetRawData(lines);
    }

    lines.emplace_back(std::to_string(animlist.size()));

    for (auto& anim : animlist)
    {
        anim.GetRawData(lines);
    }
    
    lines.emplace_back(std::to_string(crc32list.size()));

    for (auto& crc32 : crc32list)
    {
        crc32.GetRawData(lines);
    }

    return lines;
}

VecNstr nemesis::animsetdata::State::GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines = {"V3"};
    VecNstr equiplines;
    Vec<const nemesis::animsetdata::Type*> types;
    Vec<const nemesis::animsetdata::Anim*> anims;
    Vec<const nemesis::animsetdata::CRC32*> crc32s;

    for (auto& equip : equiplist)
    {
        auto compiledlines = equip.GetProcessedLines(scopeinfo);
        equiplines.insert(equiplines.end(), compiledlines.begin(), compiledlines.end());
    }

    for (auto& type : typelist)
    {
        auto compiledtypes = type.GetCompiledData(scopeinfo);
        types.insert(types.end(), compiledtypes.begin(), compiledtypes.end());
    }

    for (auto& anim : animlist)
    {
        auto compiledanims = anim.GetCompiledData(scopeinfo);
        anims.insert(anims.end(), compiledanims.begin(), compiledanims.end());
    }

    for (auto& crc32 : crc32list)
    {
        auto compiledcrc32s = crc32.GetCompiledData(scopeinfo);
        crc32s.insert(crc32s.end(), compiledcrc32s.begin(), compiledcrc32s.end());
    }

    lines.emplace_back(std::to_string(equiplines.size()));

    lines.emplace_back(std::to_string(types.size()));
    std::for_each(types.begin(),
                  types.end(),
                  [&lines, &scopeinfo](const nemesis::animsetdata::Type* type)
                  {
                      auto temp_lines = type->GetCompiledLines(scopeinfo);
                      lines.insert(lines.end(), temp_lines.begin(), temp_lines.end());
                  });
    
    lines.emplace_back(std::to_string(anims.size()));
    std::for_each(anims.begin(),
                  anims.end(),
                  [&lines, &scopeinfo](const nemesis::animsetdata::Anim* anim)
                  {
                      auto temp_lines = anim->GetCompiledLines(scopeinfo);
                      lines.insert(lines.end(), temp_lines.begin(), temp_lines.end());
                  });
    
    lines.emplace_back(std::to_string(crc32s.size()));
    std::for_each(crc32s.begin(),
                  crc32s.end(),
                  [&lines, &scopeinfo](const nemesis::animsetdata::CRC32* crc32)
                  {
                      auto temp_lines = crc32->GetCompiledLines(scopeinfo);
                      lines.insert(lines.end(), temp_lines.begin(), temp_lines.end());
                  });

    return lines;
}
