#include "ExAnimation/LinkedExAnimList.h"

#include "Core/CompileState.h"

void nemesis::LinkedExAnimList::CompileAsXmlTo(
    VecStr& lines,
    std::string& node_id,
    size_t count,
    nemesis::CompileState& state,
    const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>& add_clip_func) const
{
    std::string msg_node_id  = (count == 0 ? node_id : node_id + "-ex-" + std::to_string(count++));
    std::string var_node_id  = node_id + "-ex-" + std::to_string(count++);
    std::string clip_node_id = node_id + "-ex-" + std::to_string(count++);
    std::string next_node_id = node_id + "-ex-" + std::to_string(count);

    add_clip_func(lines, clip_node_id, Config->AnimPath);
    lines.reserve(lines.size() + 23);
    lines.emplace_back("\t\t<hkobject name=\"" + var_node_id
                       + "\" class=\"hkbVariableBindingSet\" signature=\"0x338ad4ff\">");
    lines.emplace_back("\t\t\t<hkparam name=\"bindings\" numelements=\"1\">");
    lines.emplace_back("\t\t\t\t<hkobject>");
    lines.emplace_back("\t\t\t\t\t<hkparam name=\"memberPath\">selectedGeneratorIndex</hkparam>");
    lines.emplace_back("\t\t\t\t\t<hkparam name=\"variableIndex\">" + state.GetVariableID(Config->Variable)
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
    lines.emplace_back("\t\t\t<hkparam name=\"name\">ExAnimationMSG_" + msg_node_id + "</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"generators\" numelements=\"2\">");
    lines.emplace_back("\t\t\t\t" + next_node_id + " " + clip_node_id);
    lines.emplace_back("\t\t\t</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"selectedGeneratorIndex\">0</hkparam>");
    lines.emplace_back("\t\t\t<hkparam name=\"currentGeneratorIndex\">0</hkparam>");
    lines.emplace_back("\t\t</hkobject>");
    lines.emplace_back("");

    if (!Next)
    {
        node_id = next_node_id;
        return;
    }

    Next->CompileAsXmlTo(lines, node_id, count, state, add_clip_func);
}

void nemesis::LinkedExAnimList::SetupConfig(const nemesis::ExAnimationRequest& request)
{
    auto& pack = request.GetPack();
    Config     = UPtr<nemesis::LinkedExAnimList::ExAnimConfig>(new nemesis::LinkedExAnimList::ExAnimConfig());
    Config->Variable = pack.GetVariableName();
    Config->Order    = pack.GetOrder();
    Config->AnimPath = request.GetExAnimPath();
}

nemesis::LinkedExAnimList::LinkedExAnimList(nemesis::LinkedExAnimList& linked_ex_anim)
{
    Config = std::move(linked_ex_anim.Config);
}

nemesis::LinkedExAnimList::LinkedExAnimList(const nemesis::ExAnimationRequest& request)
{
    SetupConfig(request);
}

void nemesis::LinkedExAnimList::Add(const nemesis::ExAnimationRequest& request)
{
    uint8_t order = request.GetPack().GetOrder();

    if (order < Config->Order)
    {
        Next = std::make_unique<nemesis::LinkedExAnimList>(request);
        return;
    }
    else if (order > Config->Order)
    {
        Next = UPtr<nemesis::LinkedExAnimList>(new nemesis::LinkedExAnimList(*this));
    }

    SetupConfig(request);
}

VecStr nemesis::LinkedExAnimList::CompileAsXml(
    std::string& node_id,
    nemesis::CompileState& state,
    const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>& add_clip_func) const
{
    VecStr lines;
    CompileAsXmlTo(lines, node_id, 0, state, add_clip_func);
    return lines;
}

void nemesis::LinkedExAnimList::CompileAsXmlTo(
    VecStr& lines,
    std::string& node_id,
    nemesis::CompileState& state,
    const std::function<void(VecStr&, const std::string&, const std::filesystem::path&)>& add_clip_func) const
{
    CompileAsXmlTo(lines, node_id, 0, state, add_clip_func);
}
