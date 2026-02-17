#pragma once

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkVector4;

    struct hkQuaternion : nemesis::hkVariant
    {
    private:
        float X{}; // align 16
        float Y{}, Z{}, R{};

    public:
        hkQuaternion() noexcept;
        hkQuaternion(float x, float y, float z, float r) noexcept;

        nemesis::hkQuaternion& operator=(const nemesis::hkVector4& vec4);
        nemesis::hkQuaternion& operator=(const nemesis::hkQuaternion& quaternion);

        float GetX() const noexcept;
        float GetY() const noexcept;
        float GetZ() const noexcept;
        float GetR() const noexcept;

        void SetX(float x) noexcept;
        void SetY(float y) noexcept;
        void SetZ(float z) noexcept;
        void SetR(float r) noexcept;
    };
}
