#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbHandIkControlsModifierHand.h"

namespace nemesis
{
    struct hkbHandIkControlsModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x9f0488bb,
                                                "hkbHandIkControlsModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbHandIkControlsModifier);

        nemesis::hkArray<nemesis::hkbHandIkControlsModifierHand> m_hands;

    public:
        hkbHandIkControlsModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandIkControlsModifier::Class;
}
