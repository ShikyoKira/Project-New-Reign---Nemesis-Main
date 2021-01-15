#include <unordered_map>

#include "Global.h"

#include "utilities/condtype.h"
#include "utilities/conditionsyntax.h"

using namespace std;
namespace ns = nemesis::syntax;

nemesis::CondType nemesis::ParseConditionType(std::string_view line)
{
    const unordered_map<string, nemesis::CondType> typeMap = {
        {ns::If(), nemesis::CondType::IF},
        {ns::ElseIf(), nemesis::CondType::ELSEIF},
        {ns::Else(), nemesis::CondType::ELSE},
        {ns::EndIf(), nemesis::CondType::ENDIF},
        {ns::ForEach(), nemesis::CondType::FOREACH},
        {ns::ModCode(), nemesis::CondType::MOD_CODE},
        {ns::Original(), nemesis::CondType::ORIGINAL},
        {ns::LowerOriginal(), nemesis::CondType::LOWER_ORIGINAL},
        {ns::Close(), nemesis::CondType::CLOSE},
        {ns::Aster(), nemesis::CondType::ASTERISK},
    };

    for (auto& type : typeMap)
    {
        if (line.find(type.first) != NOT_FOUND) return type.second;
    }

    return nemesis::CondType::NONE;
}
