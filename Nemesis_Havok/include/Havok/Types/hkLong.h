#pragma once

#include <cstdint>

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
	struct hkLong : nemesis::hkVariant
    {
    private:
        int64_t value{};

    public:
        hkLong() = default;
        hkLong(int64_t val) noexcept;

        nemesis::hkLong& operator=(const hkLong& ulong) noexcept;
        nemesis::hkLong& operator=(int64_t val) noexcept;

        int64_t AsInt64() const noexcept;
        int32_t AsInt32() const noexcept;
	};
}
