#pragma once

#include "Havok/Base/hkWorldMemoryAvailableWatchDog.h"

namespace nemesis
{
    struct hkpDefaultWorldMemoryWatchDog : nemesis::hkWorldMemoryAvailableWatchDog
    {
        static constexpr nemesis::hkClass Class{0x77d6b19f,
                                                "hkpDefaultWorldMemoryWatchDog",
                                                &nemesis::hkWorldMemoryAvailableWatchDog::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpDefaultWorldMemoryWatchDog);

        int m_freeHeapMemoryRequested;

    public:
        hkpDefaultWorldMemoryWatchDog() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpDefaultWorldMemoryWatchDog::Class;
}
