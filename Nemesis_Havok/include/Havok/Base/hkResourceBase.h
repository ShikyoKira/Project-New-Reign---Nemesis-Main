#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkResourceBase : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x660d7cac,
                                                "hkResourceBase",
                                                &nemesis::hkReferencedObject::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    public:
        hkResourceBase(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkResourceBase::Class;
}
