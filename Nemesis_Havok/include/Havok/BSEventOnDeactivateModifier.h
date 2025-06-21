#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSEventOnDeactivateModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x1062d993,
                                                "BSEventOnDeactivateModifier",
                                                &nemesis::hkbModifier::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSEventOnDeactivateModifier);

        nemesis::hkbEventProperty m_event;

    public:
        BSEventOnDeactivateModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSEventOnDeactivateModifier::Class;
}
