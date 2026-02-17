#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbRotateCharacterModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xdc40bf4a,
                                                "hkbRotateCharacterModifierInternalState",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbRotateCharacterModifierInternalState);

        float m_angle{};

    public:
        hkbRotateCharacterModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRotateCharacterModifierInternalState::Class;
}
