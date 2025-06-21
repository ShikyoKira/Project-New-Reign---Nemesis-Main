#pragma once

#include "Havok/Types/hkVector4.h"

namespace nemesis
{
    struct hkVector8 : nemesis::hkVariant
	{
    private:
        nemesis::hkVector4 m_lin;
        nemesis::hkVector4 m_ang;

	public:
        hkVector8() = default;
        hkVector8(const nemesis::hkVector4& lin_vec4, const nemesis::hkVector4& ang_vec4) noexcept;

        nemesis::hkVector4& GetLin();
        nemesis::hkVector4& GetAng();

        const nemesis::hkVector4& GetLin() const;
        const nemesis::hkVector4& GetAng() const;
	};
}
