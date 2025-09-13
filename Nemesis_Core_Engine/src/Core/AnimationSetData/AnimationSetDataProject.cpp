#include "Core/AnimationSetData/AnimationSetDataProject.h"

#include "Core/CoreObject.h"

#include "Utilities/Algorithm.h"
#include "Utilities/Sha256.h"

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
        throw nemesis::NObjectException("Syntax Error: Unexpected syntax. Expecting MOD_CODE syntax (Line: "
                                        + std::to_string(mod_value.GetLineNumber()) + ", File: "
                                        + nemesis::to_utf8_string(mod_value.GetFilePath()) + ")");
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
                throw nemesis::NObjectException("Syntax Error: Invalid token type. ORIGINAL syntax is not supported in "
                                                "nemesis::AnimationSetDataProject::ParseModObjects (Line: "
                                                + std::to_string(value.GetLineNumber()) + ", File: "
                                                + nemesis::to_utf8_string(value.GetFilePath()) + ")");
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
                throw nemesis::NObjectException(
                    "Syntax Error: Unsupport syntax (Line: " + std::to_string(value.GetLineNumber())
                    + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }

    throw nemesis::NObjectException("Syntax Error: Unclosed MOD_CODE statement (Line: "
                                    + std::to_string(mod_value.GetLineNumber())
                                    + ", File: " + nemesis::to_utf8_string(mod_value.GetFilePath()) + ")");
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
        throw nemesis::NObjectException("Syntax Error: Unexpected syntax. Expecting FOREACH syntax (Line: "
                                        + std::to_string(fe_value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(fe_value.GetFilePath()) + ")");
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
                throw nemesis::NObjectException(
                    "Syntax Error: Unsupport syntax (Line: " + std::to_string(value.GetLineNumber())
                    + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }

    throw nemesis::NObjectException("Syntax Error: Unclosed FOREACH statement (Line: "
                                    + std::to_string(fe_value.GetLineNumber())
                                    + ", File: " + nemesis::to_utf8_string(fe_value.GetFilePath()) + ")");
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
        throw nemesis::NObjectException("Syntax Error: Unexpected syntax. Expecting IF syntax (Line: "
                                        + std::to_string(if_value.GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(if_value.GetFilePath()) + ")");
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
                    throw nemesis::NObjectException(
                        "Syntax Error: ELSEIF syntax cannot come after ELSE (Line: "
                        + std::to_string(value.GetLineNumber())
                        + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
                }

                collection = std::make_unique<nemesis::CollectionObject>();
                col_ptr    = collection.get();
                if_obj->ElseIf(value.ToString(),
                               value.GetLineNumber(),
                               value.GetFilePath(),
                               manager,
                               std::move(collection));
                break;
            }
            case nemesis::LineStream::ELSE:
            {
                if (has_else)
                {
                    auto& value = token.Value;
                    throw nemesis::NObjectException("Syntax Error: ELSE syntax cannot come after ELSE (Line: "
                                                    + std::to_string(value.GetLineNumber()) + ", File: "
                                                    + nemesis::to_utf8_string(value.GetFilePath()) + ")");
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
                throw nemesis::NObjectException(
                    "Syntax Error: Unsupport syntax (Line: " + std::to_string(value.GetLineNumber())
                    + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }

    throw nemesis::NObjectException("Syntax Error: Unclosed IF statement (Line: "
                                    + std::to_string(if_value.GetLineNumber())
                                    + ", File: " + nemesis::to_utf8_string(if_value.GetFilePath()) + ")");
}

nemesis::AnimationSetDataProject::AnimationSetDataProject(const std::string& name) noexcept
    : Name(name)
{
}

std::string nemesis::AnimationSetDataProject::GetHash(nemesis::CompileState& state) const
{
    std::ostringstream oss("AnimationSetDataProject");
    SetStr hash_set;

    for (auto& header : Headers)
    {
        hash_set.insert(StateMap.at(header.ToString())->GetHash());
    }

    for (auto& header : NewHeaders)
    {
        hash_set.insert(StateMap.at(header)->GetHash());
    }

    for (auto& templt_obj : StateTemplateList)
    {
        auto& requests = state.GetRequests(templt_obj->GetClassName());

        if (requests.empty()) continue;

        hash_set.insert(requests.get_hash());
        hash_set.insert(templt_obj->GetHash());
    }

    for (auto& hash : hash_set)
    {
        oss << hash;
    }

    return nemesis::SHA256::hex(oss.str());
}

void nemesis::AnimationSetDataProject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr header_lines;
    DeqNstr state_contents;

    {
        std::scoped_lock lock(StateMutex, StateTemplateMutex);

        for (auto& header : Headers)
        {
            DeqNstr templines = StateMap.at(header.ToString())->Compile(state);

            if (templines.empty()) continue;

            header_lines.emplace_back(header);

            for (auto& line : templines)
            {
                state_contents.emplace_back(std::move(line));
            }
        }

        for (auto& header : NewHeaders)
        {
            DeqNstr templines = StateMap.at(header)->Compile(state);

            if (templines.empty()) continue;

            header_lines.emplace_back(header);

            for (auto& line : templines)
            {
                state_contents.emplace_back(std::move(line));
            }
        }

        for (auto& templt_obj : StateTemplateList)
        {
            auto& requests = state.GetRequests(templt_obj->GetClassName());

            if (requests.empty()) continue;

            for (auto& request : requests)
            {
                state.SetBaseRequest(request);
                DeqNstr templines = templt_obj->Compile(state);

                if (templines.empty()) continue;

                header_lines.emplace_back(nemesis::to_utf8_string(templt_obj->GetFilePath().stem()));

                for (auto& line : templines)
                {
                    state_contents.emplace_back(std::move(line));
                }
            }
        }

        state.SetBaseRequest(nullptr);
    }

    if (header_lines.empty()) return;

    lines.emplace_front(std::to_string(header_lines.size()));

    for (auto& line : header_lines)
    {
        lines.emplace_back(std::move(line));
    }

    for (auto& line : state_contents)
    {
        lines.emplace_back(std::move(line));
    }
}

void nemesis::AnimationSetDataProject::SerializeTo(DeqNstr& lines) const
{
    throw nemesis::NObjectException("nemesis::AnimationSetDataProject::SerializeTo is not supported");
}

UPtr<nemesis::NObject> nemesis::AnimationSetDataProject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationSetDataProject> nemesis::AnimationSetDataProject::Clone() const
{
    std::scoped_lock<std::mutex> lock(StateMutex);
    auto project     = std::make_unique<nemesis::AnimationSetDataProject>(Name);
    project->Headers = Headers;
    auto& map        = project->StateMap;

    for (auto& it : StateMap)
    {
        map[it.first] = it.second->Clone();
    }

    return project;
}

const std::string& nemesis::AnimationSetDataProject::GetName() const noexcept
{
    return Name;
}

UPtr<nemesis::AnimationSetDataState>&
nemesis::AnimationSetDataProject::AddState(UPtr<nemesis::AnimationSetDataState>&& state)
{
    std::scoped_lock<std::mutex> lock(StateMutex);
    auto& name = state->GetName();
    NewHeaders.insert(name);
    StateMap[name] = std::move(state);
    return StateMap[name];
}

SPtr<nemesis::TemplateObject>&
nemesis::AnimationSetDataProject::AddStateTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    std::scoped_lock<std::mutex> lock(StateTemplateMutex);
    return StateTemplateList.emplace_back(templt_obj);
}

nemesis::AnimationSetDataState* nemesis::AnimationSetDataProject::GetState(const std::string& name)
{
    std::scoped_lock<std::mutex> lock(StateMutex);
    auto itr = StateMap.find(name);

    if (itr == StateMap.end()) return nullptr;

    return itr->second.get();
}

const nemesis::AnimationSetDataState*
nemesis::AnimationSetDataProject::GetState(const std::string& name) const
{
    std::scoped_lock<std::mutex> lock(StateMutex);
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
    return DeserializeFromDirectory(directory_path, nemesis::to_utf8_string(directory_path.stem()));
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

    if (!is_only_number(token_ptr->Value.ToString()))
    {
        auto& token_value = stream.GetToken().Value;
        throw nemesis::NObjectException("nemesis::AnimationSetDataProject::ParseObjects format error (Line: "
                                        + std::to_string(token_value.GetLineNumber()) + ", File: "
                                        + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
    }

    VecNstr headers;

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto* token_ptr = &stream.GetToken();

        if (token_ptr->Type != nemesis::LineStream::NONE)
        {
            throw nemesis::NObjectException(
                "nemesis::AnimationSetDataProject::ParseHeaders does not support any "
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
            throw nemesis::NObjectException(
                "Syntax Error: Unsupport syntax (Line: " + std::to_string(token_value.GetLineNumber())
                + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
        }

        if (name_itr == project_names.end())
        {
            throw nemesis::NObjectException(
                "nemesis::AnimationSetDataProject::ParseObjects parsing error. Project name count "
                "and project body count do not match");
        }

        auto project     = std::make_unique<nemesis::AnimationSetDataProject>(*name_itr++);
        project->Headers = ParseHeaders(stream, manager);
        auto list
            = nemesis::AnimationSetDataState::ParseObjects(stream, manager, project->Name, project->Headers);

        if (project->Headers.size() != list.size())
        {
            throw nemesis::NObjectException("nemesis::AnimationSetDataProject::ParseObjects parsing error ("
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
        throw nemesis::NObjectException(
            "nemesis::AnimationSetDataProject::ParseObjects parsing error. Project name count and project "
            "body count do not match");
    }

    return project_list;
}
