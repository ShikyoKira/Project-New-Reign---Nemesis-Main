#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpMultiRayShapeRay : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xffdc0b65,
                                                "hkpMultiRayShapeRay",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMultiRayShapeRay);

        nemesis::hkVector4 m_start;
        nemesis::hkVector4 m_end;

    public:
        hkpMultiRayShapeRay() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMultiRayShapeRay::Class;
}
