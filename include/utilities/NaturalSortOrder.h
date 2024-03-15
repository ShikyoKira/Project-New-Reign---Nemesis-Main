#pragma once

#include <string>
#include <QCollator>

#include "utilities/line.h"

#include "core/NLine.h"

namespace nemesis
{
	struct NaturalSortAscend
    {
        bool operator()(const char* x, const char* y) const;
        bool operator()(const std::string& x, const std::string& y) const;
        bool operator()(const std::string_view& x, const std::string_view& y) const;
        bool operator()(const std::wstring& x, const std::wstring& y) const;
        bool operator()(const std::wstring_view& x, const std::wstring_view& y) const;
        bool operator()(const nemesis::Line& x, const nemesis::Line& y) const;
    };

	struct NaturalSortDescend
    {
        bool operator()(const char* x, const char* y) const;
        bool operator()(const std::string& x, const std::string& y) const;
        bool operator()(const std::string_view& x, const std::string_view& y) const;
        bool operator()(const std::wstring& x, const std::wstring& y) const;
        bool operator()(const std::wstring_view& x, const std::wstring_view& y) const;
        bool operator()(const nemesis::Line& x, const nemesis::Line& y) const;
    };

	struct NaturalSortOrder
    {
    private:
        enum mode_t
        {
            STRING,
            NUMBER
        };

        static QCollator Collator;
        static NaturalSortOrder StaticOrder;

        NaturalSortOrder();

    public:
        static int Compare(const char* x, const char* y);
        static int Compare(const wchar_t* x, const wchar_t* y);
    };
}
