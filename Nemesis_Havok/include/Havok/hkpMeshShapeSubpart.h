#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpMeshShapeSubpart : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x27336e5d,
                                                "hkpMeshShapeSubpart",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMeshShapeSubpart);

        using MeshShapeIndexStridingType = nemesis::hkInternalClassEnumItem<char>;
        using MeshShapeMaterialIndexStridingType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkRefPtr<nemesis::HavokObject> m_vertexBase;
        int m_vertexStriding;
        int m_numVertices;
        nemesis::hkRefPtr<nemesis::HavokObject> m_indexBase;
        nemesis::hkEnum<MeshShapeIndexStridingType, char> m_stridingType = {
            {"INDICES_INVALID", 0},
            {"INDICES_INT16", 1},
            {"INDICES_INT32", 2},
            {"INDICES_MAX_ID", 3},
        };
        nemesis::hkEnum<MeshShapeMaterialIndexStridingType, char> m_materialIndexStridingType = {
            {"MATERIAL_INDICES_INVALID", 0},
            {"MATERIAL_INDICES_INT8", 1},
            {"MATERIAL_INDICES_INT16", 2},
            {"MATERIAL_INDICES_MAX_ID", 3},
        };
        int m_indexStriding;
        int m_flipAlternateTriangles;
        int m_numTriangles;
        nemesis::hkRefPtr<nemesis::HavokObject> m_materialIndexBase;
        int m_materialIndexStriding;
        nemesis::hkRefPtr<nemesis::HavokObject> m_materialBase;
        int m_materialStriding;
        int m_numMaterials;
        int m_triangleOffset;

    public:
        hkpMeshShapeSubpart() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMeshShapeSubpart::Class;
}
