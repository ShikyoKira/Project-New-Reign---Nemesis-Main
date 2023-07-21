#include "hkx/HkxBehavior.h"
#include "hkx/HkxNode.h"

#include "core/IfObject.h"
#include "core/CompileState.h"
#include "core/AnimationRequestCollection.h"

bool TryGetValueInHkcString(const std::string& line, std::string& value)
{
    size_t pos = line.find("<hkcstring>");

    if (pos == NOT_FOUND) return false;

    size_t cpos = line.find("</hkcstring>", pos);

    if (cpos == NOT_FOUND) return false;

    value = line.substr(pos + 11, cpos - pos - 11);
    return true;
}

UPtr<nemesis::CollectionObject>
nemesis::HkxBehavior::ParseDataNodes(nemesis::LineStream& stream,
                                     nemesis::SemanticManager& manager,
                                     Map<std::string, nemesis::HkxNode*>& node_map)
{
    auto collection = std::make_unique<nemesis::CollectionObject>();

    while (!stream.IsEoF())
    {
        if (!nemesis::HkxNode::IsDataClass(stream)) break;

        nemesis::HkxNode* node_ptr;
        auto node         = nemesis::HkxNode::ParseHkxNode(stream, manager, node_ptr);
        node_map[node_ptr->GetNodeId()] = node_ptr;
        collection->AddObject(std::move(node));
    }

    return collection;
}

UPtr<nemesis::CollectionObject>
nemesis::HkxBehavior::ParseRegularNodes(nemesis::LineStream& stream,
                                        nemesis::SemanticManager& manager,
                                        Map<std::string, nemesis::HkxNode*>& node_map)
{
    auto collection = std::make_unique<nemesis::CollectionObject>();

    while (!stream.IsEoF())
    {
        if ((*stream).find("</hksection>") != NOT_FOUND) return collection;

        nemesis::HkxNode* node_ptr;
        auto node = nemesis::HkxNode::ParseHkxNode(stream, manager, node_ptr);
        node_map[node_ptr->GetNodeId()] = node_ptr;
        collection->AddObject(std::move(node));
    }

    return collection;
}

void nemesis::HkxBehavior::CompileTemplate(const nemesis::AnimationRequest* request,
                                           DeqNstr& lines,
                                           nemesis::CompileState& state) const
{
    auto list_itr = TemplateMap.find(request->GetTemplateName());

    if (list_itr == TemplateMap.end()) return;

    auto obj_itr = list_itr->second.find(request->GetLevel());

    if (obj_itr == list_itr->second.end()) return;

    state.SetBaseRequest(request);
    obj_itr->second->CompileTo(lines, state);

    auto requests = request->GetRequests();

    for (auto& request : requests)
    {
        CompileTemplate(request, lines, state);
    }
}

DeqNstr nemesis::HkxBehavior::CompileAllTemplates(nemesis::CompileState& state) const
{
    DeqNstr template_lines;

    for (auto& templt_list : TemplateMap)
    {
        auto& requests = state.GetRequests(templt_list.first);

        if (!requests.empty())
        {
            auto itr = templt_list.second.find(0);

            if (itr != templt_list.second.end())
            {
                itr->second->CompileTo(template_lines, state);
            }
        }

        for (auto& request : requests)
        {
            CompileTemplate(request.get(), template_lines, state);
        }
    }

    return template_lines;
}

DeqNstr nemesis::HkxBehavior::CompileAllSubTemplates(nemesis::CompileState& state) const
{
    DeqNstr lines;
    auto subrequests = state.GetSubTemplateRequestList();

    if (subrequests.empty()) return lines;

    Vec<UPtr<nemesis::CompileState>> state_list;
    USetStr checker;

    for (auto& request : subrequests)
    {
        auto new_state = std::make_unique<nemesis::CompileState>(state);
        new_state->SetCurrentSubTemplateRequest(*request);
        auto template_name = request->GetArgument(1);
        auto itr           = SubTemplateNodes.find(template_name);

        if (itr == SubTemplateNodes.end())
        {
            throw std::runtime_error("SubTemplate cannot be found (\"" + template_name + "\")");
        }

        checker.insert(request->GetArgument(0));
        itr->second->CompileTo(lines, *new_state);
        lines.emplace_back("");

        if (new_state->GetSubTemplateRequestList().empty()) continue;

        state_list.emplace_back(std::move(new_state));
    }

    for (size_t i = 0; i < state_list.size(); i++)
    {
        auto cur_state = state_list[i].get();

        for (auto& request : cur_state->GetSubTemplateRequestList())
        {
            auto& id = request->GetArgument(0);

            if (checker.find(id) != checker.end()) continue;

            auto new_state = std::make_unique<nemesis::CompileState>(state);
            new_state->SetCurrentSubTemplateRequest(*request);
            auto template_name = request->GetArgument(1);
            auto itr           = SubTemplateNodes.find(template_name);

            if (itr == SubTemplateNodes.end())
            {
                throw std::runtime_error("SubTemplate cannot be found (\"" + template_name + "\")");
            }

            checker.insert(id);
            itr->second->CompileTo(lines, *new_state);
            lines.emplace_back("");

            if (new_state->GetSubTemplateRequestList().empty()) continue;

            state_list.emplace_back(std::move(new_state));
        }
    }

    return lines;
}

void nemesis::HkxBehavior::UpdateNodeFromFile(const std::filesystem::path& filepath,
                                              nemesis::SemanticManager& manager)
{
    auto hkx_node = nemesis::HkxNode::ParseHkxNodeFromFile(filepath, manager);
    auto node     = NodeMap.find(hkx_node->GetNodeId());

    if (node == NodeMap.end())
    {
        throw std::runtime_error("Node ID cannot be found (Behavior File: " + FilePath.string()
                                 + ", Node File: " + filepath.string() + ")");
    }

    node->second->MatchAndUpdate(std::move(hkx_node));

    auto& mod_list = manager.GetModInUsedList();
    ModInUsedList.insert(mod_list.begin(), mod_list.end());
}

void nemesis::HkxBehavior::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    lines.emplace_back(R"(<?xml version="1.0" encoding="ascii"?>)", 1);
    lines.emplace_back(R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0 - r1" toplevelobject=")"
                           + RootNode->GetNodeId() + R"(">)",
                       2);
    lines.emplace_back("", 3);
    lines.emplace_back(R"(	<hksection name="__data__">)", 4);
    lines.emplace_back("", 5);

    SPtr<uintptr_t> handler_add = std::make_shared<uintptr_t>(0);
    auto hkxvalue_rgx = std::make_shared<nemesis::regex>("^\\s*<hkcstring>(.+?)<\\/hkcstring>.*$");

    auto property_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, hkxvalue_rgx](nemesis::Line& line)
        {
            if (line.find("</hkparam>") != NOT_FOUND)
            {
                state.RemoveAddLineHandler(*handler_add);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddPropertyName(str_value);
        });
    
    auto variable_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, hkxvalue_rgx, property_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="characterPropertyNames" numelements=)") != NOT_FOUND)
            {
                state.RemoveAddLineHandler(*handler_add);
                (*handler_add) = state.InsertAddLineHandler(*property_add);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddVariableName(str_value);
        });
    
    auto attribute_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, hkxvalue_rgx, variable_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="variableNames" numelements=)") != NOT_FOUND)
            {
                state.RemoveAddLineHandler(*handler_add);
                (*handler_add) = state.InsertAddLineHandler(*variable_add);
                return;
            }

            std::string str_value;

            if (!TryGetValueInHkcString(line, str_value)) return;

            state.AddAttributeName(str_value);
        });

    auto event_add = std::make_shared<std::function<void(nemesis::Line&)>>(
        [handler_add, &state, hkxvalue_rgx, attribute_add](nemesis::Line& line)
        {
            if (line.find(R"(<hkparam name="attributeNames" numelements=)") != NOT_FOUND)
            {
                state.RemoveAddLineHandler(*handler_add);
                (*handler_add) = state.InsertAddLineHandler(*attribute_add);
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

            state.RemoveAddLineHandler(*handler_add);
            (*handler_add) = state.InsertAddLineHandler(*event_add);
        });

    DeqNstr behavior_lines;
    DataNodes->CompileTo(lines, state);
    RegularNodes->CompileTo(behavior_lines, state);
    lines.emplace_back("", lines.size() + 1);

    DeqNstr template_lines    = CompileAllTemplates(state);
    DeqNstr subtemplate_lines = CompileAllSubTemplates(state);

    lines.insert(lines.end(),
                 std::make_move_iterator(subtemplate_lines.begin()),
                 std::make_move_iterator(subtemplate_lines.end()));
    lines.insert(lines.end(),
                 std::make_move_iterator(template_lines.begin()),
                 std::make_move_iterator(template_lines.end()));
    lines.insert(lines.end(),
                 std::make_move_iterator(behavior_lines.begin()),
                 std::make_move_iterator(behavior_lines.end()));

    lines.emplace_back("	</hksection>", lines.size() + 1);
    lines.emplace_back("", lines.size() + 1);
    lines.emplace_back("</hkpackfile>", lines.size() + 1);
}

void nemesis::HkxBehavior::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(R"(<?xml version="1.0" encoding="ascii"?>)", 1);
    lines.emplace_back(R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0 - r1" toplevelobject=")"
                           + RootNode->GetNodeId() + R"(">)",
                       2);
    lines.emplace_back("", 3);
    lines.emplace_back(R"(	<hksection name="__data__">)", 4);
    lines.emplace_back("", 5);

    DataNodes->SerializeTo(lines);
    lines.emplace_back("", lines.size() + 1);
    RegularNodes->SerializeTo(lines);

    lines.emplace_back("	</hksection>", lines.size() + 1);
    lines.emplace_back("", lines.size() + 1);
    lines.emplace_back("</hkpackfile>", lines.size() + 1);
}

void nemesis::HkxBehavior::AddTemplate(const SPtr<nemesis::TemplateObject>& templt_obj)
{
    auto& class_name  = templt_obj->GetClassName();
    std::string level = templt_obj->GetFilePath().stem().string().substr(class_name.length() + 1);
    TemplateMap[templt_obj->GetClassName()][std::stoi(level)] = templt_obj;
}

void nemesis::HkxBehavior::AddSubTemplateNode(const SPtr<nemesis::SubTemplateObject>& node)
{
    SubTemplateNodes[node->GetName()] = node;
}

void nemesis::HkxBehavior::UpdateNodeFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    UpdateNodeFromFile(filepath, manager);
}

void nemesis::HkxBehavior::UpdateNodeFromFile(const std::filesystem::path& filepath,
                                              const nemesis::TemplateClass* templt_class)
{
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(templt_class);
    UpdateNodeFromFile(filepath, manager);
}

nemesis::HkxNode* nemesis::HkxBehavior::GetNodeById(const std::string& node_id)
{
    auto itr = NodeMap.find(node_id);

    if (itr == NodeMap.end()) return nullptr;

    return itr->second;
}

const nemesis::HkxNode* nemesis::HkxBehavior::GetNodeById(const std::string& node_id) const
{
    auto itr = NodeMap.find(node_id);

    if (itr == NodeMap.end()) return nullptr;

    return itr->second;
}

const std::filesystem::path& nemesis::HkxBehavior::GetFilePath() const noexcept
{
    return FilePath;
}

const std::filesystem::path& nemesis::HkxBehavior::GetCachedFilePath() const noexcept
{
    return CachedFilePath;
}

bool nemesis::HkxBehavior::IsSameAsCached(nemesis::CompileState& state) const
{
    if (!std::filesystem::exists(CachedFilePath)) return false;

    auto mods = state.GetSelectedMods();

    for (auto& mod : mods)
    {
        if (ModInUsedList.find(mod) != ModInUsedList.end()) return false;
    }

    for (auto& templt_list : TemplateMap)
    {
        auto& requests = state.GetRequests(templt_list.first);

        if (!requests.empty()) return false;
    }

    return true;
}

UPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::CreateFromFile(const std::filesystem::path& filepath)
{
    UPtr<nemesis::HkxBehavior> behavior_uptr(new nemesis::HkxBehavior());
    nemesis::HkxBehavior* behavior_ptr = behavior_uptr.get();
    behavior_uptr->FilePath            = filepath;

    auto cache_path = filepath.parent_path() / (filepath.filename().string() + ".cache");

    if (std::filesystem::exists(cache_path))
    {
        behavior_uptr->CachedFilePath = cache_path;
    }

    VecNstr lines;
    GetFileLines(filepath, lines, false);
    auto start_itr = lines.begin();

    while (!nemesis::regex_match(*start_itr, nemesis::HkxNode::NodeIdRgx))
    {
        ++start_itr;
    }

    nemesis::SemanticManager manager;
    nemesis::LineStream stream(start_itr, lines.end());

    behavior_ptr->DataNodes     = ParseDataNodes(stream, manager, behavior_ptr->NodeMap);
    behavior_ptr->RegularNodes  = ParseRegularNodes(stream, manager, behavior_ptr->NodeMap);
    behavior_ptr->ModInUsedList = manager.GetModInUsedList();

    for (auto& node_pair : behavior_ptr->NodeMap)
    {
        if (node_pair.second->GetClassName() != "hkRootLevelContainer") continue;

        behavior_ptr->RootNode = node_pair.second;
        break;
    }

    return behavior_uptr;
}

UPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::CreateFromFile(const std::filesystem::path& filepath,
                                                                nemesis::ThreadPool& thread_pool)
{
    UPtr<nemesis::HkxBehavior> behavior_uptr(new nemesis::HkxBehavior());
    nemesis::HkxBehavior* behavior_ptr = behavior_uptr.get();
    behavior_uptr->FilePath            = filepath;

    thread_pool.enqueue(
        [behavior_ptr]() {
            VecNstr lines;
            GetFileLines(behavior_ptr->FilePath, lines, false);
            auto start_itr = lines.begin();

            while (!nemesis::regex_match(*start_itr, nemesis::HkxNode::NodeIdRgx))
            {
                ++start_itr;
            }

            nemesis::SemanticManager manager;
            nemesis::LineStream stream(start_itr, lines.end());

            behavior_ptr->DataNodes     = ParseDataNodes(stream, manager, behavior_ptr->NodeMap);
            behavior_ptr->RegularNodes  = ParseRegularNodes(stream, manager, behavior_ptr->NodeMap);
            behavior_ptr->ModInUsedList = manager.GetModInUsedList();

            for (auto& node_pair : behavior_ptr->NodeMap)
            {
                if (node_pair.second->GetClassName() != "hkRootLevelContainer") continue;

                behavior_ptr->RootNode = node_pair.second;
                break;
            }

        });

    return behavior_uptr;
}
