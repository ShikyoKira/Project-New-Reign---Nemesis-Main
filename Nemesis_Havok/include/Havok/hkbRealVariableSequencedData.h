#pragma once

#include "Havok/hkbRealVariableSequencedDataSample.h"
#include "Havok/hkbSequencedData.h"

namespace nemesis
{
    struct hkbRealVariableSequencedData : nemesis::hkbSequencedData
    {
        static constexpr nemesis::hkClass Class{0xe2862d02,
                                                "hkbRealVariableSequencedData",
                                                &nemesis::hkbSequencedData::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbRealVariableSequencedData);

        nemesis::hkArray<nemesis::hkbRealVariableSequencedDataSample> m_samples;
        int m_variableIndex;

    public:
        hkbRealVariableSequencedData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRealVariableSequencedData::Class;
}
