#include "core/NLine.h"
#include "core/ModLine.h"
#include "core/BreakObject.h"
#include "core/NObjectParser.h"
#include "core/CollectionObject.h"

#include "utilities/stringextension.h"

UPtr<nemesis::NObject> nemesis::NObjectParser::ParseLine(nemesis::LineStream& stream,
                                                          nemesis::SemanticManager& manager)
{
    auto token = stream.GetToken();

    switch (token.Type)
    {
        case nemesis::LineStream::TokenType::IF:
            return ParseIfObject(stream, manager);
        case nemesis::LineStream::TokenType::FOR_EACH:
            return ParseForEachObject(stream, manager);
        case nemesis::LineStream::TokenType::NONE:
            return std::make_unique<nemesis::NLine>(token.Value, manager);
        case nemesis::LineStream::TokenType::BREAK:
            return std::make_unique<nemesis::BreakObject>(token.Value, manager);
    }

    auto& token_value = token.Value;
    throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + token_value.GetFilePath().string() + ")");
}

UPtr<nemesis::IfObject> nemesis::NObjectParser::ParseIfObject(nemesis::LineStream& stream,
                                                               nemesis::SemanticManager& manager)
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

    bool has_else     = false;
    auto collection   = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr      = collection.get();
    auto if_object    = std::make_unique<nemesis::IfObject>(
        if_value, if_value.GetLineNumber(), if_value.GetFilePath(), manager, std::move(collection));

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto& ntoken = stream.GetToken();

        switch (ntoken.Type)
        {
            case nemesis::LineStream::TokenType::ELSE_IF:
            {
                if (has_else)
                {
                    throw std::runtime_error("Syntax error: ELSE IF syntax cannot come after ELSE (Line: "
                                       + std::to_string(ntoken.Value.GetLineNumber())
                                       + ", File: " + ntoken.Value.GetFilePath().string() + ")");
                }

                collection   = std::make_unique<nemesis::CollectionObject>();
                col_ptr      = collection.get();
                auto& nvalue = ntoken.Value;
                if_object->ElseIf(
                    nvalue, nvalue.GetLineNumber(), nvalue.GetFilePath(), manager, std::move(collection));
                break;
            }
            case nemesis::LineStream::TokenType::ELSE:
            {
                has_else   = true;
                collection = std::make_unique<nemesis::CollectionObject>();
                col_ptr    = collection.get();
                if_object->Else(std::move(collection));
                break;
            }
            case nemesis::LineStream::TokenType::END_IF:
            {
                return if_object;
            }
            default:
            {
                col_ptr->AddObject(std::move(ParseLine(stream, manager)));
                break;
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed IF Statement (Line: "
                             + std::to_string(if_value.GetLineNumber())
                             + ", File: " + if_value.GetFilePath().string() + ")");
}

UPtr<nemesis::ForEachObject> nemesis::NObjectParser::ParseForEachObject(nemesis::LineStream& stream,
                                                                         nemesis::SemanticManager& manager)
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
    auto col_ptr     = collection.get();
    auto fe_object   = std::make_unique<nemesis::ForEachObject>(
        fe_value, fe_value.GetLineNumber(), fe_value.GetFilePath(), manager, std::move(collection));

    auto scope = fe_object->BuildScope(manager);

    for (++stream; !stream.IsEoF(); ++stream)
    {
        auto ntoken = stream.GetToken();

        switch (ntoken.Type)
        {
            case nemesis::LineStream::TokenType::CLOSE:
            {
                return fe_object;
            }
            default:
            {
                col_ptr->AddObject(std::move(ParseLine(stream, manager)));
                break;
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed FOREACH Statement (Line: "
                             + std::to_string(fe_value.GetLineNumber())
                             + ", File: " + fe_value.GetFilePath().string() + ")");
}

Vec<UPtr<nemesis::NObject>> nemesis::NObjectParser::ParseModObjects(nemesis::LineStream& stream,
                                                                     nemesis::SemanticManager& manager)
{
    return ParseModObjects(stream, manager, [](nemesis::LineStream& s) { return !s.IsEoF(); });
}

Vec<UPtr<nemesis::NObject>> nemesis::NObjectParser::ParseModObjects(nemesis::LineStream& stream,
                                                                    nemesis::SemanticManager& manager,
                                        std::function<bool(nemesis::LineStream&)> terminator)
{
    Vec<UPtr<nemesis::NObject>> objects;

    if (stream.IsEoF()) return objects;

    Deq<const nemesis::Line*> mod_lines;
    auto token_value = stream.GetToken().Value;
    std::function<void(nemesis::LineStream&, nemesis::SemanticManager&)> set_line
        = [&mod_lines](nemesis::LineStream& stream, nemesis::SemanticManager& manager)
    { mod_lines.emplace_back(&stream.GetToken().Value); };

    for (++stream; !terminator(stream); ++stream)
    {
        auto token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::TokenType::MOD_ORG:
            {
                set_line = [&token_value, &mod_lines, &objects](nemesis::LineStream& stream,
                                                                nemesis::SemanticManager& manager)
                {
                    auto object    = ParseLine(stream, manager);
                    auto* line_ptr = dynamic_cast<nemesis::NLine*>(object.get());

                    if (!line_ptr)
                    {
                        auto& token_value = stream.GetToken().Value;
                        throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                                 + std::to_string(token_value.GetLineNumber())
                                                 + ", File: " + token_value.GetFilePath().string() + ")");
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

                    objects.emplace_back(std::move(object));
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
                throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ", File: " + token_value.GetFilePath().string() + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed MOD_CODE Statement (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + token_value.GetFilePath().string() + ")");
}

Vec<UPtr<nemesis::NObject>> nemesis::NObjectParser::ParseHkxObjects(nemesis::LineStream& stream,
                                                                    nemesis::SemanticManager& manager)
{
    auto& token = stream.GetToken();
    Vec<UPtr<nemesis::NObject>> objects;

    switch (token.Type)
    {
        case nemesis::LineStream::NONE:
            break;
        default:
            auto object = nemesis::NObjectParser::ParseLine(stream, manager);
            objects.emplace_back(std::move(object));
            return objects;
    }

    auto& line = token.Value;

    if (line.empty())
    {
        auto object = nemesis::NObjectParser::ParseLine(stream, manager);
        objects.emplace_back(std::move(object));
        return objects;
    }

    if (line.find("<") == NOT_FOUND)
    {
        VecStr list;
        StringSplit(line, list);

        for (auto& each : list)
        {
            nemesis::Line nline("				" + each, line.GetLineNumber(), line.GetFilePath());
            objects.emplace_back(std::make_unique<nemesis::NLine>(nline, manager));
        }

        return objects;
    }

    if (line.find(R"( numelements=")") == NOT_FOUND)
    {
        auto object = nemesis::NObjectParser::ParseLine(stream, manager);
        objects.emplace_back(std::move(object));
        return objects;
    }

    size_t pos = line.find(R"( numelements="0"></hkparam>)");

    if (pos == NOT_FOUND)
    {
        auto object = nemesis::NObjectParser::ParseLine(stream, manager);
        objects.emplace_back(std::move(object));
        return objects;
    }

    nemesis::Line nline = line.substr(0, pos + 17);
    objects.emplace_back(std::make_unique<nemesis::NLine>(nline, manager));

    nline = nemesis::Line("			</hkparam>", line.GetLineNumber(), line.GetFilePath());
    objects.emplace_back(std::make_unique<nemesis::NLine>(nline, manager));
    return objects;
}

Vec<UPtr<nemesis::NObject>> nemesis::NObjectParser::ParseHkxModObjects(nemesis::LineStream& stream,
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
                            throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                                     + std::to_string(token_value.GetLineNumber())
                                                     + ", File: " + token_value.GetFilePath().string() + ")");
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
                throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ", File: " + token_value.GetFilePath().string() + ")");
            }
        }
    }

    throw std::runtime_error("Syntax Error: Unclosed ModCode Statement (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + token_value.GetFilePath().string() + ")");
}
