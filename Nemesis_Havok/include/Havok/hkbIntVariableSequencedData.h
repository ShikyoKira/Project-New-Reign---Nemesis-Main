#pragma once

#include "Havok/hkbIntVariableSequencedDataSample.h"
#include "Havok/hkbSequencedData.h"

namespace nemesis
{
    struct hkbIntVariableSequencedData : nemesis::hkbSequencedData
    {
        static constexpr nemesis::hkClass Class{0x7bfc518a,
                                                "hkbIntVariableSequencedData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbIntVariableSequencedData);

        nemesis::hkArray<nemesis::hkbIntVariableSequencedDataSample> m_samples;
        int m_variableIndex;

    public:
        hkbIntVariableSequencedData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbIntVariableSequencedData::Class;
}
