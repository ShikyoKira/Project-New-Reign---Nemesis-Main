#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbRealVariableSequencedDataSample : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xbb708bbd,
                                                "hkbRealVariableSequencedDataSample",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbRealVariableSequencedDataSample);

        float m_time;
        float m_value;

    public:
        hkbRealVariableSequencedDataSample() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRealVariableSequencedDataSample::Class;
}
