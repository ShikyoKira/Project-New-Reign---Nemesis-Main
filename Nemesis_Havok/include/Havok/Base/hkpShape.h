#pragma once

#include "Havok/Base/hkpShapeBase.h"

namespace nemesis
{
    struct hkpShape : nemesis::hkpShapeBase
    {
        static constexpr nemesis::hkClass Class{0x666490a1,
                                                "hkpShape",
                                                &nemesis::hkpShapeBase::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        using hkpShapeType = nemesis::hkInternalClassEnumItem<unsigned int>;
        
        nemesis::hkUlong m_userData;

        nemesis::hkEnum<hkpShapeType, unsigned int> m_type = {
            //
            // Shape types from HK_SHAPE_INVALID to HK_SHAPE_MAX_ID_SPU are supported on the SPU.
            //

            {"HK_SHAPE_INVALID", 0},

            //
            //	The abstract base shapes
            //

            //
            // Special convex shapes, which get their private agents for better performance on
            // PPU only.  All use predictive GSK agent on SPU.
            //

            /// hkpSphereShape type.
            {"HK_SHAPE_SPHERE", 1},
            /// The first real shape.
            {"HK_FIRST_SHAPE_TYPE", 1},
            /// hkpCylinderShape type.
            {"HK_SHAPE_CYLINDER", 2},
            /// hkpTriangleShape type.
            {"HK_SHAPE_TRIANGLE", 3},
            /// hkpBoxShape type.
            {"HK_SHAPE_BOX", 4},
            /// hkpCapsuleShape type.
            {"HK_SHAPE_CAPSULE", 5},
            /// hkpConvexVerticesShape type.
            {"HK_SHAPE_CONVEX_VERTICES", 6},

            //
            // Special shape collections that are solved on the SPU.
            //

            /// All shapes which inherit from hkpShapeCollection have this as an alternate type.
            {"HK_SHAPE_COLLECTION", 7},

            /// All shapes which inherit from hkpBvTreeShape have this as an alternate type.
            {"HK_SHAPE_BV_TREE", 8},

            /// hkpListShape type.
            {"HK_SHAPE_LIST", 9},

            /// hkpMoppBvTreeShape type.
            {"HK_SHAPE_MOPP", 10},

            /// hkpConvexTranslateShape type.
            {"HK_SHAPE_CONVEX_TRANSLATE", 11},

            /// hkpConvexTransformShape type.
            {"HK_SHAPE_CONVEX_TRANSFORM", 12},

            /// hkpSampledHeightFieldShape type.
            {"HK_SHAPE_SAMPLED_HEIGHT_FIELD", 13},

            /// hkpExtendedMeshShape type.
            {"HK_SHAPE_EXTENDED_MESH", 14},

            /// hkpTransformShape type.
            {"HK_SHAPE_TRANSFORM", 15},

            /// hkpCompressedMeshShape type.
            {"HK_SHAPE_COMPRESSED_MESH", 16},

            /// hkpCompoundShape
            {"HK_SHAPE_COMPOUND", 17},

            //
            // Shape types from HK_SHAPE_MAX_ID_SPU to HK_SHAPE_MAX_ID are NOT supported on the SPU.
            //

            // Last SPU support shape type
            {"HK_SHAPE_MAX_ID_SPU", 18},

            /// All shapes which inherit from hkpConvexShape have this as an alternate type.
            {"HK_SHAPE_CONVEX", 19},

            //HK_SHAPE_PACKED_CONVEX_VERTICES,

            /// DEPRECATED - hkpMoppEmbeddedShape type.
            {"HK_SHAPE_MOPP_EMBEDDED", 20},

            /// DEPRECATED - hkpConvexPieceShape type.
            {"HK_SHAPE_CONVEX_PIECE", 21},

            //
            //	hkpShapeCollection implementations
            //

            /// DEPRECATED - hkpMultiSphereShape type.
            {"HK_SHAPE_MULTI_SPHERE", 22},

            /// DEPRECATED - hkpConvexListShape.
            {"HK_SHAPE_CONVEX_LIST", 23},

            /// A shape collection which only returns triangles as child shapes, e.g., hkpMeshShape.
            {"HK_SHAPE_TRIANGLE_COLLECTION", 24},

            //
            // Special shapes
            //

            /// hkpMultiRayShape type.
            {"HK_SHAPE_MULTI_RAY", 25},
            /// hkpHeightFieldShape type.
            {"HK_SHAPE_HEIGHT_FIELD", 26},
            /// hkpSphereRepShape type.
            {"HK_SHAPE_SPHERE_REP", 27},
            /// hkpBvShape type.
            {"HK_SHAPE_BV", 28},
            /// hkpPlaneShape type.
            {"HK_SHAPE_PLANE", 29},

            //
            //	Single shapes which are processed by unary agents.
            //

            /// hkpPhantomCallbackShape type.
            {"HK_SHAPE_PHANTOM_CALLBACK", 30},

            //
            //	user shapes
            //

            {"HK_SHAPE_USER0", 31},
            {"HK_SHAPE_USER1", 32},
            {"HK_SHAPE_USER2", 33},

            ///	The end of the shape type list.
            {"HK_SHAPE_MAX_ID", 34},

            /// All shape flag, used by the hkpCollisionDispatcher.
            {"HK_SHAPE_ALL", -1},
        };

    public:
        hkpShape(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpShape::Class;
}
