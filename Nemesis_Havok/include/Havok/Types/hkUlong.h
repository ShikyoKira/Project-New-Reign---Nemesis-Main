#pragma once

#include <cstdint>

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
	struct hkUlong : nemesis::hkVariant
	{
    private:
        uint64_t value{};

	public:
        hkUlong() = default;
        hkUlong(uint64_t val) noexcept;

		nemesis::hkUlong& operator=(const hkUlong& ulong) noexcept;
        nemesis::hkUlong& operator=(uint64_t val) noexcept;

		uint64_t AsUint64() const noexcept;
        uint32_t AsUint32() const noexcept;
	};
}
