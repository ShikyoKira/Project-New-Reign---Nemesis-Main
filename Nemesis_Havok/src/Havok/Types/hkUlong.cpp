#include "Havok/Types/hkUlong.h"

nemesis::hkUlong::hkUlong(uint64_t val) noexcept
    : value(val)
{
}

nemesis::hkUlong& nemesis::hkUlong::operator=(const hkUlong& ulong) noexcept
{
    value = ulong.value;
    return *this;
}

nemesis::hkUlong& nemesis::hkUlong::operator=(uint64_t val) noexcept
{
    value = val;
    return *this;
}

uint64_t nemesis::hkUlong::AsUint64() const noexcept
{
    return value;
}

uint32_t nemesis::hkUlong::AsUint32() const noexcept
{
    return value;
}
