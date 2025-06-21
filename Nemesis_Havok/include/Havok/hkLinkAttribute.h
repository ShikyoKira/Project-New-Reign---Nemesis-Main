#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkLinkAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x255d8164,
                                                "hkLinkAttribute",
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
        REGISTER_HAVOK_POINTER_HEADER(hkLinkAttribute);

        using Link = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<Link, char> m_type = {
            {"NONE", 0},
            {"DIRECT_LINK", 1},
            {"CHILD", 2},
            {"MESH", 3},
            {"PARENT_NAME", 4},
        };

    public:
        hkLinkAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkLinkAttribute::Class;
}
