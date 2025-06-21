#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkMeshBoneIndexMapping.h"
#include "Havok/hkxIndexBuffer.h"
#include "Havok/hkxMaterial.h"
#include "Havok/hkxVertexAnimation.h"
#include "Havok/hkxVertexBuffer.h"

namespace nemesis
{
    struct hkxMeshSection : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xe2286cf8,
                                                "hkxMeshSection",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxMeshSection);

        nemesis::hkRefPtr<nemesis::hkxVertexBuffer> m_vertexBuffer;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxIndexBuffer>> m_indexBuffers;
        nemesis::hkRefPtr<nemesis::hkxMaterial> m_material;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkReferencedObject>> m_userChannels;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxVertexAnimation>> m_vertexAnimations;
        nemesis::hkArray<float> m_linearKeyFrameHints;
        nemesis::hkArray<nemesis::hkMeshBoneIndexMapping> m_boneMatrixMap;

    public:
        hkxMeshSection() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMeshSection::Class;
}
