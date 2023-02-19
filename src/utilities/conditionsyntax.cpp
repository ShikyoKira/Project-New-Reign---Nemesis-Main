#include "utilities/conditionsyntax.h"

namespace nemesis
{
    namespace syntax
    {
        std::string DeleteLine()
        {
            return "//* delete this line *//";
        }

        std::string Spaces()
        {
            return "\t\t\t\t\t";
        }

        std::string OpenComment()
        {
            return "<!-- ";
        }

        std::string CloseComment()
        {
            return " -->";
        }

        std::string Comment(const std::string& content)
        {
            return OpenComment() + content + CloseComment();
        }

        std::string Aster()
        {
            return OpenComment() + "*";
        }

        std::string CloseAster()
        {
            return "*" + CloseComment();
        }

        std::string Aster(const std::string& condition)
        {
            return Aster() + condition + CloseAster();
        }

        std::string_view AsterCondition(const std::string& line)
        {
            return nemesis::between(line, Aster(), CloseAster());
        }

        std::string_view AsterCondition(const nemesis::Line& line)
        {
            return AsterCondition(line.ToString());
        }

        std::string ForEach()
        {
            return OpenComment() + "FOREACH ^";
        }

        std::string ForEach(const std::string& condition)
        {
            return ForEach() + condition + EndSyntax();
        }

        std::string_view ForEachCondition(const std::string& line)
        {
            return nemesis::between(line, ForEach(), EndSyntax());
        }

        std::string_view ForEachCondition(const nemesis::Line& line)
        {
            return ForEachCondition(line.ToString());
        }

        std::string ModCode()
        {
            return OpenComment() + "MOD_CODE ~";
        }

        std::string ModCode(const std::string& condition)
        {
            return ModCode() + condition + EndModCodeSyntax();
        }

        std::string_view ModCodeCondition(const std::string& line)
        {
            return nemesis::between(line, ModCode(), EndModCodeSyntax());
        }

        std::string_view ModCodeCondition(const nemesis::Line& line)
        {
            return ModCodeCondition(line.ToString());
        }

        std::string EndModCodeSyntax()
        {
            return "~ OPEN" + CloseComment();
        }

        std::string Original()
        {
            return Comment("ORIGINAL");
        }

        std::string LowerOriginal()
        {
            return Comment("original");
        }

        std::string Close()
        {
            return Comment("CLOSE");
        }

        std::string If()
        {
            return OpenComment() + "IF ^";
        }

        std::string If(const std::string& condition)
        {
            return If() + condition + EndSyntax();
        }

        std::string_view IfCondition(const std::string& line)
        {
            return nemesis::between(line, If(), EndSyntax());
        }

        std::string_view IfCondition(const nemesis::Line& line)
        {
            return IfCondition(line.ToString());
        }

        std::string ElseIf()
        {
            return OpenComment() + "ELSEIF ^";
        }

        std::string ElseIf(const std::string& condition)
        {
            return ElseIf() + condition + EndSyntax();
        }

        std::string_view ElseIfCondition(const std::string& line)
        {
            return nemesis::between(line, ElseIf(), EndSyntax());
        }

        std::string_view ElseIfCondition(const nemesis::Line& line)
        {
            return ElseIfCondition(line.ToString());
        }

        std::string Else()
        {
            return Comment("ELSE");
        }

        std::string EndIf()
        {
            return Comment("ENDIF");
        }

        std::string EndSyntax()
        {
            return "^" + CloseComment();
        }
    }
}
