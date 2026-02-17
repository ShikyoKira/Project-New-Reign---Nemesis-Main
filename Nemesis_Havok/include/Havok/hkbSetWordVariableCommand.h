#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbVariableValue.h"

namespace nemesis
{
    struct hkbSetWordVariableCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xf3ae5fca,
                                                "hkbSetWordVariableCommand",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSetWordVariableCommand);

        using VariableType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkVector4 m_quadValue;
        uint64_t m_characterId{};
        int m_variableId{};
        nemesis::hkbVariableValue m_value;
        nemesis::hkEnum<VariableType, unsigned char> m_type = {
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
        bool m_global{};

    public:
        hkbSetWordVariableCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSetWordVariableCommand::Class;
}
