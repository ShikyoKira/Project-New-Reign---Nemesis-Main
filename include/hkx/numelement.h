#pragma once

#include "utilities/regex.h"

namespace nemesis
{
    struct NumElement
    {
    private:
        bool (nemesis::NumElement::*addelement_ptr)(const std::string&) = &nemesis::NumElement::TryAddElement;

        size_t element_pos;
        size_t element_len;
        size_t element = 0;

        std::string& refline;
        std::string end_keyword;
        std::string element_keyword;

        bool end = false;

        bool TryGetDigitView(const std::string& line, std::string_view& sv);
        bool TryGetBracketView(const std::string& line, std::string_view& sv);

        bool TryAddElementAsNodeId(const std::string& line);
        bool TryAddElementAsHkxObj(const std::string& line);
        bool TryAddElementAsNumber(const std::string& line);
        bool TryAddElementAsVector(const std::string& line);
        bool TryAddElement(const std::string& line);

        void TryEnd(const std::string& line);

    public:
        NumElement(const std::string& keyword, std::string& refline);

        void CheckLine(const std::string& line);
        bool IsEnd();

        static bool TryCreateNumElement(std::string& line, UPtr<nemesis::NumElement>& numelement_ptr);

    private:
        static constexpr std::string_view element_validity_sv = "numelements=\"";
        static constexpr std::string_view hkxparam_sv         = "<hkparam";
        static constexpr std::string_view nhkxparam_sv        = "</hkparam>";

        static const nemesis::regex numelement_rgx;
        static const nemesis::regex hkxelement_rgx;
        static const nemesis::regex vecelement_rgx;
        static const nemesis::regex element_validity_rgx;

        friend UPtr<nemesis::NumElement>;
    };
}
