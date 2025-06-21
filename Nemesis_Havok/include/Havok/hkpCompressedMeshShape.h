#pragma once

#include "Havok/hkpShapeCollection.h"

#include "Havok/hkAabb.h"
#include "Havok/hkpCompressedMeshShapeBigTriangle.h"
#include "Havok/hkpCompressedMeshShapeChunk.h"
#include "Havok/hkpCompressedMeshShapeConvexPiece.h"
#include "Havok/hkpNamedMeshMaterial.h"

namespace nemesis
{
    struct hkpCompressedMeshShape : nemesis::hkpShapeCollection
    {
        static constexpr nemesis::hkClass Class{0xa62d5e6e,
                                                "hkpCompressedMeshShape",
                                                &nemesis::hkpShapeCollection::Class,
                                                304,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCompressedMeshShape);

        using WeldingType = nemesis::hkInternalClassEnumItem<unsigned char>;
        using MaterialType = nemesis::hkInternalClassEnumItem<unsigned char>;

        int m_bitsPerIndex;
        int m_bitsPerWIndex;
        int m_wIndexMask;
        int m_indexMask;
        float m_radius;
        nemesis::hkEnum<WeldingType, unsigned char> m_weldingType = {
            {"WELDING_TYPE_ANTICLOCKWISE", 0},
            {"WELDING_TYPE_CLOCKWISE", 4},
            {"WELDING_TYPE_TWO_SIDED", 5},
            {"WELDING_TYPE_NONE", 6},
        };
        nemesis::hkEnum<MaterialType, unsigned char> m_materialType = {
            {"MATERIAL_NONE", 0},
            {"MATERIAL_SINGLE_VALUE_PER_CHUNK", 1},
            {"MATERIAL_ONE_BYTE_PER_TRIANGLE", 2},
            {"MATERIAL_TWO_BYTES_PER_TRIANGLE", 3},
            {"MATERIAL_FOUR_BYTES_PER_TRIANGLE", 4},
        };
        nemesis::hkArray<unsigned int> m_materials;
        nemesis::hkArray<unsigned short> m_materials16;
        nemesis::hkArray<unsigned char> m_materials8;
        nemesis::hkArray<nemesis::hkTransform> m_transforms_0;
        nemesis::hkArray<nemesis::hkQsTransform> m_transforms_1;
        nemesis::hkArray<nemesis::hkVector4> m_bigVertices;
        nemesis::hkArray<nemesis::hkpCompressedMeshShapeBigTriangle> m_bigTriangles;
        nemesis::hkArray<nemesis::hkpCompressedMeshShapeChunk> m_chunks;
        nemesis::hkArray<nemesis::hkpCompressedMeshShapeConvexPiece> m_convexPieces;
        float m_error;
        nemesis::hkAabb m_bounds;
        unsigned int m_defaultCollisionFilterInfo;
        nemesis::hkRefPtr<nemesis::HavokObject> m_meshMaterials;
        unsigned short m_materialStriding;
        unsigned short m_numMaterials;
        nemesis::hkArray<nemesis::hkpNamedMeshMaterial> m_namedMaterials;
        nemesis::hkVector4 m_scaling;

    public:
        hkpCompressedMeshShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCompressedMeshShape::Class;
}
