#include "Global.h"

#include <external/exprtk/exprtk.hpp>
#include <utility>

#include "utilities/compute.h"

using namespace std;

namespace nemesis
{
    bool calculate(string& equation, string format, string filename, int linecount, bool noDecimal)
    {
        using expression_t = exprtk::expression<double>;
        using parser_t     = exprtk::parser<double>;

        string expression_string = equation;
        expression_t expression;
        parser_t parser;

        if (!parser.compile(expression_string, expression))
            ErrorMessage(1151, std::move(format), std::move(filename), linecount, equation);

        equation = to_string(noDecimal ? static_cast<int>(expression.value()) : expression.value());
        return true;
    }
} // namespace nemesis
