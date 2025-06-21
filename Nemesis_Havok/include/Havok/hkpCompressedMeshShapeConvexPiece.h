#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpCompressedMeshShapeConvexPiece : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x385bb842,
                                                "hkpCompressedMeshShapeConvexPiece",
                                                nullptr,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCompressedMeshShapeConvexPiece);

        nemesis::hkVector4 m_offset;
        nemesis::hkArray<unsigned short> m_vertices;
        nemesis::hkArray<unsigned char> m_faceVertices;
        nemesis::hkArray<unsigned short> m_faceOffsets;
        unsigned short m_reference;
        unsigned short m_transformIndex;

    public:
        hkpCompressedMeshShapeConvexPiece() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCompressedMeshShapeConvexPiece::Class;
}
