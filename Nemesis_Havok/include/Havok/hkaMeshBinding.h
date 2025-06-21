#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkaMeshBindingMapping.h"
#include "Havok/hkaSkeleton.h"
#include "Havok/hkxMesh.h"

namespace nemesis
{
    struct hkaMeshBinding : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x81d9950b,
                                                "hkaMeshBinding",
                                                &nemesis::hkReferencedObject::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaMeshBinding);

        nemesis::hkRefPtr<nemesis::hkxMesh> m_mesh;
        nemesis::hkStringPtr m_originalSkeletonName;
        nemesis::hkStringPtr m_name;
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_skeleton;
        nemesis::hkArray<nemesis::hkaMeshBindingMapping> m_mappings;
        nemesis::hkArray<nemesis::hkTransform> m_boneFromSkinMeshTransforms;

    public:
        hkaMeshBinding() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaMeshBinding::Class;
}
