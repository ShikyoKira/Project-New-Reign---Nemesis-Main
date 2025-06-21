#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkMemoryTracker : virtual nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x7bd5c66f,
                                                "hkMemoryTracker",
                                                nullptr,
                                                1,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkMemoryTracker);

        unsigned char unk0;

    public:
        hkMemoryTracker() noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMemoryTracker::Class;
}
