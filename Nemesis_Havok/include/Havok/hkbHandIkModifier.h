#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbHandIkModifierHand.h"

namespace nemesis
{
    struct hkbHandIkModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xef8bc2f7,
                                                "hkbHandIkModifier",
                                                &nemesis::hkbModifier::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbHandIkModifier);

        using BlendCurve = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkArray<nemesis::hkbHandIkModifierHand> m_hands;
        nemesis::hkEnum<BlendCurve, char> m_fadeInOutCurve = {
            {"BLEND_CURVE_SMOOTH", 0},
            {"BLEND_CURVE_LINEAR", 1},
            {"BLEND_CURVE_LINEAR_TO_SMOOTH", 2},
            {"BLEND_CURVE_SMOOTH_TO_LINEAR", 3},
        };
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_internalHandData;

    public:
        hkbHandIkModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandIkModifier::Class;
}
