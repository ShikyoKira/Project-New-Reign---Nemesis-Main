#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbMoveCharacterModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x28f67ba0,
                                                "hkbMoveCharacterModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbMoveCharacterModifierInternalState);

        float m_timeSinceLastModify{};

    public:
        hkbMoveCharacterModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbMoveCharacterModifierInternalState::Class;
}
