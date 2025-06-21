#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbSequenceInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x419b9a05,
                                                "hkbSequenceInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSequenceInternalState);

        nemesis::hkArray<int> m_nextSampleEvents;
        nemesis::hkArray<int> m_nextSampleReals;
        nemesis::hkArray<int> m_nextSampleBools;
        nemesis::hkArray<int> m_nextSampleInts;
        float m_time;
        bool m_isEnabled;

    public:
        hkbSequenceInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSequenceInternalState::Class;
}
