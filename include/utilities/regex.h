#pragma once

#include <regex>
#include <optional>

#include "Global.h"

namespace nemesis
{
    struct Line;

    namespace detail
    {
        using underlying_regex      = std::regex;
        using underlying_smatch     = std::smatch;
        using underlying_iterator   = std::sregex_iterator;
        using underlying_exception  = std::regex_error;

        using underlying_wregex     = std::wregex;
        using underlying_wsmatch    = std::wsmatch;
        using underlying_witerator  = std::wsregex_iterator;

#define FWD(val) std::forward<decltype(val)>(val)
        auto underlying_regex_search  = [](auto&&... args) { return std::regex_search(FWD(args)...); };
        auto underlying_regex_match   = [](auto&&... args) { return std::regex_match(FWD(args)...); };
        auto underlying_regex_replace = [](auto&&... args) { return std::regex_replace(FWD(args)...); };
#undef FWD
    } // namespace detail

    namespace regex_constants = std::regex_constants;

    class regex
    {
    public:
        regex() = default;
        regex(const std::string& str);
        regex(const char* str);
        regex(const std::string& str, regex_constants::syntax_option_type flag);
        regex(const char* str, regex_constants::syntax_option_type flag);

        std::string to_string() const;
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
        bool empty() const;

    private:
        detail::underlying_smatch match_;
    };

    class regex_iterator
    {
    public:
        regex_iterator() = default;
        regex_iterator(const std::string& str, const nemesis::regex& reg);
        regex_iterator(const nemesis::Line& str, const nemesis::regex& reg);

        regex_iterator& operator++();
        nemesis::smatch operator*();
        nemesis::smatch* operator->();
        bool operator!=(const regex_iterator& other);

    private:
        std::string str_;
        std::sregex_iterator it_;
        std::optional<nemesis::smatch> currentVal_;

        void advanceIt();
        smatch* updateCurrentVal();
    };

    bool regex_search(const std::string& line, nemesis::smatch& match, const nemesis::regex& rgx);
    bool regex_search(const std::string& line, const nemesis::regex& rgx);

    bool regex_match(const std::string& line, nemesis::smatch& match, const nemesis::regex& rgx);
    bool regex_match(const std::string& line, const nemesis::regex& rgx);

    std::string regex_replace(const std::string& line, const nemesis::regex& rgx, const std::string& replace);

    // =====================================================//
    //                    WIDE CHARACTERS                   //
    // =====================================================//

    class wregex
    {
    public:
        wregex(const std::wstring& str);
        wregex(const wchar_t* str);

        std::wstring to_wstring() const;
        const detail::underlying_wregex& to_wregex() const;

    private:
        std::wstring str_;
        detail::underlying_wregex reg_;
    };

    class wsmatch
    {
    public:
        wsmatch() = default;

        wsmatch(const detail::underlying_wsmatch& match)
            : match_(match)
        {
        }

        std::wstring operator[](size_t number) const;
        std::wstring str(size_t number = 0) const;
        size_t position(size_t number = 0) const;
        size_t size() const;

    private:
        detail::underlying_wsmatch match_;
    };

    class wregex_iterator
    {
    public:
        wregex_iterator() = default;
        wregex_iterator(const std::wstring& str, const nemesis::wregex& reg);

        wregex_iterator& operator++();
        nemesis::wsmatch operator*();
        nemesis::wsmatch* operator->();
        bool operator!=(const wregex_iterator& other);

    private:
        std::wstring str_;
        detail::underlying_witerator it_;
        std::optional<nemesis::wsmatch> currentVal_;

        void advanceIt();
        wsmatch* updateCurrentVal();
    };

    bool wregex_search(const std::wstring& line, nemesis::wsmatch& match, const nemesis::wregex& rgx);
    bool wregex_search(const std::wstring& line, const nemesis::wregex& rgx);

    bool wregex_match(const std::wstring& line, nemesis::wsmatch& match, const nemesis::wregex& rgx);
    bool wregex_match(const std::wstring& line, const nemesis::wregex& rgx);

    std::wstring wregex_replace(const std::wstring& line, const nemesis::wregex& rgx, const std::wstring& replace);

} // namespace nemesis
