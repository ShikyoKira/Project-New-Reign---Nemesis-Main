#include "AlternateAnimation/AlterAnim.h"
#include "AlternateAnimation/AlterAnimGroup.h"

#include "Core/CompileState.h"

nemesis::AlterAnim::AlterAnim(const nemesis::AlterAnimGroup& group,
                              const std::filesystem::path& anim_path) noexcept
    : AnimPath(anim_path)
    , Group(group)
{
}

void nemesis::AlterAnim::AddAlternateSlots(const std::filesystem::path& relative_dir_path,
                                           const std::filesystem::path& dir_path,
                                           const std::string& prefix,
                                           size_t slot_size)
{
    PrefixSet.emplace(
        prefix,
        std::make_unique<nemesis::AlterAnimSet>(relative_dir_path, dir_path, AnimPath, prefix, slot_size));

    std::scoped_lock<std::mutex> lock(PrefixSetCacheMutex);
    PrefixSetCache = nullptr;
}

VecStr nemesis::AlterAnim::CompileAsXml(
    std::string& node_id,
    nemesis::CompileState& state,
    const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>& add_clip_func) const
{
    size_t count = 0;
    VecStr lines;
    VecStr alter_node_list;
    Vec<const std::filesystem::path*> alternate_list;

    for (auto& alter_list : PrefixSet)
    {
        for (auto alter : alter_list.second->GetSlots())
        {
            alter_node_list.emplace_back(alter == nullptr ? ""
                                                          : node_id + "-aa-" + std::to_string(count++));
            alternate_list.emplace_back(alter);
        }
    }

    if (count == 0) return lines;

    std::string msg_node_id = node_id;
    std::string var_node_id = node_id + "-aa-" + std::to_string(count++);
    node_id = node_id + "-aa-" + std::to_string(count++);

    for (size_t i = 0; i < alternate_list.size(); ++i)
    {
        if (!alternate_list[i]) continue;

        add_clip_func(lines, alter_node_list[i], *alternate_list[i]);
    }

    lines.reserve(lines.size() + alter_node_list.size() + 23);
    lines.emplace_back("\t\t<hkobject name=\"" + var_node_id
                       + "\" class=\"hkbVariableBindingSet\" signature=\"0x338ad4ff\">");
    lines.emplace_back("\t\t\t<hkparam name=\"bindings\" numelements=\"1\">");
    lines.emplace_back("\t\t\t\t<hkobject>");
    lines.emplace_back("\t\t\t\t\t<hkparam name=\"memberPath\">selectedGeneratorIndex</hkparam>");
    lines.emplace_back("\t\t\t\t\t<hkparam name=\"variableIndex\">"
                       + state.GetVariableID(Group.GetVariableName())
                       + "</hkparam>");
    lines.emplace_back("\t\t\t\t\t<hkparam name=\"bitIndex\">-1</hkparam>");
    lines.emplace_back("\t\t\t\t\t<hkparam name=\"bindingType\">BINDING_TYPE_VARIABLE</hkparam>");
    lines.emplace_back("\t\t\t\t</hkobject>");
    lines.emplace_back("\t\t\t</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"indexOfBindingToEnable\">-1</hkparam>");
    lines.emplace_back("\t\t</hkobject>");
    lines.emplace_back("");

    lines.emplace_back("\t\t<hkobject name=\"" + msg_node_id
                       + "\" class=\"hkbManualSelectorGenerator\" signature=\"0xd932fab8\">");
    lines.emplace_back("\t\t\t<hkparam name=\"variableBindingSet\">" + var_node_id + "</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"userData\">0</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"name\">AlterAnimationMSG_" + msg_node_id + "</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"generators\" numelements=\""
                       + std::to_string(alter_node_list.size() + 1) + "\">");
    lines.emplace_back("\t\t\t\t" + node_id);

    for (auto& alter_node_id : alter_node_list)
    {
        lines.emplace_back("\t\t\t\t" + (alter_node_id.empty() ? node_id : alter_node_id));
    }

    lines.emplace_back("\t\t\t</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"selectedGeneratorIndex\">0</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"currentGeneratorIndex\">0</hkparam>");
    lines.emplace_back("\t\t</hkobject>");
    lines.emplace_back("");
    return lines;
}

const std::filesystem::path nemesis::AlterAnim::GetAnimPath() const noexcept
{
    return AnimPath;
}

const Vec<const nemesis::AlterAnimSet*>& nemesis::AlterAnim::GetAnimationSetList() const noexcept
{
    std::scoped_lock<std::mutex> lock(PrefixSetCacheMutex);

    if (PrefixSetCache) return *PrefixSetCache;

    PrefixSetCache = std::make_unique<Vec<const nemesis::AlterAnimSet*>>();

    for (auto& prefix : PrefixSet)
    {
        PrefixSetCache->emplace_back(prefix.second.get());
    }

    return *PrefixSetCache;
}

const nemesis::AlterAnimGroup& nemesis::AlterAnim::GetGroup() const noexcept
{
    return Group;
}
