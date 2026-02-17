#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventSequencedData.h"
#include "Havok/hkbRealVariableSequencedData.h"
#include "Havok/hkbBoolVariableSequencedData.h"
#include "Havok/hkbIntVariableSequencedData.h"
#include "Havok/hkbSequenceStringData.h"

namespace nemesis
{
    struct hkbSequence : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x43182ca3,
                                                "hkbSequence",
                                                &nemesis::hkbModifier::Class,
                                                248,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSequence);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbEventSequencedData>> m_eventSequencedData;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbRealVariableSequencedData>> m_realVariableSequencedData;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbBoolVariableSequencedData>> m_boolVariableSequencedData;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbIntVariableSequencedData>> m_intVariableSequencedData;
        nemesis::hkEventId m_enableEventId;
        nemesis::hkEventId m_disableEventId;
        nemesis::hkRefPtr<nemesis::hkbSequenceStringData> m_stringData;
        nemesis::hkRefPtr<nemesis::HavokObject> m_variableIdMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_eventIdMap;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_nextSampleEvents;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_nextSampleReals;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_nextSampleBools;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_nextSampleInts;
        float m_time{};
        bool m_isEnabled{};

    public:
        hkbSequence() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSequence::Class;
}
