#pragma once

#include "Havok/Base/hkResourceContainer.h"

#include "Havok/hkMemoryResourceHandle.h"

namespace nemesis
{
    struct hkMemoryResourceContainer : nemesis::hkResourceContainer
    {
        static constexpr nemesis::hkClass Class{0x4762f92a,
                                                "hkMemoryResourceContainer",
                                                &nemesis::hkResourceContainer::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkMemoryResourceContainer);

        bool m_nameIsAllocated{};
        nemesis::hkStringPtr m_name;
        nemesis::hkRefPtr<nemesis::hkMemoryResourceContainer> m_parent;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkMemoryResourceHandle>> m_resourceHandles;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkMemoryResourceContainer>> m_children;

    public:
        hkMemoryResourceContainer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMemoryResourceContainer::Class;
}
