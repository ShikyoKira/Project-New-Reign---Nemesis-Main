#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbTransformVectorModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xf93e0e24,
                                                "hkbTransformVectorModifier",
                                                &nemesis::hkbModifier::Class,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbTransformVectorModifier);

        nemesis::hkQuaternion m_rotation;
        nemesis::hkVector4 m_translation;
        nemesis::hkVector4 m_vectorIn;
        nemesis::hkVector4 m_vectorOut;
        bool m_rotateOnly;
        bool m_inverse;
        bool m_computeOnActivate;
        bool m_computeOnModify;

    public:
        hkbTransformVectorModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbTransformVectorModifier::Class;
}
