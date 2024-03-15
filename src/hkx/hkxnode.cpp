#include <array>

#include "hkx/HkxNode.h"

#include "utilities/conditionsyntax.h"
#include "utilities/stringextension.h"

#include "core/ModLine.h"
#include "core/NObjectParser.h"

namespace ns = nemesis::syntax;

const USetStr nemesis::HkxNode::DataClasses
    = {"hkbBehaviorGraphStringData", "hkbVariableValueSet", "hkbBehaviorGraphData", "hkRootLevelContainer"};

const nemesis::regex nemesis::HkxNode::NodeIdRgx(
    "^\\s*\\<hkobject name\\=\"(.+?)\" class\\=\"(.+?)\" signature\\=\"(.+?)\"\\>.*$");

bool nemesis::HkxNode::IsNodeEnd(nemesis::LineStream& stream, bool& start)
{
    if (stream.IsEoF()) return true;

    int pos         = stream.GetPosition();
    auto& token     = stream.GetToken();
    auto& token_val = token.Value;

    if (token_val.find("</hksection>") != NOT_FOUND) return true;

    if (token.Type != nemesis::LineStream::TokenType::NONE)
    {
        auto ctoken = token;

        do
        {
            if (ctoken.Type == nemesis::LineStream::TokenType::CLOSE
                || ctoken.Type == nemesis::LineStream::TokenType::MOD_CLOSE
                || ctoken.Type == nemesis::LineStream::TokenType::END_IF)
            {
                stream -= (stream.GetPosition() - pos);
                return false;
            }

            ++stream;

            if (stream.IsEoF())
            {
                stream -= (stream.GetPosition() - pos);
                return false;
            }

            ctoken = stream.GetToken();
        } while (ctoken.Type != nemesis::LineStream::TokenType::NONE);
    }

    size_t fpos = 0;
    constexpr std::array<std::string_view, 3> hkxheader_checkers
        = {R"(<hkobject name=")", R"(" class=")", R"(" signature=")"};

    for (auto& checker : hkxheader_checkers)
    {
        fpos = token_val.find(checker, fpos);

        if (fpos != NOT_FOUND) continue;

        stream -= (stream.GetPosition() - pos);

        return false;
    }

    stream -= (stream.GetPosition() - pos);

    if (start)
    {
        start = false;
        return false;
    }

    return true;
}

void nemesis::HkxNode::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    Data->CompileTo(lines, state);
}

void nemesis::HkxNode::SerializeTo(DeqNstr& lines) const
{
    Data->SerializeTo(lines);
}

UPtr<nemesis::NObject> nemesis::HkxNode::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::Clone() const
{
    auto hkxnode       = std::make_unique<nemesis::HkxNode>();
    hkxnode->NodeId    = NodeId;
    hkxnode->ClassName = ClassName;
    hkxnode->Data      = Data == nullptr ? nullptr : Data->Clone();
    return hkxnode;
}

const std::string& nemesis::HkxNode::GetNodeId() const noexcept
{
    return NodeId;
}

const std::string& nemesis::HkxNode::GetClassName() const noexcept
{
    return ClassName;
}

void nemesis::HkxNode::MatchAndUpdate(const nemesis::HkxNode& hkxnode)
{
    std::scoped_lock<std::mutex> lock(UpdaterMutex);
    Data->MatchAndUpdate(*hkxnode.Data);
}

bool nemesis::HkxNode::IsDataClass(nemesis::LineStream& stream)
{
    auto* start = &*stream;

    while (!stream.IsEoF())
    {
        nemesis::smatch match;

        if (nemesis::regex_match(*stream, match, NodeIdRgx))
        {
            while (start != &*stream)
            {
                --stream;
            }

            return DataClasses.find(match[2]) != DataClasses.end();
        }

        ++stream;
    }

    return false;
}

UPtr<nemesis::NObject> nemesis::HkxNode::ParseHkxNode(nemesis::LineStream& stream,
                                                      nemesis::SemanticManager& manager)
{
    nemesis::HkxNode* node;
    return ParseHkxNode(stream, manager, node);
}

UPtr<nemesis::NObject> nemesis::HkxNode::ParseHkxNode(nemesis::LineStream& stream,
                                                      nemesis::SemanticManager& manager,
                                                      nemesis::HkxNode*& node)
{
    auto& token       = stream.GetToken();
    auto& token_value = token.Value;
    auto hkx_node     = std::make_unique<nemesis::HkxNode>();
    node              = hkx_node.get();
    bool start        = true;

    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr    = collection.get();

    switch (token.Type)
    {
        case nemesis::LineStream::TokenType::IF:
        {
            ++stream;
            nemesis::smatch match;

            if (!nemesis::regex_match(*stream, match, NodeIdRgx))
            {
                throw std::runtime_error("Behavior Format Error: Node Id not found (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ", File: " + token_value.GetFilePath().string() + ")");
            }
            
            hkx_node->NodeId = match[1];
            hkx_node->ClassName = match[2];

            for (; !IsNodeEnd(stream, start); ++stream)
            {
                auto& ntoken = stream.GetToken();

                if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

                if (ntoken.Type == nemesis::LineStream::TokenType::MOD_OPEN)
                {
                    auto objects = nemesis::NObjectParser::ParseHkxModObjects(stream, manager);

                    for (auto& object : objects)
                    {
                        col_ptr->AddObject(std::move(object));
                    }
                }
                else if (ntoken.Type == nemesis::LineStream::TokenType::END_IF)
                {
                    ++stream;

                    if (IsNodeEnd(stream, start))
                    {
                        hkx_node->Data = std::move(collection);
                        auto if_obj    = std::make_unique<nemesis::IfObject>(token_value,
                                                                          token_value.GetLineNumber(),
                                                                          token_value.GetFilePath(),
                                                                          manager,
                                                                          std::move(hkx_node));
                        return if_obj;
                    }

                    hkx_node->Data = std::make_unique<CollectionObject>();
                    col_ptr        = hkx_node->Data.get();
                    col_ptr->AddObject(std::make_unique<nemesis::IfObject>(token_value,
                                                                           token_value.GetLineNumber(),
                                                                           token_value.GetFilePath(),
                                                                           manager,
                                                                           std::move(collection)));

                    for (; !IsNodeEnd(stream, start); ++stream)
                    {
                        auto& ntoken = stream.GetToken();

                        if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

                        Vec<UPtr<nemesis::NObject>> objects
                            = ntoken.Type != nemesis::LineStream::TokenType::MOD_OPEN
                                  ? nemesis::NObjectParser::ParseHkxObjects(stream, manager)
                                  : nemesis::NObjectParser::ParseHkxModObjects(stream, manager);

                        for (auto& object : objects)
                        {
                            col_ptr->AddObject(std::move(object));
                        }
                    }

                    return hkx_node;
                }
                else
                {
                    Vec<UPtr<nemesis::NObject>> objects
                        = ntoken.Type != nemesis::LineStream::TokenType::MOD_OPEN
                              ? nemesis::NObjectParser::ParseHkxObjects(stream, manager)
                              : nemesis::NObjectParser::ParseHkxModObjects(stream, manager);

                    for (auto& object : objects)
                    {
                        col_ptr->AddObject(std::move(object));
                    }
                }
            }

            throw std::runtime_error("Syntax Error: Unclosed If Statement (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + token_value.GetFilePath().string() + ")");
        }
        case nemesis::LineStream::TokenType::NONE:
        {
            nemesis::smatch match;

            if (!nemesis::regex_match(token_value, match, NodeIdRgx))
            {
                throw std::runtime_error("Behavior Format Error: Node Id not found (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ", File: " + token_value.GetFilePath().string() + ")");
            }

            hkx_node->NodeId    = match[1];
            hkx_node->ClassName = match[2];

            for (; !IsNodeEnd(stream, start); ++stream)
            {
                auto& ntoken = stream.GetToken();

                if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

                Vec<UPtr<nemesis::NObject>> objects
                    = ntoken.Type != nemesis::LineStream::TokenType::MOD_OPEN
                          ? nemesis::NObjectParser::ParseHkxObjects(stream, manager)
                          : nemesis::NObjectParser::ParseHkxModObjects(stream, manager);
                
                for (auto& object : objects)
                {
                    col_ptr->AddObject(std::move(object));
                }
            }

            hkx_node->Data = std::move(collection);
            return hkx_node;
        }
        default:
        {
            throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + token_value.GetFilePath().string() + ")");
        }
    }
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::ParseHkxNodeFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    return ParseHkxNodeFromFile(filepath, manager);
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::ParseHkxNodeFromFile(const std::filesystem::path& filepath,
                                                      const nemesis::TemplateClass* template_class)
{
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return ParseHkxNodeFromFile(filepath, manager);
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::ParseHkxNodeFromFile(const std::filesystem::path& filepath,
                                                      nemesis::SemanticManager& manager)
{
    VecNstr lines;
    GetFileLines(filepath, lines);
    nemesis::LineStream stream(lines.begin(), lines.end());

    auto hkx_node     = std::make_unique<nemesis::HkxNode>();
    auto token        = stream.GetToken();
    auto& token_value = token.Value;
    bool start        = true;

    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr    = collection.get();

    nemesis::smatch match;

    if (!nemesis::regex_match(token_value, match, NodeIdRgx))
    {
        throw std::runtime_error("Behavior Format Error: Node Id not found (Line: "
                                 + std::to_string(token_value.GetLineNumber())
                                 + ", File: " + token_value.GetFilePath().string() + ")");
    }

    hkx_node->NodeId    = match[1];
    hkx_node->ClassName = match[2];
    hkx_node->Data      = std::move(collection);
    int counter         = 0;

    for (; !IsNodeEnd(stream, start); ++stream)
    {
        counter++;
        auto& ntoken = stream.GetToken();

        if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

        Vec<UPtr<nemesis::NObject>> objects
            = ntoken.Type != nemesis::LineStream::TokenType::MOD_OPEN
                  ? nemesis::NObjectParser::ParseHkxObjects(stream, manager)
                  : nemesis::NObjectParser::ParseHkxModObjects(stream, manager);

        for (auto& object : objects)
        {
            col_ptr->AddObject(std::move(object));
        }
    }

    return hkx_node;
}
