#include <unordered_map>

#include "Global.h"

#include "utilities/condtype.h"
#include "utilities/conditionsyntax.h"

using namespace std;
namespace ns = nemesis::syntax;

namespace nemesis
{
    const UMap<string, nemesis::CondType> conditionTypeMap = {
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
}

nemesis::CondType nemesis::ParseConditionType(std::string_view line)
{
    for (auto& type : nemesis::conditionTypeMap)
    {
        if (line.find(type.first) != NOT_FOUND) return type.second;
    }

    return nemesis::CondType::NONE;
}
