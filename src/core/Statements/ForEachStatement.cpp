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
            for (int i = 0; i < LoopCycle; i++)
            {
                action();
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

            if (nemesis::regex_match(Expression, match, "^" + template_name + "_([0-9]+)$"))
            {
                auto num = std::stoul(match.str(1));

                switch (num)
                {
                    case 0:
                    {
                        ForEachFunction
                            = [&template_name](nemesis::CompileState& state, std::function<void()> action)
                        {
                            auto& requests = state.GetRequests(template_name);

                            if (requests.empty()) return;

                            std::string group = template_name + "_1";
                            Vec<const nemesis::AnimationRequest*> anim_requests;

                            for (auto& request: requests)
                            {
                                anim_requests.emplace_back(request.get());
                            }

                            state.QueueChildRequestList(group, anim_requests);
                            action();
                            state.DequeChildRequestList(group);

                        };
                        return;
                    }
                    case 1:
                    {
                        ForEachFunction =
                            [&template_name, this](nemesis::CompileState& state, std::function<void()> action)
                        {
                            auto& requests = state.GetRequests(template_name);

                            for (auto& req : requests)
                            {
                                state.QueueCurrentRequest(Expression, req.get());
                                action();
                                state.DequeCurrentRequest(Expression);
                            }
                        };
                        return;
                    }
                    default:
                        break;
                }

                nemesis::smatch fmatch;

                if (nemesis::regex_match(
                        FilePath.string(), fmatch, "^" + template_name + "_([1-9]+)\\.[^\\.]+$"))
                {
                    auto fnum = std::stoul(fmatch.str(1));

                    if (num - 1 > fnum)
                    {
                        throw std::runtime_error("Template can only access to current request, parent "
                                                 "requests and immediate child request. It "
                                                 "cannot access to anything beyond the child requests");
                    }

                    if (num - 1 == fnum)
                    {
                        ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
                        {
                            auto* request = state.GetBaseRequest();
                            auto requests = request->GetRequests();

                            for (auto& req : requests)
                            {
                                state.QueueCurrentRequest(Expression, req);
                                action();
                                state.DequeCurrentRequest(Expression);
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

                    for (auto& req : requests)
                    {
                        state.QueueCurrentRequest(Expression, req);
                        action();
                        state.DequeCurrentRequest(Expression);
                    }
                };
                return;
            }

            OptionName = &Components.front();
            ForEachFunction = [this](nemesis::CompileState& state, std::function<void()> action)
            {
                auto request  = state.GetBaseRequest();
                auto& name    = *OptionName;
                auto& options = request->GetOptions(name);

                for (auto& option : options)
                {
                    state.QueueOption(name, option);
                    action();
                    state.DequeueOption(name);
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

                for (auto& each : list)
                {
                    state.QueueCurrentMapValue(key, *each);
                    action();
                    state.DequeCurrentMapValue(key);
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

                for (auto& option : options)
                {
                    state.QueueRequestOption(request, name, option);
                    action();
                    state.DequeueRequestOption(request, name);
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

                for (auto& each : list)
                {
                    state.QueueCurrentRequestMapValue(request, key, *each);
                    action();
                    state.DequeCurrentRequestMapValue(request, key);
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
