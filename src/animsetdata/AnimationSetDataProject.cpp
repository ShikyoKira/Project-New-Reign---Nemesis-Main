#include "animsetdata/AnimationSetDataProject.h"

#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/CompileState.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

bool nemesis::AnimationSetDataProject::IsProjectEnd(nemesis::LineStream& stream, bool& start)
{
    auto* token_ptr = &stream.GetToken();

    while (token_ptr->Type != nemesis::LineStream::NONE)
    {
        ++stream;
        token_ptr = &stream.GetToken();
    }

    return token_ptr->Value == "V3";
}

Vec<UPtr<nemesis::NObject>>
nemesis::AnimationSetDataProject::ParseModObjects(nemesis::LineStream& stream,
                                                  nemesis::SemanticManager& manager,
                                                  std::function<void(nemesis::NLine*)> add_nline_event)
{
    Vec<UPtr<nemesis::NObject>> object_list;

    if (stream.IsEoF()) return object_list;

    auto* mod_token = &stream.GetToken();
    auto& mod_value = mod_token->Value;

    if (mod_token->Type != nemesis::LineStream::MOD_OPEN)
    {
        throw std::runtime_error("Syntax Error: Unexpected syntax. Expecting MOD_CODE syntax (Line: "
                                 + std::to_string(mod_value.GetLineNumber())
                                 + ". File: " + mod_value.GetFilePath().string() + ")");
    }

    Deq<const nemesis::Line*> mod_lines;

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::MOD_CLOSE:
            {
                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto col_ptr    = collection.get();
                auto mod_object = std::make_unique<nemesis::ModObject>(mod_value,
                                                                       mod_value.GetLineNumber(),
                                                                       mod_value.GetFilePath(),
                                                                       manager,
                                                                       std::move(collection));

                for (auto& mod_line : mod_lines)
                {
                    auto uptr = std::make_unique<nemesis::NLine>(*mod_line, manager);
                    add_nline_event(uptr.get());
                    col_ptr->AddObject(std::move(uptr));
                }

                object_list.emplace_back(std::move(mod_object));
                return object_list;
            }
            case nemesis::LineStream::MOD_ORG:
            {
                auto& value = token.Value;
                throw std::runtime_error("Invalid token type. ORIGINAL syntax is not supported in nemesis::AnimationSetDataProject::ParseModObjects (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ". File: " + value.GetFilePath().string() + ")");
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                mod_lines.emplace_back(&value);
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ". File: " + value.GetFilePath().string() + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed MOD_CODE statement (Line: "
                             + std::to_string(mod_value.GetLineNumber())
                             + ", File: " + mod_value.GetFilePath().string() + ")");
}

UPtr<nemesis::ForEachObject>
nemesis::AnimationSetDataProject::ParseForEachObjects(nemesis::LineStream& stream,
                                                      nemesis::SemanticManager& manager,
                                                      std::function<void(nemesis::NLine*)> add_nline_event)
{
    if (stream.IsEoF()) return nullptr;

    auto* fe_token = &stream.GetToken();
    auto& fe_value = fe_token->Value;

    if (fe_token->Type != nemesis::LineStream::FOR_EACH)
    {
        throw std::runtime_error("Syntax Error: Unexpected syntax. Expecting FOREACH syntax (Line: "
                                 + std::to_string(fe_value.GetLineNumber())
                                 + ". File: " + fe_value.GetFilePath().string() + ")");
    }

    auto collection  = std::make_unique<nemesis::CollectionObject>();
    auto* col_ptr    = collection.get();
    auto foreach_obj = std::make_unique<nemesis::ForEachObject>(
        fe_value, fe_value.GetLineNumber(), fe_value.GetFilePath(), manager, std::move(collection));

    auto scope = foreach_obj->BuildScope(manager);

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::CLOSE:
            {
                return foreach_obj;
            }
            case nemesis::LineStream::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager, add_nline_event);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    col_ptr->AddObject(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = ParseIfObjects(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = ParseForEachObjects(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                auto uptr   = std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager);
                add_nline_event(uptr.get());
                col_ptr->AddObject(std::move(uptr));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ". File: " + value.GetFilePath().string() + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed FOREACH statement (Line: "
                             + std::to_string(fe_value.GetLineNumber())
                             + ", File: " + fe_value.GetFilePath().string() + ")");
}

UPtr<nemesis::IfObject>
nemesis::AnimationSetDataProject::ParseIfObjects(nemesis::LineStream& stream,
                                                 nemesis::SemanticManager& manager,
                                                 std::function<void(nemesis::NLine*)> add_nline_event)
{
    if (stream.IsEoF()) return nullptr;

    auto* if_token = &stream.GetToken();
    auto& if_value = if_token->Value;

    if (if_token->Type != nemesis::LineStream::IF)
    {
        throw std::runtime_error("Syntax Error: Unexpected syntax. Expecting IF syntax (Line: "
                                 + std::to_string(if_value.GetLineNumber())
                                 + ". File: " + if_value.GetFilePath().string() + ")");
    }

    bool has_else   = false;
    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto* col_ptr   = collection.get();
    auto if_obj     = std::make_unique<nemesis::IfObject>(
        if_value, if_value.GetLineNumber(), if_value.GetFilePath(), manager, std::move(collection));

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::END_IF:
            {
                return if_obj;
            }
            case nemesis::LineStream::ELSE_IF:
            {
                auto& value = token.Value;

                if (has_else)
                {
                    throw std::runtime_error("Syntax error: ELSEIF syntax cannot come after ELSE (Line: "
                                             + std::to_string(value.GetLineNumber())
                                             + ", File: " + value.GetFilePath().string() + ")");
                }

                collection = std::make_unique<nemesis::CollectionObject>();
                col_ptr    = collection.get();
                if_obj->ElseIf(
                    value, value.GetLineNumber(), value.GetFilePath(), manager, std::move(collection));
                break;
            }
            case nemesis::LineStream::ELSE:
            {
                if (has_else)
                {
                    auto& value = token.Value;
                    throw std::runtime_error("Syntax error: ELSE syntax cannot come after ELSE (Line: "
                                             + std::to_string(value.GetLineNumber())
                                             + ", File: " + value.GetFilePath().string() + ")");
                }

                has_else   = true;
                collection = std::make_unique<nemesis::CollectionObject>();
                col_ptr    = collection.get();
                if_obj->Else(std::move(collection));
                break;
            }
            case nemesis::LineStream::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager, add_nline_event);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    col_ptr->AddObject(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = ParseIfObjects(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = ParseForEachObjects(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                auto uptr   = std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager);
                add_nline_event(uptr.get());
                col_ptr->AddObject(std::move(uptr));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ". File: " + value.GetFilePath().string() + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed IF statement (Line: "
                             + std::to_string(if_value.GetLineNumber())
                             + ", File: " + if_value.GetFilePath().string() + ")");
}

nemesis::AnimationSetDataProject::AnimationSetDataProject(const std::string& name) noexcept
    : Name(name)
{
}

void nemesis::AnimationSetDataProject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr state_contents;
    size_t count          = 0;
    auto& counter_element = lines.emplace_back("");

    for (auto& header : Headers)
    {
        DeqNstr templines = StateMap.at(header)->Compile(state);

        if (templines.empty()) continue;

        ++count;
        lines.emplace_back(header);
        state_contents.insert(state_contents.end(),
                              std::make_move_iterator(templines.begin()),
                              std::make_move_iterator(templines.end()));
    }

    counter_element = std::to_string(count);
    lines.insert(lines.end(),
                 std::make_move_iterator(state_contents.begin()),
                 std::make_move_iterator(state_contents.end()));
}

void nemesis::AnimationSetDataProject::SerializeTo(DeqNstr& lines) const
{
    throw std::runtime_error("nemesis::AnimationSetDataProject::SerializeTo is not supported");
}

UPtr<nemesis::NObject> nemesis::AnimationSetDataProject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationSetDataProject> nemesis::AnimationSetDataProject::Clone() const
{
    auto project     = std::make_unique<nemesis::AnimationSetDataProject>(Name);
    project->Headers = Headers;
    auto& map        = project->StateMap;

    for (auto& it : StateMap)
    {
        map[it.first] = std::move(it.second->Clone());
    }

    return project;
}

UPtr<nemesis::AnimationSetDataState>&
nemesis::AnimationSetDataProject::AddState(UPtr<nemesis::AnimationSetDataState>&& state)
{
    std::scoped_lock<std::mutex> lock(UpdaterMutex);
    auto& name = state->GetName();
    Headers.emplace_back(name);
    StateMap[name] = std::move(state);
    return StateMap[name];
}

const std::string& nemesis::AnimationSetDataProject::GetName() const noexcept
{
    return Name;
}

nemesis::AnimationSetDataState* nemesis::AnimationSetDataProject::GetState(const std::string& name)
{
    auto itr = StateMap.find(name);

    if (itr == StateMap.end()) return nullptr;

    return itr->second.get();
}

const nemesis::AnimationSetDataState*
nemesis::AnimationSetDataProject::GetState(const std::string& name) const
{
    auto itr = StateMap.find(name);

    if (itr == StateMap.end()) return nullptr;

    return itr->second.get();
}

void nemesis::AnimationSetDataProject::SerializeToDirectory(const std::filesystem::path& directory_path) const
{
    std::filesystem::create_directories(directory_path);
    
    for (auto& state : StateMap)
    {
        std::filesystem::path filepath = state.second->GetName() + ".txt";
        state.second->SerializeToFile(directory_path / filepath);
    }
}

UPtr<nemesis::AnimationSetDataProject>
nemesis::AnimationSetDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path)
{
    return DeserializeFromDirectory(directory_path, directory_path.stem().string());
}

UPtr<nemesis::AnimationSetDataProject>
nemesis::AnimationSetDataProject::DeserializeFromDirectory(const std::filesystem::path& directory_path,
                                                           const std::string project_name)
{
    auto project = std::make_unique<nemesis::AnimationSetDataProject>(project_name);

    for (auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (!entry.is_regular_file()) continue;

        if (entry.path().extension() != ".txt") continue;

        auto state = nemesis::AnimationSetDataState::DeserializeFromFile(entry.path());
        project->AddState(std::move(state));
    }

    return project;
}

VecNstr nemesis::AnimationSetDataProject::ParseHeaders(nemesis::LineStream& stream,
                                                      nemesis::SemanticManager& manager)
{
    auto token_ptr = &stream.GetToken();

    if (!isOnlyNumber(token_ptr->Value))
    {
        auto& token_value = stream.GetToken().Value;
        throw std::runtime_error("nemesis::AnimationSetDataProject::ParseObjects format error (Line: "
                                 + std::to_string(token_value.GetLineNumber())
                                 + ". File: " + token_value.GetFilePath().string() + ")");
    }

    VecNstr headers;

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto* token_ptr = &stream.GetToken();

        if (token_ptr->Type != nemesis::LineStream::NONE)
        {
            throw std::runtime_error("nemesis::AnimationSetDataProject::ParseHeaders does not support any "
                                     "kind of syntax for header");
        }

        if (token_ptr->Value == "V3") return headers;

        headers.emplace_back(token_ptr->Value);
    }

    return headers;
}

Vec<UPtr<nemesis::AnimationSetDataProject>> nemesis::AnimationSetDataProject::ParseObjects(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, const VecNstr& project_names)
{
    Vec<UPtr<nemesis::AnimationSetDataProject>> project_list;
    auto name_itr = project_names.begin();

    for (; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        if (token.Type != nemesis::LineStream::NONE)
        {
            auto& token_value = stream.GetToken().Value;
            throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ". File: " + token_value.GetFilePath().string() + ")");
        }

        if (name_itr == project_names.end())
        {
            throw std::runtime_error(
                "nemesis::AnimationSetDataProject::ParseObjects parsing error. Project name count "
                "and project body count do not match");
        }

        auto project     = std::make_unique<nemesis::AnimationSetDataProject>(*name_itr++);
        project->Headers = ParseHeaders(stream, manager);
        auto list
            = nemesis::AnimationSetDataState::ParseObjects(stream, manager, project->Name, project->Headers);

        if (project->Headers.size() != list.size())
        {
            throw std::runtime_error("nemesis::AnimationSetDataProject::ParseObjects parsing error ("
                                     + project->Name
                                     + "). State name count and state body count do not match");
        }

        for (size_t i = 0; i < list.size(); ++i)
        {
            project->StateMap[list[i]->GetName()] = std::move(list[i]);
        }

        project_list.emplace_back(std::move(project));
        --stream;
    }

    if (name_itr != project_names.end())
    {
        throw std::runtime_error(
            "nemesis::AnimationSetDataProject::ParseObjects parsing error. Project name count and project "
            "body count do not match");
    }

    return project_list;
}
