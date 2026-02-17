#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpBroadPhaseHandle : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x940569dc,
                                                "hkpBroadPhaseHandle",
                                                nullptr,
                                                4,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        unsigned int m_id{};

    public:
        hkpBroadPhaseHandle(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBroadPhaseHandle::Class;
}
