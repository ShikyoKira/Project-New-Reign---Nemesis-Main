#pragma once

#include "Havok/hkbBoolVariableSequencedDataSample.h"
#include "Havok/hkbSequencedData.h"

namespace nemesis
{
    struct hkbBoolVariableSequencedData : nemesis::hkbSequencedData
    {
        static constexpr nemesis::hkClass Class{0x37416fce,
                                                "hkbBoolVariableSequencedData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbBoolVariableSequencedData);

        nemesis::hkArray<nemesis::hkbBoolVariableSequencedDataSample> m_samples;
        int m_variableIndex;

    public:
        hkbBoolVariableSequencedData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBoolVariableSequencedData::Class;
}
