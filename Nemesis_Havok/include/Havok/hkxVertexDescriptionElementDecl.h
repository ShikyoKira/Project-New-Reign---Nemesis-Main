#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkxVertexDescriptionElementDecl : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x483a429b,
                                                "hkxVertexDescriptionElementDecl",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexDescriptionElementDecl);

        using DataType = nemesis::hkInternalClassEnumItem<unsigned short>;
        using DataUsage = nemesis::hkInternalClassEnumItem<unsigned short>;
        using DataHint = nemesis::hkInternalClassEnumItem<unsigned short>;

        unsigned int m_byteOffset;
        nemesis::hkEnum<DataType, unsigned short> m_type = {
            {"HKX_DT_NONE", 0},
            {"HKX_DT_UINT8", 1},
            {"HKX_DT_INT16", 2},
            {"HKX_DT_UINT32", 3},
            {"HKX_DT_FLOAT", 4},
        };
        nemesis::hkEnum<DataUsage, unsigned short> m_usage = {
            {"HKX_DU_NONE", 0},
            {"HKX_DU_POSITION", 1},
            {"HKX_DU_COLOR", 2},
            {"HKX_DU_NORMAL", 4},
            {"HKX_DU_TANGENT", 8},
            {"HKX_DU_BINORMAL", 16},
            {"HKX_DU_TEXCOORD", 32},
            {"HKX_DU_BLENDWEIGHTS", 64},
            {"HKX_DU_BLENDINDICES", 128},
            {"HKX_DU_USERDATA", 256},
        };
        unsigned int m_byteStride;
        unsigned char m_numElements;
        nemesis::hkEnum<DataHint, unsigned short> m_hint = {
            {"HKX_DH_NONE", 0},
            {"HKX_DH_LIGHTMAP", 1},
        };
        nemesis::hkStringPtr m_channelID;

    public:
        hkxVertexDescriptionElementDecl() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexDescriptionElementDecl::Class;
}
