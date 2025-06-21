#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxNode.h"
#include "Havok/hkxMesh.h"

namespace nemesis
{
    struct hkxSkinBinding : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x5a93f338,
                                                "hkxSkinBinding",
                                                &nemesis::hkReferencedObject::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxSkinBinding);

        nemesis::hkRefPtr<nemesis::hkxMesh> m_mesh;
        nemesis::hkRefPtr<nemesis::hkxNode> m_mapping;
        int m_numMapping;
        nemesis::hkArray<nemesis::hkCString> m_nodeNames;
        nemesis::hkArray<nemesis::hkMatrix4> m_bindPose;
        nemesis::hkMatrix4 m_initSkinTransform;

    public:
        hkxSkinBinding() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSkinBinding::Class;
}
