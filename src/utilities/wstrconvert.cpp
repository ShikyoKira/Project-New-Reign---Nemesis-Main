#include "Global.h"

#if __cplusplus > 201402L
#include "wstrconvert.h"

std::wstring_convert<deletable_facet<std::codecvt<wchar_t, char, std::mbstate_t>>> wstrConv;
#endif
