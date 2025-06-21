#pragma once

#include "Havok/Base/hkpAction.h"

#include "Havok/hkpEntity.h"

namespace nemesis
{
    struct hkpBinaryAction : nemesis::hkpAction
    {
        static constexpr nemesis::hkClass Class{0xc00f3403,
                                                "hkpBinaryAction",
                                                &nemesis::hkpAction::Class,
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
        nemesis::hkRefPtr<nemesis::hkpEntity> m_entityA;
        nemesis::hkRefPtr<nemesis::hkpEntity> m_entityB;

    public:
        hkpBinaryAction(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBinaryAction::Class;
}
