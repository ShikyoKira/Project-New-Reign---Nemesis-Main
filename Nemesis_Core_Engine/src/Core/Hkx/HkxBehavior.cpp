#include "Core/Hkx/HkxBehavior.h"
#include "Core/Hkx/HkxNode.h"

#include "Core/AnimationRequestCollection.h"
#include "Core/CompilationManager.h"
#include "Core/CompileState.h"
#include "Core/IfObject.h"

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/StringExtension.h"

UPtr<nemesis::CollectionObject> nemesis::HkxBehavior::ParseDataNodes(nemesis::LineStream& stream,
                                                                     nemesis::SemanticManager& manager,
                                                                     nemesis::HkxBehavior& behavior)
{
    auto collection = std::make_unique<nemesis::CollectionObject>();

    while (!stream.IsEoF())
    {
        if (!nemesis::HkxNode::IsDataClass(stream)) break;

        nemesis::HkxNode* node_ptr;
        auto node                               = nemesis::HkxNode::ParseHkxNode(stream, manager, node_ptr);
        behavior.NodeMap[node_ptr->GetNodeId()] = node_ptr;
        node_ptr->SetHkxDocument(&behavior);
        collection->AddObject(std::move(node));
    }

    return collection;
}

UPtr<nemesis::CollectionObject> nemesis::HkxBehavior::ParseRegularNodes(nemesis::LineStream& stream,
                                                                        nemesis::SemanticManager& manager,
                                                                        nemesis::HkxBehavior& behavior)
{
    auto collection = std::make_unique<nemesis::CollectionObject>();

    while (!stream.IsEoF())
    {
        if ((*stream).find("</hksection>") != NOT_FOUND) return collection;

        nemesis::HkxNode* node_ptr;
        auto node                               = nemesis::HkxNode::ParseHkxNode(stream, manager, node_ptr);
        behavior.NodeMap[node_ptr->GetNodeId()] = node_ptr;
        node_ptr->SetHkxDocument(&behavior);
        collection->AddObject(std::move(node));
    }

    return collection;
}

void nemesis::HkxBehavior::PopulateContentsFromFile(nemesis::HkxBehavior& behavior)
{
    Logger::Log(LITERAL_PATH("Processing HkxBehavior: ") + PATH_TO_STRING(behavior.FilePath));

    VecNstr lines;
    GetFileLines(behavior.FilePath, lines, false);
    auto start_itr = lines.begin();

    while (!std::regex_match((*start_itr).ToString(), nemesis::HkxNode::NodeIdRgx))
    {
        ++start_itr;
    }

    nemesis::SemanticManager manager;
    nemesis::LineStream stream(start_itr, lines.end());

    behavior.DataNodes     = ParseDataNodes(stream, manager, behavior);
    behavior.RegularNodes  = ParseRegularNodes(stream, manager, behavior);
    behavior.ModInUsedList = manager.GetModInUsedList();

    for (auto& node_pair : behavior.NodeMap)
    {
        if (node_pair.second->GetClassName() != "hkRootLevelContainer") continue;

        behavior.RootNode = node_pair.second;
        break;
    }
}

void nemesis::HkxBehavior::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    lines.emplace_back(R"(<?xml version="1.0" encoding="ascii"?>)", 1);
    lines.emplace_back(R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0-r1" toplevelobject=")"
                           + RootNode->GetNodeId() + R"(">)",
                       2);
    lines.emplace_back("", 3);
    lines.emplace_back(R"(	<hksection name="__data__">)", 4);
    lines.emplace_back("", 5);

    UPtr<int> obj_count_uptr = std::make_unique<int>(0);
    int* obj_count_ptr       = obj_count_uptr.get();

    UPtr<void*> uhandler_add = std::make_unique<void*>(nullptr);
    void** handler_add       = uhandler_add.get();

    auto var_info_add = std::make_shared<std::function<void(nemesis::Line&, const nemesis::NObject&)>>(
        [handler_add, &state, this, obj_count_ptr](nemesis::Line& line, const nemesis::NObject& obj)
        {
            if (line.find("<hkobject>") != NOT_FOUND)
            {
                (*obj_count_ptr)++;
                return;
            }

            if (line.find("</hkobject>") != NOT_FOUND)
            {
                (*obj_count_ptr)--;
                return;
            }

            if (line.find("</hkparam>") != NOT_FOUND && *obj_count_ptr == 0)
            {
                auto& alter_anim_repo = state.GetAlterAnimRepository();
                auto& repo            = state.GetExAnimationRepository();
                auto proj             = repo.GetProjectByBehavior(TargetPath);
                std::string new_lines;

                constexpr auto add_variable_info = [](std::string& new_lines,
                                                      const nemesis::NObject& obj,
                                                      const nemesis::CompileState& state)
                {
                    nemesis::Line new_line1("\t\t\t\t<hkobject>");
                    nemesis::Line new_line2("\t\t\t\t\t<hkparam name=\"role\">");
                    nemesis::Line new_line3("\t\t\t\t\t\t<hkobject>");
                    nemesis::Line new_line4("\t\t\t\t\t\t\t<hkparam name=\"role\">ROLE_DEFAULT</hkparam>");
                    nemesis::Line new_line5("\t\t\t\t\t\t\t<hkparam name=\"flags\">0</hkparam>");
                    nemesis::Line new_line6("\t\t\t\t\t\t</hkobject>");
                    nemesis::Line new_line7("\t\t\t\t\t</hkparam>");
                    nemesis::Line new_line8("\t\t\t\t\t<hkparam name=\"type\">VARIABLE_TYPE_INT32</hkparam>");
                    nemesis::Line new_line9("\t\t\t\t</hkobject>");

                    state.RaiseAddLineEvent(new_line1, obj);
                    state.RaiseAddLineEvent(new_line2, obj);
                    state.RaiseAddLineEvent(new_line3, obj);
                    state.RaiseAddLineEvent(new_line4, obj);
                    state.RaiseAddLineEvent(new_line5, obj);
                    state.RaiseAddLineEvent(new_line6, obj);
                    state.RaiseAddLineEvent(new_line7, obj);
                    state.RaiseAddLineEvent(new_line8, obj);
                    state.RaiseAddLineEvent(new_line9, obj);

                    new_lines.append(new_line1 + "\n");
                    new_lines.append(new_line2 + "\n");
                    new_lines.append(new_line3 + "\n");
                    new_lines.append(new_line4 + "\n");
                    new_lines.append(new_line5 + "\n");
                    new_lines.append(new_line6 + "\n");
                    new_lines.append(new_line7 + "\n");
                    new_lines.append(new_line8 + "\n");
                    new_lines.append(new_line9 + "\n");
                };

                if (proj)
                {
                    auto pack_list = proj->GetExAnimationPackList();

                    for (size_t i = 0; i < pack_list.size(); ++i)
                    {
                        add_variable_info(new_lines, obj, state);
                    }
                }

                if (alter_anim_repo.IsSupportedBehavior(TargetPath))
                {
                    for (size_t i = 0; i < alter_anim_repo.GetAlterAnimGroupList().size(); ++i)
                    {
                        add_variable_info(new_lines, obj, state);
                    }
                }

                if (nemesis::iequals(PATH_TO_STRING(TargetPath.filename()), LITERAL_PATH("build_info.hkx")))
                {
                    for (size_t i = 0; i < alter_anim_repo.GetAlterAnimGroupList().size(); ++i)
                    {
                        add_variable_info(new_lines, obj, state);
                    }

                    for (size_t i = 0; i < alter_anim_repo.GetPrefixList().size(); ++i)
                    {
                        add_variable_info(new_lines, obj, state);
                    }

                    auto& aa_group_list = alter_anim_repo.GetAlterAnimGroupList();

                    for (auto& group : aa_group_list)
                    {
                        auto& aa_list = group->GetAlterAnimationList();

                        if (aa_list.empty()) continue;

                        for (size_t i = 0; i < aa_list.front()->GetAnimationSetList().size(); ++i)
                        {
                            add_variable_info(new_lines, obj, state);
                        }
                    }

                    for (size_t i = 0; i < aa_group_list.size(); i++)
                    {
                        add_variable_info(new_lines, obj, state);
                    }
                }

                line
                    = nemesis::Line(new_lines + line.ToString(), line.GetLineNumber(), line.GetFilePathPtr());
                state.RemoveAddLineHandler(*handler_add);
                return;
            }
        });

    auto var_info_header_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, var_info_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="variableInfos" numelements=)") == NOT_FOUND) return;

            auto address = *handler_add;
            *handler_add = state.InsertAddLineHandler(*var_info_add);
            state.RemoveAddLineHandler(address);
        });

    auto var_val_add = std::make_shared<std::function<void(nemesis::Line&, const nemesis::NObject&)>>(
        [handler_add, &state, this, var_info_header_add, obj_count_ptr](nemesis::Line& line,
                                                                        const nemesis::NObject& obj)
        {
            if (line.find("<hkobject>") != NOT_FOUND)
            {
                (*obj_count_ptr)++;
                return;
            }

            if (line.find("</hkobject>") != NOT_FOUND)
            {
                (*obj_count_ptr)--;
                return;
            }

            if (line.find("</hkparam>") != NOT_FOUND && *obj_count_ptr == 0)
            {
                auto& alter_anim_repo = state.GetAlterAnimRepository();
                auto& repo            = state.GetExAnimationRepository();
                auto proj             = repo.GetProjectByBehavior(TargetPath);
                std::string new_lines;

                constexpr auto add_variable_val = [](std::string& new_lines,
                                                     const std::string& val,
                                                     const nemesis::NObject& obj,
                                                     const nemesis::CompileState& state)
                {
                    nemesis::Line new_line1("\t\t\t\t<hkobject>");
                    nemesis::Line new_line2("\t\t\t\t\t<hkparam name=\"value\">" + val + "</hkparam>");
                    nemesis::Line new_line3("\t\t\t\t</hkobject>");

                    state.RaiseAddLineEvent(new_line1, obj);
                    state.RaiseAddLineEvent(new_line2, obj);
                    state.RaiseAddLineEvent(new_line3, obj);

                    new_lines.append(new_line1 + "\n");
                    new_lines.append(new_line2 + "\n");
                    new_lines.append(new_line3 + "\n");
                };

                if (proj)
                {
                    auto pack_list = proj->GetExAnimationPackList();

                    for (size_t i = 0; i < pack_list.size(); ++i)
                    {
                        add_variable_val(new_lines, "0", obj, state);
                    }
                }

                if (alter_anim_repo.IsSupportedBehavior(TargetPath))
                {
                    for (size_t i = 0; i < alter_anim_repo.GetAlterAnimGroupList().size(); ++i)
                    {
                        add_variable_val(new_lines, "0", obj, state);
                    }
                }

                if (nemesis::iequals(PATH_TO_STRING(TargetPath.filename()), LITERAL_PATH("build_info.hkx")))
                {
                    for (size_t i = 0; i < alter_anim_repo.GetAlterAnimGroupList().size(); ++i)
                    {
                        add_variable_val(new_lines, "0", obj, state);
                    }

                    for (auto& prefix : alter_anim_repo.GetPrefixList())
                    {
                        add_variable_val(new_lines, std::to_string(prefix.GetId()), obj, state);
                    }

                    auto& aa_group_list = alter_anim_repo.GetAlterAnimGroupList();

                    for (auto& group : aa_group_list)
                    {
                        auto& aa_list = group->GetAlterAnimationList();

                        if (aa_list.empty()) continue;

                        size_t slot_index = 1;

                        for (auto& aa_set : aa_list.front()->GetAnimationSetList())
                        {
                            add_variable_val(new_lines, std::to_string(slot_index), obj, state);
                            slot_index += aa_set->GetSize();
                        }
                    }

                    for (auto& group : aa_group_list)
                    {
                        add_variable_val(new_lines, std::to_string(group->GetId()), obj, state);
                    }
                }

                line
                    = nemesis::Line(new_lines + line.ToString(), line.GetLineNumber(), line.GetFilePathPtr());

                auto address = *handler_add;
                *handler_add = state.InsertAddLineHandler(*var_info_header_add);
                state.RemoveAddLineHandler(address);
                return;
            }
        });

    auto var_val_header_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, var_val_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="wordVariableValues" numelements=)") == NOT_FOUND) return;

            auto address = *handler_add;
            *handler_add = state.InsertAddLineHandler(*var_val_add);
            state.RemoveAddLineHandler(address);
        });

    auto property_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, var_val_header_add](nemesis::Line& line)
        {
            if (line.find("</hkparam>") != NOT_FOUND)
            {
                auto address = *handler_add;
                *handler_add = state.InsertAddLineHandler(*var_val_header_add);
                state.RemoveAddLineHandler(address);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddPropertyName(str_value);
        });

    auto variable_add = std::make_shared<std::function<void(nemesis::Line&, const nemesis::NObject&)>>(
        [handler_add, &state, property_add, this](nemesis::Line& line, const nemesis::NObject& obj)
        {
            if (line.find("</hkparam>") != NOT_FOUND)
            {
                auto& alter_anim_repo = state.GetAlterAnimRepository();
                auto& repo            = state.GetExAnimationRepository();
                auto proj             = repo.GetProjectByBehavior(TargetPath);
                std::string new_lines;

                if (proj)
                {
                    auto pack_list = proj->GetExAnimationPackList();

                    for (auto& pack : pack_list)
                    {
                        nemesis::Line new_line("\t\t\t\t<hkcstring>" + pack->GetVariableName()
                                               + "</hkcstring>");
                        state.RaiseAddLineEvent(new_line, obj);
                        new_lines.append(new_line + "\n");
                    }
                }

                if (alter_anim_repo.IsSupportedBehavior(TargetPath))
                {
                    for (auto& group : alter_anim_repo.GetAlterAnimGroupList())
                    {
                        nemesis::Line new_line("\t\t\t\t<hkcstring>" + group->GetVariableName()
                                               + "</hkcstring>");
                        state.RaiseAddLineEvent(new_line, obj);
                        new_lines.append(new_line + "\n");
                    }
                }

                if (nemesis::iequals(PATH_TO_STRING(TargetPath.filename()), LITERAL_PATH("build_info.hkx")))
                {
                    auto checksum = std::to_string(state.GetManager().GetFullCheckSum());

                    for (auto& group : alter_anim_repo.GetAlterAnimGroupList())
                    {
                        nemesis::Line new_line("\t\t\t\t<hkcstring>FNISaa" + group->GetName()
                                               + "_crc</hkcstring>");
                        state.RaiseAddLineEvent(new_line, obj);
                        new_lines.append(new_line + "\n");
                    }

                    for (auto& prefix : alter_anim_repo.GetPrefixList())
                    {
                        nemesis::Line new_line("\t\t\t\t<hkcstring>Nemesis_AA_Mod_" + prefix.GetName() + "_"
                                               + checksum + "</hkcstring>");
                        state.RaiseAddLineEvent(new_line, obj);
                        new_lines.append(new_line + "\n");
                    }

                    auto& aa_group_list = alter_anim_repo.GetAlterAnimGroupList();

                    for (auto& group : aa_group_list)
                    {
                        auto& aa_list = group->GetAlterAnimationList();

                        if (aa_list.empty()) continue;

                        for (auto& aa_set : aa_list.front()->GetAnimationSetList())
                        {
                            nemesis::Line new_line(
                                "\t\t\t\t<hkcstring>Nemesis_AA_Group_" + std::to_string(group->GetId())
                                + "_Mod_"
                                + std::to_string(
                                    alter_anim_repo.GetPrefixByName(aa_set->GetPrefix())->GetId())
                                + "_" + checksum + "</hkcstring>");
                            state.RaiseAddLineEvent(new_line, obj);
                            new_lines.append(new_line + "\n");
                        }
                    }

                    for (auto& group : aa_group_list)
                    {
                        nemesis::Line new_line("\t\t\t\t<hkcstring>Nemesis_AA_" + group->GetName() + "_"
                                               + checksum + "</hkcstring>");
                        state.RaiseAddLineEvent(new_line, obj);
                        new_lines.append(new_line + "\n");
                    }
                }

                line
                    = nemesis::Line(new_lines + line.ToString(), line.GetLineNumber(), line.GetFilePathPtr());
                return;
            }
            else if (line.find(R"(<hkparam name="characterPropertyNames" numelements=)") != NOT_FOUND)
            {
                auto address = *handler_add;
                *handler_add = state.InsertAddLineHandler(*property_add);
                state.RemoveAddLineHandler(address);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddVariableName(str_value);
        });

    auto attribute_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, variable_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="variableNames" numelements=)") != NOT_FOUND)
            {
                auto address = *handler_add;
                *handler_add = state.InsertAddLineHandler(*variable_add);
                state.RemoveAddLineHandler(address);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddAttributeName(str_value);
        });

    auto event_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, attribute_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="attributeNames" numelements=)") != NOT_FOUND)
            {
                auto address = *handler_add;
                *handler_add = state.InsertAddLineHandler(*attribute_add);
                state.RemoveAddLineHandler(address);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddEventName(str_value);
        });

    (*handler_add) = state.InsertAddLineHandler(
        [handler_add, &state, event_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="eventNames" numelements=)") == NOT_FOUND) return;

            auto address = *handler_add;
            *handler_add = state.InsertAddLineHandler(*event_add);
            state.RemoveAddLineHandler(address);
        });

    DeqNstr behavior_lines;

    {
        std::scoped_lock<std::mutex> lock(NodeMutex);
        DataNodes->CompileTo(lines, state);

        for (auto& node : NewNodes)
        {
            node.second->CompileTo(lines, state);
        }

        RegularNodes->CompileTo(behavior_lines, state);
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

    for (auto& line : behavior_lines)
    {
        lines.emplace_back(std::move(line));
    }

    lines.emplace_back("	</hksection>", lines.size() + 1);
    lines.emplace_back("", lines.size() + 1);
    lines.emplace_back("</hkpackfile>", lines.size() + 1);
}

void nemesis::HkxBehavior::SerializeTo(DeqNstr& lines) const
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
        DataNodes->SerializeTo(lines);

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

UPtr<nemesis::NObject> nemesis::HkxBehavior::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::Clone() const
{
    throw std::runtime_error("nemesis::HkxBehavior cannot be cloned");
}

size_t nemesis::HkxBehavior::GetSize() const
{
    return DataNodes->Size() + NodeMap.size() + RegularNodes->Size();
}

UPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::ParseFromFile(const std::filesystem::path& filepath)
{
    Logger::Log(LITERAL_PATH("HkxBehavior: ") + PATH_TO_STRING(filepath));

    UPtr<nemesis::HkxBehavior> hkxfile_uptr(new nemesis::HkxBehavior());
    auto filename            = PATH_TO_STRING(filepath.filename());
    hkxfile_uptr->FilePath   = filepath;
    hkxfile_uptr->TargetPath = filepath.parent_path()
                               / ((nemesis::istarts_with(filename, LITERAL_PATH("nemesis_"))
                                       ? PATH_TO_STRING(filepath.stem()).substr(8)
                                       : PATH_TO_STRING(filepath.stem()))
                                  + LITERAL_PATH(".hkx"));
    hkxfile_uptr->RelativePath = PATH_TO_STRING(hkxfile_uptr->TargetPath)
                                     .substr(PATH_TO_STRING(NemesisInfo::DataPath()).length() + 1);

    auto cache_path = filepath;
    cache_path.replace_extension(LITERAL_PATH(".cache"));

    if (std::filesystem::exists(cache_path))
    {
        hkxfile_uptr->CachedFilePath = cache_path;
    }

    PopulateContentsFromFile(*hkxfile_uptr);
    return hkxfile_uptr;
}

UPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::ParseFromFile(const std::filesystem::path& filepath,
                                                               nemesis::ThreadPool& thread_pool)
{
    Logger::Log(LITERAL_PATH("HkxBehavior: ") + PATH_TO_STRING(filepath));

    UPtr<nemesis::HkxBehavior> hkxfile_uptr(new nemesis::HkxBehavior());
    auto filename            = PATH_TO_STRING(filepath.filename());
    hkxfile_uptr->FilePath   = filepath;
    hkxfile_uptr->TargetPath = filepath.parent_path()
                               / ((nemesis::istarts_with(filename, LITERAL_PATH("nemesis_"))
                                       ? PATH_TO_STRING(filepath.stem()).substr(8)
                                       : PATH_TO_STRING(filepath.stem()))
                                  + LITERAL_PATH(".hkx"));
    hkxfile_uptr->RelativePath = PATH_TO_STRING(hkxfile_uptr->TargetPath)
                                     .substr(PATH_TO_STRING(NemesisInfo::DataPath()).length() + 1);

    auto cache_path = filepath;
    cache_path.replace_extension(LITERAL_PATH(".cache"));

    if (std::filesystem::exists(cache_path))
    {
        hkxfile_uptr->CachedFilePath = cache_path;
    }

    thread_pool.enqueue([hkxfile_ptr = hkxfile_uptr.get()] { PopulateContentsFromFile(*hkxfile_ptr); });
    return hkxfile_uptr;
}
