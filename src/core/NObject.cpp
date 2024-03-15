#include "core/NLine.h"
#include "core/ModLine.h"
#include "core/NObject.h"
#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/LineStream.h"
#include "core/CompileState.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

Vec<UPtr<nemesis::NObject>> nemesis::NObject::ParseModObjects(nemesis::LineStream& stream,
                                                              nemesis::SemanticManager& manager)
{
    return nemesis::NObject::ParseModObjects(stream, manager, [](const nemesis::Line& nline) {});
}

Vec<UPtr<nemesis::NObject>>
nemesis::NObject::ParseModObjects(nemesis::LineStream& stream,
                                  nemesis::SemanticManager& manager,
                                  std::function<void(const nemesis::Line&)> add_nline_event)
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

    Deq<const nemesis::Line*> ori_lines;
    Deq<const nemesis::Line*> mod_lines;
    Deq<const nemesis::Line*>* lines = &mod_lines;

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::MOD_CLOSE:
            {
                size_t i = 0;

                for (; i < ori_lines.size(); ++i)
                {
                    auto& line_ptr = *ori_lines[i];
                    auto nline_ptr = std::make_unique<nemesis::NLine>(line_ptr, manager);

                    if (i < mod_lines.size())
                    {
                        nline_ptr->AddModLine(mod_value,
                                              mod_value.GetLineNumber(),
                                              mod_value.GetFilePath(),
                                              manager,
                                              *mod_lines[i]);
                    }
                    else
                    {
                        nline_ptr->AddModLine(
                            mod_value, mod_value.GetLineNumber(), mod_value.GetFilePath(), manager);
                    }

                    object_list.emplace_back(std::move(nline_ptr));
                }

                if (i >= mod_lines.size()) return object_list;

                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto col_ptr    = collection.get();
                auto mod_object = std::make_unique<nemesis::ModObject>(mod_value,
                                                                       mod_value.GetLineNumber(),
                                                                       mod_value.GetFilePath(),
                                                                       manager,
                                                                       std::move(collection));

                for (auto itr = mod_lines.begin() + i; itr != mod_lines.end(); itr++)
                {
                    col_ptr->AddObject(std::make_unique<nemesis::NLine>(**itr, manager));
                }

                object_list.emplace_back(std::move(mod_object));
                return object_list;
            }
            case nemesis::LineStream::MOD_ORG:
            {
                lines = &ori_lines;
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                lines->emplace_back(&value);
                add_nline_event(value);
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

UPtr<nemesis::ForEachObject> nemesis::NObject::ParseForEachObject(nemesis::LineStream& stream,
                                                                  nemesis::SemanticManager& manager)
{
    return nemesis::NObject::ParseForEachObject(stream, manager, [](const nemesis::Line& nline) {});
}

UPtr<nemesis::ForEachObject>
nemesis::NObject::ParseForEachObject(nemesis::LineStream& stream,
                                     nemesis::SemanticManager& manager,
                                     std::function<void(const nemesis::Line&)> add_nline_event)
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

    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto* col_ptr   = collection.get();
    auto fe_obj     = std::make_unique<nemesis::ForEachObject>(
        fe_value, fe_value.GetLineNumber(), fe_value.GetFilePath(), manager, std::move(collection));

    auto scope = fe_obj->BuildScope(manager);

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::CLOSE:
            {
                return fe_obj;
            }
            case nemesis::LineStream::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    col_ptr->AddObject(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = ParseIfObject(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = ParseForEachObject(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                col_ptr->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                add_nline_event(value);
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

UPtr<nemesis::IfObject> nemesis::NObject::ParseIfObject(nemesis::LineStream& stream,
                                                        nemesis::SemanticManager& manager)
{
    return nemesis::NObject::ParseIfObject(stream, manager, [](const nemesis::Line& nline) {});
}

UPtr<nemesis::IfObject>
nemesis::NObject::ParseIfObject(nemesis::LineStream& stream,
                                nemesis::SemanticManager& manager,
                                std::function<void(const nemesis::Line&)> add_nline_event)
{
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
                auto mod_objects = ParseModObjects(stream, manager);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    col_ptr->AddObject(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = ParseIfObject(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = ParseForEachObject(stream, manager, add_nline_event);
                col_ptr->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                col_ptr->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                add_nline_event(value);
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

    throw std::runtime_error("Syntax Error: Unclosed IF Statement (Line: "
                             + std::to_string(if_value.GetLineNumber())
                             + ", File: " + if_value.GetFilePath().string() + ")");
}

UPtr<nemesis::CollectionObject> nemesis::NObject::ParseAsCollection(nemesis::LineStream& stream,
                                                                    nemesis::SemanticManager& manager)
{
    return ParseAsCollection(stream, manager, [](const nemesis::Line&) {});
}

UPtr<nemesis::CollectionObject>
nemesis::NObject::ParseAsCollection(nemesis::LineStream& stream,
                                    nemesis::SemanticManager& manager,
                                    std::function<void(const nemesis::Line&)> add_nline_event)
{
    auto collection = std::make_unique<nemesis::CollectionObject>();

    if (stream.IsEoF()) return collection;

    for (; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    collection->AddObject(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = ParseIfObject(stream, manager, add_nline_event);
                collection->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = ParseForEachObject(stream, manager, add_nline_event);
                collection->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                collection->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                add_nline_event(value);
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

    return collection;
}

DeqNstr nemesis::NObject::Compile(nemesis::CompileState& state) const
{
    DeqNstr lines;
    CompileTo(lines, state);
    return lines;
}

DeqNstr nemesis::NObject::Serialize() const
{
    DeqNstr lines;
    SerializeTo(lines);
    return lines;
}
