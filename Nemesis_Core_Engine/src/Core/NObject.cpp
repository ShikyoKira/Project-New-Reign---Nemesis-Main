#include "Core/NObject.h"
#include "Core/BreakObject.h"
#include "Core/CollectionObject.h"
#include "Core/CompileState.h"
#include "Core/ForEachObject.h"
#include "Core/IfObject.h"
#include "Core/LineStream.h"
#include "Core/ModLine.h"
#include "Core/ModObject.h"
#include "Core/NLine.h"

#include "Utilities/Algorithm.h"
#include "Utilities/StringExtension.h"

UPtr<nemesis::NObject> nemesis::NObject::ParseLine(nemesis::LineStream& stream,
                                                   nemesis::SemanticManager& manager)
{
    return nemesis::NObject::ParseLine(stream, manager, [](const nemesis::Line& nline) {});
}

UPtr<nemesis::NObject> nemesis::NObject::ParseLine(nemesis::LineStream& stream,
                                                   nemesis::SemanticManager& manager,
                                                   std::function<void(const nemesis::Line&)> add_nline_event)
{
    auto token = stream.GetToken();

    switch (token.Type)
    {
        case nemesis::LineStream::TokenType::IF:
            return ParseIfObject(stream, manager);
        case nemesis::LineStream::TokenType::FOR_EACH:
            return ParseForEachObject(stream, manager);
        case nemesis::LineStream::TokenType::NONE:
        {
            auto nline  = std::make_unique<nemesis::NLine>(token.Value, manager);
            add_nline_event(token.Value);
            return nline;
        }
        case nemesis::LineStream::TokenType::BREAK:
            return std::make_unique<nemesis::BreakObject>(token.Value, manager);
        default:
            break;
    }

    auto& token_value = token.Value;
    throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
}

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

    if (mod_token->Type != nemesis::LineStream::TokenType::MOD_OPEN)
    {
        throw std::runtime_error("Syntax Error: Unexpected syntax. Expecting MOD_CODE syntax (Line: "
                                 + std::to_string(mod_value.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(mod_value.GetFilePath()) + ")");
    }

    Deq<const nemesis::Line*> mod_lines;
    auto& token_value = stream.GetToken().Value;
    std::function<void(nemesis::LineStream&, nemesis::SemanticManager&)> set_line
        = [&mod_lines](nemesis::LineStream& stream, nemesis::SemanticManager& manager)
    { mod_lines.emplace_back(&stream.GetToken().Value); };

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::TokenType::MOD_CLOSE:
            {
                if (mod_lines.empty()) return object_list;

                auto collection = std::make_unique<nemesis::CollectionObject>();

                for (auto& mod_line : mod_lines)
                {
                    collection->AddObject(std::make_unique<nemesis::NLine>(*mod_line, manager));
                }

                object_list.emplace_back(std::make_unique<nemesis::ModObject>(token_value,
                                                                              token_value.GetLineNumber(),
                                                                              token_value.GetFilePath(),
                                                                              manager,
                                                                              std::move(collection)));
                return object_list;
            }
            case nemesis::LineStream::TokenType::MOD_ORG:
            {
                set_line = [&token_value, &mod_lines, &object_list](nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager)
                {
                    auto object    = ParseLine(stream, manager);
                    auto* line_ptr = dynamic_cast<nemesis::NLine*>(object.get());

                    if (!line_ptr)
                    {
                        auto& token_value = stream.GetToken().Value;
                        throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                                 + std::to_string(token_value.GetLineNumber()) + ", File: "
                                                 + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
                    }

                    if (!mod_lines.empty())
                    {
                        auto& mod_line = mod_lines.front();
                        line_ptr->AddModLine(token_value,
                                             mod_line->GetLineNumber(),
                                             mod_line->GetFilePath(),
                                             manager,
                                             *mod_line);
                        mod_lines.pop_front();
                    }

                    object_list.emplace_back(std::move(object));
                };
                break;
            }
            case nemesis::LineStream::TokenType::NONE:
            {
                auto& value = token.Value;
                set_line(stream, manager);
                add_nline_event(value);
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed MOD_CODE statement (Line: "
                             + std::to_string(mod_value.GetLineNumber())
                             + ", File: " + nemesis::to_utf8_string(mod_value.GetFilePath()) + ")");
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

    if (fe_token->Type != nemesis::LineStream::TokenType::FOR_EACH)
    {
        throw std::runtime_error("Syntax Error: Unexpected syntax. Expecting FOREACH syntax (Line: "
                                 + std::to_string(fe_value.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(fe_value.GetFilePath()) + ")");
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
            case nemesis::LineStream::TokenType::CLOSE:
            {
                return fe_obj;
            }
            case nemesis::LineStream::TokenType::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    col_ptr->AddObject(std::move(*it));
                }

                break;
            }
            default:
            {
                col_ptr->AddObject(ParseLine(stream, manager, add_nline_event));
                break;
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed FOREACH statement (Line: "
                             + std::to_string(fe_value.GetLineNumber())
                             + ", File: " + nemesis::to_utf8_string(fe_value.GetFilePath()) + ")");
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

    if (if_token->Type != nemesis::LineStream::TokenType::IF)
    {
        throw std::runtime_error("Syntax Error: Unexpected syntax. Expecting IF syntax (Line: "
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
            case nemesis::LineStream::TokenType::END_IF:
            {
                return if_obj;
            }
            case nemesis::LineStream::TokenType::ELSE_IF:
            {
                auto& value = token.Value;

                if (has_else)
                {
                    throw std::runtime_error("Syntax Error: ELSEIF syntax cannot come after ELSE (Line: "
                                             + std::to_string(value.GetLineNumber()) + ", File: "
                                             + nemesis::to_utf8_string(value.GetFilePath()) + ")");
                }

                collection = std::make_unique<nemesis::CollectionObject>();
                col_ptr    = collection.get();
                if_obj->ElseIf(
                    value, value.GetLineNumber(), value.GetFilePath(), manager, std::move(collection));
                break;
            }
            case nemesis::LineStream::TokenType::ELSE:
            {
                if (has_else)
                {
                    auto& value = token.Value;
                    throw std::runtime_error("Syntax Error: ELSE syntax cannot come after ELSE (Line: "
                                             + std::to_string(value.GetLineNumber()) + ", File: "
                                             + nemesis::to_utf8_string(value.GetFilePath()) + ")");
                }

                has_else   = true;
                collection = std::make_unique<nemesis::CollectionObject>();
                col_ptr    = collection.get();
                if_obj->Else(std::move(collection));
                break;
            }
            case nemesis::LineStream::TokenType::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    col_ptr->AddObject(std::move(*it));
                }

                break;
            }
            default:
            {
                col_ptr->AddObject(ParseLine(stream, manager));
                break;
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed IF Statement (Line: "
                             + std::to_string(if_value.GetLineNumber())
                             + ", File: " + nemesis::to_utf8_string(if_value.GetFilePath()) + ")");
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
            case nemesis::LineStream::TokenType::MOD_OPEN:
            {
                auto mod_objects = ParseModObjects(stream, manager);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    collection->AddObject(std::move(*it));
                }

                break;
            }
            default:
            {
                collection->AddObject(ParseLine(stream, manager));
                break;
            }
        }
    }

    return collection;
}

Vec<UPtr<nemesis::NObject>> nemesis::NObject::ParseHkxObjects(nemesis::LineStream& stream,
                                                              nemesis::SemanticManager& manager)
{
    auto& token = stream.GetToken();
    Vec<UPtr<nemesis::NObject>> objects;

    switch (token.Type)
    {
        case nemesis::LineStream::NONE:
            break;
        default:
            auto object = ParseLine(stream, manager);
            objects.emplace_back(std::move(object));
            return objects;
    }

    auto& line = token.Value;

    if (line.empty())
    {
        auto object = ParseLine(stream, manager);
        objects.emplace_back(std::move(object));
        return objects;
    }

    if (line.find("<") == NOT_FOUND)
    {
        static std::regex space_rgx("^([\\s\\t]*).*$");
        std::smatch match;
        std::string space;

        if (std::regex_match(line.ToString(), match, space_rgx))
        {
            space = match[1];
        }
        else
        {
            space = "\t\t\t\t";
        }

        VecStr list;
        StringSplit(line, list);

        for (auto& each : list)
        {
            nemesis::Line nline(
                space + each, line.GetLineNumber(), line.GetFilePathPtr()->shared_from_this());
            objects.emplace_back(std::make_unique<nemesis::NLine>(nline, manager));
        }

        return objects;
    }

    if (line.find(R"( numelements=")") == NOT_FOUND)
    {
        auto object = ParseLine(stream, manager);
        objects.emplace_back(std::move(object));
        return objects;
    }

    size_t pos = line.find(R"( numelements="0"></hkparam>)");

    if (pos == NOT_FOUND)
    {
        auto object = ParseLine(stream, manager);
        objects.emplace_back(std::move(object));
        return objects;
    }

    nemesis::Line nline = line.substr(0, pos + 17);
    objects.emplace_back(std::make_unique<nemesis::NLine>(nline, manager));

    nline = nemesis::Line(
        "			</hkparam>", line.GetLineNumber(), line.GetFilePathPtr()->shared_from_this());
    objects.emplace_back(std::make_unique<nemesis::NLine>(nline, manager));
    return objects;
}

Vec<UPtr<nemesis::NObject>> nemesis::NObject::ParseHkxModObjects(nemesis::LineStream& stream,
                                                                 nemesis::SemanticManager& manager)
{
    Vec<UPtr<nemesis::NObject>> objects;
    Deq<const nemesis::Line*> mod_lines;
    auto token_value = stream.GetToken().Value;
    std::function<void(nemesis::LineStream&, nemesis::SemanticManager&)> set_line
        = [&mod_lines](nemesis::LineStream& stream, nemesis::SemanticManager& manager)
    { mod_lines.emplace_back(&stream.GetToken().Value); };

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::TokenType::MOD_ORG:
            {
                set_line = [&token_value, &mod_lines, &objects](nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager)
                {
                    auto obj_list = ParseHkxObjects(stream, manager);

                    for (auto& obj : obj_list)
                    {
                        auto* line_ptr = dynamic_cast<nemesis::NLine*>(obj.get());

                        if (!line_ptr)
                        {
                            auto& token_value = stream.GetToken().Value;
                            throw std::runtime_error(
                                "Syntax Error: Unsupported syntax (Line: "
                                + std::to_string(token_value.GetLineNumber())
                                + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
                        }

                        if (!mod_lines.empty())
                        {
                            auto& mod_line = mod_lines.front();
                            line_ptr->AddModLine(token_value,
                                                 mod_line->GetLineNumber(),
                                                 mod_line->GetFilePath(),
                                                 manager,
                                                 *mod_line);
                            mod_lines.pop_front();
                        }

                        objects.emplace_back(std::move(obj));
                    }
                };
                break;
            }
            case nemesis::LineStream::TokenType::MOD_CLOSE:
            {
                if (!mod_lines.empty())
                {
                    auto collection = std::make_unique<nemesis::CollectionObject>();

                    for (auto& mod_line : mod_lines)
                    {
                        collection->AddObject(std::make_unique<nemesis::NLine>(*mod_line, manager));
                    }

                    objects.emplace_back(std::make_unique<nemesis::ModObject>(token_value,
                                                                              token_value.GetLineNumber(),
                                                                              token_value.GetFilePath(),
                                                                              manager,
                                                                              std::move(collection)));
                }

                return objects;
            }
            case nemesis::LineStream::TokenType::NONE:
            {
                set_line(stream, manager);
                break;
            }
            default:
            {
                auto& token_value = token.Value;
                throw std::runtime_error(
                    "Syntax Error: Unsupported syntax (Line: " + std::to_string(token_value.GetLineNumber())
                    + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed ModCode Statement (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
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
