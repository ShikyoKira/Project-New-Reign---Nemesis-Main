#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbCombineTransformsModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xfd1f0b79,
                                                "hkbCombineTransformsModifier",
                                                &nemesis::hkbModifier::Class,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCombineTransformsModifier);

        nemesis::hkVector4 m_translationOut;
        nemesis::hkQuaternion m_rotationOut;
        nemesis::hkVector4 m_leftTranslation;
        nemesis::hkQuaternion m_leftRotation;
        nemesis::hkVector4 m_rightTranslation;
        nemesis::hkQuaternion m_rightRotation;
        bool m_invertLeftTransform;
        bool m_invertRightTransform;
        bool m_invertResult;

    public:
        hkbCombineTransformsModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCombineTransformsModifier::Class;
}
