#pragma once

#include "Havok/hkpMeshShape.h"
#include "Havok/hkpStorageMeshShapeSubpartStorage.h"

namespace nemesis
{
    struct hkpStorageMeshShape : nemesis::hkpMeshShape
    {
        static constexpr nemesis::hkClass Class{0xbefd8b39,
                                                "hkpStorageMeshShape",
                                                &nemesis::hkpMeshShape::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpStorageMeshShape);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpStorageMeshShapeSubpartStorage>> m_storage;

    public:
        hkpStorageMeshShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStorageMeshShape::Class;
}
