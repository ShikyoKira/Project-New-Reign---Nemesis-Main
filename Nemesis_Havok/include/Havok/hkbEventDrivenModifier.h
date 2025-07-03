#pragma once

#include "Havok/Base/hkbModifierWrapper.h"

namespace nemesis
{
    struct hkbEventDrivenModifier : nemesis::hkbModifierWrapper
    {
        static constexpr nemesis::hkClass Class{0x7ed3f44e,
                                                "hkbEventDrivenModifier",
                                                &nemesis::hkbModifierWrapper::Class,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEventDrivenModifier);

        nemesis::hkEventId m_activateEventId;
        nemesis::hkEventId m_deactivateEventId;
        bool m_activeByDefault;
        bool m_isActive;

    public:
        hkbEventDrivenModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventDrivenModifier::Class;
}
