#pragma once

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkQuaternion;

	struct hkVector4 : nemesis::hkVariant
	{
    private:
        float X{}; // align 16
        float Y{}, Z{}, W{};

	public:
        hkVector4() noexcept;
        hkVector4(float x, float y, float z, float w) noexcept;

        nemesis::hkVector4& operator=(const nemesis::hkQuaternion& quaternion);
        nemesis::hkVector4& operator=(const nemesis::hkVector4& vec4);

        float GetX() const noexcept;
        float GetY() const noexcept;
        float GetZ() const noexcept;
        float GetW() const noexcept;

        void SetX(float x) noexcept;
        void SetY(float y) noexcept;
        void SetZ(float z) noexcept;
        void SetW(float w) noexcept;
	};
}
