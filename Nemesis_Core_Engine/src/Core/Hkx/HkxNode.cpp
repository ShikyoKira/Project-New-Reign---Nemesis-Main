#include <array>

#include "Core/Hkx/HkxNode.h"
#include "Core/Hkx/HkxFile.h"

#include "Utilities/File.h"
#include "Utilities/Algorithm.h"
#include "Utilities/ThreadPool.h"
#include "Utilities/OnScopeEnds.h"
#include "Utilities/ConditionSyntax.h"
#include "Utilities/StringExtension.h"

#include "Core/ModLine.h"
#include "Core/ModClass.h"
#include "Core/CompileState.h"
#include "Core/NObjectParser.h"

#include "AlternateAnimation/AlterAnim.h"

namespace ns = nemesis::syntax;

const std::regex nemesis::HkxNode::NodeIdRgx(
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
    size_t size = lines.size();
    Data->CompileTo(lines, state);

    if (size >= lines.size() || ClassName != "hkbClipGenerator" || !HkxDocument) return;

    auto& target       = HkxDocument->GetTargetPath();
    auto& alter_anim_repo = state.GetAlterAnimRepository();
    auto& ex_anim_repo = state.GetExAnimationRepository();
    auto proj          = ex_anim_repo.GetProjectByBehavior(target);

    if (!proj || !alter_anim_repo.IsSupportedBehavior(target)) return;

    static std::regex anim_path_rgx("^.*<hkparam name=\"animationName\">([^<]+)</hkparam>.*$");
    std::filesystem::path anim_path;
    size_t anim_pos;
    VecStr temp_lines;
    UPtr<nemesis::LinkedExAnimList> linked_ex_anim;
    const nemesis::AlterAnim* alternate = nullptr;

    for (size_t i = size; i < lines.size(); i++)
    {
        temp_lines.emplace_back(lines[i].ToString());
        std::smatch match;

        if (!std::regex_match(lines[i].ToString(), match, anim_path_rgx)) continue;

        anim_path = match.str(1);

        if (proj)
        {
            linked_ex_anim = proj->GetLinkedExAnim(anim_path);
        }

        if (alter_anim_repo.IsSupportedBehavior(target))
        {
            auto aa_group = alter_anim_repo.GetAlterAnimGroupByAnimation(anim_path.string());

            if (aa_group)
            {
                alternate = aa_group->GetAlternateSet(anim_path.string());
            }
        }

        if (!linked_ex_anim && !alternate) return;

        anim_pos = i - size;

        for (i++ ; i < lines.size(); i++)
        {
            temp_lines.emplace_back(lines[i].ToString());
        }
    }

    const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)> add_clip_func
        = [&temp_lines,
           anim_pos](VecStr& lines, const std::string& clip_node_id, const std::filesystem::path& anim_path)
    {
        lines.emplace_back("\t\t<hkobject name=\"" + clip_node_id
                           + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");

        for (size_t i = 1; i < temp_lines.size(); i++)
        {
            if (anim_pos == i)
            {
                lines.emplace_back("\t\t\t<hkparam name=\"animationName\">"
                                   + XmlStringEncode(anim_path.string()) + "</hkparam>");
                continue;
            }

            lines.emplace_back(temp_lines[i]);
        }
    };

    std::string node_id = NodeId;
    VecStr alter_anim_lines;
    VecStr ex_anim_lines;

    if (alternate)
    {
        alter_anim_lines = alternate->CompileAsXml(node_id, state, add_clip_func);
    }

    if (linked_ex_anim)
    {
        ex_anim_lines = linked_ex_anim->CompileAsXml(node_id, state, add_clip_func);
    }

    lines[size] = "\t\t<hkobject name=\"" + node_id
                  + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">";
    lines.insert(lines.end(), ex_anim_lines.begin(), ex_anim_lines.end());
    lines.insert(lines.end(), alter_anim_lines.begin(), alter_anim_lines.end());
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
    hkxnode->Data      = !Data ? nullptr : Data->Clone();
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

const std::filesystem::path& nemesis::HkxNode::GetFilePath() const noexcept
{
    return FilePath;
}

const nemesis::HkxFile* nemesis::HkxNode::GetHkxDocument() const noexcept
{
    return HkxDocument;
}

void nemesis::HkxNode::SetHkxDocument(const nemesis::HkxFile* hkx_document) noexcept
{
    HkxDocument = hkx_document;
}

void nemesis::HkxNode::MatchAndUpdate(const nemesis::HkxNode& hkxnode)
{
    std::scoped_lock<std::mutex> lock(UpdaterMutex);
    Data->MatchAndUpdate(*hkxnode.Data);
}

void nemesis::HkxNode::MatchAndUpdate(const std::string& mod_code, const nemesis::HkxNode& hkxnode)
{
    std::scoped_lock<std::mutex> lock(UpdaterMutex);
    Data->MatchAndUpdate(mod_code, *hkxnode.Data);
}

bool nemesis::HkxNode::IsDataClass(nemesis::LineStream& stream)
{
    static const USetStr DataClasses = {
        "hkbBehaviorGraphStringData", "hkbVariableValueSet", "hkbBehaviorGraphData", "hkRootLevelContainer"};

    auto* start = &*stream;

    while (!stream.IsEoF())
    {
        std::smatch match;

        if (std::regex_match((*stream).ToString(), match, NodeIdRgx))
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
    auto& token        = stream.GetToken();
    auto& token_value  = token.Value;
    auto hkx_node      = std::make_unique<nemesis::HkxNode>();
    hkx_node->FilePath = token_value.GetFilePath();
    node               = hkx_node.get();
    bool start         = true;

    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr    = collection.get();

    switch (token.Type)
    {
        case nemesis::LineStream::TokenType::IF:
        {
            ++stream;
            std::smatch match;

            if (!std::regex_match((*stream).ToString(), match, NodeIdRgx))
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
            std::smatch match;

            if (!std::regex_match(token_value.ToString(), match, NodeIdRgx))
            {
                throw std::runtime_error("Behavior Format Error: Node Id not found (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ", File: " + token_value.GetFilePath().string() + ")");
            }

            hkx_node->NodeId    = match[1];
            hkx_node->ClassName = match[2];

            auto add_line
                = [col_ptr, &stream, &manager](const nemesis::LineStream::Token& token)
            {
                Vec<UPtr<nemesis::NObject>> objects
                    = token.Type != nemesis::LineStream::TokenType::MOD_OPEN
                          ? nemesis::NObjectParser::ParseHkxObjects(stream, manager)
                          : nemesis::NObjectParser::ParseHkxModObjects(stream, manager);

                for (auto& object : objects)
                {
                    col_ptr->AddObject(std::move(object));
                }
            };

            if (hkx_node->ClassName == "hkbVariableValueSet")
            {
                for (; !IsNodeEnd(stream, start); ++stream)
                {
                    auto& ntoken = stream.GetToken();

                    if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

                    static std::regex vec4_rgx("^([\\t]+)\\((-?[0-9]+\\.[0-9]+) (-?[0-9]+\\.[0-9]+) "
                                               "(-?[0-9]+\\.[0-9]+) (-?[0-9]+\\.[0-9]+)\\).*$");
                    std::smatch vec4_match;

                    if (!std::regex_match(ntoken.Value.ToString(), vec4_match, vec4_rgx))
                    {
                        add_line(ntoken);
                        continue;
                    }

                    std::string spaces = vec4_match[1];

                    for (size_t i = 2; i < 6; i++)
                    {
                        std::string val = vec4_match.str(i);

                        if (std::stod(val) == 0)
                        {
                            val = "0.000000";
                        }

                        col_ptr->AddObject(std::make_unique<nemesis::NLine>(spaces + val,
                                                                            ntoken.Value.GetLineNumber(),
                                                                            ntoken.Value.GetFilePath(),
                                                                            manager));
                    }
                }

                hkx_node->Data = std::move(collection);
                return hkx_node;
            }
            else
            {
                constexpr std::array<std::string_view, 31> coordinate_class_list
                    = {"hkbPoseMatchingGenerator",
                       "hkbCharacterData",
                       "hkbFootIkDriverInfo",
                       "hkbHandIkDriverInfo",
                       "hkbMirroredSkeletonInfo",
                       "BSComputeAddBoneAnimModifier",
                       "BSDecomposeVectorModifier",
                       "BSDirectAtModifier",
                       "BSDistTriggerModifier",
                       "BSLookAtModifier",
                       "BSPassByTargetTriggerModifier",
                       "BSTweenerModifier",
                       "hkbCombineTransformsModifier",
                       "hkbComputeDirectionModifier",
                       "hkbComputeRotationFromAxisAngleModifier",
                       "hkbComputeRotationToTargetModifier",
                       "hkbDampingModifier",
                       "hkbEvaluateHandleModifier",
                       "hkbFootIkControlsModifier",
                       "hkbFootIkModifier",
                       "hkbGetUpModifier",
                       "hkbGetWorldFromModelModifier",
                       "hkbHandIkControlsModifier",
                       "hkbKeyframeBonesModifier",
                       "hkbLookAtModifier",
                       "hkbMoveCharacterModifier",
                       "hkbProxyModifier",
                       "hkbRotateCharacterModifier",
                       "hkbSenseHandleModifier",
                       "hkbTransformVectorModifier",
                       "hkbTwistModifier"};

                if (std::find(coordinate_class_list.begin(), coordinate_class_list.end(), hkx_node->ClassName) != coordinate_class_list.end())
                {
                    for (; !IsNodeEnd(stream, start); ++stream)
                    {
                        auto& ntoken = stream.GetToken();

                        if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

                        static std::regex vec4_rgx("^([\\t]+)<hkparam name\\=\"([^\"]+)\">\\((-?[0-9]+\\.[0-9]+) (-?[0-9]+\\.[0-9]+) "
                                                   "(-?[0-9]+\\.[0-9]+) (-?[0-9]+\\.[0-9]+)\\)</hkparam>.*$");
                        std::smatch vec4_match;

                        if (!std::regex_match(ntoken.Value.ToString(), vec4_match, vec4_rgx))
                        {
                            add_line(ntoken);
                            continue;
                        }

                        std::string spaces     = vec4_match[1];
                        std::string param_name = vec4_match[2];

                        col_ptr->AddObject(
                            std::make_unique<nemesis::NLine>(spaces + "<hkparam name=\"" + param_name + "\">",
                                                             ntoken.Value.GetLineNumber(),
                                                             ntoken.Value.GetFilePath(),
                                                             manager));

                        for (size_t i = 3; i < 7; i++)
                        {
                            std::string val = vec4_match.str(i);

                            if (std::stod(val) == 0)
                            {
                                val = "0.000000";
                            }

                            col_ptr->AddObject(std::make_unique<nemesis::NLine>(spaces + "\t" + val,
                                                                                ntoken.Value.GetLineNumber(),
                                                                                ntoken.Value.GetFilePath(),
                                                                                manager));
                        }

                        col_ptr->AddObject(
                            std::make_unique<nemesis::NLine>(spaces + "</hkparam>",
                                                             ntoken.Value.GetLineNumber(),
                                                             ntoken.Value.GetFilePath(),
                                                             manager));
                    }

                    hkx_node->Data = std::move(collection);
                    return hkx_node;
                }
            }
            
            for (; !IsNodeEnd(stream, start); ++stream)
            {
                auto& ntoken = stream.GetToken();

                if (ntoken.Value.find(" SERIALIZE_IGNORED ") != NOT_FOUND) continue;

                add_line(ntoken);
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

void nemesis::HkxNode::Deserialize(nemesis::HkxNode& hkx_node,
                                   nemesis::LineStream& stream,
                                   nemesis::SemanticManager& manager)
{
    auto token        = stream.GetToken();
    auto& token_value = token.Value;

    std::smatch match;

    if (!std::regex_match(token_value.ToString(), match, NodeIdRgx))
    {
        throw std::runtime_error("Behavior Format Error: Node Id not found (Line: "
                                 + std::to_string(token_value.GetLineNumber())
                                 + ", File: " + token_value.GetFilePath().string() + ")");
    }

    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr    = collection.get();

    hkx_node.NodeId    = match[1];
    hkx_node.ClassName = match[2];
    hkx_node.Data      = std::move(collection);
    bool start          = true;

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
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::DeserializeHkxNodeFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    return DeserializeHkxNodeFromFile(filepath, manager);
}

UPtr<nemesis::HkxNode>
nemesis::HkxNode::DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                             const nemesis::TemplateClass* template_class)
{
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return DeserializeHkxNodeFromFile(filepath, manager);
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                                                    nemesis::SemanticManager& manager)
{
    auto hkx_node      = std::make_unique<nemesis::HkxNode>();
    hkx_node->FilePath = filepath;

    VecNstr lines;
    GetFileLines(filepath, lines);
    nemesis::LineStream stream(lines.begin(), lines.end());
    Deserialize(*hkx_node, stream, manager);
    return hkx_node;
}

UPtr<nemesis::HkxNode> nemesis::HkxNode::DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                                                    nemesis::ThreadPool& threadpool)
{
    auto hkx_node      = std::make_unique<nemesis::HkxNode>();
    hkx_node->FilePath = filepath;
    auto node_ptr      = hkx_node.get();

    threadpool.enqueue(
        [filepath, node_ptr]()
        {
            VecNstr lines;
            GetFileLines(filepath, lines);
            nemesis::LineStream stream(lines.begin(), lines.end());
            SemanticManager manager;
            Deserialize(*node_ptr, stream, manager);
        });
    return hkx_node;
}

UPtr<nemesis::HkxNode>
nemesis::HkxNode::DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                             const nemesis::TemplateClass* template_class,
                                             nemesis::ThreadPool& threadpool)
{
    auto hkx_node      = std::make_unique<nemesis::HkxNode>();
    hkx_node->FilePath = filepath;
    auto node_ptr      = hkx_node.get();

    threadpool.enqueue(
        [filepath, template_class, node_ptr]()
        {
            VecNstr lines;
            GetFileLines(filepath, lines);
            nemesis::LineStream stream(lines.begin(), lines.end());
            SemanticManager manager;
            manager.SetCurrentTemplateClass(template_class);
            Deserialize(*node_ptr, stream, manager);
        });
    return hkx_node;
}
