#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbStateMachineActiveTransitionInfo.h"

namespace nemesis
{
    struct hkbManualSelectorGeneratorInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x492c6137,
                                                "hkbManualSelectorGeneratorInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbManualSelectorGeneratorInternalState);

        char m_currentGeneratorIndex;
        char m_generatorIndexAtActivate;
        nemesis::hkArray<nemesis::hkbStateMachineActiveTransitionInfo> m_activeTransitions;

    public:
        hkbManualSelectorGeneratorInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbManualSelectorGeneratorInternalState::Class;
}
