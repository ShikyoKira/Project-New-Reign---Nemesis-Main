#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkArrayTypeAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd404a39a,
                                                "hkArrayTypeAttribute",
                                                nullptr,
                                                1,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkArrayTypeAttribute);

        using ArrayType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<ArrayType, char> m_type = {
            {"NONE", 0},
            {"POINTSOUP", 1},
            {"ENTITIES", 2},
        };

    public:
        hkArrayTypeAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkArrayTypeAttribute::Class;
}
