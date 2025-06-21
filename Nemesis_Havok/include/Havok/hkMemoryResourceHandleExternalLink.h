#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkMemoryResourceHandleExternalLink : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x3144d17c,
                                                "hkMemoryResourceHandleExternalLink",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkMemoryResourceHandleExternalLink);

        nemesis::hkStringPtr m_memberName;
        nemesis::hkStringPtr m_externalId;
        bool m_externalIdIsAllocated;
        bool m_memberNameIsAllocated;

    public:
        hkMemoryResourceHandleExternalLink() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMemoryResourceHandleExternalLink::Class;
}
