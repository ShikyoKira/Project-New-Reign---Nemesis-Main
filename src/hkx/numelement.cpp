#include "hkx/numelement.h"

#include "utilities/stringextension.h"

const nemesis::regex nemesis::NumElement::numelement_rgx(R"(^[\s]+[0-9])");
const nemesis::regex nemesis::NumElement::hkxelement_rgx(R"(^[\s]+<[a-zA-Z]+?>)");
const nemesis::regex nemesis::NumElement::vecelement_rgx(R"(^[\s]+(.+?))");
const nemesis::regex nemesis::NumElement::element_validity_rgx("^([\\s]+<)(hkparam) .+ numelements=\"(.+?)\">(<\\/hkparam>|)");

bool nemesis::NumElement::TryGetDigitView(const std::string& line, std::string_view& sv)
{
    sv = line;

    for (size_t i = 0; i < line.length(); i++)
    {
        auto& ch = line[i];

        if (!std::isdigit(ch))
        {
            if (ch != ' ' && ch != '\t') return false;

            continue;
        }

        sv = sv.substr(i);
        return true;
    }

    return false;
}

bool nemesis::NumElement::TryGetBracketView(const std::string& line, std::string_view& sv)
{
    sv = line;

    for (size_t i = 0; i < line.length(); i++)
    {
        auto& ch = line[i];

        if (ch != '(')
        {
            if (ch != ' ' && ch != '\t') return false;

            continue;
        }

        sv = sv.substr(i);
        return true;
    }

    return false;
}

bool nemesis::NumElement::TryAddElementAsNodeId(const std::string& line)
{
    auto counter = std::count(line.begin(), line.end(), '#');

    if (counter == 0) return false;

    element += counter;
    return true;
}

bool nemesis::NumElement::TryAddElementAsHkxObj(const std::string& line)
{
    if (!StringStartWith(line, element_keyword)) return false;

    element++;
    return true;
}

bool nemesis::NumElement::TryAddElementAsNumber(const std::string& line)
{
    std::string_view sv;
    
    if (!TryGetDigitView(line, sv)) return false;

    element++;
    bool next = false;

    for (size_t i = 0; i < sv.length(); i++)
    {
        auto& ch = sv[i];

        if (next && std::isdigit(ch))
        {
            element++;
            next = false;
            continue;
        }

        if (ch != ' ' && ch != '\t') continue;

        next = true;
    }

    return true;
}

bool nemesis::NumElement::TryAddElementAsVector(const std::string& line)
{
    std::string_view sv;

    if (!TryGetBracketView(line, sv)) return false;

    size_t counter = 0;

    for (size_t i = 0; i < sv.length(); i++)
    {
        switch (sv[i])
        {
            case '(':
                counter++;
                break;
            case ')':
                element += (--counter == 0);
                break;
        }
    }

    return true;
}

bool nemesis::NumElement::TryAddElement(const std::string& line)
{
    if (TryAddElementAsNodeId(line))
    {
        addelement_ptr = &nemesis::NumElement::TryAddElementAsNodeId;
        return true;
    }

    nemesis::smatch sm;

    if (nemesis::regex_search(line, sm, hkxelement_rgx))
    {
        element++;
        element_keyword = sm.str(0);
        addelement_ptr  = &nemesis::NumElement::TryAddElementAsHkxObj;
        return true;
    }

    if (nemesis::regex_search(line, sm, vecelement_rgx))
    {
        if (!TryAddElementAsVector(line)) return false;

        addelement_ptr  = &nemesis::NumElement::TryAddElementAsVector;
        return true;
    }

    if (!nemesis::regex_search(line, sm, numelement_rgx)) return false;

    if (!TryAddElementAsNumber(line)) return false;

    addelement_ptr = &nemesis::NumElement::TryAddElementAsNumber;
    return true;
}

void nemesis::NumElement::TryEnd(const std::string& line)
{
    if (!StringStartWith(line, end_keyword)) return;

    refline.replace(element_pos, element_len, std::to_string(element));
    end = true;
}

nemesis::NumElement::NumElement(const std::string& end_keyword, std::string& refline)
    : end_keyword(end_keyword)
    , refline(refline)
    , element_pos{}
    , element_len{}
{
}

void nemesis::NumElement::CheckLine(const std::string& line)
{
    if (end) throw std::runtime_error("NumElement has already ended");

    if ((this->*addelement_ptr)(line)) return;

    TryEnd(line);
}

bool nemesis::NumElement::IsEnd()
{
    return end;
}

bool nemesis::NumElement::TryCreateNumElement(std::string& line, UPtr<nemesis::NumElement>& numelement_ptr)
{
    std::string_view sv(line);
    size_t pos2 = sv.find(element_validity_sv);

    if (pos2 == NOT_FOUND) return false;

    size_t pos = sv.find(hkxparam_sv);

    if (pos == NOT_FOUND || pos2 < pos) return false;

    if (sv.find(nhkxparam_sv, pos2) != NOT_FOUND) return false;

    numelement_ptr = std::make_unique<nemesis::NumElement>(line.substr(0, pos) + nhkxparam_sv.data(), line);
    pos            = pos2 + element_validity_sv.size();
    numelement_ptr->element_pos = pos;
    numelement_ptr->element_len = line.find("\">", pos) - pos;
    return true;
}
