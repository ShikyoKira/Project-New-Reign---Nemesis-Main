#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkBaseObject : virtual nemesis::HavokObject
    {
    protected:
        static constexpr nemesis::hkClass Class{0xe0708a00,
                                                "hkBaseObject",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    public:
        hkBaseObject(unsigned int signature) noexcept;

        virtual void SerializeTo(nemesis::Serializer& serializer) const;
        virtual void DeserializeFrom(nemesis::Deserializer& deserializer);
    };

    constexpr nemesis::hkClass nemesis::hkBaseObject::Class;
}
