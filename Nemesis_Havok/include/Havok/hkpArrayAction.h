#pragma once

#include "Havok/Base/hkpAction.h"

#include "Havok/hkpEntity.h"

namespace nemesis
{
    struct hkpArrayAction : nemesis::hkpAction
    {
        static constexpr nemesis::hkClass Class{0x674bcd2d,
                                                "hkpArrayAction",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpArrayAction);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpEntity>> m_entities;

    public:
        hkpArrayAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpArrayAction::Class;
}
