#include "utilities/conditionsyntax.h"

std::string nemesis::syntax::ForEach()
{
    return "<!-- FOREACH ^";
}

std::string nemesis::syntax::ForEach(std::string condition)
{
    return ForEach() + condition + EndSyntax();
}

std::string nemesis::syntax::ModCode()
{
    return "<!-- MOD_CODE ~";
}

std::string nemesis::syntax::ModCode(std::string condition)
{
    return ModCode() + condition + EndModCodeSyntax();
}

std::string nemesis::syntax::EndModCodeSyntax()
{
    return "~ -->";
}

std::string nemesis::syntax::Original()
{
    return "<!-- ORIGINAL -->";
}

std::string nemesis::syntax::Close()
{
    return "<!-- CLOSE -->";
}

std::string nemesis::syntax::If()
{
    return "<!-- IF ^";
}

std::string nemesis::syntax::If(std::string condition)
{
    return If() + condition + EndSyntax();
}

std::string nemesis::syntax::ElseIf()
{
    return "<!-- ELSEIF ^";
}

std::string nemesis::syntax::ElseIf(std::string condition)
{
    return ElseIf() + condition + EndSyntax();
}

std::string nemesis::syntax::EndIf()
{
    return "<!-- ENDIF -->";
}

std::string nemesis::syntax::EndSyntax()
{
    return "^ -->";
}
