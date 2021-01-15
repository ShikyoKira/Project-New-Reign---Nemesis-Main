#include <functional>

#include "Global.h"

#include "utilities/conditionsyntax.h"
#include "utilities/conditiondetails.h"

using namespace std;
namespace ns = nemesis::syntax;
namespace sf = std::filesystem;

nemesis::CondDetails::CondDetails(nemesis::CondType _type) noexcept
{
    type = _type;
}

nemesis::CondDetails::CondDetails(nemesis::CondType _type, const string& _condition) noexcept
{
    type      = _type;
    condition = _condition;
}

bool nemesis::CondDetails::templateCheck(const sf::path& path,
                                         const string& format,
                                         const nemesis::Line& line,
                                         CondDetailsList& condtype)
{
    const pair<string, nemesis::CondType> condProcess[static_cast<uint>(nemesis::CondType::ALL)] =
    {
        {ns::ForEach(), nemesis::CondType::FOREACH},
        {ns::If(), nemesis::CondType::IF},
        {ns::ElseIf(), nemesis::CondType::ELSEIF},
        {ns::Else(), nemesis::CondType::ELSE},
    };

    CondDetailsProcess curprocess(path, format, line);

    for (auto& condition : condProcess)
    {
        uint pos = line.find(condition.first);

        if (pos == NOT_FOUND) continue;

        uint pos2 = line.find(ns::EndSyntax(), pos);

        if (pos2 == NOT_FOUND) continue;

        switch (condition.second)
        {
            case nemesis::CondType::FOREACH:
                condtype.emplace_back(CondDetails(condition.second, string(ns::ForEachCondition(line))));
                return true;
            case nemesis::CondType::IF:
                condtype.emplace_back(CondDetails(condition.second, string(ns::IfCondition(line))));
                return true;
            case nemesis::CondType::ELSEIF:
                curprocess.IfValidation(condtype);
                condtype.back() = CondDetails(condition.second, string(ns::ElseIfCondition(line)));
                return true;
            case nemesis::CondType::ELSE:
                curprocess.IfValidation(condtype);
                condtype.back() = CondDetails(condition.second);
                return true;
            default:
                break;
        }
    }

    return false;
}

bool nemesis::CondDetails::templateCheckRev(const sf::path& path,
                                            const string& format,
                                            const nemesis::Line& line,
                                            CondDetailsList& condtype)
{
    const pair<string, nemesis::CondType> conditionPair[static_cast<uint>(nemesis::CondType::ALL)] =
    {
        {ns::ForEach(), nemesis::CondType::FOREACH},
        {ns::Close(), nemesis::CondType::CLOSE},
        {ns::If(), nemesis::CondType::IF},
        {ns::ElseIf(), nemesis::CondType::ELSEIF},
        {ns::Else(), nemesis::CondType::ELSE},
        {ns::EndIf(), nemesis::CondType::ENDIF},
    };

    CondDetailsProcess curprocess(path, format, line);

    for (auto& condition : conditionPair)
    {
        uint pos = line.find(condition.first);

        if (pos == NOT_FOUND) continue;

        uint pos2 = line.find(ns::EndSyntax(), pos);

        if (pos2 == NOT_FOUND) continue;

        if (condition.first.find(ns::EndSyntax()) != NOT_FOUND)
        {
            condtype.emplace_back(CondDetails(condition.second, ""));
            continue;
        }

        switch (condition.second)
        {
            case nemesis::CondType::FOREACH:
                curprocess.ForEachValidationRev(condtype);
                condtype.pop_back();
                return true;
            case nemesis::CondType::IF:
                curprocess.IfValidationRev(condtype);
                condtype.pop_back();
                return true;
            case nemesis::CondType::ELSEIF:
                curprocess.IfValidationRev(condtype);
                condtype.back() = CondDetails(condition.second);
                return true;
            case nemesis::CondType::ELSE:
                curprocess.EndIfValidationRev(condtype);
                condtype.back() = CondDetails(condition.second);
                return true;
            default:
                return false;
        }
    }

    return false;
}

bool nemesis::CondDetails::modCheck(const sf::path& path,
                                    const string& format,
                                    const nemesis::Line& line,
                                    CondDetailsList& condtype)
{
    uint pos = line.find(ns::ModCode());

    if (pos == NOT_FOUND) return false;

    uint pos2 = line.find(ns::EndModCodeSyntax(), pos);

    if (pos2 == NOT_FOUND) return false;

    condtype.emplace_back(nemesis::CondDetails(nemesis::CondType::MOD_CODE, string(ns::ModCodeCondition(line))));
    return true;
}

bool nemesis::CondDetails::modCheckRev(const sf::path& path,
                                       const string& format,
                                       const nemesis::Line& line,
                                       CondDetailsList& condtype)
{
    uint pos = line.find(ns::ModCode());

    if (pos == NOT_FOUND) return false;

    uint pos2 = line.find(ns::EndModCodeSyntax(), pos);

    if (pos2 == NOT_FOUND) return false;

    switch (condtype.back().type)
    {
        case nemesis::CondType::CLOSE:
        case nemesis::CondType::ORIGINAL:
            condtype.pop_back();
            return true;
        default:
            ErrorMessage(1180, format, path, line.GetLineNumber());
    }
}

bool nemesis::CondDetails::originalScope(const sf::path& path,
                                         const string& format,
                                         const nemesis::Line& line,
                                         CondDetailsList& condtype)
{
    if (line.find(ns::Original()) == NOT_FOUND) return false;

    if (condtype.back().type == nemesis::CondType::MOD_CODE) return true;

    ErrorMessage(1216, format, path, line.GetLineNumber());
}

bool nemesis::CondDetails::originalScopeRev(const sf::path& path,
                                            const string& format,
                                            const nemesis::Line& line,
                                            CondDetailsList& condtype)
{
    if (line.find(ns::Original()) == NOT_FOUND) return false;

    if (condtype.back().type != nemesis::CondType::CLOSE)
    {
        ErrorMessage(1216, format, path, line.GetLineNumber());
    }

    condtype.back() = nemesis::CondDetails(nemesis::CondType::ORIGINAL);
    return true;
}

bool nemesis::CondDetails::closeScope(const sf::path& path,
                                      const string& format,
                                      const nemesis::Line& line,
                                      CondDetailsList& condtype)
{
    unordered_map<nemesis::CondType, const char*> SyntaxToString =
    {
        {nemesis::CondType::IF, "IF"},
        {nemesis::CondType::FOREACH, "FOREACH"},
        {nemesis::CondType::MOD_CODE, "MOD_CODE"},
    };

    if (line.find(ns::Close()) != NOT_FOUND)
    {
        if (condtype.empty()) ErrorMessage(1118, format, path, line.GetLineNumber());

        switch (condtype.back().type)
        {
            case nemesis::CondType::FOREACH:
            case nemesis::CondType::MOD_CODE:
                return true;
            default:
                const char* scope = SyntaxToString[condtype.back().type];
                ErrorMessage(1213, format, path, scope, line.GetLineNumber());
        }
    }

    if (line.find(ns::EndIf()) != NOT_FOUND)
    {
        if (condtype.empty()) ErrorMessage(1118, format, path, line.GetLineNumber());

        switch (condtype.back().type)
        {
            case nemesis::CondType::IF:
            case nemesis::CondType::ELSEIF:
            case nemesis::CondType::ELSE:
                return true;
            default:
                const char* scope = SyntaxToString[condtype.back().type];
                ErrorMessage(1214, format, path, scope, line.GetLineNumber());
        }
    }

    return false;
}

bool nemesis::CondDetails::closeScopeRev(const nemesis::Line& line, CondDetailsList& condtype)
{
    if (line.find(ns::Close()) != NOT_FOUND)
    {
        condtype.emplace_back(CondDetails(nemesis::CondType::CLOSE));
        return true;
    }

    if (line.find(ns::EndIf()) != NOT_FOUND)
    {
        condtype.emplace_back(CondDetails(nemesis::CondType::ENDIF));
        return true;
    }

    return false;
}

nemesis::Line nemesis::CondDetails::getOriginalLine(deque<nemesis::Line>* edits)
{
    UPtr<nemesis::Line> pline;

    if (edits->empty())
    {
        pline = make_unique<nemesis::Line>(ns::DeleteLine(), 0);
    }
    else
    {
        pline = make_unique<nemesis::Line>(edits->front());
        edits->pop_front();
    }

    return *pline;
}

nemesis::CondDetails::CondDetailsProcess::CondDetailsProcess(sf::path _path,
                                                             const string& _format,
                                                             const nemesis::Line& _line)
    : path(_path)
    , format(_format)
    , line(_line)
{
}

void nemesis::CondDetails::CondDetailsProcess::IfValidation(CondDetailsList& condtype) const noexcept
{
    if (condtype.empty()) ErrorMessage(1119, format, path, line.GetLineNumber());

    switch (condtype.back().type)
    {
        case nemesis::CondType::IF:
        case nemesis::CondType::ELSEIF:
            break;
        case nemesis::CondType::ELSE:
        default:
            ErrorMessage(1215, format, path, line.GetLineNumber());
    }
}

void nemesis::CondDetails::CondDetailsProcess::IfValidationRev(CondDetailsList& condtype) const noexcept
{
    if (condtype.empty()) ErrorMessage(1120, format, path, line.GetLineNumber());

    switch (condtype.back().type)
    {
        case nemesis::CondType::ELSEIF:
        case nemesis::CondType::ELSE:
        case nemesis::CondType::ENDIF:
            break;
        default:
            ErrorMessage(1215, format, path, line.GetLineNumber());
    }
}

void nemesis::CondDetails::CondDetailsProcess::EndIfValidationRev(CondDetailsList& condtype) const noexcept
{
    if (condtype.empty()) ErrorMessage(1120, format, path, line.GetLineNumber());

    if (condtype.back().type == nemesis::CondType::ENDIF) return;

    ErrorMessage(1215, format, path, line.GetLineNumber());
}

void nemesis::CondDetails::CondDetailsProcess::ForEachValidationRev(CondDetailsList& condtype) const noexcept
{
    if (condtype.empty()) ErrorMessage(1120, format, path);

    if (condtype.back().type == nemesis::CondType::CLOSE) return;

    ErrorMessage(1180, format, path, line.GetLineNumber());
}
