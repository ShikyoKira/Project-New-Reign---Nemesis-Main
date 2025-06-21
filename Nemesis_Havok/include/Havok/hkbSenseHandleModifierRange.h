#pragma once

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbSenseHandleModifierRange : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xfb56b692,
                                                "hkbSenseHandleModifierRange",
                                                nullptr,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSenseHandleModifierRange);

        nemesis::hkbEventProperty m_event;
        float m_minDistance;
        float m_maxDistance;
        bool m_ignoreHandle;

    public:
        hkbSenseHandleModifierRange() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSenseHandleModifierRange::Class;
}
