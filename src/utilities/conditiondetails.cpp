#include "utilities/conditiondetails.h"

using namespace std;

nemesis::CondDetails::CondDetails(nemesis::CondType _type)
{
    type = _type;
}

nemesis::CondDetails::CondDetails(nemesis::CondType _type,
                                            std::string _condition)
{
    type      = _type;
    condition = _condition;
}

bool nemesis::CondDetails::templateCheck(filesystem::path path,
                                         const string& format,
                                         const nemesis::Line& line,
                                         vector<nemesis::CondDetails>& condtype)
{
    string condt = "<!-- FOREACH ^";
    uint pos     = line.find(NEM_FOREACH("test"));

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            pos += condt.length();
            condtype.push_back(CondDetails(nemesis::FOREACH, line.substr(pos, pos2 - pos)));
            return true;
        }
    }

    condt = "<!-- CONDITION START ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            pos += condt.length();
            condtype.push_back(CondDetails(nemesis::CONDITION_START, line.substr(pos, pos2 - pos)));
            return true;
        }
    }

    condt = "<!-- CONDITION ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.size() == 0) ErrorMessage(1119, format, path, line.linenum);

            if (condtype.back().type != nemesis::CONDITION_START
                && condtype.back().type != nemesis::CONDITION)
            {
                ErrorMessage(1215, format, path, line.linenum);
            }

            pos += condt.length();
            condtype.back() = CondDetails(nemesis::CONDITION, line.substr(pos, pos2 - pos));
            return true;
        }
    }

    if (line.find("<!-- CONDITION -->") != NOT_FOUND)
    {
        if (condtype.size() == 0) ErrorMessage(1119, format, path, line.linenum);

        if (condtype.back().type != nemesis::CONDITION_START
            && condtype.back().type != nemesis::CONDITION)
        {
            ErrorMessage(1215, format, path, line.linenum);
        }

        condtype.back() = CondDetails(nemesis::CONDITION_DEFAULT);
        return true;
    }

    condt = "<!-- NEW ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            pos += condt.length();
            condtype.push_back(CondDetails(nemesis::NEW, line.substr(pos, pos2 - pos)));

            return true;
        }
    }

    condt = "<!-- NEW ORDER ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            pos += condt.length();
            condtype.push_back(CondDetails(nemesis::NEW_ORDER, line.substr(pos, pos2 - pos)));

            return true;
        }
    }

    return false;
}

bool nemesis::CondDetails::templateCheckRev(std::filesystem::path path,
                                            const std::string& format,
                                            const nemesis::Line& line,
                                            std::vector<nemesis::CondDetails>& condtype)
{
    string condt = "<!-- FOREACH ^";
    uint pos     = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.back().type != nemesis::CLOSE) ErrorMessage(1180, format, path, line.linenum);

            condtype.pop_back();
            return true;
        }
    }

    condt = "<!-- CONDITION START ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.back().type != nemesis::CONDITION_DEFAULT
                && condtype.back().type != nemesis::CONDITION_END
                && condtype.back().type != nemesis::CONDITION)
            {
                ErrorMessage(1180, format, path, line.linenum);
            }

            condtype.pop_back();
            return true;
        }
    }

    condt = "<!-- CONDITION ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.size() == 0) ErrorMessage(1119, format, path, line.linenum);

            if (condtype.back().type != nemesis::CONDITION_DEFAULT
                && condtype.back().type != nemesis::CONDITION_END
                && condtype.back().type != nemesis::CONDITION)
            {
                ErrorMessage(1215, format, path, line.linenum);
            }

            condtype.back() = CondDetails(nemesis::CONDITION);
            return true;
        }
    }

    if (line.find("<!-- CONDITION -->") != NOT_FOUND)
    {
        if (condtype.size() == 0) ErrorMessage(1119, format, path, line.linenum);

        if (condtype.back().type != nemesis::CONDITION_END)
        {
            ErrorMessage(1215, format, path, line.linenum);
        }

        condtype.back() = CondDetails(nemesis::CONDITION_DEFAULT);
        return true;
    }

    condt = "<!-- NEW ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.back().type != nemesis::CLOSE) ErrorMessage(1180, format, path, line.linenum);

            condtype.pop_back();
            return true;
        }
    }

    condt = "<!-- NEW ORDER ^";
    pos   = line.find(condt);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("^ -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.back().type != nemesis::CLOSE) ErrorMessage(1180, format, path, line.linenum);

            condtype.pop_back();
            return true;
        }
    }

    return false;
}

bool nemesis::CondDetails::modCheck(filesystem::path path,
                                    const string& format,
                                    const nemesis::Line& line,
                                    vector<nemesis::CondDetails>& condtype)
{
    const string modcond = "<!-- MOD_CODE ~";
    uint pos             = line.find(modcond);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("~ OPEN -->", pos);

        if (pos2 != NOT_FOUND)
        {
            pos += modcond.length();
            condtype.push_back(nemesis::CondDetails(nemesis::MOD_CODE, line.substr(pos, pos2 - pos)));

            return true;
        }
    }
    
    return false;
}

bool nemesis::CondDetails::modCheckRev(filesystem::path path,
                                    const string& format,
                                    const nemesis::Line& line,
                                    vector<nemesis::CondDetails>& condtype)
{
    const string modcond = "<!-- MOD_CODE ~";
    uint pos             = line.find(modcond);

    if (pos != NOT_FOUND)
    {
        uint pos2 = line.find("~ OPEN -->", pos);

        if (pos2 != NOT_FOUND)
        {
            if (condtype.back().type != nemesis::CLOSE && condtype.back().type != nemesis::ORIGINAL)
            {
                ErrorMessage(1180, format, path, line.linenum);
            }

            condtype.pop_back();
            return true;
        }
    }

    return false;
}

bool nemesis::CondDetails::originalScope(filesystem::path path,
                                         const string& format,
                                         const nemesis::Line& line,
                                         vector<nemesis::CondDetails>& condtype)
{
    if (line.find("<!-- ORIGINAL -->") != NOT_FOUND)
    {
        if (condtype.back().type != nemesis::MOD_CODE) ErrorMessage(1213, format, path, line.linenum);

        return true;
    }

    return false;
}

bool nemesis::CondDetails::originalScopeRev(filesystem::path path,
                                            const string& format,
                                            const nemesis::Line& line,
                                            vector<nemesis::CondDetails>& condtype)
{
    if (line.find("<!-- ORIGINAL -->") != NOT_FOUND)
    {
        if (condtype.back().type != nemesis::CLOSE) ErrorMessage(1216, format, path, line.linenum);

        condtype.back() = nemesis::CondDetails(nemesis::ORIGINAL);
        return true;
    }

    return false;
}

bool nemesis::CondDetails::closeScope(filesystem::path path,
                                      const string& format,
                                      const nemesis::Line& line,
                                      vector<nemesis::CondDetails>& condtype)
{
    if (line.find("<!-- CLOSE -->") != NOT_FOUND)
    {
        if (condtype.size() == 0) ErrorMessage(1171, format, path, line.linenum);

        if (condtype.back().type == nemesis::CONDITION || condtype.back().type == nemesis::CONDITION_START
            || condtype.back().type == nemesis::CONDITION_DEFAULT)
        {
            const char* scope;

            switch (condtype.back().type)
            {
                case nemesis::NEW:
                {
                    scope = "NEW";
                    break;
                }
                case nemesis::NEW_ORDER:
                {
                    scope = "NEW_ORDER";
                    break;
                }
                case nemesis::FOREACH:
                {
                    scope = "FOREACH";
                    break;
                }
                case nemesis::MOD_CODE:
                {
                    scope = "MOD_CODE";
                    break;
                }
            }

            ErrorMessage(1213, format, path, scope, line.linenum);
        }

        return true;
    }
    
    if (line.find("<!-- CONDITION END -->") != NOT_FOUND)
    {
        if (condtype.size() == 0) ErrorMessage(1118, format, path, line.linenum);

        if (condtype.back().type != nemesis::CONDITION && condtype.back().type != nemesis::CONDITION_START
            && condtype.back().type != nemesis::CONDITION_DEFAULT)
        {
            const char* scope;

            switch (condtype.back().type)
            {
                case nemesis::NEW:
                {
                    scope = "NEW";
                    break;
                }
                case nemesis::NEW_ORDER:
                {
                    scope = "NEW_ORDER";
                    break;
                }
                case nemesis::FOREACH:
                {
                    scope = "FOREACH";
                    break;
                }
                case nemesis::MOD_CODE:
                {
                    scope = "MOD_CODE";
                    break;
                }
            }

            ErrorMessage(1214, format, path, scope, line.linenum);
        }

        return true;
    }

    return false;
}

bool nemesis::CondDetails::closeScopeRev(const nemesis::Line& line, vector<nemesis::CondDetails>& condtype)
{
    if (line.find("<!-- CLOSE -->") != NOT_FOUND)
    {
        condtype.push_back(CondDetails(nemesis::CLOSE));
        return true;
    }

    if (line.find("<!-- CONDITION END -->") != NOT_FOUND)
    {
        condtype.push_back(CondDetails(nemesis::CONDITION_END));
        return true;
    }

    return false;
}

nemesis::Line nemesis::CondDetails::getOriginalLine(std::deque<nemesis::Line>* edits)
{
    nemesis::Line vline;

    if (edits->empty())
    {
        vline         = "//* delete this line *//";
        vline.linenum = 0;
    }
    else
    {
        vline = edits->front();
        edits->pop_front();
    }

    return vline;
}
