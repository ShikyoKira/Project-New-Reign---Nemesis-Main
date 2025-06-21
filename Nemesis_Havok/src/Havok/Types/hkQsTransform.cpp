#include "Havok/Types/hkQsTransform.h"

nemesis::hkVector4& nemesis::hkQsTransform::GetTranslation() noexcept
{
    return m_translation;
}

const nemesis::hkVector4& nemesis::hkQsTransform::GetTranslation() const noexcept
{
    return m_translation;
}

nemesis::hkQuaternion& nemesis::hkQsTransform::GetRotation() noexcept
{
    return m_rotation;
}

const nemesis::hkQuaternion& nemesis::hkQsTransform::GetRotation() const noexcept
{
    return m_rotation;
}

nemesis::hkVector4& nemesis::hkQsTransform::GetScale() noexcept
{
    return m_scale;
}

const nemesis::hkVector4& nemesis::hkQsTransform::GetScale() const noexcept
{
    return m_scale;
}
