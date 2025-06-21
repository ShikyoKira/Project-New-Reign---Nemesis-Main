#pragma once

#include "Havok/hkbShapeSetup.h"

namespace nemesis
{
    struct hkbRigidBodySetup : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x3b082f95,
                                                "hkbRigidBodySetup",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbRigidBodySetup);

        using RigidBodyType = nemesis::hkInternalClassEnumItem<char>;

        unsigned int m_collisionFilterInfo;
        nemesis::hkEnum<RigidBodyType, char> m_type = {
            {"INVALID", -1},
            {"KEYFRAMED", 0},
            {"DYNAMIC", 1},
            {"FIXED", 2},
        };
        nemesis::hkbShapeSetup m_shapeSetup;

    public:
        hkbRigidBodySetup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRigidBodySetup::Class;
}
