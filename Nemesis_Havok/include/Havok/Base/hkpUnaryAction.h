#pragma once

#include "Havok/Base/hkpAction.h"

#include "Havok/hkpEntity.h"

namespace nemesis
{
    struct hkpUnaryAction : nemesis::hkpAction
    {
        static constexpr nemesis::hkClass Class{0x895532c0,
                                                "hkpUnaryAction",
                                                &nemesis::hkpAction::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkRefPtr<nemesis::hkpEntity> m_entity;

    public:
        hkpUnaryAction(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpUnaryAction::Class;
}
