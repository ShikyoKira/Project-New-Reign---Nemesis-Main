#include "Havok/Types/hkLong.h"

nemesis::hkLong::hkLong(int64_t val) noexcept
    : value(val)
{
}

nemesis::hkLong& nemesis::hkLong::operator=(const hkLong& ulong) noexcept
{
    value = ulong.value;
    return *this;
}

nemesis::hkLong& nemesis::hkLong::operator=(int64_t val) noexcept
{
    value = val;
    return *this;
}

int64_t nemesis::hkLong::AsInt64() const noexcept
{
    return value;
}

int32_t nemesis::hkLong::AsInt32() const noexcept
{
    return value;
}
