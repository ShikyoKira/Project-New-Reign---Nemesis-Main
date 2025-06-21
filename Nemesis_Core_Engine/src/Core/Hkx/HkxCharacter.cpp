#include "Core/Hkx/HkxCharacter.h"
#include "Core/Hkx/HkxNode.h"

#include "Core/CompileState.h"

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/StringExtension.h"

void nemesis::HkxCharacter::PopulateContentsFromFile(nemesis::HkxCharacter& character)
{
    VecNstr lines;
    GetFileLines(character.FilePath, lines, false);
    auto start_itr = lines.begin();

    while (!std::regex_match((*start_itr).ToString(), nemesis::HkxNode::NodeIdRgx))
    {
        ++start_itr;
    }

    nemesis::SemanticManager manager;
    nemesis::LineStream stream(start_itr, lines.end());

    character.RegularNodes = std::make_unique<nemesis::CollectionObject>();

    while (!stream.IsEoF())
    {
        if ((*stream).find("</hksection>") != NOT_FOUND) break;

        nemesis::HkxNode* node_ptr;
        auto node = nemesis::HkxNode::ParseHkxNode(stream, manager, node_ptr);
        node_ptr->SetHkxDocument(&character);

        character.NodeMap[node_ptr->GetNodeId()] = node_ptr;
        character.RegularNodes->AddObject(std::move(node));
    }

    character.ModInUsedList = manager.GetModInUsedList();

    for (auto& node_pair : character.NodeMap)
    {
        if (node_pair.second->GetClassName() != "hkRootLevelContainer") continue;

        character.RootNode = node_pair.second;
        break;
    }
}

void nemesis::HkxCharacter::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    lines.emplace_back(R"(<?xml version="1.0" encoding="ascii"?>)", 1);
    lines.emplace_back(R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0-r1" toplevelobject=")"
                           + RootNode->GetNodeId() + R"(">)",
                       2);
    lines.emplace_back("", 3);
    lines.emplace_back(R"(	<hksection name="__data__">)", 4);
    lines.emplace_back("", 5);

    auto uhandler_add    = std::make_unique<void*>(nullptr);
    auto handler_add     = uhandler_add.get();
    auto animation_count = std::make_shared<size_t>(0);

    auto animation_add = std::make_shared<std::function<void(nemesis::Line&, const nemesis::NObject&)>>(
        [handler_add, &state, this](nemesis::Line& line, const nemesis::NObject& obj)
        {
            if (line.find("</hkparam>") != NOT_FOUND)
            {
                state.RemoveAddLineHandler(*handler_add);
                auto& alter_anim_repo = state.GetAlterAnimRepository();
                auto alter_anim_list  = alter_anim_repo.GetAlterAnimationList();

                auto& ex_anim_repo = state.GetExAnimationRepository();
                auto anim_list     = ex_anim_repo.GetExAnimationList(TargetPath);
                std::string new_ex_lines;

                for (auto& anim : anim_list)
                {
                    nemesis::Line new_line("\t\t\t\t<hkcstring>" + XmlStringEncode(anim->string()) + "</hkcstring>\n");
                    state.RaiseAddLineEvent(new_line, obj);
                    new_ex_lines.append(new_line);
                    state.AddAnimationToOrder(anim->string());
                }

                if (alter_anim_repo.IsSupportedCharacter(TargetPath))
                {
                    for (auto& alter_anim : alter_anim_list)
                    {
                        nemesis::Line new_line("\t\t\t\t<hkcstring>" + XmlStringEncode(alter_anim.string())
                                               + "</hkcstring>\n");
                        state.RaiseAddLineEvent(new_line, obj);
                        new_ex_lines.append(new_line);
                        state.AddAnimationToOrder(alter_anim.string());
                    }
                }

                line = nemesis::Line(
                    new_ex_lines + line.ToString(), line.GetLineNumber(), line.GetFilePathPtr());
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddAnimationToOrder(str_value);
        });

    (*handler_add) = state.InsertAddLineHandler(
        [handler_add, &state, animation_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="animationNames" numelements=)") == NOT_FOUND) return;

            auto address = *handler_add;
            *handler_add = state.InsertAddLineHandler(*animation_add);
            state.RemoveAddLineHandler(address);
        });

    DeqNstr character_lines;

    {
        std::scoped_lock<std::mutex> lock(NodeMutex);

        for (auto& node : NewNodes)
        {
            node.second->CompileTo(character_lines, state);
        }

        RegularNodes->CompileTo(character_lines, state);
    }

    DeqNstr template_lines    = CompileAllTemplates(state);
    DeqNstr subtemplate_lines = CompileAllSubTemplates(state);

    for (auto& line : subtemplate_lines)
    {
        lines.emplace_back(std::move(line));
    }

    for (auto& line : template_lines)
    {
        lines.emplace_back(std::move(line));
    }

    for (auto& line : character_lines)
    {
        lines.emplace_back(std::move(line));
    }

    lines.emplace_back("	</hksection>", lines.size() + 1);
    lines.emplace_back("", lines.size() + 1);
    lines.emplace_back("</hkpackfile>", lines.size() + 1);
}

void nemesis::HkxCharacter::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(R"(<?xml version="1.0" encoding="ascii"?>)", 1);
    lines.emplace_back(R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0-r1" toplevelobject=")"
                           + RootNode->GetNodeId() + R"(">)",
                       2);
    lines.emplace_back("", 3);
    lines.emplace_back(R"(	<hksection name="__data__">)", 4);
    lines.emplace_back("", 5);

    {
        std::scoped_lock<std::mutex> lock(NodeMutex);

        for (auto& node : NewNodes)
        {
            node.second->SerializeTo(lines);
        }

        RegularNodes->SerializeTo(lines);
    }

    lines.emplace_back("	</hksection>", lines.size() + 1);
    lines.emplace_back("", lines.size() + 1);
    lines.emplace_back("</hkpackfile>", lines.size() + 1);
}

UPtr<nemesis::NObject> nemesis::HkxCharacter::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::HkxCharacter> nemesis::HkxCharacter::Clone() const
{
    throw std::runtime_error("nemesis::HkxCharacter cannot be cloned");
}

UPtr<nemesis::HkxCharacter> nemesis::HkxCharacter::ParseFromFile(const std::filesystem::path& filepath)
{
    Logger::Log(L"HkxCharacter: " + filepath.wstring());

    UPtr<nemesis::HkxCharacter> hkxfile_uptr(new nemesis::HkxCharacter());
    hkxfile_uptr->FilePath   = filepath;
    hkxfile_uptr->TargetPath = filepath.parent_path()
                               / ((nemesis::istarts_with(filepath.filename().wstring(), L"nemesis_")
                                       ? filepath.stem().wstring().substr(8)
                                       : filepath.stem().wstring())
                                  + L".hkx");
    hkxfile_uptr->RelativePath
        = hkxfile_uptr->TargetPath.wstring().substr(NemesisInfo::DataPath().wstring().length() + 1);

    auto cache_path = filepath.parent_path() / (filepath.filename().wstring() + L".cache");

    if (std::filesystem::exists(cache_path))
    {
        hkxfile_uptr->CachedFilePath = cache_path;
    }

    PopulateContentsFromFile(*hkxfile_uptr);
    return hkxfile_uptr;
}

UPtr<nemesis::HkxCharacter> nemesis::HkxCharacter::ParseFromFile(const std::filesystem::path& filepath,
                                                                 nemesis::ThreadPool& thread_pool)
{
    Logger::Log(L"HkxCharacter: " + filepath.wstring());

    UPtr<nemesis::HkxCharacter> hkxfile_uptr(new nemesis::HkxCharacter());
    hkxfile_uptr->FilePath   = filepath;
    hkxfile_uptr->TargetPath = filepath.parent_path()
                               / ((nemesis::istarts_with(filepath.filename().wstring(), L"nemesis_")
                                       ? filepath.stem().wstring().substr(8)
                                       : filepath.stem().wstring())
                                  + L".hkx");
    hkxfile_uptr->RelativePath
        = hkxfile_uptr->TargetPath.wstring().substr(NemesisInfo::DataPath().wstring().length() + 1);

    auto cache_path = filepath.parent_path() / (filepath.filename().wstring() + L".cache");

    if (std::filesystem::exists(cache_path))
    {
        hkxfile_uptr->CachedFilePath = cache_path;
    }

    thread_pool.enqueue([hkxfile_ptr = hkxfile_uptr.get()] { PopulateContentsFromFile(*hkxfile_ptr); });
    return hkxfile_uptr;
}
