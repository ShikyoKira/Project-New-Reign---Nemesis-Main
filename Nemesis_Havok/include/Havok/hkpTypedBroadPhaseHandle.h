#pragma once

#include "Havok/hkpBroadPhaseHandle.h"

namespace nemesis
{
    struct hkpTypedBroadPhaseHandle : nemesis::hkpBroadPhaseHandle
    {
        static constexpr nemesis::hkClass Class{0xf4b0f799,
                                                "hkpTypedBroadPhaseHandle",
                                                &nemesis::hkpBroadPhaseHandle::Class,
                                                12,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpTypedBroadPhaseHandle);

        char m_type{};
        char m_ownerOffset{};
        char m_objectQualityType{};
        unsigned int m_collisionFilterInfo{};

    public:
        hkpTypedBroadPhaseHandle() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTypedBroadPhaseHandle::Class;
}
