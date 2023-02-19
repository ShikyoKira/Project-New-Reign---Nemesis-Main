#ifndef ALPHANUM_HPP_
#define ALPHANUM_HPP_

#include <string>

struct alphanum_less
{
    bool operator()(const std::string& x, const std::string& y) const;
    bool operator()(const std::string_view& x, const std::string_view& y) const;

    typedef std::string first_argument_type;
    typedef std::string second_argument_type;
    typedef bool result_type;
};

#endif