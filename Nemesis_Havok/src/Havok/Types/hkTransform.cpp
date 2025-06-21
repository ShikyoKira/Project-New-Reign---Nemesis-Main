#include "Havok/Types/hkTransform.h"

nemesis::hkRotation& nemesis::hkTransform::GetRotation()
{
    return m_rotation;
}

const nemesis::hkRotation& nemesis::hkTransform::GetRotation() const
{
    return m_rotation;
}

nemesis::hkVector4& nemesis::hkTransform::GetTranslation()
{
    return m_translation;
}

const nemesis::hkVector4& nemesis::hkTransform::GetTranslation() const
{
    return m_translation;
}
