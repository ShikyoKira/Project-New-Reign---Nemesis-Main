#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbRoleAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x3eb2e082,
                                                "hkbRoleAttribute",
                                                nullptr,
                                                4,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbRoleAttribute);

        using Role      = nemesis::hkInternalClassEnumItem<short>;
        using RoleFlags = nemesis::hkInternalClassEnumItem<short, true>;

        nemesis::hkEnum<Role, short> m_role = {
            {"ROLE_DEFAULT", 0},
            {"ROLE_FILE_NAME", 1},
            {"ROLE_BONE_INDEX", 2},
            {"ROLE_BONE_INDEX_MAP", 3},
            {"ROLE_EVENT_ID", 4},
            {"ROLE_VARIABLE_INDEX", 5},
            {"ROLE_ATTRIBUTE_INDEX", 6},
            {"ROLE_TIME", 7},
        };
        nemesis::hkEnum<RoleFlags, short> m_flags = {
            {"0", 0},
            {"FLAG_NONE", 0},
            {"FLAG_RAGDOLL", 1},
            {"FLAG_NORMALIZED", 2},
            {"FLAG_NOT_VARIABLE", 4},
            {"FLAG_HIDDEN", 8},
            {"FLAG_OUTPUT", 16},
            {"FLAG_NOT_CHARACTER_PROPERTY", 32},
        };

    public:
        hkbRoleAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRoleAttribute::Class;
}
