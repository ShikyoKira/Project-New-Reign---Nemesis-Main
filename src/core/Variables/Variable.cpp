#include "core/Variables/Variable.h"

VecStr nemesis::Variable::GetValues() const
{
    return VecStr();
}

std::string_view nemesis::Variable::GetExpression() const
{
    return expression;
}
