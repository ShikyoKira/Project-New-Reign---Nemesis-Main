#include <regex>

#include "Core/BreakObject.h"
#include "Core/CompileState.h"
#include "Core/SemanticManager.h"

#include "Core/Statement/ForEachStatement.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"
#include "Utilities/ConditionSyntax.h"
#include "Utilities/Line.h"
#include "Utilities/OnScopeEnds.h"

void nemesis::ForEachStatement::Parse1Component(nemesis::SemanticManager& manager)
{
    auto& key = Components.front();

    if (key == "@MotionData")
    {
        Type            = nemesis::ForEachStatement::MOTION_DATA;
        ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
        {
            auto request            = GetBaseRequest(state);
            auto list               = request->GetMotionDataList();
            auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

            for (size_t i = 0; i < list.size(); i++)
            {
                auto& each = list[i];
                state.QueueCurrentMotionData(each.ToString(), i);
                nemesis::OnScopeEnds on_scope_end(
                    [this, &state]()
                    {
                        state.DequeCurrentMotionData();
                        state.RemoveConditionCacheContaining(Expression + "[]");
                    });

                try
                {
                    action();
                }
                catch (const nemesis::BreakObject::BreakException& bex)
                {
                    if (bex.GetExpression() == Expression) break;

                    throw;
                }
            }

            for (auto& cache_info : removed_cache_list)
            {
                state.CacheConditionResult(cache_info.first, cache_info.second);
            }
        };
        return;
    }

    if (key == "@RotationData")
    {
        Type            = nemesis::ForEachStatement::ROTATION_DATA;
        ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
        {
            auto request            = GetBaseRequest(state);
            auto list               = request->GetRotationDataList();
            auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

            for (size_t i = 0; i < list.size(); i++)
            {
                auto& each = list[i];
                state.QueueCurrentRotationData(each.ToString(), i);
                nemesis::OnScopeEnds on_scope_end(
                    [this, &state]()
                    {
                        state.DequeCurrentRotationData();
                        state.RemoveConditionCacheContaining(Expression + "[]");
                    });

                try
                {
                    action();
                }
                catch (const nemesis::BreakObject::BreakException& bex)
                {
                    if (bex.GetExpression() == Expression) break;

                    throw;
                }
            }

            for (auto& cache_info : removed_cache_list)
            {
                state.CacheConditionResult(cache_info.first, cache_info.second);
            }
        };
        return;
    }

    std::smatch match;
    const std::string& templt_name = manager.GetCurrentTemplateClass()->GetName();

    if (!std::regex_match(Expression, match, std::regex("^" + templt_name + "_([0-9]+)$")))
    {
        Type            = nemesis::ForEachStatement::OPTION;
        Key             = &key;
        ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
        {
            auto request  = GetBaseRequest(state);
            auto& name    = *Key;
            auto& options = request->GetOptions(name);

            auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

            for (auto& option : options)
            {
                state.QueueOption(name, option);
                nemesis::OnScopeEnds on_scope_end(
                    [this, &state, name]()
                    {
                        state.DequeueOption(name);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                    });

                try
                {
                    action();
                }
                catch (const nemesis::BreakObject::BreakException& bex)
                {
                    if (bex.GetExpression() == Expression) break;

                    throw;
                }
            }

            for (auto& cache_info : removed_cache_list)
            {
                state.CacheConditionResult(cache_info.first, cache_info.second);
            }
        };
        return;
    }

    auto num = std::stoul(match.str(1));
    Type     = nemesis::ForEachStatement::REQUEST;

    switch (num)
    {
        case 0:
        {
            std::string group = templt_name + "_1";
            ForEachFunction
                = [this, &templt_name, group](nemesis::CompileState& state, std::function<void()> action)
            {
                auto& requests = state.GetRequests(templt_name);

                if (requests.empty()) return;

                Vec<const nemesis::AnimationRequest*> anim_requests;

                for (auto& request : requests)
                {
                    anim_requests.emplace_back(request);
                }

                state.QueueChildRequestList(group, anim_requests);
                nemesis::OnScopeEnds on_scope_end([group, &state]() { state.DequeChildRequestList(group); });

                try
                {
                    action();
                }
                catch (const nemesis::BreakObject::BreakException& bex)
                {
                    if (bex.GetExpression() == Expression) return;

                    throw;
                }
            };
            return;
        }
        case 1:
        {
            ForEachFunction = [&templt_name, this](nemesis::CompileState& state, std::function<void()> action)
            {
                auto& requests          = state.GetRequests(templt_name);
                auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                for (auto& req : requests)
                {
                    state.QueueCurrentRequest(Expression, req);
                    nemesis::OnScopeEnds on_scope_end(
                        [this, &state]()
                        {
                            state.DequeCurrentRequest(Expression);
                            state.RemoveConditionCacheContaining(Expression + "[]");
                        });

                    try
                    {
                        action();
                    }
                    catch (const nemesis::BreakObject::BreakException& bex)
                    {
                        if (bex.GetExpression() == Expression) break;

                        throw;
                    }
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

    std::smatch fmatch;
    std::string path_str = nemesis::to_utf8_string(FilePath);

    if (std::regex_match(path_str, fmatch, std::regex("^" + templt_name + "_([1-9]+[0-9]*)\\.[^\\.]+$")))
    {
        auto fnum = std::stoul(fmatch.str(1));

        if (num - 1 > fnum)
        {
            throw std::runtime_error(
                "Value Inaccessible: Template can only access to current request, parent "
                "requests and immediate child request. It "
                "cannot access to anything beyond the child requests (Expression: "
                + Expression + ", Line: " + std::to_string(LineNum)
                + ", File: " + nemesis::to_utf8_string(FilePath) + ")");
        }

        if (num - 1 == fnum)
        {
            ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
            {
                auto* request           = GetBaseRequest(state);
                auto requests           = request->GetRequests();
                auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

                for (auto& req : requests)
                {
                    auto child_requests = req->GetRequests();
                    state.QueueCurrentRequest(Expression, req);
                    nemesis::OnScopeEnds on_scope_end(
                        [this, &state]()
                        {
                            state.DequeCurrentRequest(Expression);
                            state.RemoveConditionCacheContaining(Expression + "[]");
                        });

                    try
                    {
                        action();
                    }
                    catch (const nemesis::BreakObject::BreakException& bex)
                    {
                        if (bex.GetExpression() == Expression) break;

                        throw;
                    }
                }

                for (auto& cache_info : removed_cache_list)
                {
                    state.CacheConditionResult(cache_info.first, cache_info.second);
                }
            };
            return;
        }
    }

    std::string target_request = templt_name + "_" + std::to_string(num - 1);
    ForEachFunction = [this, target_request](nemesis::CompileState& state, std::function<void()> action)
    {
        auto* request           = state.GetCurrentRequest(target_request);
        auto requests           = request->GetRequests();
        auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

        for (auto& req : requests)
        {
            state.QueueCurrentRequest(Expression, req);
            nemesis::OnScopeEnds on_scope_end(
                [this, &state]()
                {
                    state.DequeCurrentRequest(Expression);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                });

            try
            {
                action();
            }
            catch (const nemesis::BreakObject::BreakException& bex)
            {
                if (bex.GetExpression() == Expression) break;

                throw;
            }
        }

        for (auto& cache_info : removed_cache_list)
        {
            state.CacheConditionResult(cache_info.first, cache_info.second);
        }
    };
}

void nemesis::ForEachStatement::Parse2Components(nemesis::SemanticManager& manager)
{
    if (Components.front() != "@Map") return;

    auto& key = Components.back();
    Type      = nemesis::ForEachStatement::MAP;
    Key       = &key;
    UPtr<std::function<std::string(nemesis::CompileState&)>> get_key;

    if (IsComplexComponent(key))
    {
        const auto& dynamic_key = DynamicComponents.emplace_back(key, LineNum, FilePath, manager);
        get_key                 = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&dynamic_key](nemesis::CompileState& state) { return dynamic_key.GetValue(state); });
    }
    else
    {
        get_key = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&key](nemesis::CompileState& state) { return key; });
    }

    ForEachFunction
        = [this, get_key = std::move(get_key)](nemesis::CompileState& state, std::function<void()> action)
    {
        std::string key         = (*get_key)(state);
        auto request            = GetBaseRequest(state);
        auto list               = request->GetMapValueList(key);
        auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

        for (size_t i = 0; i < list.size(); i++)
        {
            auto& each = list[i];
            state.QueueCurrentMapValue(key, *each, i);
            nemesis::OnScopeEnds on_scope_end(
                [this, &state, key]()
                {
                    state.DequeCurrentMapValue(key);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                });

            try
            {
                action();
            }
            catch (const nemesis::BreakObject::BreakException& bex)
            {
                if (bex.GetExpression() == Expression) break;

                throw;
            }
        }

        for (auto& cache_info : removed_cache_list)
        {
            state.CacheConditionResult(cache_info.first, cache_info.second);
        }
    };
}

void nemesis::ForEachStatement::Parse3Components(nemesis::SemanticManager& manager)
{
    auto templt_class = manager.GetCurrentTemplateClass();
    auto& templt_name = templt_class->GetName();

    if (!std::regex_match(Components.front(), std::regex("^" + templt_name + "_([1-9]+)$")))
    {
        ThrowSyntaxError("Unsupported expression for FOREACH statement");
    }

    auto get_request = GetTargetRequest(*templt_class, manager);
    auto& option     = Components.back();

    if (option == "@MotionData")
    {
        Type            = nemesis::ForEachStatement::MOTION_DATA;
        ForEachFunction = [this, get_request](nemesis::CompileState& state, std::function<void()> action)
        {
            auto request            = (*get_request)(state);
            auto list               = request->GetMotionDataList();
            auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

            for (size_t i = 0; i < list.size(); i++)
            {
                auto& each = list[i];
                state.QueueCurrentRequestMotionData(request, each.ToString(), i);
                nemesis::OnScopeEnds on_scope_end(
                    [this, &state, &request]()
                    {
                        state.DequeCurrentRequestMotionData(request);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                    });

                try
                {
                    action();
                }
                catch (const nemesis::BreakObject::BreakException& bex)
                {
                    if (bex.GetExpression() == Expression) break;

                    throw;
                }
            }

            for (auto& cache_info : removed_cache_list)
            {
                state.CacheConditionResult(cache_info.first, cache_info.second);
            }
        };
        return;
    }

    if (option == "@RotationData")
    {
        Type            = nemesis::ForEachStatement::ROTATION_DATA;
        ForEachFunction = [this, get_request](nemesis::CompileState& state, std::function<void()> action)
        {
            auto request            = (*get_request)(state);
            auto list               = request->GetRotationDataList();
            auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

            for (size_t i = 0; i < list.size(); i++)
            {
                auto& each = list[i];
                state.QueueCurrentRequestRotationData(request, each.ToString(), i);
                nemesis::OnScopeEnds on_scope_end(
                    [this, &state, &request]()
                    {
                        state.DequeCurrentRequestRotationData(request);
                        state.RemoveConditionCacheContaining(Expression + "[]");
                    });

                try
                {
                    action();
                }
                catch (const nemesis::BreakObject::BreakException& bex)
                {
                    if (bex.GetExpression() == Expression) break;

                    throw;
                }
            }

            for (auto& cache_info : removed_cache_list)
            {
                state.CacheConditionResult(cache_info.first, cache_info.second);
            }
        };
        return;
    }

    Type = nemesis::ForEachStatement::OPTION;
    Key  = &option;
    UPtr<std::function<std::string(nemesis::CompileState&)>> get_option;

    if (IsComplexComponent(option))
    {
        const auto& dynamic_option = DynamicComponents.emplace_back(option, LineNum, FilePath, manager);
        get_option                 = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [this, &dynamic_option, templt_class](nemesis::CompileState& state)
            {
                auto option = dynamic_option.GetValue(state);
                auto* model = templt_class->GetModel(option);

                if (!model)
                {
                    ThrowSyntaxError("Unsupported option name (" + option + ")");
                }

                return option;
            });
    }
    else
    {
        auto* model = templt_class->GetModel(option);

        if (!model)
        {
            ThrowSyntaxError("Unsupported option name (" + option + ")");
        }

        get_option = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&option](nemesis::CompileState& state) { return option; });
    }

    ForEachFunction = [this, get_request, get_option = std::move(get_option)](nemesis::CompileState& state,
                                                                              std::function<void()> action)
    {
        auto request  = (*get_request)(state);
        auto name     = (*get_option)(state);
        auto& options = request->GetOptions(name);

        auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

        for (auto& option : options)
        {
            state.QueueRequestOption(request, name, option);
            nemesis::OnScopeEnds on_scope_end(
                [this, &state, request, name]()
                {
                    state.DequeueRequestOption(request, name);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                });

            try
            {
                action();
            }
            catch (const nemesis::BreakObject::BreakException& bex)
            {
                if (bex.GetExpression() == Expression) break;

                throw;
            }
        }

        for (auto& cache_info : removed_cache_list)
        {
            state.CacheConditionResult(cache_info.first, cache_info.second);
        }
    };
}

bool nemesis::ForEachStatement::Parse4Components(nemesis::SemanticManager& manager)
{
    if (Components[2] != "@Map") return false;

    auto& key = Components.back();
    Type      = nemesis::ForEachStatement::MAP;
    Key       = &key;
    UPtr<std::function<std::string(nemesis::CompileState&)>> get_key;

    if (IsComplexComponent(key))
    {
        auto sptr_manager          = std::make_shared<nemesis::SemanticManager>(manager);
        const auto& dynamic_option = DynamicComponents.emplace_back(key, LineNum, FilePath, manager);
        get_key                    = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [this, &dynamic_option, sptr_manager](nemesis::CompileState& state)
            {
                std::string key = dynamic_option.GetValue(state);

                if (!sptr_manager->HasMapInQueue(key))
                {
                    ThrowInaccessibleError("Unable to get target map value from queue");
                }

                return key;
            });
    }
    else
    {
        if (!manager.HasMapInQueue(key))
        {
            ThrowInaccessibleError("Unable to get target map value from queue");
        }

        get_key = std::make_unique<std::function<std::string(nemesis::CompileState&)>>(
            [&key](nemesis::CompileState& state) { return key; });
    }

    auto templt_class = manager.GetCurrentTemplateClass();
    auto get_request  = GetTargetRequest(*templt_class, manager);

    ForEachFunction = [this, get_request, get_key = std::move(get_key)](nemesis::CompileState& state,
                                                                        std::function<void()> action)
    {
        auto request = (*get_request)(state);
        auto key     = (*get_key)(state);
        auto list    = request->GetMapValueList(key);

        auto removed_cache_list = state.RemoveConditionCacheContaining(Expression + "[]");

        for (size_t i = 0; i < list.size(); i++)
        {
            auto& each = list[i];
            state.QueueCurrentRequestMapValue(request, key, *each, i);
            nemesis::OnScopeEnds on_scope_end(
                [this, &state, request, key]()
                {
                    state.DequeCurrentRequestMapValue(request, key);
                    state.RemoveConditionCacheContaining(Expression + "[]");
                });

            try
            {
                action();
            }
            catch (const nemesis::BreakObject::BreakException& bex)
            {
                if (bex.GetExpression() == Expression) break;

                throw;
            }
        }

        for (auto& cache_info : removed_cache_list)
        {
            state.CacheConditionResult(cache_info.first, cache_info.second);
        }
    };
    return true;
}

void nemesis::ForEachStatement::ParseComponents(nemesis::SemanticManager& manager)
{
    if (is_only_number(Expression))
    {
        LoopCycle       = std::stoi(Expression);
        ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
        {
            try
            {
                for (int i = 0; i < LoopCycle; ++i)
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

    switch (Components.size())
    {
        case 1:
        {
            Parse1Component(manager);
            return;
        }
        case 2:
        {
            Parse2Components(manager);
            return;
        }
        case 3:
        {
            Parse3Components(manager);
            return;
        }
        case 4:
        {
            if (Parse4Components(manager)) return;

            break;
        }
        default:
            break;
    }

    ThrowSyntaxError("Invalid ForEach Statement expression");
}

nemesis::ForEachStatement::ForEachStatement(const std::string& expression,
                                            size_t linenum,
                                            const std::filesystem::path& filepath,
                                            nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(expression, linenum, filepath)
{
    ParseComponents(manager);
}

nemesis::ForEachStatement::ForEachStatement(const nemesis::Line& line, nemesis::SemanticManager& manager)
    : nemesis::CompositeStatement(line, line.GetLineNumber(), line.GetFilePath())
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
    if (Type != nemesis::ForEachStatement::OPTION)
    {
        option_name.clear();
        return false;
    }

    option_name = *Key;
    return true;
}

bool nemesis::ForEachStatement::TryGetMapKey(std::string& map_key) const noexcept
{
    if (Type != nemesis::ForEachStatement::MAP)
    {
        map_key.clear();
        return false;
    }

    map_key = *Key;
    return true;
}

nemesis::ForEachStatement::ForEachType nemesis::ForEachStatement::GetType() const noexcept
{
    return Type;
}
