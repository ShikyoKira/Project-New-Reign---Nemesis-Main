#pragma once

#include "Havok/Base/hkResourceHandle.h"

namespace nemesis
{
    struct hkMemoryResourceHandle : nemesis::hkResourceHandle
    {
        static constexpr nemesis::hkClass Class{0xbffac086,
                                                "hkMemoryResourceHandle",
                                                &nemesis::hkResourceHandle::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkMemoryResourceHandle);

        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_variant;
        bool m_objectIsRerencedObject;
        bool m_nameIsAllocated;
        nemesis::hkStringPtr m_name;
        nemesis::hkArray<nemesis::hkMemoryResourceHandleExternalLink> m_references;

    public:
        hkMemoryResourceHandle() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMemoryResourceHandle::Class;
}
