#pragma once

#include <bit>
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define NOT_FOUND std::string::npos
namespace nemesis
{
    struct Line;
    struct Wline;
}

template<class T>
using Stack = std::stack<T>;

template <class T>
using Vec = std::vector<T>;

template <class T>
using Deq = std::deque<T>;

template <class T>
using SPtr = std::shared_ptr<T>;

template <class T>
using UPtr = std::unique_ptr<T>;

template <class T>
using WPtr = std::weak_ptr<T>;

template <class T>
using USet = std::unordered_set<T>;

template <class T, class P = std::less<T>>
using Set = std::set<T, P>;

template <class K, class V>
using UMap = std::unordered_map<K, V>;

template <class K, class V, class P = std::less<K>>
using Map = std::map<K, V, P>;

template <class A, class B>
using Pair = std::pair<A, B>;

template <class A, class... B>
using Tuple = std::tuple<A, B...>;

template <class T>
using VecSPtr = Vec<SPtr<T>>;

using VecChar  = Vec<char>;
using VecStr   = Vec<std::string>;
using VecWstr  = Vec<std::wstring>;
using VecNstr  = Vec<nemesis::Line>;
using VecNwstr = Vec<nemesis::Wline>;

using DeqStr   = Deq<std::string>;
using DeqWstr  = Deq<std::wstring>;
using DeqNstr  = Deq<nemesis::Line>;
using DeqNwstr = Deq<nemesis::Wline>;

using USetStr  = USet<std::string>;
using USetWstr = USet<std::wstring>;

using SetStr  = Set<std::string>;
using SetWstr = Set<std::wstring>;

using ID = UMap<std::string, int>;

using UMapStr2 = UMap<std::string, std::string>;
using UMapSetStr = UMap<std::string, Set<std::string>>;

#if __STDCPP_FLOAT16_T__
#include <stdfloat>

using Float16 = std::float16_t;
#else
#include <cmath>
#include <cstdint>
#include <limits>

struct float16
{
    uint16_t bits;

    // Default constructor
    float16() = default;

    // Implicit constructor from float
    float16(float f)
    {
        bits = float_to_half(f);
    }

    float16& operator=(float f)
    {
        bits = float_to_half(f);
        return *this;
    }

    // Implicit conversion to float
    operator float() const
    {
        return half_to_float(bits);
    }

    // Static cast compatibility
    explicit operator double() const
    {
        return static_cast<float>(*this);
    }

private:
    // Convert float to IEEE 754 half-precision bits
    static uint16_t float_to_half(float f)
    {
        uint32_t x    = std::bit_cast<uint32_t>(f);
        uint32_t sign = (x >> 31) & 0x1;
        uint32_t exp  = (x >> 23) & 0xFF;
        uint32_t mant = x & 0x7FFFFF;

        uint16_t h = 0;

        if (exp == 255)
        { // Inf or NaN
            if (mant == 0)
                h = (sign << 15) | 0x7C00; // Inf
            else
                h = (sign << 15) | 0x7E00; // NaN (canonical)
        }
        else if (exp > 142)
        { // Overflow: set to Inf
            h = (sign << 15) | 0x7C00;
        }
        else if (exp < 113)
        { // Underflow: set to zero
            h = (sign << 15);
        }
        else
        {
            // Normalized case
            int new_exp       = exp - 127 + 15;
            uint16_t new_mant = mant >> 13;
            h                 = (sign << 15) | (new_exp << 10) | new_mant;
        }

        return h;
    }

    // Convert IEEE 754 half-precision bits to float
    static float half_to_float(uint16_t h)
    {
        uint32_t sign = (h >> 15) & 0x1;
        uint32_t exp  = (h >> 10) & 0x1F;
        uint32_t mant = h & 0x3FF;
        uint32_t f;

        if (exp == 0)
        {
            if (mant == 0)
            {
                // Zero
                f = sign << 31;
            }
            else
            {
                // Subnormal
                exp = 1;
                while ((mant & 0x400) == 0)
                {
                    mant <<= 1;
                    exp--;
                }
                mant &= 0x3FF;
                exp = exp + (127 - 15);
                f   = (sign << 31) | (exp << 23) | (mant << 13);
            }
        }
        else if (exp == 0x1F)
        {
            // Inf or NaN
            f = (sign << 31) | (0xFF << 23) | (mant << 13);
        }
        else
        {
            // Normalized
            exp = exp + (127 - 15);
            f   = (sign << 31) | (exp << 23) | (mant << 13);
        }

        float result = std::bit_cast<float>(f);
        return result;
    }
};

using Float16 = float16;
#endif

#define MAX(a, b) a > b ? a : b;
#define MIN(a, b) a < b ? a : b;
