#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbBlenderGeneratorChildInternalState.h"

namespace nemesis
{
    struct hkbBlenderGeneratorInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x84717488,
                                                "hkbBlenderGeneratorInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBlenderGeneratorInternalState);

        nemesis::hkArray<nemesis::hkbBlenderGeneratorChildInternalState> m_childrenInternalStates;
        nemesis::hkArray<short> m_sortedChildren;
        float m_endIntervalWeight{};
        int m_numActiveChildren{};
        short m_beginIntervalIndex{};
        short m_endIntervalIndex{};
        bool m_initSync{};
        bool m_doSubtractiveBlend{};

    public:
        hkbBlenderGeneratorInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBlenderGeneratorInternalState::Class;
}
