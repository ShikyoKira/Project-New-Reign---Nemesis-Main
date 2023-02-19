#include "hkx/base/hkxparserbase.h"

#include "utilities/stringextension.h"

UPtr<nemesis::HkxParserBase::Vector4Info>
nemesis::HkxParserBase::Vector4Info::TryCreate(nemesis::LinkedPreprocessLine& linkedline)
{
    auto raw = linkedline.GetRawPtr();

    if (!raw) return nullptr;

    std::string_view vline = raw->ToString();
    size_t pos             = vline.find(hkxvec_prefix);
    size_t pos2;

    if (pos != NOT_FOUND)
    {
        pos += hkxvec_prefix.size();
        pos2 = vline.find(hkxvec_suffix, pos);
    }
    else
    {
        pos = vline.find(vec4_prefix);

        if (pos == NOT_FOUND) return nullptr;

        pos += vec4_prefix.size();
        pos2 = vline.find(vec4_suffix, pos);
    }

    if (pos2 == NOT_FOUND) return nullptr;

    std::string_view vline2 = vline.substr(pos, pos2 - pos);
    short count             = 0;
    pos--;
    pos2++;

    for (auto& ch : vline2)
    {
        switch (ch)
        {
            case ' ':
                count++;
            case '\t':
            case '.':
            case '-':
                break;
            default:
                if (!isdigit(ch)) return nullptr;

                break;
        }
    }

    if (count != 3) return nullptr;

    std::string spaces;
    auto linenum  = raw->GetLineNumber();
    auto filepath = raw->GetFilePath();
    auto vec4     = std::make_unique<nemesis::HkxParserBase::Vector4Info>();
    auto prefix   = std::string(vline.substr(0, pos));

    vec4->ishkx = prefix != std::string(element_spaces);

    if (vec4->ishkx)
    {
        for (auto& ch : vline)
        {
            if (ch != '\t') break;

            spaces.push_back(ch);
        }

        vec4->prefix = nemesis::Line(prefix, linenum, filepath);
        vec4->suffix = nemesis::Line(spaces + std::string(vline.substr(pos2)), linenum, filepath);
        spaces.push_back('\t');
    }
    else
    {
        spaces = std::string(element_spaces);
    }

    for (auto& element : StringSplit(std::string(vline2)))
    {
        nemesis::Line ele(spaces + element, linenum, filepath);
        vec4->elements.emplace_back(ele);
    }

    return std::move(vec4);
}

bool nemesis::HkxParserBase::IsElement(std::string_view vline)
{
    if (!read_numele)
    {
        if (!vline._Starts_with(paramstart_str)) return false;

        auto pos = vline.find(numele_str, paramstart_str.size() + 1);
        
        if (pos == NOT_FOUND) return false;

        if (vline.find(paramend_str_nospc, pos + numele_str.size()) != NOT_FOUND) return false;

        read_numele = true;
        return false;
    }

    if (vline._Starts_with(paramend_str))
    {
        read_numele = false;
    }

    return read_numele;
}

bool nemesis::HkxParserBase::IsMultiElement(nemesis::LinkedPreprocessLine& linkedline)
{
    auto raw = linkedline.GetRawPtr();

    if (!raw) return false;

    std::string_view line = raw->ToString();

    if (!IsElement(line)) return false;

    if (!line._Starts_with(element_spaces)) return false;

    if (line._Starts_with(vec4_prefix)) return false;

    if (line._Starts_with(nodeid_ele)) return true;

    if (line.find(" ") == NOT_FOUND) return false;

    line = line.substr(element_spaces.size());

    for (auto& ch : line)
    {
        switch (ch)
        {
            case '.':
            case ' ':
                break;
            default:
                if (!isdigit(ch)) return false;

                break;
        }
    }

    return true;
}

void nemesis::HkxParserBase::AddMultiElementLine(nemesis::LinkedPreprocessLine& linkedline)
{
    auto raw = linkedline.GetRawPtr();

    if (linkedline.HasCondition()) ErrorMessage(1230, raw->GetFilePath(), raw->GetLineNumber());

    VecStr elements = StringSplit(raw->ToString().substr(element_spaces.size()));
    auto linenum    = raw->GetLineNumber();
    auto filepath   = raw->GetFilePath();

    for (std::string& element : elements)
    {
        nemesis::Line new_line(element_spaces.data() + element, linenum, filepath);
        Base::AddLine(new_line);
    }
}

UPtr<nemesis::HkxParserBase::Vector4Info>
nemesis::HkxParserBase::TryGetVectorInfo(nemesis::LinkedPreprocessLine& linkedline)
{
    auto raw = linkedline.GetRawPtr();

    if (!raw) return nullptr;

    std::string_view vline = raw->ToString();
    size_t pos             = vline.find(hkxvec_prefix);
    size_t pos2;

    if (pos != NOT_FOUND)
    {
        pos += hkxvec_prefix.size();
        pos2 = vline.find(hkxvec_suffix, pos);
    }
    else
    {
        pos = vline.find(vec4_prefix);

        if (pos == NOT_FOUND) return nullptr;

        pos += vec4_prefix.size();
        pos2 = vline.find(vec4_suffix, pos);
    }

    if (pos2 == NOT_FOUND) return nullptr;

    std::string_view vline2 = vline.substr(pos, pos2 - pos);
    short count             = 0;
    pos--;
    pos2++;

    for (auto& ch : vline2)
    {
        switch (ch)
        {
            case ' ':
                count++;
            case '\t':
            case '.':
            case '-':
                break;
            default:
                if (!isdigit(ch)) return nullptr;

                break;
        }
    }

    if (count != 3) return nullptr;

    std::string spaces;
    auto linenum  = raw->GetLineNumber();
    auto filepath = raw->GetFilePath();
    auto vec4     = std::make_unique<nemesis::HkxParserBase::Vector4Info>();
    auto prefix   = std::string(vline.substr(0, pos));

    vec4->ishkx = prefix != std::string(element_spaces);

    if (vec4->ishkx)
    {
        for (auto& ch : vline)
        {
            if (ch != '\t') break;

            spaces.push_back(ch);
        }

        vec4->prefix = nemesis::Line(prefix, linenum, filepath);
        vec4->suffix = nemesis::Line(spaces + std::string(vline.substr(pos2)), linenum, filepath);
        spaces.push_back('\t');
    }
    else
    {
        spaces = std::string(element_spaces);
    }

    for (auto& element : StringSplit(std::string(vline2)))
    {
        nemesis::Line ele(spaces + element, linenum, filepath);
        vec4->elements.emplace_back(ele);
    }

    return std::move(vec4);
}

void nemesis::HkxParserBase::AddLineWithVector(nemesis::LinkedPreprocessLine& linkedline,
                                               nemesis::HkxParserBase::Vector4Info& vec4info)
{
    auto raw = linkedline.GetRawPtr();

    if (linkedline.HasCondition()) ErrorMessage(1230, raw->GetFilePath(), raw->GetLineNumber());

    if (!vec4info.ishkx)
    {
        for (auto& element : vec4info.elements)
        {
            Base::AddLine(element);
        }

        return;
    }

    Base::AddLine(vec4info.prefix);

    for (auto& element : vec4info.elements)
    {
        Base::AddLine(element);
    }

    Base::AddLine(vec4info.suffix);
}

void nemesis::HkxParserBase::AddLinkedLine(SPtr<nemesis::LinkedPreprocessLine> linkedline)
{
    auto ref_linkedline = *linkedline;

    if (IsMultiElement(ref_linkedline))
    {
        AddMultiElementLine(ref_linkedline);
        return;
    }

    auto vecinfo = Vector4Info::TryCreate(ref_linkedline);

    if (vecinfo)
    {
        AddLineWithVector(ref_linkedline, *vecinfo);
        return;
    }

    Base::AddLinkedLine(linkedline);
}

nemesis::HkxParserBase::HkxParserBase(nemesis::File& file)
    : Base(file)
{
}
