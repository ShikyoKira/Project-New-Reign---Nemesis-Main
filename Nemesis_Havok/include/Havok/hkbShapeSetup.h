#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbShapeSetup : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd7ff86be,
                                                "hkbShapeSetup",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbShapeSetup);

        using ShapeType = nemesis::hkInternalClassEnumItem<char>;

        float m_capsuleHeight{};
        float m_capsuleRadius{};
        nemesis::hkStringPtr m_fileName;
        nemesis::hkEnum<ShapeType, char> m_type = {
            {"CAPSULE", 0},
            {"FILE", 1},
        };

    public:
        hkbShapeSetup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbShapeSetup::Class;
}
