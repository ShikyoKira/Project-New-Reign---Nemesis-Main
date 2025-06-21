#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbHandIkDriverInfoHand.h"

namespace nemesis
{
    struct hkbHandIkDriverInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc299090a,
                                                "hkbHandIkDriverInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbHandIkDriverInfo);

        using BlendCurve = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkArray<nemesis::hkbHandIkDriverInfoHand> m_hands;
        nemesis::hkEnum<BlendCurve, char> m_fadeInOutCurve = {
            {"BLEND_CURVE_SMOOTH", 0},
            {"BLEND_CURVE_LINEAR", 1},
            {"BLEND_CURVE_LINEAR_TO_SMOOTH", 2},
            {"BLEND_CURVE_SMOOTH_TO_LINEAR", 3},
        };

    public:
        hkbHandIkDriverInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandIkDriverInfo::Class;
}
