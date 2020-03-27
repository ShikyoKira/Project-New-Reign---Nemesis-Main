#ifndef WSTRCONVERT_H_
#define WSTRCONVERT_H_

#if __cplusplus > 201402L
#include <codecvt>

extern std::wstring_convert<deletable_facet<std::codecvt<wchar_t, char, std::mbstate_t>>> wstrConv;
#endif

#endif
