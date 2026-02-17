#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbIntVariableSequencedDataSample : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xbe7ac63c,
                                                "hkbIntVariableSequencedDataSample",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbIntVariableSequencedDataSample);

        float m_time{};
        int m_value{};

    public:
        hkbIntVariableSequencedDataSample() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbIntVariableSequencedDataSample::Class;
}
