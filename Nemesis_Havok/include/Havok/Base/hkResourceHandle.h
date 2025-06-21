#pragma once

#include "Havok/Base/hkResourceBase.h"

#include "Havok/hkMemoryResourceHandleExternalLink.h"

namespace nemesis
{
    struct hkResourceHandle : nemesis::hkResourceBase
    {
        static constexpr nemesis::hkClass Class{0x4e94146,
                                                "hkResourceHandle",
                                                &nemesis::hkResourceBase::Class,
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
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_variant;
        nemesis::hkStringPtr m_name;
        nemesis::hkArray<nemesis::hkMemoryResourceHandleExternalLink> m_references;

    public:
        hkResourceHandle(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkResourceHandle::Class;
}
