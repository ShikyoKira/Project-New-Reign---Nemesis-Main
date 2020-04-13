#include "utilities/regex.h"
#include "debugmsg.h"

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

    std::string_view regex::to_string() const
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
            ErrorMessage(6002, e.what(), "Error code: ", e.code(), "Regex: ", rgx.to_string());
        }
        return false;
    }

    bool regex_search(const std::string& line, std::string&& rgxStr)
    {
        try
        {
            return detail::underlying_regex_search(line, detail::underlying_regex(rgxStr));
        }
        catch (const detail::underlying_exception& e)
        {
            ErrorMessage(6002, e.what(), "Error code: ", e.code(), "Regex: ", rgxStr);
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
            ErrorMessage(6002, e.what(), "Error code: ", e.code(), "Regex: ", rgx.to_string());
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
            ErrorMessage(6002, e.what(), "Error code: ", e.code(), "Regex: ", rgx.to_string());
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
            ErrorMessage(6002, e.what(), "Error code: ", e.code(), "Regex: ", rgx.to_string());
        }
        return false;
    }

    regex_iterator::regex_iterator(const std::string& str, const nemesis::regex& reg)
        : str_(str)
        , it_(str_.begin(), str_.end(), reg.to_regex())
    {}

    regex_iterator regex_iterator::operator++(int)
    {
        auto copy = *this;
        it_++;
        return copy;
    }

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
        if (!currentVal_.has_value()) currentVal_ = *it_;
        return &currentVal_.value();
    }
} // namespace nemesis
