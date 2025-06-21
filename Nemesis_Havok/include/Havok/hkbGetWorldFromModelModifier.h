#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbGetWorldFromModelModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x873fc6f7,
                                                "hkbGetWorldFromModelModifier",
                                                &nemesis::hkbModifier::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbGetWorldFromModelModifier);

        nemesis::hkVector4 m_translationOut;
        nemesis::hkQuaternion m_rotationOut;

    public:
        hkbGetWorldFromModelModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGetWorldFromModelModifier::Class;
}
