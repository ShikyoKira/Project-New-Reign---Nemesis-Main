#pragma once

#include "Havok/Base/hkpWorldObject.h"

namespace nemesis
{
    struct hkpPhantom : nemesis::hkpWorldObject
    {
        static constexpr nemesis::hkClass Class{0x9b7e6f86,
                                                "hkpPhantom",
                                                &nemesis::hkpWorldObject::Class,
                                                240,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_overlapListeners;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_phantomListeners;

    public:
        hkpPhantom(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPhantom::Class;
}
