#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxSplineControlPoint : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkxSplineControlPoint",
                                                nullptr,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxSplineControlPoint);

        using ControlType = nemesis::hkInternalClassEnumItem<unsigned char>;
        using ControlType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkVector4 m_position;
        nemesis::hkVector4 m_tangentIn;
        nemesis::hkVector4 m_tangentOut;
        nemesis::hkEnum<ControlType, unsigned char> m_inType = {
            {"BEZIER_SMOOTH", 0},
            {"BEZIER_CORNER", 1},
            {"LINEAR", 2},
            {"CUSTOM", 3},
        };
        nemesis::hkEnum<ControlType, unsigned char> m_outType = {
            {"BEZIER_SMOOTH", 0},
            {"BEZIER_CORNER", 1},
            {"LINEAR", 2},
            {"CUSTOM", 3},
        };

    public:
        hkxSplineControlPoint() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSplineControlPoint::Class;
}
