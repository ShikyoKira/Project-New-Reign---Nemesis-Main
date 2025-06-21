#pragma once

#include "Havok/Types/hkVector4.h"
#include "Havok/Types/hkQuaternion.h"

namespace nemesis
{
    struct hkQsTransform : nemesis::hkVariant
    {
    private:
        nemesis::hkVector4 m_translation;
        nemesis::hkQuaternion m_rotation;
        nemesis::hkVector4 m_scale;

    public:
        nemesis::hkVector4& GetTranslation() noexcept;
        const nemesis::hkVector4& GetTranslation() const noexcept;

        nemesis::hkQuaternion& GetRotation() noexcept;
        const nemesis::hkQuaternion& GetRotation() const noexcept;

        nemesis::hkVector4& GetScale() noexcept;
        const nemesis::hkVector4& GetScale() const noexcept;
    };
}
