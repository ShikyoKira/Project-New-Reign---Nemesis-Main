#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpAgentEntry : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0,
                                                "hkpAgentEntry",
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
        unsigned char m_streamCommand;
        unsigned char m_agentType;
        unsigned char m_numContactPoints;
        unsigned char m_size;

    public:
        hkpAgentEntry(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAgentEntry::Class;
}
