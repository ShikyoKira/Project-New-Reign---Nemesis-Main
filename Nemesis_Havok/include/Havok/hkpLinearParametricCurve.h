#pragma once

#include "Havok/Base/hkpParametricCurve.h"

namespace nemesis
{
    struct hkpLinearParametricCurve : nemesis::hkpParametricCurve
    {
        static constexpr nemesis::hkClass Class{0xd7b3be03,
                                                "hkpLinearParametricCurve",
                                                &nemesis::hkpParametricCurve::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpLinearParametricCurve);

        float m_smoothingFactor;
        bool m_closedLoop;
        nemesis::hkVector4 m_dirNotParallelToTangentAlongWholePath;
        nemesis::hkArray<nemesis::hkVector4> m_points;
        nemesis::hkArray<float> m_distance;

    public:
        hkpLinearParametricCurve() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLinearParametricCurve::Class;
}
