#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbCharacterControlCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x7a195d1d,
                                                "hkbCharacterControlCommand",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterControlCommand);

        using CharacterControlCommand = nemesis::hkInternalClassEnumItem<unsigned char>;

        uint64_t m_characterId{};
        nemesis::hkEnum<CharacterControlCommand, unsigned char> m_command = {
            {"COMMAND_HIDE", 0},
            {"COMMAND_SHOW", 1},
        };
        int m_padding{};

    public:
        hkbCharacterControlCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterControlCommand::Class;
}
