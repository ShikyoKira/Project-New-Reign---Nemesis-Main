#include "stringsplit.h"

using namespace std;

void StringSplit(string line, vecstr& container)
{
	container = vecstr((istream_iterator<string>(istringstream(line))), istream_iterator<string>());
}
