#include "Havok/Types/hkQuaternion.h"
#include "Havok/Types/hkVector4.h"

nemesis::hkVector4::hkVector4() noexcept
    : X(0)
    , Y(0)
    , Z(0)
    , W(0)
{
}

nemesis::hkVector4::hkVector4(float x, float y, float z, float w) noexcept
    : X(x)
    , Y(y)
    , Z(z)
    , W(w)
{
}

nemesis::hkVector4& nemesis::hkVector4::operator=(const nemesis::hkQuaternion& quaternion)
{
    X = quaternion.GetX();
    Y = quaternion.GetY();
    Z = quaternion.GetZ();
    W = quaternion.GetR();
    return *this;
}

nemesis::hkVector4& nemesis::hkVector4::operator=(const nemesis::hkVector4& vec4)
{
    X = vec4.X;
    Y = vec4.Y;
    Z = vec4.Z;
    W = vec4.W;
    return *this;
}

float nemesis::hkVector4::GetX() const noexcept
{
    return X;
}

float nemesis::hkVector4::GetY() const noexcept
{
    return Y;
}

float nemesis::hkVector4::GetZ() const noexcept
{
    return Z;
}

float nemesis::hkVector4::GetW() const noexcept
{
    return W;
}

void nemesis::hkVector4::SetX(float x) noexcept
{
    X = x;
}

void nemesis::hkVector4::SetY(float y) noexcept
{
    Y = y;
}

void nemesis::hkVector4::SetZ(float z) noexcept
{
    Z = z;
}

void nemesis::hkVector4::SetW(float w) noexcept
{
    W = w;
}
