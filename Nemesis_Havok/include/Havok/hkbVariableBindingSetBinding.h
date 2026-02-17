#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
	struct hkbVariableBindingSetBinding : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x4d592f72,
                                                "hkbVariableBindingSetBinding",
                                                nullptr,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbVariableBindingSetBinding);

        using BindingType = nemesis::hkInternalClassEnumItem<char>;
        using MemberType = nemesis::hkInternalClassEnumItem<unsigned char>;
        using InternalBindingFlags = nemesis::hkInternalClassEnumItem<char, true>;

        nemesis::hkStringPtr m_memberPath;
        nemesis::hkRefPtr<nemesis::HavokObject> m_memberClass;
        int m_offsetInObjectPlusOne{};
        int m_offsetInArrayPlusOne{};
        nemesis::hkVariableId m_rootVariableIndex;
        nemesis::hkVariableId m_variableIndex;
        char m_bitIndex{};
        nemesis::hkEnum<BindingType, char> m_bindingType = {
            {"BINDING_TYPE_VARIABLE", 0},
            {"BINDING_TYPE_CHARACTER_PROPERTY", 1},
        };
        nemesis::hkEnum<MemberType, unsigned char> m_memberType = {
            {"TYPE_VOID", 0},         {"TYPE_BOOL", 1},
            {"TYPE_CHAR", 2},         {"TYPE_INT8", 3},
            {"TYPE_UINT8", 4},        {"TYPE_INT16", 5},
            {"TYPE_UINT16", 6},       {"TYPE_INT32", 7},
            {"TYPE_UINT32", 8},       {"TYPE_INT64", 9},
            {"TYPE_UINT64", 10},      {"TYPE_REAL", 11},
            {"TYPE_VECTOR4", 12},     {"TYPE_QUATERNION", 13},
            {"TYPE_MATRIX3", 14},     {"TYPE_ROTATION", 15},
            {"TYPE_QSTRANSFORM", 16}, {"TYPE_MATRIX4", 17},
            {"TYPE_TRANSFORM", 18},   {"TYPE_ZERO", 19},
            {"TYPE_POINTER", 20},     {"TYPE_FUNCTIONPOINTER", 21},
            {"TYPE_ARRAY", 22},       {"TYPE_INPLACEARRAY", 23},
            {"TYPE_ENUM", 24},        {"TYPE_STRUCT", 25},
            {"TYPE_SIMPLEARRAY", 26}, {"TYPE_HOMOGENEOUSARRAY", 27},
            {"TYPE_VARIANT", 28},     {"TYPE_CSTRING", 29},
            {"TYPE_ULONG", 30},       {"TYPE_FLAGS", 31},
            {"TYPE_HALF", 32},        {"TYPE_STRINGPTR", 33},
            {"TYPE_RELARRAY", 34},    {"TYPE_MAX", 35},
        };
        char m_variableType{};
        nemesis::hkEnum<InternalBindingFlags, char> m_flags = {
            //{"FLAG_NONE", 0},
            {"0", 0},
            {"FLAG_OUTPUT", 1},
        };

    public:
        hkbVariableBindingSetBinding() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbVariableBindingSetBinding::Class;
}
