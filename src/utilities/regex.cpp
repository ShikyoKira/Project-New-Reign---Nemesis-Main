#include "debugmsg.h"

#include "utilities/regex.h"
#include "utilities/algorithm.h"

namespace nemesis
{
    regex::regex(const std::string& str)
        : str_(str)
        , reg_(str_)
    {}

    regex::regex(const char* str)
        : str_(str)
        , reg_(str_)
    {}

    std::string regex::to_string() const
    {
        return reg_.str();
    }

    const detail::underlying_regex& regex::to_regex() const
    {
        return reg_;
    }

    std::string smatch::operator[](size_t number) const
    {
        return match_[number];
    }

    std::string smatch::str(size_t number) const
    {
        return match_.str(number);
    }

    size_t smatch::position(size_t number) const
    {
        return match_.position(number);
    }

    size_t smatch::size() const
    {
        return match_.size();
    }

    bool regex_search(const std::string& line, nemesis::smatch& n_match, const regex& rgx)
    {
        n_match     = {};
        detail::underlying_smatch match;
        try
        {
            auto result = detail::underlying_regex_search(line, match, rgx.to_regex());

            if (!result) return false;

            n_match = {match};
            return true;
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         std::string(e.what()) + "\nError code: " + std::to_string(e.code())
                             + "\nRegex: " + rgx.to_string());
        }
        return false;
    }

    bool regex_search(const std::string& line, const regex& rgxStr)
    {
        try
        {
            return detail::underlying_regex_search(line, rgxStr.to_regex());
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         std::string(e.what()) + "\nError code: " + std::to_string(e.code())
                             + "\nRegex: " + rgxStr.to_string());
        }

        return false;
    }

    std::string regex_replace(const std::string& line, const regex& rgx, const std::string& replace)
    {
        try
        {
            return detail::underlying_regex_replace(line, rgx.to_regex(), replace);
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         std::string(e.what()) + "\nError code: " + std::to_string(e.code())
                             + "\nRegex: " + rgx.to_string());
        }

        return line;
    }

    bool regex_match(const std::string& line, smatch& match, const regex& rgx)
    {
        match = {};
        try
        {
            detail::underlying_smatch m;
            auto result = detail::underlying_regex_match(line, m, rgx.to_regex());

            if (!result) return false;

            match = {m};
            return true;
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         std::string(e.what()) + "\nError code: " + std::to_string(e.code())
                             + "\nRegex: " + rgx.to_string());
        }
        return false;
    }

    bool regex_match(const std::string& line, const regex& rgx)
    {
        try
        {
            return detail::underlying_regex_match(line, rgx.to_regex());
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         std::string(e.what()) + "\nError code: " + std::to_string(e.code())
                             + "\nRegex: " + rgx.to_string());
        }
        return false;
    }

    regex_iterator::regex_iterator(const std::string& str, const nemesis::regex& reg)
        : str_(str)
        , it_(str_.begin(), str_.end(), reg.to_regex())
    {}

    regex_iterator& regex_iterator::operator++()
    {
        it_++;
        return *this;
    }

    nemesis::smatch regex_iterator::operator*()
    {
        return *updateCurrentVal();
    }

    nemesis::smatch* regex_iterator::operator->()
    {
        return updateCurrentVal();
    }

    bool regex_iterator::operator!=(const regex_iterator& other)
    {
        return it_ != other.it_;
    }

    void regex_iterator::advanceIt()
    {
        it_++;
        currentVal_ = std::nullopt;
    }

    nemesis::smatch* regex_iterator::updateCurrentVal()
    {
        currentVal_ = *it_;
        return &currentVal_.value();
    }

    
    // =====================================================//
    //                    WIDE CHARACTERS                   //
    // =====================================================//

    wregex::wregex(const std::wstring& str)
        : str_(str)
        , reg_(str_)
    {
    }

    wregex::wregex(const wchar_t* str)
        : str_(str)
        , reg_(str_)
    {
    }

    std::wstring wregex::to_wstring() const
    {
        return reg_.str();
    }

    const detail::underlying_wregex& wregex::to_wregex() const
    {
        return reg_;
    }

    std::wstring wsmatch::operator[](size_t number) const
    {
        return match_[number];
    }

    std::wstring wsmatch::str(size_t number) const
    {
        return match_.str(number);
    }

    size_t wsmatch::position(size_t number) const
    {
        return match_.position(number);
    }

    size_t wsmatch::size() const
    {
        return match_.size();
    }

    bool wregex_search(const std::wstring& line, nemesis::wsmatch& n_match, const wregex& rgx)
    {
        n_match = {};
        detail::underlying_wsmatch match;

        try
        {
            auto result = detail::underlying_regex_search(line, match, rgx.to_wregex());

            if (!result) return false;

            n_match = {match};
            return true;
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         nemesis::transform_to<std::wstring>(std::string(e.what())) + L"\nError code: "
                             + std::to_wstring(e.code()) + L"\nRegex: " + rgx.to_wstring());
        }

        return false;
    }

    bool wregex_search(const std::wstring& line, const wregex& rgxStr)
    {
        try
        {
            return detail::underlying_regex_search(line, rgxStr.to_wregex());
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         nemesis::transform_to<std::wstring>(std::string(e.what())) + L"\nError code: "
                             + std::to_wstring(e.code()) + L"\nRegex: " + rgxStr.to_wstring());
        }

        return false;
    }

    std::wstring wregex_replace(const std::wstring& line, const wregex& rgx, const std::wstring& replace)
    {
        try
        {
            return detail::underlying_regex_replace(line, rgx.to_wregex(), replace);
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         nemesis::transform_to<std::wstring>(std::string(e.what())) + L"\nError code: "
                             + std::to_wstring(e.code()) + L"\nRegex: " + rgx.to_wstring());
        }

        return line;
    }

    bool wregex_match(const std::wstring& line, wsmatch& match, const wregex& rgx)
    {
        match = {};
        try
        {
            detail::underlying_wsmatch m;
            auto result = detail::underlying_regex_match(line, m, rgx.to_wregex());

            if (!result) return false;

            match = {m};
            return true;
        }
        catch (const detail::underlying_exception& e)
        {
        }
        return false;
    }

    bool wregex_match(const std::wstring& line, const wregex& rgx)
    {
        try
        {
            return detail::underlying_regex_match(line, rgx.to_wregex());
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002,
                         nemesis::transform_to<std::wstring>(std::string(e.what())) + L"\nError code: "
                             + std::to_wstring(e.code()) + L"\nRegex: " + rgx.to_wstring());
        }
        return false;
    }

    wregex_iterator::wregex_iterator(const std::wstring& str, const nemesis::wregex& reg)
        : str_(str)
        , it_(str_.begin(), str_.end(), reg.to_wregex())
    {
    }

    wregex_iterator& wregex_iterator::operator++()
    {
        it_++;
        return *this;
    }

    nemesis::wsmatch wregex_iterator::operator*()
    {
        return *updateCurrentVal();
    }

    nemesis::wsmatch* wregex_iterator::operator->()
    {
        return updateCurrentVal();
    }

    bool wregex_iterator::operator!=(const wregex_iterator& other)
    {
        return it_ != other.it_;
    }

    void wregex_iterator::advanceIt()
    {
        it_++;
        currentVal_ = std::nullopt;
    }

    nemesis::wsmatch* wregex_iterator::updateCurrentVal()
    {
        currentVal_ = *it_;
        return &currentVal_.value();
    }

} // namespace nemesis
