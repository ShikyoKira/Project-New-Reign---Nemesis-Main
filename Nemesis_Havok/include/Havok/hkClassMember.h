#pragma once

#include "Havok/hkClass.h"
#include "Havok/hkRefPtr.h"
#include "Havok/hkClassEnum.h"
#include "Havok/hkCustomAttributes.h"

#include "Havok/Types/hkCString.h"

namespace nemesis
{
    struct hkClassMember : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x5c7ea4c2,
                                                "hkClassMember",
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
        REGISTER_HAVOK_POINTER_HEADER(hkClassMember);

        using Type = nemesis::hkInternalClassEnumItem<unsigned char>;
        using FlagValues = nemesis::hkInternalClassEnumItem<unsigned short, true>;

        nemesis::hkCString m_name;
        nemesis::hkRefPtr<nemesis::hkClass> m_class;
        nemesis::hkRefPtr<nemesis::hkClassEnum> m_enum;
        nemesis::hkEnum<Type, unsigned char> m_type = {
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
        nemesis::hkEnum<Type, unsigned char> m_subtype = {
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
        short m_cArraySize{};
        nemesis::hkEnum<FlagValues, unsigned short> m_flags = {
            {"FLAGS_NONE", 0},
            {"ALIGN_8", 128},
            {"ALIGN_16", 256},
            {"NOT_OWNED", 512},
            {"SERIALIZE_IGNORED", 1024},
            {"ALIGN_32", 2048},
            {"ALIGN_REAL", 256},
        };
        unsigned short m_offset{};
        nemesis::hkRefPtr<nemesis::hkCustomAttributes> m_attributes;

    public:
        hkClassMember() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkClassMember::Class;
}
