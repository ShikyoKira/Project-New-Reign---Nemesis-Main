#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxMeshSection.h"
#include "Havok/hkxMeshUserChannelInfo.h"

namespace nemesis
{
    struct hkxMesh : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xf2edcc5f,
                                                "hkxMesh",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxMesh);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxMeshSection>> m_sections;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxMeshUserChannelInfo>> m_userChannelInfos;

    public:
        hkxMesh() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMesh::Class;
}
