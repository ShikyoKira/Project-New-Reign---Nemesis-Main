#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbBoolVariableSequencedDataSample : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x514763dc,
                                                "hkbBoolVariableSequencedDataSample",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBoolVariableSequencedDataSample);

        float m_time;
        bool m_value;

    public:
        hkbBoolVariableSequencedDataSample() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBoolVariableSequencedDataSample::Class;
}
