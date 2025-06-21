#pragma once

#include "Havok/Base/hkpConvexShape.h"

namespace nemesis
{
    struct hkpSphereShape : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0x795d9fa,
                                                "hkpSphereShape",
                                                &nemesis::hkpConvexShape::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpSphereShape);

        std::array<unsigned int, 3> m_pad16;

    public:
        hkpSphereShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSphereShape::Class;
}
