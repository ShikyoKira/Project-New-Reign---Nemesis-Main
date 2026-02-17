#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbBehaviorEventsInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x66840004,
                                                "hkbBehaviorEventsInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorEventsInfo);

        uint64_t m_characterId{};
        nemesis::hkArray<short> m_externalEventIds;
        int m_padding{};

    public:
        hkbBehaviorEventsInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorEventsInfo::Class;
}
