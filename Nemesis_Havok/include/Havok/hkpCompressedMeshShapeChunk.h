#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpCompressedMeshShapeChunk : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x5d0d67bd,
                                                "hkpCompressedMeshShapeChunk",
                                                nullptr,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCompressedMeshShapeChunk);

        nemesis::hkVector4 m_offset;
        nemesis::hkArray<unsigned short> m_vertices;
        nemesis::hkArray<unsigned short> m_indices;
        nemesis::hkArray<unsigned short> m_stripLengths;
        nemesis::hkArray<unsigned short> m_weldingInfo;
        unsigned int m_materialInfo{};
        unsigned short m_reference{};
        unsigned short m_transformIndex{};

    public:
        hkpCompressedMeshShapeChunk() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCompressedMeshShapeChunk::Class;
}
