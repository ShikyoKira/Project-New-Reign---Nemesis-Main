#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpStorageMeshShapeSubpartStorage : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xbf27438,
                                                "hkpStorageMeshShapeSubpartStorage",
                                                &nemesis::hkReferencedObject::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpStorageMeshShapeSubpartStorage);

        nemesis::hkArray<float> m_vertices;
        nemesis::hkArray<unsigned short> m_indices16;
        nemesis::hkArray<unsigned int> m_indices32;
        nemesis::hkArray<unsigned char> m_materialIndices;
        nemesis::hkArray<unsigned int> m_materials;
        nemesis::hkArray<unsigned short> m_materialIndices16;

    public:
        hkpStorageMeshShapeSubpartStorage() noexcept;
        hkpStorageMeshShapeSubpartStorage(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpStorageMeshShapeSubpartStorage::Class;
}
