#pragma once

#include "Havok/Base/hkpConvexShape.h"

#include "Havok/hkFourTransposedPoints.h"
#include "Havok/hkpConvexVerticesShapeFourVectors.h"
#include "Havok/hkpConvexVerticesConnectivity.h"

namespace nemesis
{
    struct hkpConvexVerticesShape : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0x28726ad8,
                                                "hkpConvexVerticesShape",
                                                &nemesis::hkpConvexShape::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexVerticesShape);

        nemesis::hkVector4 m_aabbHalfExtents;
        nemesis::hkVector4 m_aabbCenter;
        nemesis::hkArray<nemesis::hkpConvexVerticesShapeFourVectors> m_rotatedVertices_0;
        nemesis::hkArray<nemesis::hkFourTransposedPoints> m_rotatedVertices_1;
        nemesis::hkArray<nemesis::hkQsTransform> m_rotatedVertices_2;
        int m_numVertices{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_convexPiece;
        nemesis::hkRefPtr<nemesis::HavokObject> m_externalObject;
        nemesis::hkRefPtr<nemesis::HavokObject> m_getFaceNormals;
        nemesis::hkArray<nemesis::hkVector4> m_planeEquations;
        nemesis::hkRefPtr<nemesis::hkpConvexVerticesConnectivity> m_connectivity;

    public:
        hkpConvexVerticesShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexVerticesShape::Class;
}
