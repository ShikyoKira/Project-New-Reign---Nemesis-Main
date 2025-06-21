#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbMessageLog : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x26a196c5,
                                                "hkbMessageLog",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbMessageLog);

        nemesis::hkRefPtr<nemesis::HavokObject> m_messages;
        int m_maxMessages;

    public:
        hkbMessageLog() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbMessageLog::Class;
}
