#include "Havok/Types/hkQuaternion.h"
#include "Havok/Types/hkVector4.h"

nemesis::hkQuaternion::hkQuaternion() noexcept
    : X(0)
    , Y(0)
    , Z(0)
    , R(0)
{
}

nemesis::hkQuaternion::hkQuaternion(float x, float y, float z, float r) noexcept
    : X(x)
    , Y(y)
    , Z(z)
    , R(r)
{
}

nemesis::hkQuaternion& nemesis::hkQuaternion::operator=(const nemesis::hkVector4& vec4)
{
    X = vec4.GetX();
    Y = vec4.GetY();
    Z = vec4.GetZ();
    R = vec4.GetW();
    return *this;
}

nemesis::hkQuaternion& nemesis::hkQuaternion::operator=(const nemesis::hkQuaternion& quaternion)
{
    X = quaternion.X;
    Y = quaternion.Y;
    Z = quaternion.Z;
    R = quaternion.R;
    return *this;
}

float nemesis::hkQuaternion::GetX() const noexcept
{
    return X;
}

float nemesis::hkQuaternion::GetY() const noexcept
{
    return Y;
}

float nemesis::hkQuaternion::GetZ() const noexcept
{
    return Z;
}

float nemesis::hkQuaternion::GetR() const noexcept
{
    return R;
}

void nemesis::hkQuaternion::SetX(float x) noexcept
{
    X = x;
}

void nemesis::hkQuaternion::SetY(float y) noexcept
{
    Y = y;
}

void nemesis::hkQuaternion::SetZ(float z) noexcept
{
    Z = z;
}

void nemesis::hkQuaternion::SetR(float r) noexcept
{
    R = r;
}
