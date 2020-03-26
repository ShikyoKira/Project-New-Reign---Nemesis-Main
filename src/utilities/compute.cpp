#include <external/exprtk/exprtk.hpp>

#include "utilities/compute.h"

using namespace std;

namespace nemesis
{
	bool calculate(string& equation, string format, string filename, int linecount, bool noDecimal)
	{
		typedef exprtk::expression<double> expression_t;
		typedef exprtk::parser<double>         parser_t;

		string expression_string = equation;
		expression_t expression;
		parser_t parser;

		if (!parser.compile(expression_string, expression)) ErrorMessage(1151, format, filename, linecount, equation);

		equation = to_string(noDecimal ? static_cast<int>(expression.value()) : expression.value());
		return true;
	}
}
