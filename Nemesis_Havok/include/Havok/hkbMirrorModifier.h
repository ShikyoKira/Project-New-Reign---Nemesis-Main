#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbMirrorModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xa9a271ea,
                                                "hkbMirrorModifier",
                                                &nemesis::hkbModifier::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbMirrorModifier);

        bool m_isAdditive{};

    public:
        hkbMirrorModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbMirrorModifier::Class;
}
