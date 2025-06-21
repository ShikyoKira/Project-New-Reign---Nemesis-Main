#pragma once

#include "Havok/hkbRoleAttribute.h"

namespace nemesis
{
    struct hkbVariableInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9e746ba2,
                                                "hkbVariableInfo",
                                                nullptr,
                                                6,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbVariableInfo);

        using VariableType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkbRoleAttribute m_role;
        nemesis::hkEnum<VariableType, char> m_type = {
            {"VARIABLE_TYPE_INVALID", -1},
            {"VARIABLE_TYPE_BOOL", 0},
            {"VARIABLE_TYPE_INT8", 1},
            {"VARIABLE_TYPE_INT16", 2},
            {"VARIABLE_TYPE_INT32", 3},
            {"VARIABLE_TYPE_REAL", 4},
            {"VARIABLE_TYPE_POINTER", 5},
            {"VARIABLE_TYPE_VECTOR3", 6},
            {"VARIABLE_TYPE_VECTOR4", 7},
            {"VARIABLE_TYPE_QUATERNION", 8},
        };

    public:
        hkbVariableInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbVariableInfo::Class;
}
