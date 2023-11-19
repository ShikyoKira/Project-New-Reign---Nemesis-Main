#include "core/BreakObject.h"
#include "core/CompileState.h"
#include "core/SemanticManager.h"

#include "core/Statements/ForEachStatement.h"

#include "core/Template/TemplateClass.h"

#include "utilities/line.h"
#include "utilities/regex.h"
#include "utilities/templatecategory.h"
#include "utilities/conditionsyntax.h"

void nemesis::ForEachStatement::ParseComponents(nemesis::SemanticManager& manager)
{
    if (isOnlyNumber(Expression))
    {
        LoopCycle       = std::stoi(Expression);
        ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
        {
            try
            {
                for (int i = 0; i < LoopCycle; i++)
                {
                    action();
                }
            }
            catch (const nemesis::BreakObject::BreakException& bex)
            {
                if (bex.GetExpression() != Expression) throw;
            }
        };
        return;
    }

    auto template_class = manager.GetCurrentTemplateClass();
    auto& template_name = template_class->GetName();

    switch (Components.size())
    {
        case 1:
        {
            nemesis::smatch match;

            if (!nemesis::regex_match(Expression, match, "^" + template_name + "_([0-9]+)$"))
            {
                OptionName      = &Components.front();
                ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
                {
                    auto request  = state.GetBaseRequest();
                    auto& name    = *OptionName;
                    auto& options = request->GetOptions(name);

                    auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                    for (auto& option : options)
                    {
                        state.QueueOption(name, option);

                        try
                        {
                            action();
                        }
                        catch (const nemesis::BreakObject::BreakException& bex)
                        {
                            if (bex.GetExpression() != Expression) throw;

                            state.DequeueOption(name);
                            state.RemoveConditionCacheContaining(Expression + "[]");
                            break;
                        }

                        state.DequeueOption(name);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                    }

                    for (auto& cache_info : removed_cache_list)
                    {
                        state.CacheConditionResult(cache_info.first, cache_info.second);
                    }
                };
                return;
            }

            auto num = std::stoul(match.str(1));

            switch (num)
            {
                case 0:
                {
                    std::string group = template_name + "_1";
                    ForEachFunction   = [this, &template_name, group](nemesis::CompileState& state,
                                                                    std::function<void()> action)
                    {
                        auto& requests = state.GetRequests(template_name);

                        if (requests.empty()) return;

                        Vec<const nemesis::AnimationRequest*> anim_requests;

                        for (auto& request : requests)
                        {
                            anim_requests.emplace_back(request.get());
                        }

                        state.QueueChildRequestList(group, anim_requests);

                        try
                        {
                            action();
                        }
                        catch (const nemesis::BreakObject::BreakException& bex)
                        {
                            if (bex.GetExpression() != Expression) throw;
                        }

                        state.DequeChildRequestList(group);
                    };
                    return;
                }
                case 1:
                {
                    ForEachFunction
                        = [&template_name, this](nemesis::CompileState& state, std::function<void()> action)
                    {
                        auto& requests = state.GetRequests(template_name);

                        auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                        for (auto& req : requests)
                        {
                            state.QueueCurrentRequest(Expression, req.get());

                            try
                            {
                                action();
                            }
                            catch (const nemesis::BreakObject::BreakException& bex)
                            {
                                if (bex.GetExpression() != Expression) throw;

                                state.DequeCurrentRequest(Expression);
                                state.RemoveConditionCacheContaining(Expression + "[]");
                                break;
                            }

                            state.DequeCurrentRequest(Expression);
                            state.RemoveConditionCacheContaining(Expression + "[]");
                        }

                        for (auto& cache_info : removed_cache_list)
                        {
                            state.CacheConditionResult(cache_info.first, cache_info.second);
                        }
                    };
                    return;
                }
                default:
                    break;
            }

            nemesis::smatch fmatch;

            if (nemesis::regex_match(FilePath.string(), fmatch, "^" + template_name + "_([1-9]+)\\.[^\\.]+$"))
            {
                auto fnum = std::stoul(fmatch.str(1));

                if (num - 1 > fnum)
                {
                    throw std::runtime_error(
                        "Template can only access to current request, parent "
                        "requests and immediate child request. It "
                        "cannot access to anything beyond the child requests (Expression: "
                        + Expression + ", Line: " + std::to_string(LineNum) + ", File: " + FilePath.string()
                        + ")");
                }

                if (num - 1 == fnum)
                {
                    ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
                    {
                        auto* request = state.GetBaseRequest();
                        auto requests = request->GetRequests();

                        auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                        for (auto& req : requests)
                        {
                            auto child_requests = req->GetRequests();

                            state.QueueCurrentRequest(Expression, req);

                            try
                            {
                                action();
                            }
                            catch (const nemesis::BreakObject::BreakException& bex)
                            {
                                if (bex.GetExpression() != Expression) throw;

                                state.DequeCurrentRequest(Expression);
                                state.RemoveConditionCacheContaining(Expression + "[]");
                                break;
                            }

                            state.DequeCurrentRequest(Expression);
                            state.RemoveConditionCacheContaining(Expression + "[]");
                        }

                        for (auto& cache_info : removed_cache_list)
                        {
                            state.CacheConditionResult(cache_info.first, cache_info.second);
                        }
                    };
                    return;
                }
            }

            std::string target_request = template_name + "_" + std::to_string(num - 1);
            ForEachFunction
                = [target_request, this](nemesis::CompileState& state, std::function<void()> action)
            {
                auto* request = state.GetCurrentRequest(target_request);
                auto requests = request->GetRequests();

                auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                for (auto& req : requests)
                {
                    state.QueueCurrentRequest(Expression, req);

                    try
                    {
                        action();
                    }
                    catch (const nemesis::BreakObject::BreakException& bex)
                    {
                        if (bex.GetExpression() != Expression) throw;

                        state.DequeCurrentRequest(Expression);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                        break;
                    }

                    state.DequeCurrentRequest(Expression);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                }

                for (auto& cache_info : removed_cache_list)
                {
                    state.CacheConditionResult(cache_info.first, cache_info.second);
                }
            };
            return;
        }
        case 2:
        {
            if (Components.front() != "@Map") break;

            const std::string& key = Components.back();
            ForEachFunction        = [this, key](nemesis::CompileState& state, std::function<void()> action)
            {
                auto request = state.GetBaseRequest();
                auto list    = request->GetMapValueList(key);

                auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                for (auto& each : list)
                {
                    state.QueueCurrentMapValue(key, *each);

                    try
                    {
                        action();
                    }
                    catch (const nemesis::BreakObject::BreakException& bex)
                    {
                        if (bex.GetExpression() != Expression) throw;

                        state.DequeCurrentMapValue(key);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                        break;
                    }

                    state.DequeCurrentMapValue(key);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                }

                for (auto& cache_info : removed_cache_list)
                {
                    state.CacheConditionResult(cache_info.first, cache_info.second);
                }
            };
            return;
        }
        case 3:
        {
            if (!nemesis::regex_match(Components.front(), "^" + template_name + "_([1-9]+)$"))
            {
                throw std::runtime_error(
                    "Syntax error: Unsupported expression for FOREACH statement (Expression: " + Expression
                    + ", Line: " + std::to_string(LineNum) + ", FilePath: " + FilePath.string() + ")");
            }

            auto get_request = GetTargetRequest(*template_class, manager);

            OptionName = &Components[2];
            ForEachFunction = [this, get_request](nemesis::CompileState& state, std::function<void()> action)
            {
                auto request  = (*get_request)(state);
                auto& name    = *OptionName;
                auto& options = request->GetOptions(name);

                auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                for (auto& option : options)
                {
                    state.QueueRequestOption(request, name, option);

                    try
                    {
                        action();
                    }
                    catch (const nemesis::BreakObject::BreakException& bex)
                    {
                        if (bex.GetExpression() != Expression) throw;

                        state.DequeueRequestOption(request, name);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                        break;
                    }

                    state.DequeueRequestOption(request, name);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                }

                for (auto& cache_info : removed_cache_list)
                {
                    state.CacheConditionResult(cache_info.first, cache_info.second);
                }
            };
            return;
        }
        case 4:
        {
            if (Components[2] != "@Map") break;

            const std::string& key = Components.back();

            auto template_class = manager.GetCurrentTemplateClass();
            auto get_request    = GetTargetRequest(*template_class, manager);

            ForEachFunction
                = [this, get_request, key](nemesis::CompileState& state, std::function<void()> action)
            {
                auto request = (*get_request)(state);
                auto list    = request->GetMapValueList(key);

                auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                for (auto& each : list)
                {
                    state.QueueCurrentRequestMapValue(request, key, *each);

                    try
                    {
                        action();
                    }
                    catch (const nemesis::BreakObject::BreakException& bex)
                    {
                        if (bex.GetExpression() != Expression) throw;

                        state.DequeCurrentRequestMapValue(request, key);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                        break;
                    }

                    state.DequeCurrentRequestMapValue(request, key);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                }

                for (auto& cache_info : removed_cache_list)
                {
                    state.CacheConditionResult(cache_info.first, cache_info.second);
                }
            };
            return;
        }
        default:
            break;
    }

    throw std::runtime_error("Syntax error: Invalid ForEach Statement expression (Expression: " + Expression
                             + ", Line: " + std::to_string(LineNum) + ", File: " + FilePath.string() + ")");
}

nemesis::ForEachStatement::ForEachStatement(const std::string& expression,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            nemesis::SemanticManager& manager)
    : nemesis::Statement(expression, linenum, filepath)
{
    ParseComponents(manager);
}

nemesis::ForEachStatement::ForEachStatement(const nemesis::Line& line,
                                            nemesis::SemanticManager& manager)
    : nemesis::Statement(line)
{
    ParseComponents(manager);
}

std::string nemesis::ForEachStatement::Serialize() const
{
    return nemesis::syntax::ForEach(Expression);
}

void nemesis::ForEachStatement::ForEach(nemesis::CompileState& state, std::function<void()> action) const
{
    ForEachFunction(state, action);
}

bool nemesis::ForEachStatement::TryGetOptionName(std::string& option_name) const noexcept
{
    if (OptionName == nullptr)
    {
        option_name.clear();
        return false;
    }

    option_name = *OptionName;
    return true;
}
