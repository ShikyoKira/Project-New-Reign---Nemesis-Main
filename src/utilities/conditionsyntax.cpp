#include "utilities/conditionsyntax.h"

namespace nemesis
{
    namespace syntax
    {
        std::string DeleteLine() noexcept
        {
            return "//* delete this line *//";
        }

        std::string Spaces() noexcept
        {
            return "\t\t\t\t\t";
        }

        std::string OpenComment() noexcept
        {
            return "<!-- ";
        }

        std::string CloseComment() noexcept
        {
            return " -->";
        }

        std::string Comment(const std::string& content) noexcept
        {
            return OpenComment() + content + CloseComment();
        }

        std::string Aster() noexcept
        {
            return OpenComment() + "*";
        }

        std::string CloseAster() noexcept
        {
            return "*" + CloseComment();
        }

        std::string Aster(const std::string& condition) noexcept
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

        std::string ForEach() noexcept
        {
            return OpenComment() + "FOREACH ^";
        }

        std::string ForEach(const std::string& condition) noexcept
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

        std::string ModCode() noexcept
        {
            return OpenComment() + "MOD_CODE ~";
        }

        std::string ModCode(const std::string& condition) noexcept
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

        std::string EndModCodeSyntax() noexcept
        {
            return "~ OPEN" + CloseComment();
        }

        std::string ModOriginal() noexcept
        {
            return Comment("MOD_CODE_ORIGINAL");
        }

        std::string LowerOriginal() noexcept
        {
            return Comment("original");
        }

        std::string Close() noexcept
        {
            return Comment("CLOSE");
        }

        std::string ModClose() noexcept
        {
            return Comment("MOD_CODE_CLOSE");
        }

        std::string If() noexcept
        {
            return OpenComment() + "IF ^";
        }

        std::string If(const std::string& condition) noexcept
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

        std::string ElseIf() noexcept
        {
            return OpenComment() + "ELSEIF ^";
        }

        std::string ElseIf(const std::string& condition) noexcept
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

        std::string Else() noexcept
        {
            return Comment("ELSE");
        }

        std::string EndIf() noexcept
        {
            return Comment("ENDIF");
        }

        std::string EndSyntax() noexcept
        {
            return "^" + CloseComment();
        }
    }
}
