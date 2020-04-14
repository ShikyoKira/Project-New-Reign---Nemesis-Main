#pragma once

#include "Global.h"
#include <boost/regex.hpp>

namespace nemesis
{
    namespace detail
    {
        using underlying_regex     = boost::regex;
        using underlying_smatch    = boost::smatch;
        using underlying_iterator  = boost::sregex_iterator;
        using underlying_exception = boost::regex_error;

#define FWD(val) std::forward<decltype(val)>(val)
        auto underlying_regex_search  = [](auto&&... args) { return boost::regex_search(FWD(args)...); };
        auto underlying_regex_match   = [](auto&&... args) { return boost::regex_match(FWD(args)...); };
        auto underlying_regex_replace = [](auto&&... args) { return boost::regex_replace(FWD(args)...); };
#undef FWD
    } // namespace detail

    class regex
    {
    public:
        regex(const std::string& str);
        regex(const char* str);

        std::string_view to_string() const;
        const detail::underlying_regex& to_regex() const;

    private:
        std::string str_;
        detail::underlying_regex reg_;
    };

    class smatch
    {
    public:
        smatch() = default;

        smatch(const detail::underlying_smatch& match)
            : match_(match)
        {}

        std::string operator[](size_t number) const;
        std::string str(size_t number = 0) const;
        size_t position(size_t number = 0) const;
        size_t size() const;

    private:
        detail::underlying_smatch match_;
    };

    class regex_iterator
    {
    public:
        regex_iterator() = default;
        regex_iterator(const std::string& str, const nemesis::regex& reg);

        regex_iterator operator++(int);
        regex_iterator& operator++();
        nemesis::smatch operator*();
        nemesis::smatch* operator->();
        bool operator!=(const regex_iterator& other);

    private:
        std::string str_;
        detail::underlying_iterator it_;
        std::optional<nemesis::smatch> currentVal_;

        void advanceIt();
        smatch* updateCurrentVal();
    };

    bool regex_search(const std::string& line, nemesis::smatch& match, const nemesis::regex& rgx);
    bool regex_search(const std::string& line, const nemesis::regex& rgx);

    bool regex_match(const std::string& line, nemesis::smatch& match, const nemesis::regex& rgx);
    bool regex_match(const std::string& line, const nemesis::regex& rgx);

    std::string regex_replace(const std::string& line, const nemesis::regex& rgx, const std::string& replace);

} // namespace nemesis
