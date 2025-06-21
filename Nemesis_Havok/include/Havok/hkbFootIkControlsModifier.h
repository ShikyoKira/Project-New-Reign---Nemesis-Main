#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbFootIkControlData.h"
#include "Havok/hkbFootIkControlsModifierLeg.h"

namespace nemesis
{
    struct hkbFootIkControlsModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xe5b6f544,
                                                "hkbFootIkControlsModifier",
                                                &nemesis::hkbModifier::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkControlsModifier);

        nemesis::hkbFootIkControlData m_controlData;
        nemesis::hkArray<nemesis::hkbFootIkControlsModifierLeg> m_legs;
        nemesis::hkVector4 m_errorOutTranslation;
        nemesis::hkQuaternion m_alignWithGroundRotation;

    public:
        hkbFootIkControlsModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkControlsModifier::Class;
}
