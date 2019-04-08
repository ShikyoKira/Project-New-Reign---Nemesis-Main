#include "compute.h"
#include <exprtk.hpp>

using namespace std;

bool calculate(string& equation, string format, string filename, int linecount, bool noDecimal)
{
	typedef exprtk::expression<double> expression_t;
	typedef exprtk::parser<double>         parser_t;

	std::string expression_string = equation;

	expression_t expression;

	parser_t parser;

	if (!parser.compile(expression_string, expression))
	{
		ErrorMessage(1151, format, filename, linecount, equation);
	}

	if (noDecimal)
	{
		equation = to_string(int(expression.value()));
	}
	else
	{
		equation = to_string(expression.value());
	}

	return true;
}
