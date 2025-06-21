#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkGeometryTriangle.h"

namespace nemesis
{
    struct hkGeometry : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x98dd8bdc,
                                                "hkGeometry",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkGeometry);

        nemesis::hkArray<nemesis::hkVector4> m_vertices;
        nemesis::hkArray<nemesis::hkGeometryTriangle> m_triangles;

    public:
        hkGeometry() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkGeometry::Class;
}
