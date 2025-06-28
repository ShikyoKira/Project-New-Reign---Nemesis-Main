#include <mutex>

#include "Core/Statement/RunningNumberStatement.h"

#include "Utilities/Algorithm.h"

nemesis::RunningNumberStatement::RunningNumberStatement(const std::string& expression,
                                                        size_t linenum,
                                                        const std::filesystem::path& filepath,
                                                        const nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    if (Components.size() != 2)
    {
        ThrowSyntaxError("RunningNumber only accepts 1 argument");
    }

    auto key = Components.back();

    if (!IsComplexComponent(key)) return;

    DynamicComponents.emplace_back(key, linenum, filepath, manager);
}

std::string nemesis::RunningNumberStatement::GetValue(nemesis::CompileState& state) const
{
    std::string key;

    if (!DynamicComponents.empty())
    {
        key = DynamicComponents.back().GetValue(state);
    }
    else
    {
        key = Components.back();
    }

    const size_t fixed_start_num = 500000;
    static UMap<std::string, size_t> RunningNumberMap;
    static std::mutex ReadingMutex;
    std::scoped_lock lock(ReadingMutex);

    auto itr = RunningNumberMap.find(key);

    if (itr != RunningNumberMap.end()) return std::to_string(itr->second);

    return std::to_string(RunningNumberMap[key] = fixed_start_num + RunningNumberMap.size());
}
