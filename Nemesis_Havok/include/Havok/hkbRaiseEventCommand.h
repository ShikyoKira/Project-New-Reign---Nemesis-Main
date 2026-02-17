#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbRaiseEventCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa0a7bf9c,
                                                "hkbRaiseEventCommand",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbRaiseEventCommand);

        uint64_t m_characterId{};
        bool m_global{};
        int m_externalId{};

    public:
        hkbRaiseEventCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRaiseEventCommand::Class;
}
