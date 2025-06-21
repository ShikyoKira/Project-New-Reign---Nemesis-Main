#pragma once

#include "Havok/Base/hkpShape.h"

#include "Havok/hkpMultiRayShapeRay.h"

namespace nemesis
{
    struct hkpMultiRayShape : nemesis::hkpShape
    {
        static constexpr nemesis::hkClass Class{0xea2e7ec9,
                                                "hkpMultiRayShape",
                                                &nemesis::hkpShape::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMultiRayShape);

        nemesis::hkArray<nemesis::hkpMultiRayShapeRay> m_rays;
        float m_rayPenetrationDistance;

    public:
        hkpMultiRayShape() noexcept;
               
        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMultiRayShape::Class;
}
