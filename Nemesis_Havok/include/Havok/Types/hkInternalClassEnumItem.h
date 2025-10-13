#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <sstream>

#include "Nemesis_Havok/include/Utilities/Types.h"

namespace nemesis
{
    template <typename T, bool Flags = false>
    class hkInternalClassEnumItem
    {
        static_assert(std::is_integral_v<T>,
                      "hkInternalClassEnumItem can only accept numeric data type");

    private:
        Vec<Pair<std::string, T>> EnumerableMap;

        bool TryMatchEnum(const std::string& token, T& val) const
        {
            for (auto& each : EnumerableMap)
            {
                if (each.first != token) continue;

                val = each.second;
                return true;
            }

            return false;
        }

        std::string StringReplace(std::string str, const std::string& from, const std::string& to) const
        {
            if (from.empty() || from == to) return str;

            size_t pos = 0;

            while ((pos = str.find(from, pos)) != std::string::npos)
            {
                str.replace(pos, from.length(), to);
                pos += to.length();
            }

            return str;
        }

    public:
        hkInternalClassEnumItem(std::initializer_list<std::pair<std::string, T>> values)
        {
            *this = values;
        }

        hkInternalClassEnumItem<T, Flags>& operator=(std::initializer_list<std::pair<std::string, T>> values)
        {
            EnumerableMap.clear();
            EnumerableMap.assign(values);
            return *this;
        }

        std::string operator[](T i) const
        {
            if constexpr (Flags)
            {
                T remaining = i;
                std::string val;

                if (i == 0)
                {
                    for (const Pair<std::string, T>& each : EnumerableMap)
                    {
                        if (0 != each.second) continue;

                        return each.first;
                    }

                    return "0";
                }

                for (auto it = EnumerableMap.rbegin(); it != EnumerableMap.rend(); ++it)
                {
                    const Pair<std::string, T>& each = *it;

                    if (each.first == "0") continue;

                    if ((remaining & each.second) != each.second) continue;

                    val += each.first + "|";
                    remaining ^= each.second;
                }

                if (remaining != 0)
                {
                    std::ostringstream oss;
                    oss << "<!-- UNKNOWN BITS -->0x" << std::hex << remaining;
                    val += oss.str() + "|";
                }

                val.pop_back();
                return val;
            }
            else
            {
                for (const Pair<std::string, T>& each : EnumerableMap)
                {
                    if (i != each.second) continue;

                    return each.first;
                }

                return std::to_string(i);
            }
        }

        T operator[](const std::string& val) const
        {
            if constexpr (Flags)
            {
                if (val == "0")
                {
                    T token_val;

                    if (TryMatchEnum(val, token_val)) return token_val;

                    return 0;
                }

                std::stringstream ss(val);
                std::string token;
                T combined_val = 0;

                while (std::getline(ss, token, '|'))
                {
                    T token_val;

                    if (TryMatchEnum(token, token_val))
                    {
                        combined_val |= token_val;
                        continue;
                    }

                    std::string t = StringReplace(token, "<!-- UNKNOWN BITS -->", "");

                    try
                    {
                        combined_val |= std::stoi(t.substr(2), nullptr, 16);
                    }
                    catch (const std::exception&)
                    {
                        throw std::invalid_argument("Parsing Error: Invalid enum value '" + token + "'.");
                    }
                }

                return combined_val;
            }
            else
            {
                for (const Pair<std::string, T>& each : EnumerableMap)
                {
                    if (val != each.first) continue;

                    return each.second;
                }

                try
                {
                    return std::stoi(val);
                }
                catch (const std::exception&)
                {
                    throw std::invalid_argument("Parsing Error: Invalid enum value '" + val + "'.");
                }
            }
        }
    };
}
