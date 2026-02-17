#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbSetWorldFromModelModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xafcfa211,
                                                "hkbSetWorldFromModelModifier",
                                                &nemesis::hkbModifier::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSetWorldFromModelModifier);

        nemesis::hkVector4 m_translation;
        nemesis::hkQuaternion m_rotation;
        bool m_setTranslation{};
        bool m_setRotation{};

    public:
        hkbSetWorldFromModelModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSetWorldFromModelModifier::Class;
}
