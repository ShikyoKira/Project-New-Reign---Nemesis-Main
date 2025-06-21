#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkcdShape : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0,
                                                "hkcdShape",
                                                &nemesis::hkReferencedObject::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkcdShape);

        using ShapeType = nemesis::hkInternalClassEnumItem<unsigned char>;
        using DispatchType = nemesis::hkInternalClassEnumItem<unsigned char>;
        using ShapeInfoCodecType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<ShapeType, unsigned char> m_type = {
            //
            //	SPU supported shapes
            //

            {"SPHERE", 0},                              ///< hkpSphereShape type.
            {"CYLINDER", 1},                            ///< hkpCylinderShape type.
            {"TRIANGLE", 2},                            ///< hkpTriangleShape type.
            {"BOX", 3},                                 ///< hkpBoxShape type.
            {"CAPSULE", 4},                             ///< hkpCapsuleShape type.
            {"CONVEX_VERTICES", 5},                     ///< hkpConvexVerticesShape type.
            {"TRI_SAMPLED_HEIGHT_FIELD_COLLECTION", 6}, ///< hkpTriSampledHeightFieldCollection type.
            {"TRI_SAMPLED_HEIGHT_FIELD_BV_TREE", 7},    ///< hkpTriSampledHeightFieldBvTreeShape type.
            {"LIST", 8},                                ///< hkpListShape type.
            {"MOPP", 9},                                ///< hkpMoppBvTreeShape type.
            {"CONVEX_TRANSLATE", 10},                   ///< hkpConvexTranslateShape type.
            {"CONVEX_TRANSFORM", 11},                   ///< hkpConvexTransformShape type.
            {"SAMPLED_HEIGHT_FIELD", 12},               ///< hkpSampledHeightFieldShape type.
            {"EXTENDED_MESH", 13},                      ///< hkpExtendedMeshShape type.
            {"TRANSFORM", 14},                          ///< hkpTransformShape type.
            {"COMPRESSED_MESH", 15},                    ///< hkpCompressedMeshShape type.
            {"STATIC_COMPOUND", 16},                    ///< hkpStaticCompoundShape type.
            {"BV_COMPRESSED_MESH", 17},                 ///< hkpBvCompressedMeshShape type.
            {"COLLECTION",
             18}, ///< All shapes which inherit from hkpShapeCollection have this as an alternate type.
            {"USER0", 19}, ///< Custom user type.
            {"USER1", 20}, ///< Custom user type.
            {"USER2", 21}, ///< Custom user type.

            //
            //	Non-SPU supported shapes
            //

            {"BV_TREE", 22}, ///< All shapes which inherit from hkpBvTreeShape have this as an alternate type.
            {"CONVEX", 23},  ///< All shapes which inherit from hkpConvexShape have this as an alternate type.
            {"CONVEX_PIECE", 24}, ///< DEPRECATED - hkpConvexPieceShape type.
            {"MULTI_SPHERE", 25}, ///< DEPRECATED - hkpMultiSphereShape type.
            {"CONVEX_LIST", 26},  ///< DEPRECATED - hkpConvexListShape.
            {"TRIANGLE_COLLECTION",
             27}, ///< A shape collection which only returns triangles as child shapes, e.g., hkpMeshShape.
            {"HEIGHT_FIELD", 28},     ///< hkpHeightFieldShape type.
            {"SPHERE_REP", 29},       ///< hkpSphereRepShape type.
            {"BV", 30},               ///< hkpBvShape type.
            {"PLANE", 31},            ///< hkpPlaneShape type.
            {"PHANTOM_CALLBACK", 32}, ///< hkpPhantomCallbackShape type.
            {"MULTI_RAY", 33},        ///< hkpMultiRayShape type.
            {"INVALID", 34},          ///< Invalid shape

            {"FIRST_SHAPE_TYPE", 0},    ///< The first real shape.
            {"MAX_SPU_SHAPE_TYPE", 22}, ///< Last SPU support shape type.
            {"MAX_PPU_SHAPE_TYPE", 35}, ///< The end of the shape type list.
            {"ALL_SHAPE_TYPES", -1},    ///< All shapes flag, used by the hkpCollisionDispatcher.
        };
        nemesis::hkEnum<DispatchType, unsigned char> m_dispatchType = {
            {"CONVEX_IMPLICIT", 0},
            {"CONVEX", 1},
            {"HEIGHT_FIELD", 2},
            {"COMPOSITE", 3},
            {"USER", 4},
            {"NUM_DISPATCH_TYPES", 5},
        };
        unsigned char m_bitsPerKey;
        nemesis::hkEnum<ShapeInfoCodecType, unsigned char> m_shapeInfoCodecType = {
            {"NULL_CODEC", 0},
            {"UFM358", 1},
            {"MAX_NUM_CODECS", 16},
        };

    public:
        hkcdShape() noexcept;
        hkcdShape(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkcdShape::Class;
}
