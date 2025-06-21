#include <utility>

#include "Utilities/Compute.h"

#include "exprtk.hpp"


using namespace std;

namespace nemesis
{
    bool calculate(std::string& equation,
                   const std::string& format,
                   const std::filesystem::path& filepath,
                   int linenum,
                   bool noDecimal)
    {
        using expression_t = exprtk::expression<double>;
        using parser_t     = exprtk::parser<double>;

        std::string expression_string = equation;
        expression_t expression;
        parser_t parser;

        if (!parser.compile(expression_string, expression))
        {
            throw std::runtime_error("Invalid equation (Equation: " + equation + ", Format: " + format
                                     + +", Line: " + std::to_string(linenum) + +", File: " + filepath.string()
                                     + ")");
        }

        equation = to_string(noDecimal ? static_cast<int>(expression.value()) : expression.value());
        return true;
    }

    bool calculate(const std::string& equation, std::string& result, bool no_decimal)
    {
        using expression_t = exprtk::expression<double>;
        using parser_t     = exprtk::parser<double>;

        expression_t expression;
        parser_t parser;

        if (!parser.compile(equation, expression)) return false;

        result = std::to_string(no_decimal ? static_cast<int>(expression.value()) : expression.value());
        return true;
    }
}
