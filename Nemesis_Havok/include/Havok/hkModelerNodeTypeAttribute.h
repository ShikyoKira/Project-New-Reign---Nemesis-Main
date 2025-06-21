#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkModelerNodeTypeAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x338c092f,
                                                "hkModelerNodeTypeAttribute",
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
        REGISTER_HAVOK_POINTER_HEADER(hkModelerNodeTypeAttribute);

        using ModelerType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<ModelerType, char> m_type = {
            {"DEFAULT", 0},
            {"LOCATOR", 1},
        };

    public:
        hkModelerNodeTypeAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkModelerNodeTypeAttribute::Class;
}
